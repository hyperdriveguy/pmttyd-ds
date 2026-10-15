#include <memory>
#include "source_font_draw_start.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
using namespace ttyd;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Event{uint32_t fn;std::vector<uint32_t>a;bool operator==(const Event&o)const{return fn==o.fn&&a==o.a;}};
struct Fixture{
 static constexpr uint32_t Cam=0x10000,Cam2=0x20000,Gp=0x30000,Color=0x40000;
 EventScriptAssets constants;std::map<uint32_t,uint32_t>words;uint32_t color=0x12345678,currentCamera=Cam;std::shared_ptr<const void>owner=std::make_shared<int>(7);
 SourceFontDrawStartCursor cursor;SourceFontDrawStartInput input{0,&color};std::vector<Event>trace,ops;std::optional<Event>pending;const SourceFontStateCall*borrowed=nullptr;const BattleAudienceMatrixRequest*mathBorrowed=nullptr;const SourceFontDrawStartFog*fogBorrowed=nullptr;
 std::function<void(Fixture&,const Event&)>onOp;std::function<void(Fixture&,uint32_t)>onRead;int hold=-1;unsigned failures=0,effects=0;bool alive=true,probe=false;
 Fixture(){assert(constants.load("romfs/trio-attack-events.pmevt"));words={{0x80415fd0,Gp},{Gp+0x16c,1},{0x80429578,0xaabbccdd},{Color,0x55667788},{0,0x10203040}};for(unsigned i=0;i<12;++i){words[Cam+0x11c+4*i]=bits(float(i+1));words[Cam2+0x11c+4*i]=bits(float(100+i));}}
 uint32_t word(uint32_t a){uint32_t v;const auto i=words.find(a);if(i!=words.end())v=i->second;else assert(constants.readWord(a,v));trace.push_back({1,{a,v}});if(onRead)onRead(*this,a);return v;}
 bool op(uint32_t fn,std::vector<uint32_t>a){Event next{fn,std::move(a)};if(!pending){pending=next;trace.push_back(next);ops.push_back(next);++effects;if(onOp)onOp(*this,next);}else assert(*pending==next);if(probe){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 static void append(std::vector<uint32_t>&v,const BattleActMatrix&m){for(float f:m)v.push_back(bits(f));}
 SourceFontDrawStartServices services(){SourceFontDrawStartServices s;s.common.user=this;s.common.lifetime=owner;s.common.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.common.actors.memory.user=this;s.common.actors.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
 s.camera=[](void*p)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.op(0x8000f2e4,{}))return {};return f.currentCamera;};
 s.common.request=[](void*p,const SourceFontStateCall&q){auto&f=*static_cast<Fixture*>(p);if(f.borrowed)assert(f.borrowed==&q);else f.borrowed=&q;std::vector<uint32_t>a{q.callsite,uint32_t(q.integers.size())};a.insert(a.end(),q.integers.begin(),q.integers.end());a.push_back(bool(q.color));if(q.color)a.push_back(*q.color);a.push_back(bool(q.matrix));if(q.matrix)append(a,*q.matrix);if(!f.op(q.function,a))return false;f.borrowed=nullptr;return true;};
 s.common.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);if(f.mathBorrowed)assert(f.mathBorrowed==&q);else f.mathBorrowed=&q;std::vector<uint32_t>a{bits(q.xyz[0]),bits(q.xyz[1]),bits(q.xyz[2])};if(!f.op(q.function,a))return false;f.mathBorrowed=nullptr;BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
 s.fog=[](void*p,const SourceFontDrawStartFog&q){auto&f=*static_cast<Fixture*>(p);if(f.fogBorrowed)assert(f.fogBorrowed==&q);else f.fogBorrowed=&q;std::vector<uint32_t>a{q.callsite,q.type,q.color};for(float v:q.reals)a.push_back(bits(v));if(!f.op(q.function,a))return false;f.fogBorrowed=nullptr;return true;};return s;}
 bool run(){return resumeSourceFontDrawStart(input,services(),cursor);}
};
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup={}){Fixture expected;if(setup)setup(expected);assert(expected.run());for(unsigned gate=0;gate<expected.ops.size();++gate){Fixture f;if(setup)setup(f);f.hold=int(gate);f.probe=true;assert(!f.run());auto trace=f.trace;auto effects=f.effects;auto s=f.services();s.common.actors.audienceLifecycle.user=&expected;assert(!resumeSourceFontDrawStart(f.input,s,f.cursor));s=f.services();s.fog=nullptr;assert(!resumeSourceFontDrawStart(f.input,s,f.cursor));s=f.services();s.common.lifetime=std::make_shared<int>(9);assert(!resumeSourceFontDrawStart(f.input,s,f.cursor));auto other=f.input;other.guestColor=1;assert(!resumeSourceFontDrawStart(other,f.services(),f.cursor));other=f.input;other.privateColor=&expected.color;assert(!resumeSourceFontDrawStart(other,f.services(),f.cursor));assert(!f.run()&&f.trace==trace&&f.effects==effects);assert(f.run()&&!f.cursor.active());assert(f.trace==expected.trace&&f.effects==expected.effects&&f.words==expected.words);}}
int main(){
 retained();for(uint32_t language:{0u,1u,2u,0xffffffffu})retained([=](auto&f){f.words[Fixture::Gp+0x16c]=language;});retained([](auto&f){f.input={Fixture::Color,nullptr};});retained([](auto&f){f.input={0,nullptr};});
 retained([](auto&f){f.onOp=[](auto&g,const Event&e){if(e.fn==0x802b5084)g.color=0xaabbccdd;if(e.fn==0x802b7880)g.words[Fixture::Gp+0x16c]=0;if(e.fn==0x802b4e04)g.words[Fixture::Cam+0x11c]=bits(44.f);if(e.fn==0x802ba39c)g.words[0x80429578]=0x12345678;};});
 Fixture whole;assert(whole.run());assert(whole.ops.size()==33&&whole.ops.front().fn==0x8000f2e4&&whole.ops.back().fn==0x802b8ff4);std::vector<uint32_t>sites;for(const auto&e:whole.ops)if(e.fn!=0x8000f2e4&&e.fn!=0x8029e2b0)sites.push_back(e.a[0]);assert((sites==std::vector<uint32_t>{0x80074c20,0x80074c28,0x80074c30,0x80074c48,0x80074c5c,0x80074c6c,0x80074c70,0x80074c7c,0x80074c88,0x80074c94,0x80074ca0,0x80074cb8,0x80074cd0,0x80074ce8,0x80074d00,0x80074d20,0x80074d28,0x80074d30,0x80074d44,0x80074d4c,0x80074d60,0x80074d7c,0x80074d98,0x80074db0,0x80074dc8,0x80074dd4,0x80074e20,0x80074e3c,0x80074e48,0x80074e50,0x80074e74}));
 for(const auto&e:whole.ops)if(e.fn==0x802b7880)assert((e.a==std::vector<uint32_t>{0x80074dd4,2,0x8041e97c,0,0,0})); // Literal staticobject, no pointerload.
 Fixture late;late.onOp=[](auto&f,const Event&e){if(e.fn==0x802b5084)f.color=0xabcdef01;if(e.fn==0x802b7880)f.words[Fixture::Gp+0x16c]=0;if(e.fn==0x802b4e04){f.currentCamera=Fixture::Cam2;f.words[Fixture::Cam+0x11c]=bits(77.f);}if(e.fn==0x802ba39c)f.words[0x80429578]=0x77665544;};assert(late.run());for(const auto&e:late.ops){if(e.fn==0x802b8ab8)assert(e.a[4]==0xabcdef01);if(e.fn==0x8029e2b0)assert(e.a[1]==bits(1.f/1024.f));if(e.fn==0x802ba2fc)assert(e.a[5]==bits(77.f));if(e.fn==0x802b8ff4)assert(e.a[2]==0x77665544);}
 Fixture captured;captured.onOp=[](auto&f,const Event&e){if(e.fn==0x802b8ab8)f.color=0;};assert(captured.run());for(const auto&e:captured.ops)if(e.fn==0x802b8ab8)assert(e.a[4]==0x12345678);
 Fixture invalid;invalid.onRead=[](auto&f,uint32_t){f.alive=false;};assert(!invalid.run()&&invalid.cursor.stale());assert(invalid.ops.back().fn==0x802b7880); // No scale/GXsuffix after lifetime invalidates at GPread.
 Fixture unavailable;auto s=unavailable.services();s.common.request=nullptr;assert(!resumeSourceFontDrawStart(unavailable.input,s,unavailable.cursor)&&unavailable.cursor.pending()==0x802b8880&&unavailable.ops.size()==1);
 Fixture stale;stale.hold=0;assert(!stale.run());stale.alive=false;assert(!stale.run()&&stale.cursor.stale());auto trace=stale.trace;stale.alive=true;assert(!stale.run()&&stale.trace==trace);assert(stale.cursor.reset());
 std::cout<<"Whole JUTFont DrawStart source setup census, latecolor/GP/camera/fog reads and every-child retained effects passed\n";
}
