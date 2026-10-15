#include "battle_menu_audience_native.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_audience_item_on_native.hpp"
namespace ttyd {
namespace {
bool same(const BattleMenuAudienceNativeServices& a,const BattleMenuAudienceNativeServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user && a.allocationCurrent==b.allocationCurrent && a.trigger==b.trigger &&
 a.mario==b.mario && a.endActMenu==b.endActMenu && a.actors.user==b.actors.user &&
 a.actors.byte==b.actors.byte && a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user &&
 a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event &&
 x.user==y.user && x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
 x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running {bool& v;explicit Running(bool& f):v(f){v=true;}~Running(){v=false;}};

}
struct BattleMenuAudienceNativeCursor::State {
 BattleMenuAudienceNativeServices services;
 uint32_t phase=0,battle=0,unit=0,pending=0x8011f524;int32_t selected=0;bool poisoned=false;
 bool valid(){if(poisoned)return false;if(!services.allocationCurrent || !services.allocationCurrent(services.user)){poisoned=true;return false;}return true;}
};
BattleMenuAudienceNativeCursor::BattleMenuAudienceNativeCursor()=default;
BattleMenuAudienceNativeCursor::~BattleMenuAudienceNativeCursor()=default;
bool BattleMenuAudienceNativeCursor::active()const{return bool(state_);}
bool BattleMenuAudienceNativeCursor::stale()const{return state_ && state_->poisoned;}
uint32_t BattleMenuAudienceNativeCursor::pending()const{return state_?state_->pending:0;}
bool BattleMenuAudienceNativeCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<bool> resumeNativeBattleMenuAudience(const BattleMenuAudienceNativeServices& s,BattleMenuAudienceNativeCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_ || !m.word || !m.half || !m.storeWord || !m.storeHalf || !m.storeByte || !s.actors.byte)return {};
 if(cursor.state_ && !same(cursor.state_->services,s))return {};
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleMenuAudienceNativeCursor::State>();cursor.state_->services=s;}
 auto& c=*cursor.state_;
 const auto finish=[&](bool value)->std::optional<bool>{if(!c.valid())return {};cursor.state_.reset();return value;};
 for(;;){
  if(!c.valid())return {};
  switch(c.phase){
  case 0:c.battle=currentBattleAddress(s.actors);c.phase=1;break;
  case 1:{
   c.pending=0x8011a06c;if(!s.trigger)return {};
   const auto trigger=s.trigger(s.user,0x400);if(!c.valid() || !trigger)return {};
   if(!*trigger)return finish(false);
   if(m.word(m.user,c.battle+0xef4)&0x40000000u)return finish(false);
   c.phase=2;break;}
  case 2:{
   c.pending=0x800f87a8;if(!s.mario)return {};
   const auto unit=s.mario(s.user,c.battle);if(!c.valid() || !unit)return {};
   c.unit=*unit;c.phase=3;break;}
  case 3:
   if(m.word(m.user,c.unit+0x104)&0x10)return finish(false);
   c.selected=nativeBattleAudienceItemOn(s.actors,BattleAudiencePrivateOutputs::Index).index;
   if(c.selected==-1)return finish(false);
   // GetItemOn just published the original selected slot. Preserve the native
   // private result independently of any later cache/global changes.
   c.phase=4;break;
  case 4:
   c.pending=0x801a2bec;packedBattleAudienceSetTarget(c.selected,s.actors);
   c.phase=5;break;
  case 5:{
   c.pending=0x801220d4;if(!s.endActMenu)return {};
   const auto complete=s.endActMenu(s.user,c.battle);
   if(!c.valid() || !complete)return {};
   c.phase=6;break;}
  case 6:{
   const auto previous=m.word(m.user,c.battle+0x1c5c);
   m.storeWord(m.user,c.battle+0x1c60,previous);
   m.storeWord(m.user,c.battle+0x1c5c,11);
   m.storeWord(m.user,c.battle+0x171c,29);
   // Original BattleStageOffLightInCommand80138998/4 is return-only.
   m.storeWord(m.user,c.battle+0xef4,m.word(m.user,c.battle+0xef4)|4u);
   return finish(true);}
  default:return {};
  }
 }
}
}
