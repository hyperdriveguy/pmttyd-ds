#include "window_primitive_continuation.hpp"
#include "window_frame.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
static uint32_t bits(float x){uint32_t n;std::memcpy(&n,&x,4);return n;}
struct Event {uint32_t kind,id;std::vector<uint32_t> data;bool operator==(const Event&o)const{return kind==o.kind&&id==o.id&&data==o.data;}};
struct Fixture {
 std::map<uint32_t,uint32_t> words{{0x8041ea1c,1},{0x8041ea24,0x9000},{0x80420528,0x000000ff},{0x80420538,bits(0)},{0x80420540,bits(1.f/3)},{0x80420544,bits(2.f/3)},{0x80420548,bits(1)}};
 WindowPrimitiveServices s;WindowPrimitiveCursor c;std::vector<Event> events;std::vector<float> fifo;
 int hold=-1,waits=0;bool live=true,recurse=false;uint32_t rgba=0xffffffcf;uint32_t loseAt=0;const void* held=nullptr;
 Fixture(){for(unsigned i=0;i<12;++i)words[0x1000+0x11c+4*i]=bits(i%5==0?1.f:0.f);
  s.user=this;s.current=[](void*p){return static_cast<Fixture*>(p)->live;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.wait(nullptr))return {};const auto it=f.words.find(a);if(it==f.words.end())return {};f.events.push_back({0,a,{it->second}});return it->second;};
  s.request=[](void*p,const WindowPrimitiveRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.recurse){f.recurse=false;assert(!f.runOnce());assert(!f.c.reset());}if(f.wait(&q))return {};Event e{1,q.callsite,q.integers};for(auto v:q.reals)e.data.push_back(bits(v));if(q.color)e.data.push_back(*q.color);if(q.matrix)for(auto v:*q.matrix)e.data.push_back(bits(v));f.events.push_back(e);if(q.kind==WindowPrimitiveRequestKind::FifoFloat)f.fifo.push_back(q.reals.at(0));if(f.loseAt==q.callsite)f.live=false;return q.function==0x8000f2e4?0x1000u:0u;};
  s.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);if(f.wait(&q)){q.output[0]=1234;return false;}Event e{2,q.function,{}};for(auto v:q.left)e.data.push_back(bits(v));for(auto v:q.right)e.data.push_back(bits(v));f.events.push_back(e);q.output={};if(q.function==0x8029dd04){for(unsigned i=0;i<12;++i)q.output[i]=i%5==0?1.f:0.f;}else{assert(q.function==0x8029dd64);q.output=q.left;}return true;};
 }
 bool wait(const void*address){if(hold!=int(events.size()))return false;if(waits++<2){if(held)assert(held==address);held=address;return true;}return false;}
 bool runOnce(){return resumeOriginalWindowPrimitive(0,rgba,10,20,400,50,10,s,c);}
 void run(){for(unsigned i=0;i<5;++i)if(runOnce())return;assert(false);}
};
int main(){
 Fixture base;base.run();assert(base.fifo.size()==180);auto mesh=type0WindowFrame(10,20,400,50,10,{255,255,255,207});unsigned n=0;
 for(const auto&v:mesh.quads)for(float x:{v.x,v.y,v.z,v.u,v.v})assert(bits(base.fifo[n++])==bits(x));
 const auto expected=base.events;assert(base.runOnce());assert(base.events==expected);
 for(unsigned i=0;i<expected.size();++i){Fixture f;f.hold=int(i);f.run();assert(f.waits==3);assert(f.events==expected);}
 Fixture disabled;disabled.words[0x8041ea1c]=0;disabled.s.matrix=nullptr;disabled.run();assert(disabled.events.size()==2);assert(disabled.fifo.empty());
 Fixture missing;missing.s.request=nullptr;assert(!missing.runOnce());assert(missing.events.empty());
 Fixture noCamera;noCamera.words.erase(0x1120);assert(!noCamera.runOnce());assert(noCamera.fifo.empty());
 // Source loads borrowed color only after the first CurrentMatrix call.
 unsigned beforeColor=0,colorCall=0;for(unsigned i=0;i<expected.size();++i){if(expected[i].id==0x8007b648)beforeColor=i;if(expected[i].id==0x8007b65c)colorCall=i;}
 Fixture late;late.hold=int(beforeColor);assert(!late.runOnce());late.rgba=0x01020304;late.run();assert(late.events[colorCall].data.back()==0x01020304);
 Fixture captured;captured.hold=int(colorCall);assert(!captured.runOnce());captured.rgba=0;captured.run();assert(captured.events[colorCall].data.back()==0xffffffcf);
 unsigned firstLane=0;for(unsigned i=0;i<expected.size();++i)if(expected[i].id==0x8007b6b8)firstLane=i;
 Fixture fresh;fresh.hold=int(firstLane);assert(!fresh.runOnce());fresh.words[0x80420538]=bits(.25f);fresh.run();assert(fresh.fifo[0]==10&&fresh.fifo[2]==.25f);assert(fresh.fifo.back()==1);
 Fixture swap;swap.hold=int(colorCall);assert(!swap.runOnce());const auto previous=swap.events;assert(!resumeOriginalWindowPrimitive(1,swap.rgba,10,20,400,50,10,swap.s,swap.c));auto altered=swap.s;altered.matrix=nullptr;assert(!resumeOriginalWindowPrimitive(0,swap.rgba,10,20,400,50,10,altered,swap.c));assert(swap.events==previous);swap.recurse=true;swap.run();assert(swap.events==expected);
 Fixture lost;lost.loseAt=0x8007b65c;assert(!lost.runOnce());const auto stopped=lost.events;lost.live=true;assert(!lost.runOnce());assert(lost.events==stopped);
 for(uint32_t type:{1u,0xffffu,0x10000u,0xffffffffu}){Fixture f;assert(resumeOriginalWindowPrimitive(type,f.rgba,10,20,400,50,10,f.s,f.c));assert(f.events==expected);}
 return 0;
}
