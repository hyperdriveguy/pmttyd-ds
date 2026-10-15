#include "battle_weapon_ui_continuation.hpp"
namespace ttyd {
namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
bool same(const BattleWeaponUiServices& a,const BattleWeaponUiServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.allocationCurrent==b.allocationCurrent&&a.request==b.request&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&
 a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&
 x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Running{bool& f;explicit Running(bool& v):f(v){f=true;}~Running(){f=false;}};
enum class Mode{Init,End};
enum class Step{Entry,Free,Allocate,Publish,CommonStores,Unit,Arrays,Rows,Subtype,EndClear};
}
struct BattleWeaponUiCursor::State {
 BattleWeaponUiServices services;Mode mode=Mode::Init;Step step=Step::Entry;
 uint32_t inputBattle=0,inputCursor=0,inputRows=0,inputFlags=0;
 uint32_t battle=0,old=0,workOutput=0,cursorOutput=0,pending=0;
 bool poisoned=false;std::optional<BattleWeaponUiRequest> held;
 bool valid(){if(poisoned)return false;if(!services.allocationCurrent||!services.allocationCurrent(services.user)){poisoned=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 void store(uint32_t a,uint32_t n)const{services.actors.memory.storeWord(services.actors.memory.user,a,n);}
 template<class Build>std::optional<uint32_t> request(uint32_t fn,uint32_t site,uint32_t caller,Build build){
  pending=fn;if(!held){held=BattleWeaponUiRequest{};held->function=fn;held->callsite=site;held->caller=caller;build(*held);}
  if(!services.request)return {};
  const auto result=services.request(services.user,*held);if(!valid())return {};
  if(result){held.reset();pending=0;}return result;
 }
};
BattleWeaponUiCursor::BattleWeaponUiCursor()=default;
BattleWeaponUiCursor::~BattleWeaponUiCursor()=default;
bool BattleWeaponUiCursor::active()const{return bool(state_);}
bool BattleWeaponUiCursor::stale()const{return state_&&state_->poisoned;}
uint32_t BattleWeaponUiCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponUiCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleWeaponUiInit(uint32_t ignoredBattle,uint32_t inputCursor,uint32_t rows,uint32_t flags,const BattleWeaponUiServices& s,BattleWeaponUiCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!s.allocationCurrent||!m.word||!m.storeWord||!m.storeByte)return false;
 if(cursor.state_&&(cursor.state_->mode!=Mode::Init||cursor.state_->inputBattle!=ignoredBattle||cursor.state_->inputCursor!=inputCursor||cursor.state_->inputRows!=rows||cursor.state_->inputFlags!=flags||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponUiCursor::State>();auto& c=*cursor.state_;c.services=s;c.inputBattle=ignoredBattle;c.inputCursor=inputCursor;c.inputRows=rows;c.inputFlags=flags;}
 auto& c=*cursor.state_;
 for(;;){
  if(!c.valid())return false;
  switch(c.step){
  case Step::Entry:
   // Actual parent SP0c/SP08 initialized0 before Common; their native-owned
   // outputs are distinct from the borrowed input weapon cursor.
   c.workOutput=0;c.cursorOutput=0;c.battle=currentBattleAddress(s.actors);c.old=c.word(c.battle+0x1c78);
   c.step=c.old?Step::Free:Step::Allocate;continue;
  case Step::Free:{
   const auto result=c.request(0x8002fdc4,0x800f8920,0x8011797c,[&](auto& q){q.arguments[q.count++]=c.word(0x8041e80c);q.arguments[q.count++]=c.old;});
   if(!result)return false;
   c.step=Step::Allocate;continue;
  }
  case Step::Allocate:{
   const auto result=c.request(0x8002ff58,0x800f8948,0x80117984,[&](auto& q){q.arguments[q.count++]=c.word(0x8041e80c);q.arguments[q.count++]=0x124;});
   if(!result)return false;
   c.store(c.battle+0x1c78,*result);c.step=Step::Publish;continue;
  }
  case Step::Publish:
   c.workOutput=c.word(c.battle+0x1c78);c.step=Step::CommonStores;continue;
  case Step::CommonStores:
   c.store(c.workOutput,c.inputCursor);c.store(c.workOutput+0xc,0);c.store(c.workOutput+0x10,0);
   m.storeByte(m.user,c.workOutput+0x14,0);c.store(c.workOutput+0x18,c.word(c.inputCursor));
   c.step=Step::Unit;continue;
  case Step::Unit:{
   const auto id=sw(c.word(c.battle+0x420));const auto unit=battleRegistryUnit(c.battle,id,s.actors);
   c.cursorOutput=c.battle+(c.word(unit+8)==222?0x1bb4:0x1bc0);c.step=Step::Arrays;continue;
  }
  case Step::Arrays:{
   const auto literal=c.word(0x8042242c);c.store(c.workOutput+0x98,literal);
   for(uint32_t i=0;i<7;++i){c.store(c.workOutput+0xa4+4*i,0);c.store(c.workOutput+0xc0+4*i,1);c.store(c.workOutput+0xdc+4*i,0u-i);c.store(c.workOutput+0xf8+4*i,0);}
   c.step=Step::Rows;continue;
  }
  case Step::Rows:
   c.store(c.workOutput+8,c.inputRows);c.step=Step::Subtype;continue;
  case Step::Subtype:{
   uint32_t index=c.word(c.cursorOutput),ordinal=0;
   for(;;){
    const auto count=c.word(c.cursorOutput+8);if(sw(ordinal)>=sw(count))break;
    ++index;if(sw(index)>=sw(count))index=0;
    const auto row=c.workOutput+4*index;
    c.store(row+0x48,0);c.store(row+0x84,ordinal);c.store(row+0x20,0);
    c.store(row+0x20,c.word(row+0x20)|1);
    const auto freshCount=c.word(c.cursorOutput+8);
    const auto selected=ordinal==freshCount-1;
    c.store(row+0x34,(c.inputFlags&1)?(selected?0:10):(selected?1:11));
    ++ordinal;
   }
   if(!c.valid())return false;
   cursor.state_.reset();return true;
  }
  default:return false;
  }
 }
}
bool resumePackedBattleWeaponUiEnd(uint32_t battle,const BattleWeaponUiServices& s,BattleWeaponUiCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!s.allocationCurrent||!m.word||!m.storeWord)return false;
 if(cursor.state_&&(cursor.state_->mode!=Mode::End||cursor.state_->inputBattle!=battle||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponUiCursor::State>();auto& c=*cursor.state_;c.services=s;c.mode=Mode::End;c.inputBattle=battle;}
 auto& c=*cursor.state_;
 for(;;){
  if(!c.valid())return false;
  switch(c.step){
  case Step::Entry:
   c.old=c.word(c.inputBattle+0x1c78);if(!c.old){if(!c.valid())return false;
   cursor.state_.reset();return true;}
   c.step=Step::Free;continue;
  case Step::Free:{
   const auto result=c.request(0x8002fdc4,0x800f8920,0x8011602c,[&](auto& q){q.arguments[q.count++]=c.word(0x8041e80c);q.arguments[q.count++]=c.old;});
   if(!result)return false;
   c.step=Step::EndClear;continue;
  }
  case Step::EndClear:
   c.store(c.inputBattle+0x1c78,0);if(!c.valid())return false;
   cursor.state_.reset();return true;
  default:return false;
  }
 }
}
}
