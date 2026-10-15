#include "animation_main_time_binding.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t R=0x90000000,T=0x91000000,K=0x92000000,V=0x93000000,L=0x94000000;
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
float real(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
struct Fixture {
 std::map<uint32_t,uint32_t> words{{T+8,3},{T+0x20,3},{T+0x28,K},{T+0x40,V},{T+0x24,L},{L+8,bits(30)},{R+0x24,bits(5)},{0x8041fb28,0},{K-44,bits(-10)},{V-4,9}};
 AnimationMainEvaluation input{0,0,R,0,T,0,0};uint32_t loop=1;unsigned calls=0,hold=0;bool held=false;
 std::vector<std::string> trace;void (*afterStore)(Fixture&,uint32_t,uint32_t)=nullptr;
 Fixture(){for(unsigned i=0;i<3;++i){words[K+44*i]=bits(float(10*i));words[V+8*i]=bits(float(i==0?0:i==1?7:19));words[V+8*i+4]=11*(i+1);}}
 bool admit(std::string op){++calls;if(hold==calls && !held){held=true;return false;}trace.push_back(op);return true;}
 AnimationMainTimelineServices services(){AnimationMainTimelineServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit("r"+std::to_string(a)))return {};auto i=f.words.find(a);return i==f.words.end()?std::nullopt:std::optional<uint32_t>(i->second);};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit("w"+std::to_string(a)+"="+std::to_string(v)))return false;f.words[a]=v;if(f.afterStore)f.afterStore(f,a,v);return true;};return s;
 }
 bool run(AnimationMainTimelineCursor& c){return advanceAnimationMainTimeline(input,loop,services(),c);}
 void finish(AnimationMainTimelineCursor& c){for(unsigned i=0;i<100;++i)if(run(c))return;assert(false);}
};
void exactTrace(){
 Fixture f;AnimationMainTimelineCursor c;f.finish(c);assert(!c.active && f.words[R+0x28]==0 && f.words[R+0x2c]==1 && f.words[R+0x34]==0 && f.words[R+0x38]==1 && f.words[R+0x80]==11 && f.words[R+0x30]==bits(.5f));
 const auto read=[](uint32_t a){return "r"+std::to_string(a);};const auto write=[](uint32_t a,uint32_t v){return "w"+std::to_string(a)+"="+std::to_string(v);};
 const std::vector<std::string> expected={read(T+8),read(R+0x24),read(T+0x28),read(K),read(K+44),write(R+0x28,0),read(R+0x28),write(R+0x2c,1),read(T+0x20),read(R+0x24),read(T+0x40),read(V),read(V+8),write(R+0x34,0),read(R+0x34),write(R+0x38,1),read(R+0x34),read(T+0x40),read(V+4),write(R+0x80,11),read(R+0x28),read(T+0x28),read(R+0x24),read(K),read(R+0x2c),read(T+8),read(K+44),read(0x8041fb28),write(R+0x30,bits(.5f))};assert(f.trace==expected);
}
Fixture variant(unsigned id){Fixture f;f.loop=id&1;switch(id/2){case 0:break;case 1:f.words[R+0x24]=bits(25);break;case 2:f.words[T+8]=0;f.words[T+0x20]=0;break;case 3:f.words[T+8]=0xffffffff;f.words[T+0x20]=0xffffffff;break;case 4:f.words[R+0x24]=bits(-5);break;case 5:f.words[K+44]=0x7fc00000;break;case 6:f.words[K+44]=0;f.words[R+0x24]=0;break;default:assert(false);}return f;}
void everyHold(){
 for(unsigned id=0;id<14;++id){Fixture base=variant(id);AnimationMainTimelineCursor done;base.finish(done);assert(!done.active);
  constexpr uint32_t current[]={0,2,0xffffffff,0xffffffff,0xffffffff,1,1};
  constexpr uint32_t discrete[]={0,2,0xffffffff,0xffffffff,0xffffffff,0,0};
  constexpr uint32_t state[]={11,33,9,9,9,11,11};
  assert(base.words[R+0x28]==current[id/2] && base.words[R+0x34]==discrete[id/2] && base.words[R+0x80]==state[id/2]);
  if(id/2==5)assert(std::isnan(real(base.words[R+0x30])));
  else{const float expected[]={.5f,base.loop?.5f:0.f,base.loop?.375f:0.f,base.loop?.375f:0.f,.5f,0.f,0.f};assert(base.words[R+0x30]==bits(expected[id/2]));}
  for(unsigned hold=1;hold<=base.calls;++hold){Fixture f=variant(id);f.hold=hold;AnimationMainTimelineCursor c;for(unsigned visit=0;!f.held && visit<20;++visit)assert(!f.run(c));assert(f.held && c.active);auto changed=f.input;changed.model=1;assert(!advanceAnimationMainTimeline(changed,f.loop,f.services(),c));assert(!advanceAnimationMainTimeline(f.input,f.loop^1,f.services(),c));auto other=f.services();other.user=&base;assert(!advanceAnimationMainTimeline(f.input,f.loop,other,c));f.finish(c);assert(f.trace==base.trace && f.words==base.words);}
 }
}
void capturesAndFreshReads(){
 Fixture f;f.afterStore=[](Fixture& x,uint32_t a,uint32_t){if(a==R+0x2c)x.words[R+0x24]=bits(25);};AnimationMainTimelineCursor c;f.finish(c);assert(f.words[R+0x28]==0 && f.words[R+0x34]==2 && f.words[R+0x30]==bits(2.5f)); // independent scan times and fraction's later fresh R24.
 Fixture wrap;wrap.words[R+0x24]=bits(25);wrap.afterStore=[](Fixture& x,uint32_t a,uint32_t v){if(a==R+0x2c && v==0){x.words[R+0x28]=1;x.words[T+0x24]=L+0x100;x.words[T+0x28]=K+0x1000;x.words[L+0x108]=bits(100);x.words[K+0x1000+44]=bits(80);x.words[R+0x24]=bits(999);}};AnimationMainTimelineCursor w;wrap.finish(w);assert(wrap.words[R+0x30]==bits(.25f)); // old numerator5, new denominator100-80; no numerator replay.
 Fixture predecessor;predecessor.afterStore=[](Fixture& x,uint32_t a,uint32_t){if(a==R+0x28)x.words[R+0x28]=5;if(a==R+0x34)x.words[R+0x34]=7;};AnimationMainTimelineCursor p;for(unsigned i=0;i<10 && p.phase!=24;++i)assert(!predecessor.run(p));assert(p.phase==24 && predecessor.words[R+0x2c]==6 && predecessor.words[R+0x38]==8);predecessor.words[V+7*8+4]=99;predecessor.words[K+5*44]=bits(0);predecessor.finish(p);assert(predecessor.words[R+0x80]==99);
 Fixture clamps;clamps.loop=0;clamps.afterStore=[](Fixture& x,uint32_t a,uint32_t){if(a==R+0x38){x.words[T+8]=1;x.words[T+0x20]=1;}};AnimationMainTimelineCursor clamp;clamps.finish(clamp);assert(clamps.words[R+0x2c]==0 && clamps.words[R+0x38]==0 && clamps.words[R+0x30]==0);
 Fixture zero;zero.afterStore=[](Fixture& x,uint32_t a,uint32_t){if(a==R+0x80)x.words[0x8041fb28]=bits(10);};AnimationMainTimelineCursor z;zero.finish(z);assert(zero.words[R+0x30]==bits(10)); // denominator compared with actual freshly loaded literal.
}
void missingAndYield(){
 Fixture empty;empty.words[R+0x24]=bits(-5);empty.words.erase(V-4);AnimationMainTimelineCursor c;for(unsigned i=0;i<10 && c.phase!=24;++i)assert(!empty.run(c));assert(c.phase==24 && empty.words[R+0x34]==0xffffffff && !empty.words.count(R+0x80));const auto trace=empty.trace;assert(!empty.run(c) && c.phase==24);empty.words[V-4]=0x1234;empty.finish(c);assert(empty.words[R+0x80]==0x1234 && empty.trace.size()>trace.size());
 Fixture huge;huge.words[T+8]=0x7fffffff;huge.words[R+0x24]=0x7fc00000;for(unsigned i=0;i<8;++i)huge.words[K+44*i]=0;AnimationMainTimelineCursor h;
 for(unsigned i=0;i<8;++i){assert(!huge.run(h) && h.phase==3 && h.index==i+1 && h.pointer==K+44*(i+1));assert(!huge.words.count(R+0x28));}
 Fixture negative;negative.words[T+8]=0x80000000;negative.words[T+0x20]=0x80000000;AnimationMainTimelineCursor n;negative.finish(n);assert(negative.words[R+0x28]==0xffffffff && negative.words[R+0x34]==0xffffffff);
 // Original mulli/slwi arithmetic permits actual pre-table addresses and wraps.
 Fixture modulo;modulo.words[T+0x28]=20;modulo.words[T+8]=0;modulo.words[uint32_t(20-44)]=bits(-10);AnimationMainTimelineCursor m;modulo.finish(m);assert(modulo.words[R+0x30]==bits(.375f));
}
void actualScalar(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));for(unsigned i=0;i<17;++i){assert(native->tick(false));}
 auto& memory=native->process().vm().memory;auto read=[&](uint32_t a){uint32_t v;assert(memory.readWord(a,v));return v;};auto* pose=pool->pose(*h);BattlePoseClock clock;assert(native->clock(clock) && pool->evaluate(*h,clock));const auto reference=pose->state;const auto* aliases=native->process().animationMemory();bool current=true;auto binding=AnimationMainTimeBinding::bind(pool,memory,*aliases,[&](){return current;});assert(binding);
 const auto work=read(0x80415dc0),model=read(read(read(read(work)+16*pose->word(0x10)+8)+0xa0)),timeline=read(read(model+0x1ac)+64*pose->word(0x14)+0x3c);AnimationMainEvaluation input{work,read(work+0x10),aliases->poseAddress(unsigned(*h)),model,timeline,pose->word(0x4c),pose->word(0x54)};
 struct Forward{AnimationMainTimeServices source;uint32_t pose;bool* current;unsigned writes=0,converts=0;};Forward f{binding->services(),input.pose,&current};AnimationMainTimeServices s;s.user=&f;
 s.word=[](void* u,uint32_t a){auto& x=*static_cast<Forward*>(u);return x.source.word(x.source.user,a);};s.store=[](void* u,uint32_t a,uint32_t v){auto& x=*static_cast<Forward*>(u);if(!x.source.store(x.source.user,a,v))return false;++x.writes;if(a==x.pose+0x80)*x.current=false;return true;};s.doubleWord=[](void* u,uint32_t a){auto& x=*static_cast<Forward*>(u);return x.source.doubleWord(x.source.user,a);};s.divide=[](void* u,uint64_t n,uint32_t d,uint64_t& q){auto& x=*static_cast<Forward*>(u);return x.source.divide(x.source.user,n,d,q);};s.convertUnsigned=[](void* u,uint64_t n,float& v){auto& x=*static_cast<Forward*>(u);if(!x.source.convertUnsigned(x.source.user,n,v))return false;++x.converts;return true;};s.truncate=[](void* u,float v){auto& x=*static_cast<Forward*>(u);return x.source.truncate(x.source.user,v);};
 AnimationMainScalarCursor c;for(unsigned i=0;current && i<100;++i)assert(!advanceAnimationMainScalar(input,s,c));assert(c.active && c.phase==1 && c.timeline.phase==26 && f.converts==1);const auto writes=f.writes;const auto epoch=pose->timestamp();pose->word(0x18,0x1234);pose->word(0x1c,0x5678);current=true;auto changed=input;changed.model+=4;assert(!advanceAnimationMainScalar(changed,s,c));auto other=s;other.convertUnsigned=nullptr;assert(!advanceAnimationMainScalar(input,other,c) && f.converts==1 && f.writes==writes);assert(advanceAnimationMainScalar(input,s,c) && !c.active && f.converts==1 && f.writes==writes+1);
 for(unsigned offset:{0x20u,0x24u,0x84u,0x28u,0x2cu,0x34u,0x38u,0x80u,0x30u}){for(unsigned i=0;i<4;++i)assert(pose->state[offset+i]==reference[offset+i]);}
 pose->word(0x18,uint32_t(epoch>>32));pose->word(0x1c,uint32_t(epoch));
 current=false;AnimationMainTimelineCursor wait;assert(!advanceAnimationMainTimeline(input,1,binding->timelineServices(),wait) && wait.phase==0);current=true;for(unsigned i=0;i<100;++i)if(advanceAnimationMainTimeline(input,1,binding->timelineServices(),wait))break;assert(!wait.active);
}
}
int main(){exactTrace();everyHold();capturesAndFreshReads();missingAndYield();actualScalar();}
