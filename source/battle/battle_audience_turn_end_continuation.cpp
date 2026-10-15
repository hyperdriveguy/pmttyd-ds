#include "battle_audience_turn_end_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t signedHalf(uint16_t n){return n<0x8000?int32_t(n):int32_t(n)-0x10000;}
bool same(const BattleAudienceTurnEndServices& a,const BattleAudienceTurnEndServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user && a.divide==b.divide && a.truncate==b.truncate && a.random==b.random && a.addAudience==b.addAudience &&
 a.actors.user==b.actors.user && a.actors.byte==b.actors.byte && x.user==y.user && x.word==y.word && x.half==y.half &&
 x.storeWord==y.storeWord && x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running {bool& flag;explicit Running(bool& f):flag(f){flag=true;}~Running(){flag=false;}};
}
struct BattleAudienceTurnEndCursor::State {
 BattleAudienceTurnEndServices owner;
 uint32_t phase=0,pending=0,battle=0,mario=0,audience=0,group=0,sets=0,numOwner=0;
 int32_t numerator=0,level=0,threshold=0,target=0;
 float delta=0,bias=0,targetSum=0;
 BattleAudienceTargetCursor targetCursor;
 BattleAudienceAddNumInvocation add;
 uint32_t word(uint32_t a)const{return owner.actors.memory.word(owner.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return owner.actors.memory.half(owner.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return owner.actors.byte(owner.actors.user,a);}
 void store(uint32_t a,uint32_t n)const{owner.actors.memory.storeWord(owner.actors.memory.user,a,n);}
 uint32_t base()const{return currentBattleAddress(owner.actors)+0x2858;}
 void category(int32_t kind){
  const auto a=base(),count=word(a+0x13904);
  for(int32_t i=0;i<signedWord(count);++i)if(word(a+0x138cc+4*uint32_t(i))==uint32_t(kind))return;
  store(a+0x138cc+4*count,uint32_t(kind));store(a+0x13904,word(a+0x13904)+1);
 }
};
BattleAudienceTurnEndCursor::BattleAudienceTurnEndCursor()=default;
BattleAudienceTurnEndCursor::~BattleAudienceTurnEndCursor()=default;
bool BattleAudienceTurnEndCursor::active()const{return bool(state_);}
uint32_t BattleAudienceTurnEndCursor::pending()const{return state_?state_->pending:0;}
bool BattleAudienceTurnEndCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleAudienceTurnEnd(const BattleAudienceTurnEndServices& s,BattleAudienceTurnEndCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_ || !m.word || !m.half || !m.storeWord || !s.actors.byte)return false;
 if(cursor.state_ && !same(cursor.state_->owner,s))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleAudienceTurnEndCursor::State>();cursor.state_->owner=s;}
 auto& c=*cursor.state_;const auto finish=[&](){cursor.state_.reset();return true;};
 for(;;)switch(c.phase){
 case 0:{
  c.battle=currentBattleAddress(s.actors);c.mario=battleMarioUnit(c.battle,s.actors);c.audience=c.base();
  (void)c.word(0x8041eb00); // Original pouchGetPtr call, result unused.
  const auto info=c.word(c.battle+0x2738),flags=c.word(c.audience);
  const auto setup=c.word(info+0xc);c.group=c.word(setup+0x18);c.sets=c.word(c.group+4);
  if(flags&0x40000)return finish();
  c.store(c.audience+0x13908,c.word(c.audience+0x13908)+3);
  if(signedWord(c.word(c.audience+0x13908))>100)c.store(c.audience+0x13908,100);
  const auto count=signedWord(c.word(c.group));uint32_t sum=0;
  for(int32_t i=0;i<count;++i)sum+=c.byte(c.word(c.sets+0x30*uint32_t(i))+0xe);
  c.level=c.byte(c.mario+0xd);c.numerator=signedWord(sum*2);c.phase=1;break;
 }
 case 1:{
  c.pending=0x8019f334;if(!s.divide)return false;
  const auto value=s.divide(s.user,c.numerator,c.level);if(!value)return false;
  c.threshold=*value<5?5:*value;c.pending=0;
  if(signedHalf(c.half(c.battle))<c.threshold){c.phase=3;break;}
  const auto a=c.base();const auto multiplier=scalar(c.word(0x80424984));const auto baseline=scalar(c.word(a+0x13778));
  c.bias=scalar(c.word(0x80424988));c.delta=multiplier*baseline;c.phase=2;break;
 }
 case 2:{
  BattleAudienceTargetContinuationServices target;target.actors=s.actors;target.user=s.user;target.truncate=s.truncate;
  if(!resumePackedBattleAudienceAddTargetNum(c.delta,c.bias,target,c.targetCursor)){
   c.pending=c.targetCursor.pending()?c.targetCursor.pending():0x801a1a74;return false;}
  c.phase=3;c.pending=0;break;
 }
 case 3:
  if(signedHalf(c.half(c.battle))==c.threshold)c.category(6);
  c.phase=4;break;
 case 4:{
  c.pending=0x80005da0;if(!s.random)return false;
  const auto draw=s.random(s.user,100);if(!draw)return false;c.pending=0;
  if(*draw<signedWord(c.word(c.audience+0x13908))){
   if(signedHalf(c.half(c.battle))>=c.threshold)c.category(1);
   for(int32_t category:{4,9,10,11,12,13})c.category(category);
   c.store(c.audience+0x13908,0);
  }
  c.phase=5;break;
 }
 case 5:
  c.numOwner=c.base();if(c.word(c.numOwner)&0x40000)return finish();
  {const auto desired=scalar(c.word(c.numOwner+0x1377c)),offset=scalar(c.word(c.numOwner+0x13780));c.targetSum=desired+offset;}
  c.phase=6;break;
 case 6:{
  c.pending=0x801a1c90;if(!s.truncate)return false;
  const auto value=s.truncate(s.user,c.targetSum);if(!value)return false;
  c.target=*value<0?0:*value;
  const auto capacity=signedWord(c.word(c.numOwner+0x13790));if(c.target>capacity)c.target=capacity;
  // Source NumToTargetSub's first BaseGetPtr result is unused. GetExist has
  // another unused BaseGetPtr, then GetPtr captures fresh global yet again.
  (void)c.base();uint32_t count=0;
  for(uint32_t i=0;i<200;++i){
   (void)c.base();const auto record=c.base()+0x1bc+0x134*i,flags=c.word(record);
   if((flags&1) && !(flags&0x102)){
    const auto stateRecord=c.base()+0x1bc+0x134*i;
    if(c.byte(stateRecord+0x19)!=12)++count;
   }
  }
  c.add.amount=signedWord(uint32_t(c.target)-count);c.pending=0;c.phase=7;break;
 }
 case 7:
  c.add.battle=currentBattleAddress(s.actors);c.add.audience=c.base();c.add.pouch=c.word(0x8041eb00);
  c.add.capacity=c.half(c.add.pouch+0x8c);
  if(!c.add.capacity || !c.add.amount || (c.word(c.add.battle+0xef4)&0x100))return finish();
  c.phase=8;break;
 case 8:
  c.pending=0x801a1708;
  if(!s.addAudience || !s.addAudience(s.user,c.add))return false;
  return finish();
 default:return false;
 }
}
}
