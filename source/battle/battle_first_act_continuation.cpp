#include "battle_first_act_continuation.hpp"
#include "battle_sequence_startup.hpp"
namespace ttyd {namespace {
constexpr uint32_t FirstActBase=0x02000000,EntryPriority=10,UnitLimit=64;
struct Guard{bool&b;explicit Guard(bool&v):b(v){b=true;}~Guard(){b=false;}};
bool same(const BattleFirstActContinuationServices&a,const BattleFirstActContinuationServices&b){const auto&x=a.actors.memory;const auto&y=b.actors.memory;return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event;}
enum class Step{Dispatch,Camera,Scan,Motion,Data,Entry,StrikeUnits,StrikeRead,StrikeTarget,StrikeDamage,StrikeWeapon,StrikeWindow,StrikeStatus,StrikeFeedback,StrikeAttack,StrikeCompanion,StrikeCompanionEntry,Timer,Fade,Sound,FadeCheck,SoundOff,Barrier1,StatusOn,Barrier2,FadeCheck2,Concluded,EndAudience,Adjust,Barrier3,Message1,Announce1,SystemOn,Reserve,PerAct,Reaction,Concluded2,Slot,Message2,Announce2,Barrier4,Wall,WallEvent,LuckScan,LuckStatus,LuckEntry,LuckAdjust,LuckWaitEvent,LuckBarrier,LuckAnnounce,LuckMessage,Nozzle,Barrier5,Finish};
}
struct BattleFirstActCursor::State {
 BattleFirstActContinuationServices s;uint32_t battle=0,first=0,index=0,unit=0,npc=0,script=0,event=0,combine=0,pending=0,entrySite=0;bool luck=false,ready=true,invalid=false;Step step=Step::Dispatch;
 std::array<std::optional<uint32_t>,2>privateUnits{};std::optional<BattleFirstActRequest>held;
 uint32_t word(uint32_t a)const{return s.actors.memory.word(s.actors.memory.user,a);}uint16_t half(uint32_t a)const{return s.actors.memory.half(s.actors.memory.user,a);}uint8_t byte(uint32_t a)const{return s.actors.byte(s.actors.user,a);}
 void store(uint32_t a,uint32_t v)const{s.actors.memory.storeWord(s.actors.memory.user,a,v);}void storeHalf(uint32_t a,uint16_t v)const{s.actors.memory.storeHalf(s.actors.memory.user,a,v);}
 void set(uint32_t state){packedBattleSetSequence(battle,2,FirstActBase+state,s.actors.memory);}void inc(){packedBattleIncrementSequence(battle,2,s.actors.memory);}
 bool current(){if(invalid||!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 std::optional<uint32_t>call(uint32_t site,uint32_t fn,std::array<uint32_t,4>args={}){pending=fn;if(!held){held=BattleFirstActRequest{};held->function=fn;held->callsite=site;held->integers=args;if(fn==0x800feaf8)held->firstAttackUnits=&privateUnits;}if(!current()||!s.call)return {};auto v=s.call(s.user,*held);if(!current()||!v)return {};held.reset();pending=0;return v;}
 std::optional<uint32_t>bcall(uint32_t site,uint32_t fn){return call(site,fn,{battle});}
 void nextUnit(){++index;unit=0;step=Step::Scan;}
};
BattleFirstActCursor::BattleFirstActCursor()=default;BattleFirstActCursor::~BattleFirstActCursor()=default;
bool BattleFirstActCursor::active()const{return bool(state_);}uint32_t BattleFirstActCursor::pending()const{return state_?state_->pending:0;}bool BattleFirstActCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleFirstAct(uint32_t battle,const BattleFirstActContinuationServices&s,BattleFirstActCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||!s.actors.memory.word||!s.actors.memory.half||!s.actors.byte||!s.actors.memory.storeWord||!s.actors.memory.storeHalf)return false;
 if(cursor.state_&&(cursor.state_->battle!=battle||!same(cursor.state_->s,s)))return false;
 Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<BattleFirstActCursor::State>();auto&c=*cursor.state_;c.s=s;c.battle=battle;if(!c.current())return false;c.first=c.word(c.word(battle+0x2738)+8);}
 auto&c=*cursor.state_;if(!c.current())return false;const auto finish=[&](){cursor.state_.reset();return true;};
 for(;;){switch(c.step){
 case Step::Dispatch:{const auto state=packedBattleGetSequence(battle,2,s.actors.memory);switch(state){
  case FirstActBase:if(!c.first){c.store(battle+0xefc,0);c.step=Step::Camera;}else{c.store(battle+0xefc,c.first>=1&&c.first<=3?10:0);c.store(battle+0xef4,c.word(battle+0xef4)|2);c.step=Step::StrikeUnits;}break;
  case FirstActBase+1:c.step=Step::Timer;break;case FirstActBase+2:c.step=Step::FadeCheck;break;
  case FirstActBase+3:c.step=Step::Concluded;break;case FirstActBase+4:c.step=Step::Barrier3;break;
  case FirstActBase+5:c.step=Step::SystemOn;break;case FirstActBase+6:c.step=Step::Reaction;break;
  case FirstActBase+7:c.step=Step::Message2;break;case FirstActBase+8:c.step=Step::Barrier4;break;
  case FirstActBase+9:c.step=Step::WallEvent;break;
  case FirstActBase+10:c.store(battle+0x1905c,0);c.index=0;c.step=Step::LuckScan;break;
  case FirstActBase+11:{const auto n=c.half(battle+0x18ffa);if(n<300)c.storeHalf(battle+0x18ffa,uint16_t(n+1));c.step=Step::LuckWaitEvent;break;}
  case FirstActBase+12:if(c.word(battle+0xef4)&2){c.set(14);return finish();}c.step=Step::Nozzle;break;
  case FirstActBase+13:c.step=Step::Barrier5;break;case FirstActBase+14:c.step=Step::Finish;break;default:return finish();}break;}
 case Step::Camera:if(!c.call(0x8011e628,0x800374ac,{0x80416d70,EntryPriority,0}))return false;c.npc=c.word(c.word(battle+0x2738)+12);c.index=0;c.step=Step::Scan;break;
 case Step::Scan:if(c.index==UnitLimit){c.set(1);return finish();}c.unit=battleRegistryUnit(battle,int32_t(c.index),s.actors);if(!c.unit||!c.word(c.unit+0x2b8)){c.nextUnit();break;}if(c.word(c.npc+4)&0x100000){const auto k=int32_t(c.word(c.unit+8));if(k==222||(k>=224&&k<231)){c.nextUnit();break;}}c.step=Step::Motion;break;
 case Step::Motion:{const auto p=c.call(0x8011e680,0x8005e0c0);if(!p)return false;if(c.half(*p+0x2e)==26&&c.word(c.unit+8)==222)c.step=Step::Data;else{c.script=c.word(c.unit+0x2b8);c.entrySite=0x8011e6d4;c.step=Step::Entry;}break;}
 case Step::Data:{const auto p=c.call(0x8011e6a4,0x8012733c,{c.unit,65});if(!p)return false;c.script=*p;c.entrySite=0x8011e6b0;c.step=Step::Entry;break;}
 case Step::Entry:{const auto e=c.call(c.entrySite,0x800374ac,{c.script,EntryPriority,0});if(!e)return false;c.store(c.unit+0x2bc,c.word(*e+0x15c));c.store(*e+0x160,c.word(c.unit));c.nextUnit();break;}
 case Step::StrikeUnits:if(!c.bcall(0x8011e764,0x800feaf8))return false;c.step=Step::StrikeRead;break;
 case Step::StrikeRead:if(!c.privateUnits[0]){c.pending=0x800feaf8;return false;}c.pending=0;c.unit=*c.privateUnits[0];c.store(battle+0x420,c.word(c.unit));c.store(c.unit+0x27c,c.word(c.unit+0x27c)|0x1000);c.step=Step::StrikeTarget;break;
 case Step::StrikeTarget:if(!c.privateUnits[1]){c.pending=0x800feaf8;return false;}c.pending=0;c.npc=*c.privateUnits[1];c.script=c.word(c.npc+0x2b0);c.step=Step::StrikeDamage;break;
 case Step::StrikeDamage:{const auto e=c.call(0x8011e794,0x800374ac,{c.script,EntryPriority,0x20});if(!e)return false;if(*e){c.store(*e+0x160,c.word(c.npc));c.store(c.npc+0x278,57);c.store(c.npc+0x2b4,c.word(*e+0x15c));}c.store(c.unit+0x1c,c.word(c.unit+0x1c)|0x04000000);c.step=int32_t(c.first)<9?Step::StrikeWeapon:Step::StrikeStatus;break;}
 case Step::StrikeWeapon:{const auto p=c.call(0x8011e7e0,0x8011edb8,{c.first});if(!p)return false;c.store(c.unit+0x2a0,c.word(*p+0xb0));c.store(c.unit+0x2ac,0);c.step=Step::StrikeWindow;break;}
 case Step::StrikeWindow:if(!c.call(0x8011e7fc,0x800f75f8))return false;c.step=Step::StrikeStatus;break;
 case Step::StrikeStatus:{const auto v=c.call(0x8011e804,0x80128144,{c.unit});if(!v)return false;c.combine=uint8_t(*v);c.step=Step::StrikeFeedback;break;}
 case Step::StrikeFeedback:if(!c.call(c.combine?0x8011e818:0x8011e810,c.combine?0x8019ec28:0x8019ec60))return false;c.script=c.word(c.unit+0x2a0);c.step=Step::StrikeAttack;break;
 case Step::StrikeAttack:{const auto e=c.call(0x8011e82c,0x800374ac,{c.script,EntryPriority,0});if(!e)return false;if(*e){c.store(*e+0x160,c.word(c.unit));c.store(c.unit+0x2a8,c.word(*e+0x15c));}c.step=Step::StrikeCompanion;break;}
 case Step::StrikeCompanion:{if(c.first>=1&&c.first<=6){const auto p=c.bcall(0x8011e874,0x800f8728);if(!p)return false;c.unit=*p;}else if(c.first>=7&&c.first<=8){const auto p=c.bcall(0x8011e8a8,0x800f87a8);if(!p)return false;c.unit=*p;}else c.unit=0;if(!c.unit){c.set(1);return finish();}c.script=c.word(c.unit+0x2b8);c.step=Step::StrikeCompanionEntry;break;}
 case Step::StrikeCompanionEntry:{const auto e=c.call(c.first<=6?0x8011e88c:0x8011e8c0,0x800374ac,{c.script,EntryPriority,0});if(!e)return false;c.store(c.unit+0x2bc,c.word(*e+0x15c));c.store(*e+0x160,c.word(c.unit));c.set(1);return finish();}
 case Step::Timer:{const auto n=c.word(battle+0xefc)-1;c.store(battle+0xefc,n);if(int32_t(n)>0)return finish();c.combine=c.word(0x80422550);c.step=Step::Fade;break;}
 case Step::Fade:if(!c.call(0x8011e910,0x80013368,{23,200,c.combine}))return false;c.step=Step::Sound;break;
 case Step::Sound:{const auto v=c.call(0x8011e91c,0x800d98bc,{0x802efbc4});if(!v)return false;c.store(battle+0x19070,*v);c.set(2);c.step=Step::FadeCheck;break;}
 case Step::FadeCheck:{const auto v=c.call(0x8011e93c,0x80010d94);if(!v)return false;if(*v){c.event=c.word(battle+0x19070);c.step=c.event!=0xffffffff?Step::SoundOff:Step::Barrier1;}else c.step=Step::Barrier1;break;}
 case Step::SoundOff:if(!c.call(0x8011e95c,0x800d95f8,{c.event}))return false;c.store(battle+0x19070,0xffffffff);c.step=Step::Barrier1;break;
 case Step::Barrier1:{const auto v=c.bcall(0x8011e970,0x8011ace8);if(!v)return false;c.step=*v?Step::StatusOn:Step::Barrier2;break;}
 case Step::StatusOn:if(!c.call(0x8011e97c,0x800f7650))return false;c.step=Step::Barrier2;break;
 case Step::Barrier2:{const auto v=c.bcall(0x8011e984,0x8011ace8);if(!v)return false;if(!*v)return finish();c.step=Step::FadeCheck2;break;}
 case Step::FadeCheck2:{const auto v=c.call(0x8011e990,0x80010d94);if(!v)return false;if(!*v)return finish();c.set(3);const auto u=battleRegistryUnit(battle,int32_t(c.word(battle+0x420)),s.actors);if(u)c.store(u+0x1c,c.word(u+0x1c)&~uint32_t(0x04000000));return finish();}
 case Step::Concluded:{const auto v=c.bcall(0x8011e9d8,0x8011aa34);if(!v)return false;if(*v)c.step=Step::EndAudience;else{c.set(4);c.step=Step::Adjust;}break;}
 case Step::EndAudience:if(!c.call(0x8011e9e4,0x801a7e94))return false;packedBattleSetSequence(battle,0,3,s.actors.memory);packedBattleSetSequence(battle,7,0x07000000,s.actors.memory);return finish();
 case Step::Adjust:if(!c.bcall(0x8011ea24,0x801958bc))return false;c.step=Step::Barrier3;break;
 case Step::Barrier3:{const auto v=c.bcall(0x8011ea2c,0x8011ace8);if(!v)return false;if(!*v)return finish();c.step=Step::Message1;break;}
 case Step::Message1:{const auto v=c.bcall(0x8011ea3c,0x801956f8);if(!v)return false;c.combine=*v;c.step=Step::Announce1;break;}
 case Step::Announce1:{const auto v=c.bcall(0x8011ea48,0x80195af8);if(!v)return false;if(c.combine|*v)return finish();c.inc();c.step=Step::SystemOn;break;}
 case Step::SystemOn:if((c.held||!(c.word(battle+0xef4)&0x30))&&!c.call(0x8011ea6c,0x800f7624))return false;c.step=Step::Reserve;break;
 case Step::Reserve:if(!c.call(0x8011ea70,0x800f74b4))return false;c.store(battle+0xef4,c.word(battle+0xef4)&~uint32_t(4));c.step=Step::PerAct;break;
 case Step::PerAct:if(!c.call(0x8011ea80,0x801a7e94))return false;c.inc();c.step=Step::Reaction;break;
 case Step::Reaction:{const auto v=c.call(0x8011ea90,0x801a7e30);if(!v)return false;if(uint8_t(*v))return finish();c.step=Step::Concluded2;break;}
 case Step::Concluded2:{const auto v=c.bcall(0x8011eaa0,0x8011aa34);if(!v)return false;c.step=*v?Step::Message2:Step::Slot;if(*v)c.inc();break;}
 case Step::Slot:{const auto v=c.call(0x8011eaac,0x80203394);if(!v)return false;if(uint8_t(*v))return finish();c.inc();c.step=Step::Message2;break;}
 case Step::Message2:{const auto v=c.bcall(0x8011eac8,0x801956f8);if(!v)return false;c.combine=*v;c.step=Step::Announce2;break;}
 case Step::Announce2:{const auto v=c.bcall(0x8011ead4,0x80195af8);if(!v)return false;if(c.combine|*v)return finish();c.inc();c.step=Step::Barrier4;break;}
 case Step::Barrier4:{const auto v=c.bcall(0x8011eaf0,0x8011ace8);if(!v)return false;if(!*v)return finish();if(c.word(c.word(c.word(battle+0x2738)+12)+4)&0x10){c.set(9);c.step=Step::Wall;}else{c.set(10);return finish();}break;}
 case Step::Wall:if(!c.call(0x8011eb24,0x80148824))return false;return finish();
 case Step::WallEvent:if(!c.held)c.event=c.word(battle+0x182c0);if(c.event){const auto v=c.call(0x8011eb54,0x80036604,{c.event});if(!v)return false;if(!*v)c.store(battle+0x182c0,0);}if(!c.word(battle+0x182c0)){c.set(10);}return finish();
 case Step::LuckScan:if(c.index==UnitLimit){if(c.luck)c.step=Step::LuckEntry;else{c.set(12);return finish();}break;}c.unit=battleRegistryUnit(battle,int32_t(c.index),s.actors);if(c.unit&&c.byte(c.unit+0x304))c.step=Step::LuckStatus;else ++c.index;break;
 case Step::LuckStatus:{const auto v=c.call(0x8011ebc4,0x80127814,{c.unit,27});if(!v)return false;if(!*v)c.luck=true;++c.index;c.step=Step::LuckScan;break;}
 case Step::LuckEntry:{const auto e=c.call(0x8011ebf4,0x800374ac,{0x80416d40,EntryPriority,0});if(!e)return false;c.store(*e+0x160,uint32_t(-5));c.store(battle+0x1905c,c.word(*e+0x15c));const auto a=c.word(battle+0x18ff4),b=c.word(battle+0x18ff0);c.store(battle+0x18ff4,a|0xffffffff);c.store(battle+0x18ff0,b|0xffffffff);c.step=Step::LuckAdjust;break;}
 case Step::LuckAdjust:if(!c.bcall(0x8011ec2c,0x801958bc))return false;c.set(11);c.storeHalf(battle+0x18ffa,0);return finish();
 case Step::LuckWaitEvent:if(!c.held)c.event=c.word(battle+0x1905c);if(c.event){const auto v=c.call(0x8011ec98,0x80036604,{c.event});if(!v)return false;if(*v)c.ready=false;else c.store(battle+0x1905c,0);}c.step=Step::LuckBarrier;break;
 case Step::LuckBarrier:{const auto v=c.bcall(0x8011ecbc,0x8011ace8);if(!v)return false;if(!*v)c.ready=false;c.step=Step::LuckAnnounce;break;}
 case Step::LuckAnnounce:{const auto v=c.bcall(0x8011ecd0,0x80195af8);if(!v)return false;if(*v)c.ready=false;c.step=Step::LuckMessage;break;}
 case Step::LuckMessage:{const auto v=c.bcall(0x8011ece4,0x801956f8);if(!v)return false;if(*v)c.ready=false;if(c.ready)c.set(12);return finish();}
 case Step::Nozzle:if(!c.call(0x8011ed3c,0x80148414,{0}))return false;c.set(13);c.step=Step::Barrier5;break;
 case Step::Barrier5:{const auto v=c.bcall(0x8011ed58,0x8011ace8);if(!v)return false;if(!*v)return finish();c.set(14);c.step=Step::Finish;break;}
 case Step::Finish:c.store(battle+0xef4,c.word(battle+0xef4)&~uint32_t(2));packedBattleSetSequence(battle,0,2,s.actors.memory);packedBattleSetSequence(battle,3,0x03000000,s.actors.memory);return finish();
 }}
}
}
