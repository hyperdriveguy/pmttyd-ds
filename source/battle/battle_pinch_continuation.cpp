#include "battle_pinch_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
int32_t signed16(uint16_t n){return n<0x8000?int32_t(n):int32_t(n)-0x10000;}
int32_t signed32(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
bool same(const BattlePinchServices& a,const BattlePinchServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user && a.status==b.status && a.truncate==b.truncate && a.sound==b.sound &&
 a.actors.user==b.actors.user && a.actors.byte==b.actors.byte && x.user==y.user && x.word==y.word && x.half==y.half &&
 x.storeWord==y.storeWord && x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
struct Running {bool& flag;explicit Running(bool& f):flag(f){flag=true;}~Running(){flag=false;}};
}
struct BattlePinchCursor::State {
 BattlePinchServices owner;bool direct=false,adapterCaptured=false;
 uint32_t input=0,phase=0,slot=0,unit=0,unitPhase=0,group=0,pending=0;
 int32_t mode=0;float delta=0,bias=0;
 BattleAudienceTargetCursor target;
 uint32_t word(uint32_t a)const{return owner.actors.memory.word(owner.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return owner.actors.memory.half(owner.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return owner.actors.byte(owner.actors.user,a);}
 void store(uint32_t a,uint32_t n)const{owner.actors.memory.storeWord(owner.actors.memory.user,a,n);}
 void storeByte(uint32_t a,uint8_t n)const{owner.actors.memory.storeByte(owner.actors.memory.user,a,n);}
 bool adapter(bool danger){
  if(!adapterCaptured){
   // Whole BaseGetPtr first; multiplier, baseline, bias then fmuls.
   const auto a=currentBattleAddress(owner.actors)+0x2858;
   const auto multiplier=scalar(word(danger?0x804249a0:0x80424994));
   const auto baseline=scalar(word(a+0x13778));bias=scalar(word(0x80424988));
   delta=baseline*multiplier;adapterCaptured=true;
  }
  BattleAudienceTargetContinuationServices s;s.actors=owner.actors;s.user=owner.user;s.truncate=owner.truncate;
  if(!resumePackedBattleAudienceAddTargetNum(delta,bias,s,target)){
   pending=target.pending()?target.pending():0x801a1a74;return false;}
  adapterCaptured=false;pending=0;return true;
 }
 bool updateUnit(){
  for(;;)switch(unitPhase){
  case 0:{
   const auto definition=word(unit+0x10);const auto hp=signed16(half(unit+0x10c));const auto threshold=byte(definition+0xd);
   if(hp<=threshold && hp<signed16(half(unit+0x108))){
    const auto flags=word(unit+0x138);
    if(!(flags&0x20000000u)){
     store(unit+0x138,flags|0x20000000u);
     if(!mode && word(unit+8)==222){unitPhase=1;break;}
    }
   }else {const auto flags=word(unit+0x138);if(flags&0x20000000u)store(unit+0x138,flags&0xdfffffffu);}
   unitPhase=2;break;
  }
  case 1:if(!adapter(true))return false;unitPhase=2;break;
  case 2:{
   const auto definition=word(unit+0x10);const auto hp=signed16(half(unit+0x10c));const auto threshold=byte(definition+0xc);
   if(hp<=threshold && hp<signed16(half(unit+0x108))){
    const auto flags=word(unit+0x138);
    if(!(flags&0x10000000u)){
     store(unit+0x138,flags|0x10000000u);
     if(!(word(unit+0x138)&0x20000000u) && !mode && word(unit+8)==222){unitPhase=3;break;}
    }
   }else {const auto flags=word(unit+0x138);if(flags&0x10000000u)store(unit+0x138,flags&0xefffffffu);}
   return true;
  }
  case 3:return adapter(false);
  default:return false;
  }
 }
};
BattlePinchCursor::BattlePinchCursor()=default;
BattlePinchCursor::~BattlePinchCursor()=default;
bool BattlePinchCursor::active()const{return bool(state_);}
uint32_t BattlePinchCursor::pending()const{return state_?state_->pending:0;}
bool BattlePinchCursor::reset(){if(invoking_)return false;state_.reset();return true;}
struct BattlePinchRunner {
 static bool run(uint32_t input,int32_t mode,bool direct,const BattlePinchServices& s,BattlePinchCursor& cursor){
  const auto& m=s.actors.memory;
  if(cursor.invoking_ || !input || !m.word || !m.half || !m.storeWord || !m.storeByte || !s.actors.byte)return false;
  if(cursor.state_){const auto& c=*cursor.state_;if(c.input!=input || c.mode!=mode || c.direct!=direct || !same(c.owner,s))return false;}
  Running running(cursor.invoking_);
  if(!cursor.state_){cursor.state_=std::make_unique<BattlePinchCursor::State>();auto& c=*cursor.state_;
   c.owner=s;c.input=input;c.mode=mode;c.direct=direct;if(direct)c.unit=input;}
  auto& c=*cursor.state_;
  if(direct){if(!c.updateUnit())return false;cursor.state_.reset();return true;}
  const auto finish=[&](){cursor.state_.reset();return true;};
  for(;;)switch(c.phase){
  case 0:
   if(c.slot==64){c.slot=0;c.phase=1;break;}
   if(!c.unit){c.unit=battleRegistryUnit(input,int32_t(c.slot),s.actors);c.unitPhase=0;}
   if(c.unit && !c.updateUnit())return false;
   c.unit=0;++c.slot;break;
  case 1:
   if(c.slot==64){if(!c.group){c.group=1;c.slot=0;break;}c.phase=5;break;}
   c.unit=battleRegistryUnit(input,int32_t(c.slot),s.actors);
   if(!c.unit || !(c.word(c.unit+0x138)&(c.group?0x10000000u:0x20000000u))){++c.slot;break;}
   c.phase=2;break;
  case 2:{
   c.pending=0x80127814;if(!s.status)return false;
   const auto status=s.status(s.user,c.unit,27);if(!status)return false;
   c.pending=0;
   // GetBelong source returns a byte; parent extsb compares with zero.
   if(*status || c.byte(c.unit+0xc)!=0){++c.slot;c.phase=1;break;}
   const auto kind=signed32(c.word(c.unit+8));
   if((kind==222 || (kind>=224 && kind<231)) && signed16(c.half(c.unit+0x10c))>0){c.phase=3;break;}
   // First prequalified actor ends this group even for wrong kind/dead HP.
   if(!c.group){c.group=1;c.slot=0;c.phase=1;}else c.phase=5;
   break;
  }
  case 3:{
   c.storeByte(input+0x18ff8,uint8_t(c.byte(input+0x18ff8)-1));
   const auto countdown=c.byte(input+0x18ff8);
   if(countdown>0 && countdown<0x80)return finish();
   c.storeByte(input+0x18ff8,60);
   if(!(c.word(input+0x1720)&1))return finish();
   c.phase=4;break;
  }
  case 4:
   c.pending=0x800d98bc;if(!s.sound || !s.sound(s.user,c.group?0x802efc14:0x802efc04))return false;
   return finish();
  case 5:{
   const auto mario=battleMarioUnit(input,s.actors);if(mario)c.store(mario+0x238,0);
   c.storeByte(input+0x18ff8,0);return finish();
  }
  default:return false;
  }
 }
};
bool resumePackedBattleCheckAllPinch(uint32_t b,int32_t mode,const BattlePinchServices& s,BattlePinchCursor& c){return BattlePinchRunner::run(b,mode,false,s,c);}
bool resumePackedBattleUnitPinch(uint32_t u,int32_t mode,const BattlePinchServices& s,BattlePinchCursor& c){return BattlePinchRunner::run(u,mode,true,s,c);}
}
