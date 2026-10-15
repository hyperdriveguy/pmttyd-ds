#include "battle_turn_continuation.hpp"
#include "battle_sequence_startup.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
int32_t s16(uint16_t n){return n<0x8000?int32_t(n):int32_t(n)-65536;}
int32_t s32(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t s8(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
// Pin callbacks/users across holds, including otherwise unused memory hooks.
// This comparison does not include audienceLifecycle; caller still owns all
// service lifetimes and cannot infer allocation validity from pointer equality.
bool same(const BattleTurnServices& a,const BattleTurnServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user && a.call==b.call && a.actors.user==b.actors.user && a.actors.byte==b.actors.byte &&
 x.user==y.user && x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
 x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running{bool& flag;Running(bool& f):flag(f){flag=true;}~Running(){flag=false;}};
// Native resumable program counter, separate from the original sequence bank.
// A child hold keeps this Step and captured arguments; source recursive dispatch
// runs immediately and does not add a simulation tick or rendering delay.
enum class Step{Dispatch,First,Intro,Timer,Button,IntroDisplay,Wait,Concluded,Scan,Eligible,GetStatus,
 Shadow,Damage,Recovery,ResetMove,Event,RestSound,FinishScan,FreezePosition,FreezeHeight,FreezeEffect,FreezeSound,
 Heal,Body,Part,Position,Height,Effect,HappyTrials,HappyDraw,BurnEntry,BurnPoll,Phase,Audience};
}
struct BattleTurnCursor::State {
 BattleTurnServices services;uint32_t battle=0,pending=0,index=0,slot=0,unit=0,part=0;
 // changed controls whether this source pass needs its odd wait state;
 // aggregate collects recovery returns for one REST sound. Neither is a unit
 // snapshot. held preserves original request operands across unavailable calls.
 Step step=Step::Dispatch;bool changed=false,burnActive=false,burnRegistering=false,hpBookkeeping=false;uint32_t aggregate=0,eventScript=0;
 int32_t amount=0,height=0,trials=0,successes=0,threshold=0;std::array<float,3> position{};
 std::optional<BattleTurnRequest> held;
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return services.actors.memory.half(services.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void store(uint32_t a,uint32_t v)const{services.actors.memory.storeWord(services.actors.memory.user,a,v);}
 void storeHalf(uint32_t a,uint16_t v)const{services.actors.memory.storeHalf(services.actors.memory.user,a,v);}
 // Source sequence channel3 is Turn (base 0x03000000). skip consumes an
 // action/wait pair; recurse re-enters Dispatch within the SAME invocation.
 void inc(){packedBattleIncrementSequence(battle,3,services.actors.memory);}
 void set(uint32_t channel,uint32_t value){packedBattleSetSequence(battle,channel,value,services.actors.memory);}
 // Argument construction happens once, even for source reads in the builder.
 template<class Build> std::optional<BattleTurnOutput> call(uint32_t fn,Build build){
  pending=fn;if(!held){held=BattleTurnRequest{};held->function=fn;build(*held);}
  if(!services.call)return {};
  auto out=services.call(services.user,*held);if(out){held.reset();pending=0;}return out;
 }
 std::optional<BattleTurnOutput> call(uint32_t fn){return call(fn,[](BattleTurnRequest&){});}
 std::optional<BattleTurnOutput> actor(uint32_t fn){return call(fn,[&](BattleTurnRequest& r){r.integers[0]=unit;});}
 void scan(){slot=0;unit=0;step=Step::Scan;}
 void next(){++slot;unit=0;step=Step::Scan;}
 void recurse(){step=Step::Dispatch;}
 void skip(){inc();inc();recurse();}
 // Even Turn indexes: 6 Poison, 8 Burn detection, 9 Burn entry/poll;
 // 0xa/0xc HP/FP regeneration, 0xe/0x10 Happy HP/FP. Recovery spans
 // 0x12..0x3e with one odd wait after each status, in the original table order.
 bool recovery()const{return index>=0x12 && index<=0x3e;}
 bool regen()const{return index==0xa || index==0xc;}
 bool happy()const{return index==0xe || index==0x10;}
 bool flower()const{return index==0xc || index==0x10;}
 int32_t status()const{
  // Original status IDs in recovery order, not a contiguous enum: Sleep,
  // Stop, Dizzy, Confuse, Electric, Dodgy, Burn, Freeze, Huge, Tiny, Poison,
  // Attack Up/Down, Defense Up/Down, Allergic, HP/FP Regen, Invisible,
  // Fast, Slow, Payback, Hold Fast. Preserve pair indexing and selector values.
  static constexpr int32_t ids[]={1,2,3,5,6,7,8,9,10,11,4,12,13,14,15,0,23,24,18,19,20,21,22};
  return recovery()?ids[(index-0x12)/2]:index==6?4:index==8 || index==9?8:index==0xa?23:24;
 }
 float signedFloat(int32_t n)const{
  // Original xoris/lfd/fsubs bias; keep actual source bias reads and binary32 rounding.
  const uint64_t bits=0x4330000000000000ULL|uint64_t(uint32_t(n)^0x80000000u);
  const auto high=word(0x802efb78),low=word(0x802efb7c);
  const uint64_t bias=(uint64_t(high)<<32)|low;
  double d,b;std::memcpy(&d,&bits,8);std::memcpy(&b,&bias,8);return float(d-b);
 }
};
BattleTurnCursor::BattleTurnCursor()=default;
BattleTurnCursor::~BattleTurnCursor()=default;
bool BattleTurnCursor::active()const{return bool(state_);}
uint32_t BattleTurnCursor::pending()const{return state_?state_->pending:0;}
bool BattleTurnCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleTurn(uint32_t battle,const BattleTurnServices& services,BattleTurnCursor& cursor){
 const auto& m=services.actors.memory;
 if(cursor.invoking_ || !battle || !m.word || !m.half || !m.storeWord || !m.storeHalf || !services.actors.byte)return false;
 if(cursor.state_ && (cursor.state_->battle!=battle || !same(cursor.state_->services,services)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleTurnCursor::State>();cursor.state_->services=services;cursor.state_->battle=battle;}
 auto& c=*cursor.state_;const auto finish=[&](){cursor.state_.reset();return true;};
 for(;;)switch(c.step){
 // Bank subtraction is unsigned: values outside original 78 entries finish.
 // Reset per-pass flags on recursive dispatch, never while resuming a child.
 case Step::Dispatch:
  c.changed=false;c.aggregate=0;c.burnActive=false;c.burnRegistering=false;c.hpBookkeeping=false;c.index=packedBattleGetSequence(battle,3,m)-0x03000000u;
  if(c.index>0x4d)return finish();
  switch(c.index){
  case 0:c.inc();return finish();
  case 1:c.step=Step::First;break;
  case 2:case 5:case 0x4a:c.inc();c.recurse();break;
  case 3:c.step=Step::Intro;break;
  case 4:c.step=Step::Timer;break;
  // B+2 Burn substate: 0 starts registration (store1 before child holds),
  // 1 polls all nonnull units, later values use ordinary event wait. Registration
  // is followed by polling in this invocation; active burns yield without inc.
  case 9:{
   const auto substate=c.half(battle+2);
   if(substate==0){c.storeHalf(battle+2,1);c.burnRegistering=true;c.scan();}
   else if(substate==1){c.slot=0;c.step=Step::Scan;}
   else c.step=Step::Wait;
   break;}
  case 0xe:case 0x10:if(s16(c.half(battle))<=1){c.skip();break;}c.scan();break;
  // Three battle countdown words are at +0x19050/+0x19054/+0x19058;
  // decrement nonzero raw words once. Unit passes 0x46/0x48 instead update
  // eligible units' unsigned halfwords +0x13c/+0x13e. No host timer is added.
  case 0x40:case 0x42:case 0x44:{const auto a=battle+0x19050+2*(c.index-0x40);const auto timer=c.word(a);if(timer)c.store(a,timer-1);c.skip();break;}
  case 0x46:case 0x48:c.scan();break;
  case 0x4b:c.inc();c.set(4,0x04000000);return finish();
  case 0x4c:c.step=Step::Phase;break;
  case 0x4d:c.step=Step::Audience;break;
  default:if(c.index&1)c.step=Step::Wait;else c.scan();break;
  }break;
 case Step::First:
  if(!c.call(0x8011b1ec,[&](BattleTurnRequest& r){r.integers[0]=battle;}))return false;
  c.inc();c.recurse();break;
 // First-turn info byte +0x18 enables the 120-invocation intro countdown
 // B+0xefc. Timer decrements before signed expiry test; physical mask0x300
 // can advance early. Display queues camera8/class1 callback0x8012c350 at
 // original float priority0x80422594, then yields with the bank unchanged.
 case Step::Intro:
  if(s16(c.half(battle))!=1 || c.byte(c.word(battle+0x2738)+0x18)==0){c.skip();break;}
  c.store(battle+0xefc,120);c.inc();c.recurse();break;
 case Step::Timer:{const auto timer=c.word(battle+0xefc);if(timer)c.store(battle+0xefc,timer-1);
  if(s32(c.word(battle+0xefc))<=0){c.inc();return finish();}c.step=Step::Button;break;}
 case Step::Button:{auto out=c.call(0x8011a06c,[](BattleTurnRequest& r){r.integers[0]=0x300;});if(!out)return false;
  if(out->value){c.inc();return finish();}c.step=Step::IntroDisplay;break;}
 case Step::IntroDisplay:
  if(!c.call(0x80010bc4,[&](BattleTurnRequest& r){r.integers={8,1,0x8012c350,0};r.reals[0]=scalar(c.word(0x80422594));}))return false;
  return finish();
 // Engaged barrier0 completes this invocation without bank advancement;
 // nullopt retains the native child. Ready1 then checks real conclusion, writing
 // channel0=3 and end channel7=0x07000000 if concluded, otherwise inc once.
 case Step::Wait:{auto out=c.call(0x8011ace8,[&](BattleTurnRequest& r){r.integers[0]=battle;});if(!out)return false;
  if(!out->value)return finish();
  c.step=Step::Concluded;break;}
 case Step::Concluded:{auto out=c.call(0x8011aa34,[&](BattleTurnRequest& r){r.integers[0]=battle;});if(!out)return false;
  if(out->value){c.set(0,3);c.set(7,0x07000000);}else c.inc();return finish();}
 // Visit all 64 registry slots. Recovery is selected BEFORE the ordinary
 // null/status27/HP eligibility checks: earlier recovery indexes call even for
 // null units, while >=0x32 explicitly skip nulls. Do not unify these filters.
 case Step::Scan:
  if(c.slot==64){
   if(c.index==9){if(c.burnRegistering){c.burnRegistering=false;c.slot=0;c.step=Step::Scan;break;}if(c.burnActive)return finish();c.storeHalf(battle+2,uint16_t(c.half(battle+2)+1));return finish();}
   if(c.recovery() && c.index!=0x20 && c.aggregate){c.step=Step::RestSound;break;}
   c.step=Step::FinishScan;break;
  }
  c.unit=battleRegistryUnit(battle,int32_t(c.slot),services.actors);
  if(c.index==9 && !c.burnRegistering){if(c.unit)c.step=Step::BurnPoll;else c.next();break;}
  if(c.recovery()){
   if(c.index>=0x32 && !c.unit){c.next();break;}
   c.step=Step::Recovery;break;
  }
  if(!c.unit){c.next();break;}c.step=Step::Eligible;break;
 // CheckStatus27 precedes the fresh signed HP read. Happy counts come
 // from badge bytes +0x2ee/+0x2ef, not a synthesized badge/equipment snapshot.
 case Step::Eligible:{auto out=c.call(0x80127814,[&](BattleTurnRequest& r){r.integers={c.unit,27};});if(!out)return false;
  if(out->value || s16(c.half(c.unit+0x10c))<=0){c.next();break;}
  if(c.index==0x46 || c.index==0x48){const auto a=c.unit+(c.index==0x46?0x13c:0x13e);const auto n=c.half(a);if(n)c.storeHalf(a,uint16_t(n-1));c.next();break;}
  if(c.happy()){c.trials=c.byte(c.unit+(c.index==0xe?0x2ee:0x2ef));c.successes=0;c.step=Step::HappyTrials;break;}
  c.step=Step::GetStatus;break;}
 // Only positive signed strength reaches periodic work. Burn detection
 // resets B+2 and stops scanning for this pass. HP regeneration bookkeeping is
 // gated by unit+0x104 mask0x40000000 before the resource child executes.
 case Step::GetStatus:{auto out=c.call(0x80127f34,[&](BattleTurnRequest& r){r.integers={c.unit,uint32_t(c.status())};});if(!out)return false;
  c.amount=s8(out->strength);if(c.amount<=0){c.next();break;}
  if(c.index==6){c.step=Step::Shadow;break;}
  if(c.index==8){c.changed=true;c.storeHalf(battle+2,0);c.step=Step::FinishScan;break;}
  if(c.index==9){c.step=Step::BurnEntry;break;}
  c.hpBookkeeping=c.index==0xa && !(c.word(c.unit+0x104)&0x40000000u);
  if(c.hpBookkeeping)c.store(c.unit+0x208,uint32_t(s16(c.half(c.unit+0x10c))));
  c.step=Step::Heal;break;}
 // Recovery0 may still have mutated status; it is an acknowledged return.
 // Freeze pass0x20 has special damage/effect/audio and no aggregate REST sound.
 // Slow pass0x3a resets move status before creating the recovery event.
 case Step::Recovery:{auto out=c.call(0x80127890,[&](BattleTurnRequest& r){r.integers={c.unit,uint32_t(c.status())};});if(!out)return false;
  if(c.index!=0x20)c.aggregate|=out->value;
  if(!out->value){c.next();break;}
  if(c.index==0x20){c.step=Step::Shadow;break;}
  c.eventScript=0x80353704;c.step=c.index==0x3a?Step::ResetMove:Step::Event;break;}
 case Step::ResetMove:if(!c.actor(0x801266d4))return false;c.step=Step::Event;break;
 // Shadow child0x80125d08 nonzero skips Poison damage; Freeze still
 // proceeds to its position/effect/sound. Ordinary DirectDamage uses actor-5,
 // part0, FP0, source codes279 (Poison)/282 (Freeze), pattern0/recovery1.
 case Step::Shadow:{auto out=c.actor(0x80125d08);if(!out)return false;
  if(!out->value){if(c.index==6)c.changed=true;c.step=Step::Damage;break;}
  if(c.index==6)c.next();else c.step=Step::FreezePosition;break;}
 case Step::Damage:
  if(!c.call(0x800fdfd4,[&](BattleTurnRequest& r){r.integers={uint32_t(-5),c.unit,0,uint32_t(c.index==6?c.amount:1),0,c.index==6?279u:282u,0,1};}))return false;
  if(c.index==6)c.next();else c.step=Step::FreezePosition;break;
 // Freeze presentation uses actual unit position plus signed source height,
 // original scale literal0x8042256c and positional key0x802efb94. Position
 // remains private typed output; no temporary guest stack addresses are minted.
 case Step::FreezePosition:{auto out=c.actor(0x80128c7c);if(!out)return false;c.position=out->position;c.step=Step::FreezeHeight;break;}
 case Step::FreezeHeight:{auto out=c.actor(0x80128414);if(!out)return false;c.position[1]+=c.signedFloat(s32(out->value));c.step=Step::FreezeEffect;break;}
 case Step::FreezeEffect:
  if(!c.call(0x801c3690,[&](BattleTurnRequest& r){r.integers={0,1};r.reals={c.position[0],c.position[1],c.position[2],scalar(c.word(0x8042256c))};}))return false;
  c.step=Step::FreezeSound;break;
 case Step::FreezeSound:
  if(!c.call(0x800d9848,[&](BattleTurnRequest& r){r.integers[0]=0x802efb94;r.reals={c.position[0],c.position[1],c.position[2]};}))return false;
  c.changed=true;c.next();break;
 // evtEntry(script, priority10, flags0) returns a GUEST ENTRY POINTER,
 // not its ID. Engaged0 is allocation failure but still marks this pass changed.
 // On success read fresh entry+0x15c ID into unit+0x2a8, then unit ID into
 // entry+0x160. A hold must not replay earlier damage/resource/effect work.
 case Step::Event:{auto out=c.call(0x800374ac,[&](BattleTurnRequest& r){r.integers={c.eventScript,10,0};});if(!out)return false;
  if(out->value){c.store(c.unit+0x2a8,c.word(out->value+0x15c));c.store(out->value+0x160,c.word(c.unit));}
  c.changed=true;c.next();break;}
 case Step::RestSound:
  if(!c.call(0x800d98bc,[](BattleTurnRequest& r){r.integers[0]=0x802efb80;}))return false;
  c.step=Step::FinishScan;break;
 // Unchanged passes skip action+wait immediately. Changed Sleep0x12 and
 // Happy HP0xe yield after inc; other changed cases may dispatch their wait
 // immediately. This boundary is source scheduling, not render-frame timing.
 case Step::FinishScan:
  if(c.index==0x46 || c.index==0x48 || !c.changed){c.skip();break;}
  c.inc();if(c.index==0x12 || c.index==0xe)return finish();c.recurse();break;
 // HP/FP mutation belongs to the actual child. For admitted HP regen,
 // unit+0x208/+0x20c capture before/after HP; +0x200/+0x204 copy +0x202/
 // +0x206, and halfword+0x1fe ORs bit0. Preserve post-child fresh reads.
 case Step::Heal:
  if(!c.call(c.flower()?0x80126b58:0x80126bec,[&](BattleTurnRequest& r){r.integers={c.unit,uint32_t(c.amount)};}))return false;
  if(c.hpBookkeeping){
   c.store(c.unit+0x20c,uint32_t(s16(c.half(c.unit+0x10c))));c.storeHalf(c.unit+0x200,c.half(c.unit+0x202));
   c.storeHalf(c.unit+0x204,c.half(c.unit+0x206));c.storeHalf(c.unit+0x1fe,uint16_t(c.half(c.unit+0x1fe)|1));}
  c.step=Step::Body;break;
 // Body ID -> actual part pointer -> part XYZ -> unit height. Regen adds
 // five to height; Happy does not. Original bias/scale load order and fused
 // multiply-add below are deliberate, not interchangeable host arithmetic.
 case Step::Body:{auto out=c.actor(0x80128c98);if(!out)return false;c.part=out->value;c.step=Step::Part;break;}
 case Step::Part:{auto out=c.call(0x80128cd8,[&](BattleTurnRequest& r){r.integers={c.unit,c.part};});if(!out)return false;c.part=out->value;c.step=Step::Position;break;}
 case Step::Position:{auto out=c.call(0x80128b10,[&](BattleTurnRequest& r){r.integers[0]=c.part;});if(!out)return false;c.position=out->position;c.step=Step::Height;break;}
 case Step::Height:{auto out=c.actor(0x80128414);if(!out)return false;
  c.height=s32(out->value+uint32_t(c.regen()?5:0));
  // Status regeneration loads bias before scale; Happy loads scale first.
  float h,scale;if(c.regen()){h=c.signedFloat(c.height);scale=scalar(c.word(c.unit+0x114));}
  else{scale=scalar(c.word(c.unit+0x114));h=c.signedFloat(c.height);}
  c.position[1]=std::fma(scale,h,c.position[1]);c.step=Step::Effect;break;}
 // Recovery visual kind0=HP, 1=FP. Regeneration uses event0x80416bc0;
 // Happy HP/FP use0x80351a3c/0x80351b10. Allocation/effect zero still completes
 // its child; missing real providers must hold rather than invent that result.
 case Step::Effect:
  if(!c.call(0x8014335c,[&](BattleTurnRequest& r){r.integers={c.flower()?1u:0u,uint32_t(c.amount)};r.reals={c.position[0],c.position[1],c.position[2]};}))return false;
  c.eventScript=c.regen()?0x80416bc0:c.index==0xe?0x80351a3c:0x80351b10;c.step=Step::Event;break;
 // Each badge copy consumes one irand(100). Threshold is exactly
 // 33 - 3 * (signed current / signed maximum), INTEGER division first. Re-read
 // current/max between trials; maximum0 holds at its original function token.
 // Happy HP wraps/narrows the signed sum before upper clamp and bypasses the
 // HP helper; Happy FP uses its resource child. Do not replace with float odds.
 case Step::HappyTrials:
  if(c.trials<=0){
   if(!c.successes){c.next();break;}c.amount=c.successes;
   if(c.flower()){c.step=Step::Heal;break;}
   c.storeHalf(c.unit+0x10c,uint16_t(uint32_t(s16(c.half(c.unit+0x10c)))+uint32_t(c.amount)));
   if(s16(c.half(c.unit+0x10c))>s16(c.half(c.unit+0x108)))c.storeHalf(c.unit+0x10c,c.half(c.unit+0x108));
   c.step=Step::Body;break;
  }else{
   const auto current=s16(c.half(c.unit+(c.flower()?0x112:0x10c))),maximum=s16(c.half(c.unit+(c.flower()?0x10e:0x108)));
   if(!maximum){c.pending=c.flower()?0x8011e098:0x8011dee0;return false;}
   c.threshold=33-3*(current/maximum);c.step=Step::HappyDraw;break;
  }
 case Step::HappyDraw:{auto out=c.call(0x80005da0,[](BattleTurnRequest& r){r.integers[0]=100;});if(!out)return false;
  if(s32(out->value)<c.threshold)++c.successes;
  --c.trials;c.step=Step::HappyTrials;break;}
 // Register eligible burns with child0x80195f38(unit,1), then poll every
 // nonnull unit via0x80195d7c, including units whose eligibility has changed.
 case Step::BurnEntry:
  if(!c.call(0x80195f38,[&](BattleTurnRequest& r){r.integers={c.unit,1};}))return false;
  c.next();break;
 case Step::BurnPoll:{auto out=c.actor(0x80195d7c);if(!out)return false;if(out->value)c.burnActive=true;c.next();break;}
 case Step::Phase:
  if(!c.call(0x8011c2b4,[&](BattleTurnRequest& r){r.integers[0]=battle;}))return false;
  return finish();
 // Only acknowledged audience TurnEnd resets Turn to0x03000001 and clears
 // B+0xef4 bit0x2000. Phase/audience children retain their own actual services;
 // finishing this invocation does not finish a battle or restore the field.
 case Step::Audience:
  if(!c.call(0x8019f298))return false;
  c.set(3,0x03000001);c.store(battle+0xef4,c.word(battle+0xef4)&~0x2000u);return finish();
 }
}
}
