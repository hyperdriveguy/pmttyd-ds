#include "battle_menu_audience_native.hpp"
#include "battle_audience_item_on.hpp"
#include <cstring>
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
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Running {bool& v;explicit Running(bool& f):v(f){v=true;}~Running(){v=false;}};
// Whole GetItemOn801a2d58 specialized ONLY for its checker's original output
// signature (&privateIndex,0,0,0,0). Source cache stores remain guest operations;
// the private index cannot alias them. No output address is manufactured.
int32_t firstEligible(const BattleActorServices& a){
 const auto& m=a.memory;auto word=[&](uint32_t p){return m.word(m.user,p);};
 const auto base=currentBattleAddress(a)+0x2858;
 auto member=currentBattleAddress(a)+0x2858+0x1bc;
 for(uint32_t i=0;i<200;++i,member+=0x134){
  const auto flags=word(member);if(!(flags&1) || (flags&0x100) || (flags&2))continue;
  const auto raw=m.half(m.user,member+0x1c);
  const auto held=raw<0x8000?int32_t(raw):int32_t(raw)-65536;
  if(held==-1)continue;
  const auto item=currentBattleAddress(a)+0x2858+uint32_t(held)*0x48+0xf25c;
  const auto state=signedWord(word(item+4));if(state>4 && state!=7)continue;
  m.storeWord(m.user,base+0x138bc,i);
  // The original next operation writes i to private SP08. Native return holds
  // that private value; subsequent source reads cannot observe this local.
  m.storeWord(m.user,base+0x138c0,word(member+0x48));
  m.storeWord(m.user,base+0x138c4,word(member+0x4c));
  const auto z=scalar(word(member+0x50)),offset=scalar(word(0x804249cc));
  m.storeWord(m.user,base+0x138c8,bits(z+offset));
  return int32_t(i);
 }
 return -1;
}
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
   c.selected=firstEligible(s.actors);
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
