#include "battle_present_item_native.hpp"
#include "battle_packed_recovery.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n){int32_t v;std::memcpy(&v,&n,4);return v;}
float scalar(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
uint32_t bits(float n){uint32_t v;std::memcpy(&v,&n,4);return v;}
double number(uint32_t hi,uint32_t lo){uint64_t n=(uint64_t(hi)<<32)|lo;double v;std::memcpy(&v,&n,8);return v;}
bool same(const BattlePresentItemNativeServices& a,const BattlePresentItemNativeServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.allocationCurrent==b.allocationCurrent&&a.subtractDoubleSingle==b.subtractDoubleSingle&&a.fma==b.fma&&a.add==b.add&&a.effect==b.effect&&
 a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&
 x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Running{bool& f;explicit Running(bool& v):f(v){f=true;}~Running(){f=false;}};
enum class Step{Entry,Recovery,Position,Convert,Operands,Fma,Add,Effect,Clear};
}
struct BattlePresentItemNativeCursor::State {
 BattlePresentItemNativeServices services;
 Step step=Step::Entry;bool poisoned=false;
 uint32_t battle=0,unit=0,pending=0x80120024;
 int32_t item=0,height=0;
 std::array<uint32_t,3> position{};
 std::array<float,3> effectPosition{};
 double encoded=0,bias=0;
 float scale=0,constant=0,converted=0,y=0,offset=0;
 bool valid(){if(poisoned)return false;if(!services.allocationCurrent||!services.allocationCurrent(services.user)){poisoned=true;return false;}return true;}
 uint32_t word(uint32_t at)const{return services.actors.memory.word(services.actors.memory.user,at);}
 void put(uint32_t at,uint32_t n)const{services.actors.memory.storeWord(services.actors.memory.user,at,n);}
};
BattlePresentItemNativeCursor::BattlePresentItemNativeCursor()=default;
BattlePresentItemNativeCursor::~BattlePresentItemNativeCursor()=default;
bool BattlePresentItemNativeCursor::active()const{return bool(state_);}
bool BattlePresentItemNativeCursor::stale()const{return state_&&state_->poisoned;}
uint32_t BattlePresentItemNativeCursor::pending()const{return state_?state_->pending:0;}
bool BattlePresentItemNativeCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<int32_t> resumeNativeBattlePresentItem(const BattlePresentItemNativeServices& s,BattlePresentItemNativeCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!s.allocationCurrent||!m.word||!m.half||!m.storeWord||!m.storeHalf||!m.storeByte)return {};
 if(cursor.state_&&!same(cursor.state_->services,s))return {};
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattlePresentItemNativeCursor::State>();cursor.state_->services=s;}
 auto& c=*cursor.state_;if(!c.valid())return {};
 const auto finish=[&](int32_t n)->std::optional<int32_t>{if(!c.valid())return {};cursor.state_.reset();return n;};
 for(;;){
  if(!c.valid())return {};
  switch(c.step){
  case Step::Entry:{
   c.battle=currentBattleAddress(s.actors);
   c.item=integer(c.word(currentBattleAddress(s.actors)+0x1603c));
   if(c.item>=339)return finish(0);
   const auto target=integer(c.word(currentBattleAddress(s.actors)+0x16044));
   if(target< -1||target>=64)return finish(0);
   c.unit=battleRegistryUnit(c.battle,target,s.actors);
   if(c.item==0)return finish(0);
   if(c.item!=123&&c.item!=124){
    if(c.item<0)return finish(0);
    m.storeByte(m.user,c.battle+0x1c74,1);
    const auto state=c.word(c.battle+0x171c);
    if(state==12||state==13)m.storeByte(m.user,c.battle+0x1c74,2);
    c.put(c.battle+0x1720,c.word(c.battle+0x1720)&0xffeeeeeeu);
    const auto previous=c.word(c.battle+0x1c5c);
    c.put(c.battle+0x1c60,previous);c.put(c.battle+0x1c5c,12);c.put(c.battle+0x171c,29);
    return finish(1);
   }
   c.step=Step::Recovery;continue;
  }
  case Step::Recovery:
   if(c.item==123)recoverPackedBattleHp(c.unit,1,s.actors);else recoverPackedBattleFp(c.unit,1,s.actors);
   c.step=Step::Position;continue;
  case Step::Position:{
   // Source GetPos80128c7c: each axis load followed immediately by its own
   // native private output store; disjoint ownership replaces source stack only.
   for(unsigned axis=0;axis<3;++axis)c.position[axis]=c.word(c.unit+0x3c+4*axis);
   const auto h=m.half(m.user,c.unit+0xce);c.height=h<32768?int32_t(h):int32_t(h)-65536;
   c.encoded=number(0x43300000,uint32_t(c.height)^0x80000000u);
   const auto hi=c.word(0x802efe78),lo=c.word(0x802efe7c);c.bias=number(hi,lo);
   c.scale=scalar(c.word(c.unit+0x114));c.constant=scalar(c.word(0x804225f8));
   c.step=Step::Convert;continue;
  }
  case Step::Convert:{
   c.pending=c.item==123?0x801200f8:0x80120178;
   if(!s.subtractDoubleSingle)return {};
   const auto value=s.subtractDoubleSingle(s.user,c.encoded,c.bias);if(!c.valid()||!value)return {};
   c.converted=*value;c.step=Step::Operands;continue;
  }
  case Step::Operands:
   c.y=scalar(c.position[1]);c.effectPosition[0]=scalar(c.position[0]);c.effectPosition[2]=scalar(c.position[2]);
   c.step=Step::Fma;continue;
  case Step::Fma:{
   c.pending=c.item==123?0x80120108:0x80120188;
   if(!s.fma)return {};
   const auto value=s.fma(s.user,c.scale,c.converted,c.constant);if(!c.valid()||!value)return {};
   c.offset=*value;c.step=Step::Add;continue;
  }
  case Step::Add:{
   c.pending=c.item==123?0x8012010c:0x8012018c;
   if(!s.add)return {};
   const auto value=s.add(s.user,c.y,c.offset);if(!c.valid()||!value)return {};
   c.effectPosition[1]=*value;c.position[1]=bits(*value);c.step=Step::Effect;continue;
  }
  case Step::Effect:
   c.pending=0x8014335c;
   if(!s.effect)return {};
   {const bool ready=s.effect(s.user,c.item==123?0:1,1,c.effectPosition);if(!c.valid()||!ready)return {};}
   c.step=Step::Clear;continue;
  case Step::Clear:
   c.put(currentBattleAddress(s.actors)+0x1603c,0);return finish(0);
  }
 }
}
}
