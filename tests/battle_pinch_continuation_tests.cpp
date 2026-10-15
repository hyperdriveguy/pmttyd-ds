#include <memory>
#include "battle_pinch_continuation.hpp"
#include "battle_main_continuation.hpp"
#include "battle_audience_target.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Fixture {
 static constexpr uint32_t B=0x10000000,C=0x11000000,U=0x20000000,D=0x30000000;
 static const EventScriptAssets& assets(){static const auto value=[](){EventScriptAssets a;assert(a.load("romfs/trio-attack-events.pmevt"));return a;}();return value;}
 std::map<uint32_t,uint8_t> data;const EventScriptAssets& source=assets();
 struct Write {uint32_t address,value;unsigned bytes;};std::vector<Write> writes;
 std::vector<uint32_t> reads,sounds,statusUnits;std::vector<float> quotients;
 unsigned conversions=0;int32_t statusResult=0;bool holdConversion=false,holdSound=false,holdStatus=false;
 std::function<void(unsigned)> onConvert;
 std::function<void(uint32_t)> onStatus;
 std::function<void(uint32_t)> onStore;
 BattlePinchCursor* borrowedCursor=nullptr;bool checkedReentry=false;
 Fixture(){word(0x8041eb90,B);audience(B);audience(C);writes.clear();}
 void byte(uint32_t a,uint8_t n){data[a]=n;}
 uint8_t byte(uint32_t a){const auto i=data.find(a);if(i!=data.end())return i->second;
  if((a>=B && a<B+0x19100) || (a>=C && a<C+0x19100) || (a>=U && a<U+0x40000) || (a>=D && a<D+0x4000))return 0;
  uint32_t n;assert(source.readWord(a&~3u,n));return uint8_t(n>>(24-8*(a&3)));}
 void word(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)byte(a+i,uint8_t(n>>(24-8*i)));}
 uint32_t word(uint32_t a){reads.push_back(a);uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|byte(a+i);return n;}
 void half(uint32_t a,uint16_t n){byte(a,uint8_t(n>>8));byte(a+1,uint8_t(n));}
 uint16_t half(uint32_t a){reads.push_back(a);return uint16_t(uint16_t(byte(a))<<8|byte(a+1));}
 void audience(uint32_t b,float baseline=8,float desired=10,float offset=0){word(b+0x15fd0,bits(baseline));word(b+0x15fd4,bits(desired));word(b+0x15fd8,bits(offset));word(b+0x15fe8,200);}
 uint32_t actor(unsigned slot,int kind=222,int hp=1,int max=10,uint8_t side=0){const auto u=U+slot*0x1000,d=D+slot*0x100;
  word(B+0x20+4*slot,u);word(u,slot);word(u+8,uint32_t(kind));word(u+0x10,d);byte(u+0xc,side);
  half(u+0x10c,uint16_t(hp));half(u+0x108,uint16_t(max));byte(d+0xc,5);byte(d+0xd,1);word(u+0x238,99);return u;}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
  a.byte=[](void* u,uint32_t p){auto& f=*static_cast<Fixture*>(u);f.reads.push_back(p);return f.byte(p);};
  a.memory.word=[](void* u,uint32_t p){return static_cast<Fixture*>(u)->word(p);};
  a.memory.half=[](void* u,uint32_t p){return static_cast<Fixture*>(u)->half(p);};
  a.memory.storeWord=[](void* u,uint32_t p,uint32_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({p,n,4});f.word(p,n);if(f.onStore)f.onStore(p);};
  a.memory.storeByte=[](void* u,uint32_t p,uint8_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({p,n,1});f.byte(p,n);if(f.onStore)f.onStore(p);};return a;
 }
 static std::optional<int32_t> convert(void* u,float q){auto& f=*static_cast<Fixture*>(u);const auto index=++f.conversions;f.quotients.push_back(q);
  if(f.onConvert)f.onConvert(index);
  if(f.holdConversion || !std::isfinite(q) || q>=2147483648.f || q<-2147483648.f)return {};
  return int32_t(q);}
 BattleAudienceTargetContinuationServices target(){BattleAudienceTargetContinuationServices s;s.actors=actors();s.user=this;s.truncate=convert;return s;}
 BattlePinchServices services(){BattlePinchServices s;s.actors=actors();s.user=this;s.truncate=convert;
  s.status=[](void* u,uint32_t actor,int32_t id)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(u);assert(id==27);f.statusUnits.push_back(actor);
    if(f.onStatus)f.onStatus(actor);
    if(f.borrowedCursor){assert(!f.borrowedCursor->reset());assert(!resumePackedBattleCheckAllPinch(B,0,f.services(),*f.borrowedCursor));f.checkedReentry=true;}
    return f.holdStatus?std::nullopt:std::optional<int32_t>(f.statusResult);};
  s.sound=[](void* u,uint32_t key){auto& f=*static_cast<Fixture*>(u);if(f.holdSound)return false;f.sounds.push_back(key);return true;};return s;
 }
 unsigned stores(uint32_t a)const{unsigned n=0;for(const auto& w:writes)if(w.address==a)++n;return n;}
};
void target(){
 Fixture original;assert(original.word(0x802fa328)==0x43300000 && original.word(0x802fa32c)==0x80000000);
 for(float baseline:{4.f,8.f,10.f})for(float desired:{0.f,4.f,8.f,10.f,19.f})for(float delta:{-2.5f,-1.f,-.25f,0.f,.25f,1.f,2.5f})for(float bias:{-3.f,0.f,3.f}){
  Fixture a,b;a.audience(Fixture::B,baseline,desired);b.audience(Fixture::B,baseline,desired);
  assert(packedBattleAudienceAddTargetNum(delta,bias,a.actors()));BattleAudienceTargetCursor c;
  assert(resumePackedBattleAudienceAddTargetNum(delta,bias,b.target(),c));
  if(a.word(Fixture::B+0x15fd4)!=b.word(Fixture::B+0x15fd4))std::cerr<<baseline<<" "<<desired<<" "<<delta<<" "<<bias<<" => "<<scalar(a.word(Fixture::B+0x15fd4))<<" "<<scalar(b.word(Fixture::B+0x15fd4))<<"\n";
  assert(a.word(Fixture::B+0x15fd4)==b.word(Fixture::B+0x15fd4));assert(a.word(Fixture::B+0x15fd8)==b.word(Fixture::B+0x15fd8));
 }
 Fixture held;BattleAudienceTargetCursor c;held.holdConversion=true;
 assert(!resumePackedBattleAudienceAddTargetNum(2.5,3,held.target(),c));
 assert(c.pending()==0x801a1984 && held.stores(Fixture::B+0x15fd8)==1 && held.stores(Fixture::B+0x15fd4)==0);
 const auto count=held.writes.size(),reads=held.reads.size();
 assert(!resumePackedBattleAudienceAddTargetNum(2.5,3,held.target(),c) && held.writes.size()==count && held.reads.size()==reads);
 assert(!resumePackedBattleAudienceAddTargetNum(2.5,4,held.target(),c));
 auto wrong=held.target();wrong.user=nullptr;assert(!resumePackedBattleAudienceAddTargetNum(2.5,3,wrong,c));
 held.holdConversion=false;held.word(0x8041eb90,Fixture::C);
 assert(resumePackedBattleAudienceAddTargetNum(2.5,3,held.target(),c));
 assert(scalar(held.word(Fixture::B+0x15fd4))==11.25f && scalar(held.word(Fixture::C+0x15fd4))==10);
 Fixture later;BattleAudienceTargetCursor lc;later.onConvert=[&](unsigned n){if(n==2)later.holdConversion=true;};
 assert(!resumePackedBattleAudienceAddTargetNum(2.5,0,later.target(),lc));
 assert(later.stores(Fixture::B+0x15fd4)==1);const auto prefix=later.writes.size();
 assert(!resumePackedBattleAudienceAddTargetNum(2.5,0,later.target(),lc) && later.writes.size()==prefix);
 later.onConvert={};later.holdConversion=false;assert(resumePackedBattleAudienceAddTargetNum(2.5,0,later.target(),lc));
 assert(scalar(later.word(Fixture::B+0x15fd4))==11.25f && later.stores(Fixture::B+0x15fd8)==1);
 // Exact original weighted target capture versus unweighted fresh load.
 for(bool weighted:{false,true}){Fixture f;BattleAudienceTargetCursor n;f.audience(Fixture::B,8,weighted?10:8);
  f.onConvert=[&](unsigned){f.word(Fixture::B+0x15fd4,bits(100));};
  assert(resumePackedBattleAudienceAddTargetNumSub(.5,f.target(),n));
  assert(scalar(f.word(Fixture::B+0x15fd4))==(weighted?10.25f:100.5f));}
 // Parent and Sub independently capture the source global; clamps remain parent.
 Fixture owners;BattleAudienceTargetCursor oc;
 owners.onStore=[&](uint32_t a){if(a==Fixture::B+0x15fd8)owners.word(0x8041eb90,Fixture::C);};
 assert(resumePackedBattleAudienceAddTargetNum(1,0,owners.target(),oc));
 assert(scalar(owners.word(Fixture::B+0x15fd4))==10 && scalar(owners.word(Fixture::C+0x15fd4))==10.5f);
 // Source clamps store conditionally, including negative capacity after lower0.
 Fixture clamps;BattleAudienceTargetCursor cc;clamps.audience(Fixture::B,8,-1,1001);clamps.word(Fixture::B+0x15fe8,0xffffffff);
 assert(resumePackedBattleAudienceAddTargetNum(0,0,clamps.target(),cc));
 assert(scalar(clamps.word(Fixture::B+0x15fd4))==-1 && scalar(clamps.word(Fixture::B+0x15fd8))==1000);
 assert(clamps.stores(Fixture::B+0x15fd4)==2 && clamps.stores(Fixture::B+0x15fd8)==2);
 Fixture domain;BattleAudienceTargetCursor dc;
 assert(!resumePackedBattleAudienceAddTargetNum(1e30f,0,domain.target(),dc));const auto effects=domain.writes.size(),r=domain.reads.size();
 assert(!resumePackedBattleAudienceAddTargetNum(1e30f,0,domain.target(),dc) && domain.writes.size()==effects && domain.reads.size()==r);
}
void unit(){
 for(int hp:{-1,0,1,2,5,6,10})for(int mode:{0,1}){Fixture f;const auto u=f.actor(0,222,hp);BattlePinchCursor c;
  assert(resumePackedBattleUnitPinch(u,mode,f.services(),c));const auto flags=f.word(u+0x138);
  assert(bool(flags&0x20000000u)==(hp<=1 && hp<10));assert(bool(flags&0x10000000u)==(hp<=5 && hp<10));
  const auto effects=f.writes.size();assert(resumePackedBattleUnitPinch(u,mode,f.services(),c));assert(f.writes.size()==effects);
 }
 Fixture full;const auto u=full.actor(0,222,1,1);full.word(u+0x138,0x30000123);BattlePinchCursor fc;
 assert(resumePackedBattleUnitPinch(u,0,full.services(),fc));assert(full.word(u+0x138)==0x123);
 Fixture late;const auto a=late.actor(0);BattlePinchCursor lc;late.holdConversion=true;
 assert(!resumePackedBattleUnitPinch(a,0,late.services(),lc));assert(late.word(a+0x138)==0x20000000u);
 const auto prefix=late.writes.size();assert(!resumePackedBattleUnitPinch(a,0,late.services(),lc) && late.writes.size()==prefix);
 late.holdConversion=false;late.onConvert=[&](unsigned){late.half(a+0x10c,10);};
 assert(resumePackedBattleUnitPinch(a,0,late.services(),lc));assert(late.word(a+0x138)==0x20000000u); // Second test reloads HP after first callback.
 Fixture fresh;const auto v=fresh.actor(0,222,2);BattlePinchCursor pc;fresh.onConvert=[&](unsigned){fresh.word(v+0x138,0xdeadbeef);};
 assert(resumePackedBattleUnitPinch(v,0,fresh.services(),pc));assert(fresh.word(v+0x138)==0xdeadbeef);
 Fixture global;const auto gu=global.actor(0);global.audience(Fixture::C,20,20);global.word(0x8041eb90,Fixture::C);BattlePinchCursor gc;
 assert(resumePackedBattleUnitPinch(gu,0,global.services(),gc));
 assert(scalar(global.word(Fixture::B+0x15fd4))==10 && scalar(global.word(Fixture::C+0x15fd4))==23);
 Fixture recursive;BattleAudienceTargetCursor rc;bool checked=false;
 recursive.onConvert=[&](unsigned){assert(!rc.reset());assert(!resumePackedBattleAudienceAddTargetNum(1,0,recursive.target(),rc));checked=true;};
 assert(resumePackedBattleAudienceAddTargetNum(1,0,recursive.target(),rc) && checked);
 Fixture suppressed;const auto s=suppressed.actor(0);BattlePinchCursor sc;
 assert(resumePackedBattleUnitPinch(s,1,suppressed.services(),sc));assert(suppressed.conversions==0);
 assert(resumePackedBattleUnitPinch(s,0,suppressed.services(),sc));assert(suppressed.conversions==0);
}
void all(){
 Fixture f;for(unsigned i=0;i<64;++i)f.actor(i,i?1:222,1);f.word(Fixture::B+0x1720,1);BattlePinchCursor c;
 assert(resumePackedBattleCheckAllPinch(Fixture::B,1,f.services(),c));
 for(unsigned i=0;i<64;++i)assert(f.word(Fixture::U+i*0x1000+0x138)==0x30000000u);
 assert(f.sounds==std::vector<uint32_t>{0x802efc04});
 for(unsigned n=0;n<59;++n)assert(resumePackedBattleCheckAllPinch(Fixture::B,1,f.services(),c));
 assert(f.sounds.size()==1);
 assert(resumePackedBattleCheckAllPinch(Fixture::B,1,f.services(),c));assert(f.sounds.size()==2);
 for(unsigned timer:{0u,128u,129u}){Fixture w;w.actor(0);w.word(Fixture::B+0x1720,1);w.byte(Fixture::B+0x18ff8,uint8_t(timer));BattlePinchCursor wc;
  assert(resumePackedBattleCheckAllPinch(Fixture::B,1,w.services(),wc));assert(w.byte(Fixture::B+0x18ff8)==(timer==128?127:60));assert(w.sounds.size()==(timer==128?0u:1u));}
 Fixture wait;wait.actor(0);wait.word(Fixture::B+0x1720,1);wait.holdSound=true;BattlePinchCursor wc;
 assert(!resumePackedBattleCheckAllPinch(Fixture::B,1,wait.services(),wc) && wc.pending()==0x800d98bc);
 const auto prefix=wait.writes.size(),reads=wait.reads.size();assert(!resumePackedBattleCheckAllPinch(Fixture::B,1,wait.services(),wc) && wait.writes.size()==prefix && wait.reads.size()==reads);
 wait.holdSound=false;assert(resumePackedBattleCheckAllPinch(Fixture::B,1,wait.services(),wc));assert(wait.sounds.size()==1 && wait.byte(Fixture::B+0x18ff8)==60);
 Fixture quiet;quiet.actor(0);BattlePinchCursor qc;assert(resumePackedBattleCheckAllPinch(Fixture::B,1,quiet.services(),qc));assert(quiet.byte(Fixture::B+0x18ff8)==60 && quiet.sounds.empty());
 // First prequalified wrong-kind/dead actor masks later living actors.
 for(bool wrongKind:{false,true}){Fixture shadow;shadow.actor(0,wrongKind?220:222,wrongKind?1:0);shadow.actor(1,224,1);shadow.word(Fixture::B+0x1720,1);BattlePinchCursor sc;
  assert(resumePackedBattleCheckAllPinch(Fixture::B,1,shadow.services(),sc));assert(shadow.sounds.empty() && shadow.byte(Fixture::B+0x18ff8)==0);}
 Fixture second;second.actor(0,222,0);second.actor(1,224,2);second.word(Fixture::B+0x1720,1);BattlePinchCursor secondCursor;
 // Remove dead slot's second-group bit after first-group status. Retained U
 // still ends group1, but fresh group2 registry/flags can reach slot1.
 second.onStatus=[&](uint32_t u){if(u==Fixture::U)second.word(u+0x138,0x20000000u);};
 assert(resumePackedBattleCheckAllPinch(Fixture::B,1,second.services(),secondCursor));assert(second.sounds==std::vector<uint32_t>{0x802efc14});
 Fixture status;status.actor(0);status.holdStatus=true;BattlePinchCursor statusCursor;
 assert(!resumePackedBattleCheckAllPinch(Fixture::B,1,status.services(),statusCursor));const auto changes=status.writes.size();
 assert(!resumePackedBattleCheckAllPinch(Fixture::B,1,status.services(),statusCursor) && status.writes.size()==changes);
 status.holdStatus=false;status.borrowedCursor=&statusCursor;assert(resumePackedBattleCheckAllPinch(Fixture::B,1,status.services(),statusCursor));assert(status.checkedReentry);
 Fixture live;live.actor(0);const auto old=live.actor(1,224,1),replacement=live.actor(63,224,1);BattlePinchCursor liveCursor;
 live.onConvert=[&](unsigned){live.word(Fixture::B+0x24,replacement);live.word(Fixture::B+0x20+4*63,0);};
 assert(resumePackedBattleCheckAllPinch(Fixture::B,0,live.services(),liveCursor));
 assert(live.word(old+0x138)==0 && live.word(replacement+0x138)==0x30000000u);
 Fixture sides;const auto first=sides.actor(0),next=sides.actor(1,224);sides.word(Fixture::B+0x1720,1);BattlePinchCursor sidesCursor;
 sides.onStatus=[&](uint32_t u){if(u==first)sides.byte(u+0xc,1);};
 assert(resumePackedBattleCheckAllPinch(Fixture::B,1,sides.services(),sidesCursor));
 assert((sides.statusUnits==std::vector<uint32_t>{first,next}) && sides.sounds==std::vector<uint32_t>{0x802efc04});
 Fixture skipped;const auto skip=skipped.actor(0);skipped.statusResult=1;BattlePinchCursor skipCursor;
 // Both warning groups skip status27 actors before GetBelong. Final source
 // Mario lookup independently reads allegiance, so isolate order via callback.
 skipped.onStatus=[&](uint32_t u){assert(u==skip);skipped.byte(u+0xc,1);};
 assert(resumePackedBattleCheckAllPinch(Fixture::B,1,skipped.services(),skipCursor));assert(skipped.sounds.empty());
 Fixture empty;const auto mario=empty.actor(0,222,10);empty.byte(Fixture::B+0x18ff8,88);BattlePinchCursor ec;
 assert(resumePackedBattleCheckAllPinch(Fixture::B,0,empty.services(),ec));assert(empty.word(mario+0x238)==0 && empty.byte(Fixture::B+0x18ff8)==0);
}
void sequence(){
 struct Context {Fixture f;BattlePinchCursor pinch;unsigned updates=0,turns=0;bool turnReady=false;} c;
 c.f.actor(0);c.f.word(Fixture::B+4,2);
 BattleSequenceDispatchServices s;s.actors=c.f.actors();s.user=&c;
 s.request=[](void* u,const BattleSequenceDispatchRequest& q){auto& c=*static_cast<Context*>(u);assert(q.battle==Fixture::B);
  if(q.function==0x8011efd8){++c.updates;assert(q.mode==0);return resumePackedBattleCheckAllPinch(q.battle,q.mode,c.f.services(),c.pinch);}
  assert(q.function==0x8011c510);++c.turns;return c.turnReady; // Whole Turn owner admission fixture only.
 };
 BattleSequenceDispatchCursor manager;
 assert(!resumePackedBattleSequenceManager(s,manager) && manager.pending()==0x8011c510);
 const auto stores=c.f.writes.size();const auto conversions=c.f.conversions;
 c.f.word(Fixture::B+4,4);c.f.word(0x8041eb90,Fixture::C);
 assert(!resumePackedBattleSequenceManager(s,manager) && c.updates==1 && c.f.writes.size()==stores && c.f.conversions==conversions);
 c.turnReady=true;assert(resumePackedBattleSequenceManager(s,manager) && c.updates==1 && c.turns==3);
}
}
int main(){target();unit();all();sequence();std::cout<<"Whole source pinch graph, retained audience chunks, source warning order/cadence and child holds passed\n";}
