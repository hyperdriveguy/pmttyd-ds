#include "animation_main_time_binding.hpp"
#include "animation_sample_time.hpp"
#include "field_npc_clock.hpp"
#include "effect_maya_animation.hpp"
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
constexpr uint32_t R=0x90000000,T=0x91000000,L=0x92000000,GP=0x93000000;
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
float real(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
struct Fixture {
 std::map<uint32_t,uint32_t> words{{R+0xc,0},{T+0x24,L},{L,1},{0x80415fd0,GP},{0x800000f8,162000000},{GP+0x40,0},{GP+0x44,40500000},{GP+0x38,0},{GP+0x3c,20250000},{R+0x1c,0},{R+0x18,0},{0x8041fba4,bits(60)},{0x8041fba8,bits(1000)},{0x8041fb28,0},{R+0x44,bits(1)},{L+8,bits(10)},{L+4,0}};
 uint64_t bias=0x4330000080000000ull,convertedArgument=0;unsigned calls=0,hold=0,converts=0,divides=0,truncates=0;bool held=false;
 std::vector<std::string> trace;AnimationMainEvaluation input{0,0,R,0,T,0,0};
 bool admit(std::string name){++calls;if(hold==calls && !held){held=true;return false;}trace.push_back(name);return true;}
 AnimationMainTimeServices services(){AnimationMainTimeServices s;s.user=this;
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.admit("r"+std::to_string(a)))return {};assert(f.words.count(a));return f.words[a];};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);if(!f.admit("w"+std::to_string(a)))return false;f.words[a]=v;return true;};
  s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{auto& f=*static_cast<Fixture*>(p);assert(a==0x802c1070);return f.admit("bias")?std::optional<uint64_t>(f.bias):std::nullopt;};
  s.divide=[](void* p,uint64_t n,uint32_t d,uint64_t& q){auto& f=*static_cast<Fixture*>(p);if(!f.admit("divide"))return false;++f.divides;return nativeFieldNpcClockDivide(nullptr,n,d,q);};
  s.convertUnsigned=[](void* p,uint64_t n,float& v){auto& f=*static_cast<Fixture*>(p);if(!f.admit("unsigned"))return false;++f.converts;f.convertedArgument=n;return effectMayaConvertUnsigned(nullptr,n,v);};
  s.truncate=[](void* p,float n)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.admit("truncate"))return {};auto v=nativeAnimationLoopConversion(nullptr,n);if(v)++f.truncates;return v;};return s;}
 bool run(AnimationMainTimeCursor& c,AnimationMainTimeResult& out){return advanceAnimationMainTime(input,services(),c,out);}
 void clock(uint64_t n){words[GP+0x40]=uint32_t(n>>32);words[GP+0x44]=uint32_t(n);words[GP+0x38]=uint32_t(n>>32);words[GP+0x3c]=uint32_t(n);}
};
void explicitTrace(){
 Fixture f;AnimationMainTimeCursor c;AnimationMainTimeResult out{99};assert(f.run(c,out) && !c.active && out.loop==1);
 const std::vector<std::string> expected={"r"+std::to_string(R+0xc),"r"+std::to_string(T+0x24),"r"+std::to_string(L),"r"+std::to_string(0x80415fd0u),"r"+std::to_string(0x800000f8u),"r"+std::to_string(GP+0x40),"r"+std::to_string(GP+0x44),"divide","r"+std::to_string(R+0x1c),"r"+std::to_string(R+0x18),"unsigned","r"+std::to_string(0x8041fba4u),"r"+std::to_string(0x8041fba8u),"r"+std::to_string(0x8041fb28u),"w"+std::to_string(R+0x20),"r"+std::to_string(R+0x20),"r"+std::to_string(R+0x44),"w"+std::to_string(R+0x20),"r"+std::to_string(T+0x24),"r"+std::to_string(R+0x20),"r"+std::to_string(L+8),"r"+std::to_string(L+4),"bias","w"+std::to_string(R+0x84),"r"+std::to_string(R+0x84),"truncate","w"+std::to_string(R+0x24)};
 assert(f.trace==expected && f.convertedArgument==1000 && f.words[R+0x20]==bits(60) && f.words[R+0x84]==bits(6) && f.words[R+0x24]==0x80000000);
}
Fixture variant(unsigned id){
 Fixture f;f.words[R+0xc]=id&1;f.clock(350ull*40500);f.words[L+4]=bits(5);f.words[L+8]=bits(15);
 switch(id/2){case 0:break;case 1:f.words[L]=0;break;case 2:f.clock(100ull*40500);f.words[L+4]=bits(10);f.words[L+8]=bits(20);break;case 3:f.words[L]=0;f.clock(100ull*40500);f.words[L+4]=bits(10);f.words[L+8]=bits(20);break;case 4:f.words[L+4]=bits(12);f.words[L+8]=bits(12);break;case 5:f.words[L]=0;f.clock(200ull*40500);break;case 6:f.words[L]=0;f.words[R+0x44]=bits(-1);break;default:assert(false);}
 return f;
}
void allHolds(){
 for(unsigned id=0;id<14;++id){Fixture base=variant(id);const auto initial=base.words;AnimationMainTimeCursor complete;AnimationMainTimeResult expected;assert(base.run(complete,expected));const float selected[]={11,15,10,10,12,12,5};assert(std::fabs(real(base.words[R+0x24])-selected[id/2])<1e-5f);
  for(unsigned i=1;i<=base.calls;++i){Fixture f=variant(id);f.hold=i;AnimationMainTimeCursor c;AnimationMainTimeResult out{99};assert(!f.run(c,out) && c.active && f.held && out.loop==99);auto changed=f.input;changed.normals=1;assert(!advanceAnimationMainTime(changed,f.services(),c,out));auto other=f.services();other.user=&base;assert(!advanceAnimationMainTime(f.input,other,c,out));assert(f.run(c,out) && !c.active && out.loop==expected.loop && f.words==base.words && f.trace==base.trace && f.divides==1 && f.converts==1);}
  (void)initial;
 }
}
void capturesAndReloads(){
 Fixture f;f.clock(350ull*40500);f.words[L+4]=bits(5);f.words[L+8]=bits(15);f.hold=24;AnimationMainTimeCursor c;AnimationMainTimeResult out;
 assert(!f.run(c,out) && c.phase==31 && c.bias==f.bias); // bias loaded before ratio store.
 f.bias=0x4330000080000001ull;f.words[L]=0;f.words[0x8041fb28]=bits(100);f.hold=0;
 // Pause a second invocation exactly before fctiwz, then replace the published
 // ratio: conversion uses that fresh reload, not the computed ratio.
 assert(f.run(c,out) && out.loop==1 && f.words[R+0x24]==bits(11));
 Fixture reload;reload.clock(350ull*40500);reload.words[L+4]=bits(5);reload.words[L+8]=bits(15);reload.hold=25;AnimationMainTimeCursor q;assert(!reload.run(q,out) && q.phase==32);reload.words[R+0x84]=bits(3);reload.hold=0;assert(reload.run(q,out) && reload.words[R+0x24]==bits(-9));
 Fixture biased;biased.clock(350ull*40500);biased.words[L+4]=bits(5);biased.words[L+8]=bits(15);biased.bias=0x4330000080000001ull;AnimationMainTimeCursor b;assert(biased.run(b,out) && biased.words[R+0x24]==bits(21));
 Fixture time;time.hold=16;AnimationMainTimeCursor t;assert(!time.run(t,out) && t.phase==15);time.words[R+0x20]=bits(7);time.words[R+0x44]=bits(2);time.words[L]=0;constexpr uint32_t NewControl=L+0x100;time.words[T+0x24]=NewControl;time.words[NewControl+4]=bits(3);time.words[NewControl+8]=bits(23);time.hold=0;assert(time.run(t,out) && out.loop==1 && time.words[R+0x20]==bits(14) && time.words[R+0x24]==bits(14));
}
void numericDomains(){
 AnimationMainTimeResult out;
 Fixture under;under.clock(0);under.words[R+0x1c]=1;under.words[L]=0;AnimationMainTimeCursor u;assert(under.run(u,out) && under.convertedArgument==UINT64_MAX && real(under.words[R+0x20])>0 && under.words[R+0x24]==bits(10));
 Fixture signedClock;signedClock.clock(uint64_t(-40500));signedClock.words[L]=0;AnimationMainTimeCursor s;assert(signedClock.run(s,out) && signedClock.convertedArgument==UINT64_MAX);
 Fixture zero;zero.words[0x800000f8]=0;AnimationMainTimeCursor z;assert(!zero.run(z,out) && z.phase==7 && !zero.words.count(R+0x20));
 Fixture nan;nan.words[R+0x44]=0x7fc00000;AnimationMainTimeCursor n;assert(!nan.run(n,out) && n.phase==33 && std::isnan(real(nan.words[R+0x84])));const unsigned converts=nan.converts;assert(!nan.run(n,out) && n.phase==33 && nan.converts==converts);
 Fixture nonloop;nonloop.words[L]=0;nonloop.words[R+0x44]=0x7fc00000;AnimationMainTimeCursor nc;assert(nonloop.run(nc,out) && std::isnan(real(nonloop.words[R+0x24])));
 Fixture dynamicZero;dynamicZero.words[0x8041fb28]=bits(10);AnimationMainTimeCursor dz;assert(dynamicZero.run(dz,out) && dynamicZero.words[R+0x24]==0 && dynamicZero.words[R+0x84]==bits(60));
}
void actualBinding(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));
 for(unsigned i=0;i<17;++i){assert(native->tick(false));}
 auto& m=native->process().vm().memory;auto read=[&](uint32_t a){uint32_t v;assert(m.readWord(a,v));return v;};auto* pose=pool->pose(*h);assert(pose);BattlePoseClock clock;assert(native->clock(clock) && pool->evaluate(*h,clock));const auto reference=pose->state;
 const auto* aliases=native->process().animationMemory();bool current=true;auto binding=AnimationMainTimeBinding::bind(pool,m,*aliases,[&](){return current;});assert(binding);
 const uint32_t work=read(0x80415dc0),models=read(work),entry=read(models+16*pose->word(0x10)+8),model=read(read(entry+0xa0)),timeline=read(read(model+0x1ac)+64*pose->word(0x14)+0x3c);
 AnimationMainEvaluation input{work,read(work+0x10),aliases->poseAddress(unsigned(*h)),model,timeline,pose->word(0x4c),pose->word(0x54)};
 AnimationMainTimeCursor cursor;AnimationMainTimeResult out{99};current=false;assert(!advanceAnimationMainTime(input,binding->services(),cursor,out) && cursor.phase==0 && out.loop==99);current=true;
 assert(advanceAnimationMainTime(input,binding->services(),cursor,out));for(unsigned offset:{0x20u,0x24u,0x84u})for(unsigned b=0;b<4;++b)assert(pose->state[offset+b]==reference[offset+b]);
 assert(!AnimationMainTimeBinding::bind(pool,m,*aliases,{}));
 // Suspend after an acknowledged actual pose write. Earlier conversion/time
 // captures survive; later R20/rate reads remain fresh.
 struct Forward {AnimationMainTimeServices live;bool* current;unsigned stores=0;};
 Forward forward{binding->services(),&current};AnimationMainTimeServices heldServices;heldServices.user=&forward;
 heldServices.word=[](void* u,uint32_t a){auto& f=*static_cast<Forward*>(u);return f.live.word(f.live.user,a);};
 heldServices.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Forward*>(u);if(!f.live.store(f.live.user,a,v))return false;if(++f.stores==1)*f.current=false;return true;};
 heldServices.doubleWord=[](void* u,uint32_t a){auto& f=*static_cast<Forward*>(u);return f.live.doubleWord(f.live.user,a);};
 heldServices.divide=[](void* u,uint64_t n,uint32_t d,uint64_t& q){auto& f=*static_cast<Forward*>(u);return f.live.divide(f.live.user,n,d,q);};
 heldServices.convertUnsigned=[](void* u,uint64_t n,float& v){auto& f=*static_cast<Forward*>(u);return f.live.convertUnsigned(f.live.user,n,v);};
 heldServices.truncate=[](void* u,float v){auto& f=*static_cast<Forward*>(u);return f.live.truncate(f.live.user,v);};
 AnimationMainTimeCursor held;assert(!advanceAnimationMainTime(input,heldServices,held,out) && held.phase==15 && forward.stores==1);
 const uint64_t oldEpoch=pose->timestamp();pose->word(0x18,0x12345678);pose->word(0x1c,0x87654321);current=true;
 assert(advanceAnimationMainTime(input,heldServices,held,out) && forward.stores==4); // R20 twice total, ratio and selected time once.
 for(unsigned offset:{0x20u,0x24u,0x84u})for(unsigned b=0;b<4;++b)assert(pose->state[offset+b]==reference[offset+b]);
 pose->word(0x18,uint32_t(oldEpoch>>32));pose->word(0x1c,uint32_t(oldEpoch));
 // Source time primitives also expose mapped inactive slots without adding a
 // typed active/resource admission gate that original Main does not contain.
 const uint32_t inactive=aliases->poseAddress(255);assert(m.writeWord(inactive+0xc,2) && m.writeWord(inactive+0x44,bits(1)));
 auto inactiveInput=input;inactiveInput.pose=inactive;AnimationMainTimeCursor inactiveCursor;
 assert(advanceAnimationMainTime(inactiveInput,binding->services(),inactiveCursor,out));
 assert(read(inactive+0x20)==read(input.pose+0x20) && read(inactive+0x24)==read(input.pose+0x24));
 // A captured zero divisor stays unavailable after the live bus is repaired.
 const uint32_t gp=read(0x80415fd0),bus=read(0x800000f8);assert(m.writeWord(0x800000f8,0));AnimationMainTimeCursor wait;assert(!advanceAnimationMainTime(input,binding->services(),wait,out) && wait.phase==7);assert(m.writeWord(0x800000f8,bus));assert(!advanceAnimationMainTime(input,binding->services(),wait,out) && wait.phase==7); // captured zero divisor; repair requires a fresh invocation.
 current=false;assert(!binding->available());
 // Wrong VM with otherwise live aliases cannot create a detached fallback.
 EventNativeRuntime other(0x81000000,platform);assert(!AnimationMainTimeBinding::bind(pool,other.memory,*aliases,[](){return true;}));(void)gp;
}
}
int main(){explicitTrace();allHolds();capturesAndReloads();numericDomains();actualBinding();}
