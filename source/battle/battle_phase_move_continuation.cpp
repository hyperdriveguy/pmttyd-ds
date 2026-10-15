#include "battle_phase_move_continuation.hpp"
#include "battle_sequence_startup.hpp"
// Field offsets and masks in these notes are hex; counts, strides, states
// and icon IDs are decimal. Original addresses remain32-bit source identities.
namespace ttyd {
namespace {
int32_t s32(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t s16(uint16_t n){return n<0x8000?int32_t(n):int32_t(n)-65536;}
int32_t s8(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
bool same(const BattleMoveServices& a,const BattleMoveServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user && a.call==b.call && a.actors.user==b.actors.user && a.actors.byte==b.actors.byte &&
 a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user &&
 a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event &&
 x.user==y.user && x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
 x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running {bool& flag;explicit Running(bool& f):flag(f){flag=true;}~Running(){flag=false;}};
enum class Step {Dispatch,First,MajinaiDone,MajinaiEnd,MajinaiEntry,Wait,PhaseScan,PhaseEvent,
 SerialScan,SerialEvent,Concluded,Reset,ResetUnits,Select,SelectStatus,SelectCommit,Deferred,CommandFlags,
 Act,Cleanup,CleanupUnit,PhaseEnd,PerPhase,Reaction,Message,Announce};
}
// Source channels:4 Phase,5 Move,6 Act,7 conclusion; encoded high bytes
//04000000/05000000/06000000/07000000 are source state domains, not tick counts.
// Step is native continuation metadata for already acknowledged parent work.
struct BattleMoveCursor::State {
 BattleMoveServices services;uint32_t battle=0,index=0,pending=0,slot=0,unit=0,id=0,pouch=0,mario=0;
 int32_t capturedState=0;uint32_t message=0;
 Step step=Step::Dispatch;std::optional<BattleMoveRequest> held;
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return services.actors.memory.half(services.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void store(uint32_t a,uint32_t n)const{services.actors.memory.storeWord(services.actors.memory.user,a,n);}
 void storeHalf(uint32_t a,uint16_t n)const{services.actors.memory.storeHalf(services.actors.memory.user,a,n);}
 void storeByte(uint32_t a,uint8_t n)const{services.actors.memory.storeByte(services.actors.memory.user,a,n);}
 uint32_t sequence(uint32_t channel)const{return packedBattleGetSequence(battle,channel,services.actors.memory);}
 void set(uint32_t channel,uint32_t n)const{packedBattleSetSequence(battle,channel,n,services.actors.memory);}
 void inc(uint32_t channel)const{packedBattleIncrementSequence(battle,channel,services.actors.memory);}
 uint32_t lookup(uint32_t actor)const{return battleRegistryUnit(battle,s32(actor),services.actors);}
 template<class Build> std::optional<uint32_t> call(uint32_t fn,Build build){
  pending=fn;if(!held){held=BattleMoveRequest{};held->function=fn;build(*held);}
  if(!services.call)return {};
  const auto result=services.call(services.user,*held);if(result){held.reset();pending=0;}return result;
 }
 std::optional<uint32_t> call(uint32_t fn){return call(fn,[](BattleMoveRequest&){});}
 std::optional<uint32_t> battleCall(uint32_t fn){return call(fn,[&](BattleMoveRequest& q){q.integers[0]=battle;});}
 void loop(){step=Step::Dispatch;}
 void next(Step scan){++slot;step=scan;}
 void end(){set(0,3);set(7,0x07000000);}
};
BattleMoveCursor::BattleMoveCursor()=default;
BattleMoveCursor::~BattleMoveCursor()=default;
bool BattleMoveCursor::active()const{return bool(state_);}
uint32_t BattleMoveCursor::pending()const{return state_?state_->pending:0;}
bool BattleMoveCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleMove(uint32_t battle,const BattleMoveServices& s,BattleMoveCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_ || !battle || !m.word || !m.half || !m.storeWord || !m.storeHalf || !m.storeByte || !s.actors.byte)return false;
 if(cursor.state_ && (cursor.state_->battle!=battle || !same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleMoveCursor::State>();cursor.state_->services=s;cursor.state_->battle=battle;}
 auto& c=*cursor.state_;const auto finish=[&](){cursor.state_.reset();return true;};
 for(;;)switch(c.step){
 // Unsigned Move-state subtraction admits indices0..14. Immediate redispatch
 // versus finish below preserves source invocation boundaries; out-of-range
 // completes locally without inventing another handler or clamping state.
 case Step::Dispatch:
  c.index=c.sequence(5)-0x05000000u;
  if(c.index>14)return finish();
  switch(c.index){
  case 0:c.step=Step::First;break;
  case 1:c.inc(5);c.loop();break;
  case 2:
   c.pouch=c.word(0x8041eb00);c.mario=battleMarioUnit(battle,s.actors);
   if(c.byte(c.pouch+0x5ba)!=2 || c.sequence(4)!=0x04000004){c.inc(5);c.inc(5);c.loop();break;}
   c.storeByte(battle+0x18ff9,2);c.step=Step::MajinaiDone;break;
  case 3:case 5:case 7:c.step=Step::Wait;break;
  case 4:c.slot=0;c.step=Step::PhaseScan;break;
  case 6:c.slot=0;c.step=Step::SerialScan;break;
  case 8:c.step=Step::Reset;break;
  case 9:c.step=Step::Act;break;
  case 10:c.step=Step::Concluded;break;
  case 11:c.step=Step::Reaction;break;
  case 12:c.step=Step::Message;break;
  case 13:c.inc(5);return finish();
  case 14:c.inc(4);c.set(5,0x05000000);return finish();
  }
  break;
 case Step::First:
  if(!c.battleCall(0x8011b0d0))return false;
  c.inc(5);c.loop();break;
 case Step::MajinaiDone:
  if(!c.call(0x800f7718))return false;
  c.step=Step::MajinaiEnd;break;
 case Step::MajinaiEnd:
  if(!c.call(0x800f76d4))return false;
  c.step=Step::MajinaiEntry;break;
 case Step::MajinaiEntry:{
  const auto event=c.call(0x800374ac,[](BattleMoveRequest& q){q.integers={0x803634f8,10,0,0,0,0,0,0};});
  if(!event)return false;
  // Original has no null-event or missing-Mario guard before these source
  // stores. Caller admits real memory/valid actor invariant; engaged0 is not
  // reclassified as asynchronous unavailability.
  c.store(*event+0x160,c.word(c.mario));c.store(c.mario+0x2a8,c.word(*event+0x15c));
  c.inc(5);c.step=Step::Wait;break;
 }
 case Step::Wait:{
  const auto ready=c.battleCall(0x8011ace8);if(!ready)return false;
  if(!*ready)return finish();
  if(c.index==7){c.step=Step::Concluded;break;}
  c.inc(5);if(c.index==11)return finish();c.loop();break;
 }
 // All64 registry slots: any nonnull U with phase-script word294 gets selector1.
 // Serial bank B220 has64 eight-byte pairs; selector0 launches ONE U298 script,
 // marks only its ID word -1,then returns. No added HP/status filters.
 case Step::PhaseScan:
  if(c.slot==64){c.inc(5);c.loop();break;}
  c.unit=battleRegistryUnit(battle,int32_t(c.slot),s.actors);
  if(!c.unit || !c.word(c.unit+0x294)){++c.slot;break;}
  c.step=Step::PhaseEvent;break;
 case Step::PhaseEvent:
  if(!c.call(0x801297bc,[&](BattleMoveRequest& q){q.integers={c.unit,1,0,0,0,0,0,0};}))return false;
  c.next(Step::PhaseScan);break;
 case Step::SerialScan:
  if(c.slot==64){c.set(5,0x0500000a);c.loop();break;}
  c.id=c.word(battle+0x220+8*c.slot);
  if(c.id==0xffffffffu || !(c.unit=c.lookup(c.id)) || !c.word(c.unit+0x298)){++c.slot;break;}
  c.step=Step::SerialEvent;break;
 case Step::SerialEvent:
  if(!c.call(0x801297bc,[&](BattleMoveRequest& q){q.integers={c.unit,0,0,0,0,0,0,0};}))return false;
  c.store(battle+0x220+8*c.slot,0xffffffff);c.inc(5);return finish();
 case Step::Concluded:{
  const auto concluded=c.battleCall(0x8011aa34);if(!concluded)return false;
  if(*concluded){c.end();return finish();}
  if(c.index==7){c.set(5,0x05000006);c.loop();break;}
  c.slot=0;c.step=Step::Cleanup;break;
 }
 case Step::Reset:
  if(!c.call(0x800050b4,[&](BattleMoveRequest& q){q.integers={battle+0x18ffc,0,12,0,0,0,0,0};}))return false;
  c.slot=0;c.step=Step::ResetUnits;break;
 // Clear per-action fields with original word/half/byte widths; U27c keeps
 // only80002000. Selection bit1000 is published later and cleared by cleanup.
 case Step::ResetUnits:
  if(c.slot==64){c.slot=0;c.step=Step::Select;break;}
  c.unit=battleRegistryUnit(battle,int32_t(c.slot),s.actors);
  if(c.unit){
   c.store(c.unit+0x27c,c.word(c.unit+0x27c)&0x80002000u);
   c.store(c.unit+0x264,0);c.storeByte(c.unit+0x270,0);c.storeHalf(c.unit+0x260,0);c.storeByte(c.unit+0x273,0);
   c.store(c.unit+0x268,0);c.storeByte(c.unit+0x271,0);c.store(c.unit+0x26c,0);c.storeByte(c.unit+0x272,0);c.store(c.unit+0x280,0);
  }
  ++c.slot;break;
 case Step::Select:
  if(c.slot==64){c.step=Step::SelectCommit;break;}
  c.id=c.word(battle+0x120+4*c.slot);
  if(c.id==0xffffffffu || !(c.unit=c.lookup(c.id)) || s8(c.byte(c.unit+0x22))<=0){++c.slot;break;}
  c.capturedState=s8(c.byte(c.unit+0x20));if(c.capturedState==10){++c.slot;break;}
  c.step=Step::SelectStatus;break;
 case Step::SelectStatus:{
  const auto status=c.call(0x80127814,[&](BattleMoveRequest& q){q.integers={c.unit,27,0,0,0,0,0,0};});if(!status)return false;
  if(*status && !(c.word(c.unit+0x104)&0x10000)){c.next(Step::Select);break;}
  // Source state was captured BEFORE status service, not reloaded afterwards.
  if(c.capturedState==1){c.storeByte(c.unit+0x20,0);c.next(Step::Select);break;}
  c.step=Step::SelectCommit;break;
 }
 // Source assumes a valid selected actor, including the all64-skipped path.
 // Commit ID to B420 and lookup again before U27c OR1000. Do not fabricate
 // a no-actor success or silently make unchecked guest0 unavailable.
 case Step::SelectCommit:
  c.store(battle+0x420,c.id);c.unit=c.lookup(c.id);c.store(c.unit+0x27c,c.word(c.unit+0x27c)|0x1000);
  c.inc(5);c.set(6,0x06000000);c.slot=0;c.step=Step::Deferred;break;
 case Step::Deferred:
  if(c.slot==64){c.id=c.word(battle+0x420);c.unit=c.lookup(c.id);c.step=Step::CommandFlags;break;}
  c.id=c.word(battle+0x120+4*c.slot);
  if(c.id!=0xffffffffu){c.unit=c.lookup(c.id);if(c.unit && c.byte(c.unit+0x20)==1)c.storeByte(c.unit+0x20,0);}
  ++c.slot;break;
 // Mario222: set Bef4 bits600,then independently scan200 signed pouch halfs
 // at38a for badge IDs240..246 /247..253. Boots98/hammer99 signed level>=2
 // also clear200/400. These are source availability tests,not native inventory.
 case Step::CommandFlags:
  if(c.word(c.unit+8)==222){
   c.pouch=c.word(0x8041eb00);c.store(battle+0xef4,c.word(battle+0xef4)|0x600);
   for(uint32_t i=0;i<200;++i){const auto item=s16(c.half(c.pouch+0x38a+2*i));if(item>=240 && item<247){c.store(battle+0xef4,c.word(battle+0xef4)&~0x200u);break;}}
   if(s8(c.byte(c.pouch+0x98))>=2)c.store(battle+0xef4,c.word(battle+0xef4)&~0x200u);
   for(uint32_t i=0;i<200;++i){const auto item=s16(c.half(c.pouch+0x38a+2*i));if(item>=247 && item<254){c.store(battle+0xef4,c.word(battle+0xef4)&~0x400u);break;}}
   if(s8(c.byte(c.pouch+0x99))>=2)c.store(battle+0xef4,c.word(battle+0xef4)&~0x400u);
  }
  // Actual8011c058/8011c11c branch to common tail, immediate=0. State8
  // returns with Move9; it does NOT fall through into Act this invocation.
  return finish();
 case Step::Act:
  if(!c.battleCall(0x8011b2dc))return false;
  return finish();
 case Step::Cleanup:
  if(c.slot==64){c.step=Step::PhaseEnd;break;}
  c.unit=battleRegistryUnit(battle,int32_t(c.slot),s.actors);
  if(!c.unit){++c.slot;break;}
  c.step=Step::CleanupUnit;break;
 case Step::CleanupUnit:
  if(!c.call(0x80127434,[&](BattleMoveRequest& q){q.integers={c.unit,0,0,0,0,0,0,0};}))return false;
  c.store(c.unit+0x27c,c.word(c.unit+0x27c)&~0x1000u);c.next(Step::Cleanup);break;
 // Actual phase-end0 and no force flag2000 returns Move8. Otherwise clear
 // Majinai byte18ff9 only if2,call audience per-phase,increment5 and return.
 // Reaction later tests lowbyte only; MsgMain and AnnounceMain both always run.
 case Step::PhaseEnd:{
  const auto ended=c.call(0x8011bb88);if(!ended)return false;
  const auto flags=c.word(battle+0xef4);
  if(!*ended && !(flags&0x2000)){c.set(5,0x05000008);return finish();}
  if(c.byte(battle+0x18ff9)==2)c.storeByte(battle+0x18ff9,0);
  c.step=Step::PerPhase;break;
 }
 case Step::PerPhase:
  if(!c.call(0x801a7c68,[&](BattleMoveRequest& q){q.integers[0]=c.sequence(4);} ))return false;
  c.inc(5);return finish();
 case Step::Reaction:{
  const auto active=c.call(0x801a6ce8);if(!active)return false;
  if(*active&0xff)return finish();
  c.step=Step::Wait;break;
 }
 case Step::Message:{
  const auto message=c.battleCall(0x801956f8);if(!message)return false;c.message=*message;c.step=Step::Announce;break;
 }
 case Step::Announce:{
  const auto announce=c.battleCall(0x80195af8);if(!announce)return false;
  if(!(c.message|*announce)){c.inc(4);c.set(5,0x05000000);}return finish();
 }
 }
}
struct BattlePhaseCursor::State {uint32_t battle=0;BattleMoveServices services;bool move=false;BattleMoveCursor child;};
BattlePhaseCursor::BattlePhaseCursor()=default;
BattlePhaseCursor::~BattlePhaseCursor()=default;
bool BattlePhaseCursor::active()const{return bool(state_);}
uint32_t BattlePhaseCursor::pending()const{return state_?state_->child.pending():0;}
bool BattlePhaseCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Capture Phase gate once:04000000 increments4/resets5;01..05 own real
// Move;04000006 increments3/resets4. Other values do nothing. A held child
// cannot retest this gate or advance a simulation/render clock.
bool resumePackedBattlePhase(uint32_t battle,const BattleMoveServices& s,BattlePhaseCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_ || !battle || !m.word || !m.storeWord)return false;
 if(cursor.state_ && (cursor.state_->battle!=battle || !same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){
  cursor.state_=std::make_unique<BattlePhaseCursor::State>();auto& c=*cursor.state_;c.battle=battle;c.services=s;
  const auto phase=packedBattleGetSequence(battle,4,m);
  if(phase==0x04000000){packedBattleIncrementSequence(battle,4,m);packedBattleSetSequence(battle,5,0x05000000,m);}
  else if(phase==0x04000006){packedBattleIncrementSequence(battle,3,m);packedBattleSetSequence(battle,4,0x04000000,m);}
  else if(phase>0x04000000 && phase<0x04000006)c.move=true;
 }
 if(cursor.state_->move && !resumePackedBattleMove(battle,s,cursor.state_->child))return false;
 cursor.state_.reset();return true;
}
}
