#include "battle_audience_target_continuation.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool same(const BattleActorServices& a,const BattleActorServices& b){
 const auto& x=a.memory;const auto& y=b.memory;
 return a.user==b.user && a.byte==b.byte && x.user==y.user && x.word==y.word && x.half==y.half &&
 x.storeWord==y.storeWord && x.storeHalf==y.storeHalf && x.storeByte==y.storeByte &&
 x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running {bool& flag;explicit Running(bool& f):flag(f){flag=true;}~Running(){flag=false;}};
}
struct BattleAudienceTargetCursor::State {
 BattleAudienceTargetContinuationServices owner;
 bool sub=false;uint32_t deltaBits=0,biasBits=0,phase=0,parent=0,child=0,pending=0;
 float remaining=0,zero=0,chunk=0,desired=0,quotient=0;
};
BattleAudienceTargetCursor::BattleAudienceTargetCursor()=default;
BattleAudienceTargetCursor::~BattleAudienceTargetCursor()=default;
bool BattleAudienceTargetCursor::active()const{return bool(state_);}
uint32_t BattleAudienceTargetCursor::pending()const{return state_?state_->pending:0;}
bool BattleAudienceTargetCursor::reset(){if(invoking_)return false;state_.reset();return true;}
struct BattleAudienceTargetRunner {
 static bool run(float delta,float bias,bool sub,const BattleAudienceTargetContinuationServices& s,BattleAudienceTargetCursor& cursor){
  if(cursor.invoking_ || !s.actors.memory.word || !s.actors.memory.storeWord)return false;
  if(cursor.state_){const auto& c=*cursor.state_;
   if(c.sub!=sub || c.deltaBits!=bits(delta) || c.biasBits!=bits(bias) || c.owner.user!=s.user ||
       c.owner.truncate!=s.truncate || !same(c.owner.actors,s.actors))return false;
  }
  Running running(cursor.invoking_);
  if(!cursor.state_){cursor.state_=std::make_unique<BattleAudienceTargetCursor::State>();auto& c=*cursor.state_;
   c.owner=s;c.sub=sub;c.deltaBits=bits(delta);c.biasBits=bits(bias);c.remaining=delta;c.phase=sub?1:0;}
  auto& c=*cursor.state_;const auto& m=s.actors.memory;
  const auto real=[&](uint32_t a){return scalar(m.word(m.user,a));};
  const auto store=[&](uint32_t a,float f){m.storeWord(m.user,a,bits(f));};
  // Source xoris/43300000, fresh actual binary64 bias, then single fsubs.
  const auto signedFloat=[&](uint32_t n){const auto high=m.word(m.user,0x802fa328),low=m.word(m.user,0x802fa32c);
   const uint64_t biasBits=(uint64_t(high)<<32)|low,insert=(uint64_t(0x43300000)<<32)|(n^0x80000000u);
   double biasValue,value;std::memcpy(&biasValue,&biasBits,8);std::memcpy(&value,&insert,8);return float(value-biasValue);};
  for(;;)switch(c.phase){
  case 0:{
   c.parent=currentBattleAddress(s.actors)+0x2858;
   auto zero=real(0x80424988);float offset=real(c.parent+0x13780)+bias;store(c.parent+0x13780,offset);
   offset=real(c.parent+0x13780);
   if(offset>zero && c.remaining<zero){
    offset=offset+c.remaining;store(c.parent+0x13780,offset);offset=real(c.parent+0x13780);
    if(offset<zero){c.remaining=offset;store(c.parent+0x13780,zero);}else c.remaining=zero;
   }else {
    zero=real(0x80424988);
    if(offset<zero && c.remaining>zero){
     offset=real(c.parent+0x13780)+c.remaining;store(c.parent+0x13780,offset);offset=real(c.parent+0x13780);
     if(offset>zero){c.remaining=offset;store(c.parent+0x13780,zero);}else c.remaining=zero;
    }
   }
   c.phase=1;break;
  }
  case 1:c.child=currentBattleAddress(s.actors)+0x2858;c.zero=real(0x80424988);c.phase=2;break;
  case 2:{
   if(c.remaining==c.zero){c.phase=c.sub?5:6;break;}
   c.pending=0x801a18cc;
   if(!std::isfinite(c.remaining)){c.phase=7;return false;}
   const float before=c.remaining;c.chunk=0;
   if(c.remaining>c.zero){const float one=real(0x80424990);c.chunk=c.remaining<one?c.remaining:one;
    c.remaining=c.remaining-c.chunk;const auto zero=real(0x80424988);if(c.remaining<zero)c.remaining=zero;}
   if(c.remaining<c.zero){const float negative=real(0x804249d4);c.chunk=c.remaining>negative?c.remaining:negative;
    c.remaining=c.remaining-c.chunk;const auto zero=real(0x80424988);if(c.remaining>zero)c.remaining=zero;}
   if(c.remaining==before || !std::isfinite(c.remaining)){c.remaining=before;c.phase=7;return false;}
   if(c.chunk>c.zero){
    const float one=real(0x80424990);c.desired=real(c.child+0x1377c);const float baseline=real(c.child+0x13778),step=real(0x804249ac);
    const float ratio=c.desired/baseline,difference=ratio-one;c.quotient=difference/step;c.phase=3;break;
   }
   const auto zero=real(0x80424988);
   if(c.chunk<zero){
    const float one=real(0x80424990);c.desired=real(c.child+0x1377c);const float baseline=real(c.child+0x13778),step=real(0x804249ac);
    const float ratio=c.desired/baseline,difference=ratio-one;c.quotient=difference/step;c.phase=4;break;
   }
   store(c.child+0x1377c,real(c.child+0x1377c)+c.chunk);c.phase=2;break;
  }
  case 3:case 4:{
   c.pending=c.phase==3?0x801a1984:0x801a19f8;
   if(!s.truncate)return false;
   const auto integer=s.truncate(s.user,c.quotient);if(!integer)return false;
   if((c.phase==3 && *integer>=1) || (c.phase==4 && *integer<=-1)){
    const auto n=uint32_t(*integer)+(c.phase==3?1u:0xffffffffu);const float denominator=signedFloat(n),change=c.chunk/denominator;
    store(c.child+0x1377c,c.phase==3?c.desired+change:c.desired-change);
   }else {
    // Positive unweighted branch reaches the negative test before fresh target.
    // The actual readonly zero is reloaded at801a19cc.
    if(c.phase==3)(void)real(0x80424988);
    store(c.child+0x1377c,real(c.child+0x1377c)+c.chunk);
   }
   c.phase=2;c.pending=0;break;
  }
  case 5:cursor.state_.reset();return true;
  case 6:{
   const auto zero=real(0x80424988);if(real(c.parent+0x1377c)<zero)store(c.parent+0x1377c,zero);
   const auto rawCapacity=m.word(m.user,c.parent+0x13790);
   // Source bias load precedes the fresh target load and fsubs conversion.
   const auto high=m.word(m.user,0x802fa328),low=m.word(m.user,0x802fa32c);
   const auto target=real(c.parent+0x1377c);
   const uint64_t biasBits=(uint64_t(high)<<32)|low,insert=(uint64_t(0x43300000)<<32)|(rawCapacity^0x80000000u);
   double biasValue,value;std::memcpy(&biasValue,&biasBits,8);std::memcpy(&value,&insert,8);const float capacity=float(value-biasValue);
   if(target>capacity)store(c.parent+0x1377c,capacity);
   const auto minimum=real(0x804249d8);if(real(c.parent+0x13780)<minimum)store(c.parent+0x13780,minimum);
   const auto maximum=real(0x804249dc);if(real(c.parent+0x13780)>maximum)store(c.parent+0x13780,maximum);
   cursor.state_.reset();return true;
  }
  case 7:return false;
  default:return false;
  }
 }
};
bool resumePackedBattleAudienceAddTargetNum(float d,float b,const BattleAudienceTargetContinuationServices& s,BattleAudienceTargetCursor& c){return BattleAudienceTargetRunner::run(d,b,false,s,c);}
bool resumePackedBattleAudienceAddTargetNumSub(float d,const BattleAudienceTargetContinuationServices& s,BattleAudienceTargetCursor& c){return BattleAudienceTargetRunner::run(d,0,true,s,c);}
}
