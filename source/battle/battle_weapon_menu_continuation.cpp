#include "battle_weapon_menu_continuation.hpp"
// Field offsets and masks in these notes are hex; counts, strides, states
// and icon IDs are decimal. Original addresses remain32-bit source identities.
namespace ttyd {
namespace {
int32_t sw(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}
bool same(const BattleWeaponMenuContinuationServices& a,const BattleWeaponMenuContinuationServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.call==b.call&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&
 a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&
 x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Running{bool& f;explicit Running(bool& v):f(v){f=true;}~Running(){f=false;}};
enum class Mode{Main,Setup,End};
}
struct BattleWeaponMenuContinuationCursor::State {
 BattleWeaponMenuContinuationServices services;Mode mode=Mode::Main;
 uint32_t b=0,type=0,flags=0,m=0,u=0,pc=0,pending=0;int32_t result=0;
 std::optional<uint32_t> privateCursor;
 std::optional<BattleWeaponMenuContinuationRequest> held;
 std::unique_ptr<BattleWeaponMenuContinuationCursor> end;
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void store(uint32_t a,uint32_t n)const{services.actors.memory.storeWord(services.actors.memory.user,a,n);}
 void put(uint32_t a,uint8_t n)const{services.actors.memory.storeByte(services.actors.memory.user,a,n);}
 // Weapon records are28 bytes anchored at M=B171c: +80 weapon,+84 enabled,
 // +98 reason. Cursor+0 selection,+8 count; its output can remain unknown.
 uint32_t row()const{return m+28*word(*privateCursor);}
 bool cursorKnown(){if(privateCursor)return true;pending=0x80124cb8;return false;}
 template<class Build>std::optional<uint32_t> call(uint32_t fn,Build build){
  pending=fn;if(!held){held=BattleWeaponMenuContinuationRequest{};held->function=fn;held->callsite=pc;build(*held);}
  if(!services.call)return {};
  const auto result=services.call(services.user,*held);if(result){held.reset();pending=0;}return result;
 }
 bool finishEnd(){if(!end)end=std::make_unique<BattleWeaponMenuContinuationCursor>();
  const bool done=resumePackedBattleWeaponMenuEnd(b,services,*end);pending=done?0:end->pending();return done;}
};
BattleWeaponMenuContinuationCursor::BattleWeaponMenuContinuationCursor()=default;
BattleWeaponMenuContinuationCursor::~BattleWeaponMenuContinuationCursor()=default;
bool BattleWeaponMenuContinuationCursor::active()const{return bool(state_);}
uint32_t BattleWeaponMenuContinuationCursor::pending()const{return state_?state_->pending:0;}
uint32_t BattleWeaponMenuContinuationCursor::callsite()const{return state_?state_->pc:0;}
bool BattleWeaponMenuContinuationCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<int32_t> resumePackedBattleWeaponMenuMain(uint32_t b,uint32_t type,const BattleWeaponMenuContinuationServices& s,BattleWeaponMenuContinuationCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!m.word||!m.storeWord||!m.storeByte||!s.actors.byte)return {};
 if(cursor.state_&&(cursor.state_->mode!=Mode::Main||cursor.state_->b!=b||cursor.state_->type!=type||!same(cursor.state_->services,s)))return {};
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponMenuContinuationCursor::State>();auto& c=*cursor.state_;c.mode=Mode::Main;c.b=b;c.type=type;c.m=b+0x171c;c.services=s;c.pc=0x80121d2c;}
 auto& c=*cursor.state_;const auto finish=[&](){const auto result=c.result;cursor.state_.reset();return std::optional<int32_t>(result);};
 for(;;)switch(c.pc){
 case 0x80121d2c: {
  auto result=c.call(0x800f88cc,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.word(c.b+0x420);});
  if(!result)return {};
  c.u=*result;{c.pc=0x8012203c;continue;}
 }
 // KeyOK0 still reaches CommonDisplay and completes0 without consuming SP08.
 // Completed Main0 is a simulation return, not a provider suspension.
 case 0x8012203c: {
  auto result=c.call(0x80119df8,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.pc=*result?0x80121d48:0x8012204c;continue;
 }
 case 0x80121d48: {
  auto result=c.call(0x80124cb8,[&](auto& q){q.privateCursorOutput=&c.privateCursor;q.arguments[q.count++]=c.m;q.arguments[q.count++]=c.type;});
  if(!result)return {};
  {c.pc=0x80121d54;continue;}
 }
 case 0x80121d54: {
  auto result=c.call(0x80123db0,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;});
  if(!result)return {};
  if(c.byte(c.b+0x1c74)){c.put(c.b+0x1c74,0);c.store(c.b+0x1c70,0);}{c.pc=0x80121d74;continue;}
 }
 // Confirm trigger100 (or freshly republished pending byte) precedes navigation.
 // Enabled row: sound2520,real End,decision(T,fresh index),target setup,state10,
 // independent fresh friend/enemy-light tests on weapon64 masks200/4110.
 case 0x80121d74: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x100;});
  if(!result)return {};
  c.pc=(*result||c.byte(c.b+0x1c74))?0x80121d8c:0x80121ea8;continue;
 }
 case 0x80121d8c: {
  if(!c.cursorKnown())return {};
  c.pc=c.word(c.row()+0x84)?0x80121dac:0x80121e3c;continue;
 }
 case 0x80121dac: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2520;});
  if(!result)return {};
  {c.pc=0x80121db4;continue;}
 }
 case 0x80121db4: {
  if(!c.finishEnd())return {};
  {c.pc=0x80121dc8;continue;}
 }
 case 0x80121dc8: {
  auto result=c.call(0x80123210,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;q.arguments[q.count++]=c.word(*c.privateCursor);});
  if(!result)return {};
  {c.pc=0x80121dd0;continue;}
 }
 case 0x80121dd0: {
  auto result=c.call(0x80120fe4,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.m,10);{c.pc=0x80121ddc;continue;}
 }
 case 0x80121ddc: {
  const auto weapon=c.word(c.row()+0x80);
  c.pc=(c.word(weapon+0x64)&0x200)?0x80121e00:0x80121dfc;continue;
 }
 case 0x80121dfc: {
  auto result=c.call(0x80138994,[&](auto&){});
  if(!result)return {};
  {c.pc=0x80121e00;continue;}
 }
 case 0x80121e00: {
  const auto weapon=c.word(c.row()+0x80),flags=c.word(weapon+0x64);
  if(!(flags&0x4110)){c.pc=0x80121e30;continue;}
  c.result=1;{c.pc=0x8012204c;continue;}
 }
 case 0x80121e30: {
  auto result=c.call(0x8013898c,[&](auto&){});
  if(!result)return {};
  c.result=1;{c.pc=0x8012204c;continue;}
 }
 case 0x80121e3c: {
  if(!c.byte(c.b+0x1c74)){c.pc=0x80121e50;continue;}
  {c.pc=0x80121e54;continue;}
 }
 case 0x80121e50: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe88;});
  if(!result)return {};
  {c.pc=0x80121e54;continue;}
 }
 // Disabled row clears pending before reason test. Reason0 returns normally;
 // nonzero waits for real End,then writes state8 and rereads reason into M564.
 // Mutation by End remains visible; do not retain the earlier reason word.
 case 0x80121e54: {
  c.put(c.b+0x1c74,0);
  if(c.word(c.row()+0x98)){c.pc=0x80121e7c;continue;}
  {c.pc=0x8012204c;continue;}
 }
 case 0x80121e7c: {
  if(!c.finishEnd())return {};
  c.store(c.m,8);c.result=1;c.store(c.m+0x564,c.word(c.row()+0x98));{c.pc=0x8012204c;continue;}
 }
 // Forward: signed bounds choose Repeat20000 versus Trigger20000 at wrap.
 // Reverse similarly uses10000. A movement reaches CommonDisplay immediately,
 // skipping reverse/cancel/audience; arithmetic reloads index/count after child.
 case 0x80121ea8: {
  if(!c.cursorKnown())return {};
  const auto cursor=*c.privateCursor;const auto count=c.word(cursor+8),index=c.word(cursor);c.pc=sw(index)<sw(count-1)?0x80121ec4:0x80121ef4;continue;
 }
 case 0x80121ec4: {
  auto result=c.call(0x80119f60,[&](auto& q){q.arguments[q.count++]=0x20000;});
  if(!result)return {};
  if(*result){const auto cursor=*c.privateCursor;c.store(cursor,c.word(cursor)+1);{c.pc=0x80121ee8;continue;}}{c.pc=0x80121f20;continue;}
 }
 case 0x80121ee8: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x8012204c;continue;}
 }
 case 0x80121ef4: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x20000;});
  if(!result)return {};
  if(*result){c.store(*c.privateCursor,0);{c.pc=0x80121f14;continue;}}{c.pc=0x80121f20;continue;}
 }
 case 0x80121f14: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x8012204c;continue;}
 }
 case 0x80121f20: {
  c.pc=sw(c.word(*c.privateCursor))>0?0x80121f30:0x80121f60;continue;
 }
 case 0x80121f30: {
  auto result=c.call(0x80119f60,[&](auto& q){q.arguments[q.count++]=0x10000;});
  if(!result)return {};
  if(*result){const auto cursor=*c.privateCursor;c.store(cursor,c.word(cursor)-1);{c.pc=0x80121f54;continue;}}{c.pc=0x80121f90;continue;}
 }
 case 0x80121f54: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x8012204c;continue;}
 }
 case 0x80121f60: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x10000;});
  if(!result)return {};
  if(*result){const auto cursor=*c.privateCursor;c.store(cursor,c.word(cursor+8)-1);{c.pc=0x80121f84;continue;}}{c.pc=0x80121f90;continue;}
 }
 case 0x80121f84: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x8012204c;continue;}
 }
 // Cancel200 uses retained type2, fresh U2f3/2f4 and signed mode U20.
 // Special mode0 -> sound2521/state15; nonzero -> state12 without that sound;
 // ordinary -> sound2521/state1. No cancel then real physical-X audience child.
 case 0x80121f90: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x200;});
  if(!result)return {};
  if(!*result){c.pc=0x80122018;continue;}
  bool special=false;if(c.type==2){special=c.byte(c.u+0x2f3)!=0;if(!special)special=c.byte(c.u+0x2f4)!=0;}
  if(!special){c.pc=0x80121ffc;continue;}
  c.pc=sb(c.byte(c.u+0x20))==0?0x80121fcc:0x80121fe8;continue;
 }
 case 0x80121fcc: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2521;});
  if(!result)return {};
  {c.pc=0x80121fd4;continue;}
 }
 case 0x80121fd4: {
  if(!c.finishEnd())return {};
  c.store(c.m,15);c.result=1;{c.pc=0x8012204c;continue;}
 }
 case 0x80121fe8: {
  if(!c.finishEnd())return {};
  c.store(c.m,12);c.result=1;{c.pc=0x8012204c;continue;}
 }
 case 0x80121ffc: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2521;});
  if(!result)return {};
  {c.pc=0x80122004;continue;}
 }
 case 0x80122004: {
  if(!c.finishEnd())return {};
  c.store(c.m,1);c.result=1;{c.pc=0x8012204c;continue;}
 }
 case 0x80122018: {
  auto result=c.call(0x8011f524,[&](auto&){});
  if(!result)return {};
  c.pc=*result?0x80122028:0x8012204c;continue;
 }
 // Audience interruption waits for real End,then marks B1c74=1 while result
 // stays0. Every ordinary path still calls actual CommonDisplay once.
 case 0x80122028: {
  if(!c.finishEnd())return {};
  c.put(c.b+0x1c74,1);{c.pc=0x8012204c;continue;}
 }
 case 0x8012204c: {
  auto result=c.call(0x8011606c,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  return finish();
 }
 default:return {};
 }
}
// Whole Table -> private Cursor -> UIInit,then fresh M4 OR01000010 only
// after Init acknowledgment. No table/UI fallback or fabricated readiness.
bool resumePackedBattleWeaponMenuSetup(uint32_t b,uint32_t type,uint32_t flags,const BattleWeaponMenuContinuationServices& s,BattleWeaponMenuContinuationCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!m.word||!m.storeWord)return false;
 if(cursor.state_&&(cursor.state_->mode!=Mode::Setup||cursor.state_->b!=b||cursor.state_->type!=type||cursor.state_->flags!=flags||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponMenuContinuationCursor::State>();auto& c=*cursor.state_;c.mode=Mode::Setup;c.b=b;c.type=type;c.flags=flags;c.m=b+0x171c;c.services=s;c.pc=0x80122088;}
 auto& c=*cursor.state_;
 for(;;)switch(c.pc){
 case 0x80122088:{
  const auto value=c.call(0x80123ec0,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;});if(!value)return false;
  c.pc=0x80122098;continue;
 }
 case 0x80122098:{
  const auto value=c.call(0x80124cb8,[&](auto& q){q.arguments[q.count++]=c.m;q.arguments[q.count++]=c.type;q.privateCursorOutput=&c.privateCursor;});if(!value)return false;
  c.pc=0x8012209c;continue;
 }
 case 0x8012209c:
  if(!c.cursorKnown())return false;
  c.pc=0x801220ac;continue;
 case 0x801220ac:{
  const auto value=c.call(0x80116070,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=*c.privateCursor;q.arguments[q.count++]=c.m+0x80;q.arguments[q.count++]=c.flags;});if(!value)return false;
  c.store(c.m+4,c.word(c.m+4)|0x01000010);cursor.state_.reset();return true;
 }
 default:return false;
 }
}
// Real UIEnd finishes BEFORE fresh B1720 bit10 clear. The nested Main End
// shares this owner so a held free cannot repeat earlier sounds/decisions.
bool resumePackedBattleWeaponMenuEnd(uint32_t b,const BattleWeaponMenuContinuationServices& s,BattleWeaponMenuContinuationCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!m.word||!m.storeWord)return false;
 if(cursor.state_&&(cursor.state_->mode!=Mode::End||cursor.state_->b!=b||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponMenuContinuationCursor::State>();auto& c=*cursor.state_;c.mode=Mode::End;c.b=b;c.services=s;c.pc=0x80121cc4;}
 auto& c=*cursor.state_;
 const auto value=c.call(0x8011600c,[&](auto& q){q.arguments[q.count++]=c.b;});if(!value)return false;
 c.store(c.b+0x1720,c.word(c.b+0x1720)&~0x10u);cursor.state_.reset();return true;
}
}
