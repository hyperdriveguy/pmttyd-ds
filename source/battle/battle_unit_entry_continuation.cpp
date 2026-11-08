#include "battle_unit_entry_continuation.hpp"
#include <cstdlib>
#include <cstring>

namespace ttyd {
namespace {
constexpr uint32_t UnitBytes = 0xb34, PartBytes = 0x500, DefinitionBytes = 0x4c;
constexpr uint32_t Zero = 0x804226a4, One = 0x804226a8;
int64_t signedWord(uint32_t x) { return x < 0x80000000 ? int64_t(x) : int64_t(x)-0x100000000LL; }
uint32_t signedByte(uint8_t x) { return x < 128 ? x : uint32_t(x)-256; }
struct Guard { bool& flag; explicit Guard(bool& f):flag(f){flag=true;} ~Guard(){flag=false;} };
bool same(const BattleUnitEntryContinuationServices& a,const BattleUnitEntryContinuationServices& b) {
 const auto& x=a.actors.memory; const auto& y=b.actors.memory;
 return a.user==b.user && a.current==b.current && a.call==b.call && a.lifetime==b.lifetime &&
  !a.lifetime.owner_before(b.lifetime) && !b.lifetime.owner_before(a.lifetime) &&
  a.actors.user==b.actors.user && a.actors.byte==b.actors.byte && x.user==y.user &&
  x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
  x.storeHalf==y.storeHalf && x.storeByte==y.storeByte;
}
}
struct BattleUnitEntryCursor::State {
 BattleUnitEntryContinuationServices services;
 uint32_t battle=0,definition=0,unit=0,index=0,part=0,partDefinition=0,remaining=0,partSize=0;
 uint32_t body=0,result=0,unitOne=0,partOne=0,partZero=0;
 // Each state is the original next instruction/call PC, not a frame counter.
 uint32_t pc=0x80129014,pending=0;
 bool invalid=false,done=false;
 std::optional<BattleUnitEntryRequest> held;
 bool current(){if(invalid || !services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t w(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 void w(uint32_t a,uint32_t v)const{services.actors.memory.storeWord(services.actors.memory.user,a,v);}
 uint16_t h(uint32_t a)const{return services.actors.memory.half(services.actors.memory.user,a);}
 void h(uint32_t a,uint16_t v)const{services.actors.memory.storeHalf(services.actors.memory.user,a,v);}
 uint8_t b(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void b(uint32_t a,uint8_t v)const{services.actors.memory.storeByte(services.actors.memory.user,a,v);}
 float f(uint32_t a)const{const auto bits=w(a);float v;std::memcpy(&v,&bits,4);return v;}
 void request(uint32_t function,std::array<uint32_t,3> args={}) {
  if(!held)held=BattleUnitEntryRequest{function,pc,args,{}};
 }
 std::optional<BattleUnitEntryOutput> call(){
  pending=held->function;if(!current())return {};
  const auto r=services.call(services.user,*held);
  if(!current() || !r)return {};
  held.reset();pending=0;return r;
 }
 bool unary(uint32_t fn,uint32_t target){request(fn,{target,0,0});return bool(call());}
 bool vector(uint32_t fn,uint32_t target,uint32_t source,bool uniform){
  if(!held){request(fn,{target,0,0});const auto x=f(source);
   held->vector={x,uniform?x:f(source+4),uniform?x:f(source+8)};}
  return bool(call());
 }
 // Source loads the first pair before either store, then loads/stores the third.
 void triplet(uint32_t to,uint32_t from){const auto a=w(from),b=w(from+4);w(to,a);w(to+4,b);w(to+8,w(from+8));}
};
BattleUnitEntryCursor::BattleUnitEntryCursor()=default;
BattleUnitEntryCursor::~BattleUnitEntryCursor()=default;
uint32_t BattleUnitEntryCursor::pending()const{return state_?state_->pending:0;}
bool BattleUnitEntryCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<uint32_t> resumePackedBattleUnitEntry(uint32_t battle,uint32_t definition,
 const BattleUnitEntryContinuationServices& s,BattleUnitEntryCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_ || !s.lifetime || !s.current || !s.call || !s.actors.byte ||
  !m.word || !m.half || !m.storeWord || !m.storeHalf || !m.storeByte)return {};
 if(cursor.state_ && (cursor.state_->battle!=battle || cursor.state_->definition!=definition ||
  !same(cursor.state_->services,s)))return {};
 Guard guard(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleUnitEntryCursor::State>();
  cursor.state_->services=s;cursor.state_->battle=battle;cursor.state_->definition=definition;}
 auto& c=*cursor.state_; const auto d=definition;
 for(;;){
  if(!c.current())return {};
  if(c.done)return c.result;
  const auto u=c.unit;
  switch(c.pc){
  case 0x80129014:{
   c.request(0x800f88cc,{battle,c.index,0});const auto r=c.call();if(!r)return {};
   if(!r->value)c.pc=0x80129040;
   else if(++c.index==64){c.result=0;c.done=true;}
   break;}
  case 0x80129040:{
   c.request(0x800f8934,{UnitBytes,0,0});const auto r=c.call();if(!r)return {};
   c.unit=r->value;if(!c.unit){c.result=0;c.done=true;}else c.pc=0x8012905c;break;}
  case 0x8012905c:
   c.request(0x800050b4,{u,0,UnitBytes});if(!c.call())return {};c.pc=0x8012906c;break;
  case 0x8012906c:
   c.request(0x800f88b4,{battle,c.index,u});if(!c.call())return {};c.pc=0x80129070;break;
  case 0x80129070:
   c.w(u,c.index);c.w(u+0x10,c.w(d));c.w(u+0x18,c.w(c.w(u+0x10)+0xc0));
   c.w(u+4,c.w(c.w(d)));c.w(u+8,c.w(c.w(d)));c.pc=0x801290a4;break;
  case 0x801290a4:if(!c.unary(0x8019673c,u))return {};c.pc=0x801290ac;break;
  case 0x801290ac:if(!c.unary(0x80253e84,u))return {};c.pc=0x801290b4;break;
  case 0x801290b4:if(!c.unary(0x80195d34,u))return {};c.pc=0x801290b8;break;
  case 0x801290b8:{
   c.b(u+0xe,255);c.b(u+0xc,c.b(d+4));c.b(u+0xd,c.b(c.w(u+0x10)+0xe));
   const auto kind=signedWord(c.w(u+8));c.pc=kind==222 || (kind>=224 && kind<231)?0x801290f0:0x801290fc;break;}
  case 0x801290f0:{c.request(0x800d5eb0);const auto r=c.call();if(!r)return {};
   c.b(u+0xd,uint8_t(c.h(r->value+0x8a)));c.pc=0x801290fc;break;}
  case 0x801290fc:
   c.w(u+0x2c,c.w(d+8));for(auto off:{0x294u,0x298u,0x29cu,0x28cu,0x290u,0x2a0u,0x2a8u})c.w(u+off,0);
   c.pc=0x80129134;break;
  case 0x80129134:if(!c.vector(0x80128c6c,u,d+0xc,false))return {};c.pc=0x80129148;break;
  case 0x80129148:if(!c.vector(0x801283d8,u,d+0xc,false))return {};c.pc=0x8012915c;break;
  case 0x8012915c:if(!c.vector(0x801289a0,u,d+0xc,false))return {};c.pc=0x80129170;break;
  case 0x80129170:if(!c.vector(0x80128680,u,Zero,true))return {};c.pc=0x80129184;break;
  case 0x80129184:if(!c.vector(0x801288b4,u,Zero,true))return {};c.pc=0x80129198;break;
  case 0x80129198:if(!c.vector(0x80128604,u,Zero,true))return {};c.pc=0x801291ac;break;
  case 0x801291ac:if(!c.vector(0x80128524,u,One,true))return {};c.pc=0x801291c0;break;
  case 0x801291c0:if(!c.vector(0x80128574,u,One,true))return {};c.pc=0x801291c4;break;
  case 0x801291c4:
   {const auto kind=c.w(u+0x10);c.unitOne=c.w(One);c.triplet(u+0x84,kind+0x20);}
   for(auto row:{std::array<uint32_t,2>{0xb4,0x3c},{0xec,0x5c},{0xf8,0x68}})
    c.triplet(u+row[0],c.w(u+0x10)+row[1]);
   c.h(u+0x10a,c.h(c.w(d)+8));c.h(u+0x108,c.h(u+0x10a));c.h(u+0x10c,c.h(c.w(d)+8));
   c.h(u+0x110,c.h(c.w(d)+0xa));c.h(u+0x10e,c.h(u+0x110));c.h(u+0x112,c.h(c.w(d)+0xa));
   c.w(u+0x114,c.unitOne);c.w(u+0x27c,0);c.w(u+0x138,0);c.w(u+0x140,0xffffffff);c.pc=0x80129294;break;
  case 0x80129294:if(!c.unary(0x80125fd8,u))return {};c.pc=0x80129298;break;
  case 0x80129298:
   for(auto row:{std::array<uint32_t,2>{0xcc,0x16},{0xce,0x18},{0xd0,0x1a},{0xd2,0x1c},{0xd4,0x2c},{0xd6,0x2e}})
    c.h(u+row[0],c.h(c.w(u+0x10)+row[1]));
   c.triplet(u+0xd8,c.w(u+0x10)+0x74);c.w(u+0xe4,c.w(c.w(u+0x10)+0x80));c.w(u+0xe8,c.w(c.w(u+0x10)+0x84));
   c.b(u+0x25,c.b(c.w(u+0x10)+0x8a));c.b(u+0x26,c.b(c.w(u+0x10)+0x8b));c.pc=0x80129330;break;
  case 0x80129330:if(!c.unary(0x80127b58,u))return {};c.pc=0x80129334;break;
  case 0x80129334:
   c.w(u+0x104,c.w(c.w(u+0x10)+0xac));c.w(u+0x144,c.w(c.w(u+0x10)+0xb0));
   c.b(u+0x21,1);c.b(u+0x22,1);c.b(u+0x188,0);c.b(u+0x189,c.b(battle+8*signedByte(c.b(u+0xc))+0xa));
   for(unsigned i=0;i<4;++i)c.w(u+0x218+4*i,c.w(d+0x1c+4*i));
   c.remaining=c.b(c.w(u+0x10)+0xb4);c.partSize=c.remaining*PartBytes;c.pc=0x801293ac;break;
  case 0x801293ac:{c.request(0x800f8934,{c.partSize,0,0});const auto r=c.call();if(!r)return {};
   c.part=r->value;c.pc=0x801293bc;break;}
  case 0x801293bc:
   c.request(0x800050b4,{c.part,0,c.partSize});if(!c.call())return {};c.pc=0x801293c0;break;
  case 0x801293c0:{
   c.w(u+0x14,c.part);c.partOne=c.w(One);const auto kind=c.w(u+0x10);
   c.partZero=c.w(Zero);c.partDefinition=c.w(kind+0xb8);c.pc=0x801293d4;break;}
  case 0x801293d4:{
   const auto p=c.part;c.w(p+4,c.partDefinition);c.w(p+0x4ec,u);
   c.w(p+0x1ac,c.w(c.w(p+4)+0x40));c.w(p+0x1b0,c.w(c.w(p+4)+0x44));
   if(c.w(p+0x1ac)&1)c.w(u+0x2d4,c.w(c.w(p+4)));
   c.w(p+0x1b4,c.w(c.w(p+4)+0x38));c.w(p+0x1b8,c.w(c.w(p+4)+0x3c));
   c.h(p+0x1a2,c.h(c.w(p+4)+0x30));c.h(p+0x1a4,c.h(c.w(p+4)+0x32));
   c.w(p+0x78,c.partOne);c.b(p+0xbc,0);c.b(p+0xbd,c.b(battle+8*signedByte(c.b(u+0xc))+0xa));
   for(unsigned i=0;i<3;++i)c.w(p+0x24+4*i,c.w(c.w(p+4)+0xc+4*i));
   c.pc=0x80129494;break;}
  case 0x80129494:{
   if(!c.held){c.request(0x801284a4,{c.part,0,0});float v;std::memcpy(&v,&c.partOne,4);c.held->vector={v,v,v};}
   if(!c.call())return {};
   c.pc=0x801294a8;break;}
  case 0x801294a8:if(!c.vector(0x80128470,c.part,One,true))return {};c.pc=0x801294bc;break;
  case 0x801294bc:if(!c.vector(0x80128654,c.part,Zero,true))return {};c.pc=0x801294d0;break;
  case 0x801294d0:if(!c.vector(0x801287b4,c.part,Zero,true))return {};c.pc=0x801294e4;break;
  case 0x801294e4:if(!c.vector(0x801285d0,c.part,Zero,true))return {};c.pc=0x801294e8;break;
  case 0x801294e8:{
   const auto p=c.part;c.b(p+0x4fc,0);--c.remaining;
   for(unsigned i=0;i<3;++i)c.w(p+0x170+4*i,c.w(c.w(p+4)+0x18+4*i));
   for(unsigned i=0;i<3;++i)c.w(p+0x17c+4*i,c.partZero);
   for(unsigned i=0;i<3;++i)c.w(p+0x188+4*i,c.w(c.w(p+4)+0x24+4*i));
   for(unsigned i=0;i<3;++i)c.w(p+0x194+4*i,c.partZero);
   c.w(p+0x4f0,c.w(0x804226a0));c.h(p+0x1a0,uint16_t(c.w(d+0x18)));
   c.w(p+0x1bc,c.w(c.partDefinition+0x48));c.w(p+0x1c0,0xffffffff);c.b(p+0x1c4,0);
   c.w(p+0x208,c.partOne);c.w(p+0x20c,c.partOne);c.b(p+0x215,0);c.b(p+0x214,0);
   c.w(p+0x4e8,0xffffffff);c.w(p+8,c.w(c.w(p+4)+4));
   if(signedWord(c.remaining)>0){c.part+=PartBytes;c.w(p,c.part);c.partDefinition+=DefinitionBytes;c.pc=0x801293d4;}
   else {c.w(p,0);c.pc=0x801295b8;}break;}
  case 0x801295b8:{c.request(0x80128c98,{u,0,0});const auto r=c.call();if(!r)return {};
   c.body=r->value;c.pc=0x801295c4;break;}
  case 0x801295c4:{c.request(0x80128cd8,{u,c.body,0});const auto r=c.call();if(!r)return {};
   c.body=r->value;c.pc=0x801295d0;break;}
  case 0x801295d0:{c.request(0x8012772c,{c.body,65,0});const auto r=c.call();if(!r)return {};
   c.w(u+0x2d8,r->value);c.pc=0x801295e0;break;}
  case 0x801295e0:{c.request(0x8012772c,{c.body,43,0});const auto r=c.call();if(!r)return {};
   c.w(u+0x2dc,r->value);c.b(u+0x311,255);c.b(u+0x312,255);c.pc=0x801295f8;break;}
  case 0x801295f8:if(!c.unary(0x801011fc,u))return {};c.pc=0x801295fc;break;
  case 0x801295fc:
   c.w(u+0x314,0);c.w(u+0x318,0);c.pc=signedWord(c.w(u+8))==208?0x80129628:0x80129634;break;
  case 0x80129628:{c.request(0x800f8934,{0x44,0,0});const auto r=c.call();if(!r)return {};
   c.w(u+0x314,r->value);c.pc=0x80129634;break;}
  case 0x80129634:if(!c.unary(0x80127134,u))return {};c.pc=0x80129638;break;
  case 0x80129638:{
   c.w(u+0x288,0);const auto script=c.w(c.w(u+0x10)+0xbc);
   if(script){c.pc=0x80129658;c.request(0x800374ac,{script,10,0});}
   else {c.result=u;c.done=true;}break;}
  case 0x80129658:{const auto r=c.call();if(!r)return {};
   if(!r->event)std::abort();
   r->event->owner=int32_t(signedWord(c.w(u)));c.w(u+0x288,r->event->id);c.result=u;c.done=true;break;}
  default:std::abort();
  }
 }
}
struct BattleDisplayEntryCursor::State {
 BattleUnitEntryContinuationServices services;
 uint32_t unit=0,part=0,bank=0,pc=0x8010120c,pending=0;
 bool invalid=false,done=false;
 std::optional<BattleUnitEntryRequest> held;
 bool current(){if(invalid || !services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t w(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 void w(uint32_t a,uint32_t v)const{services.actors.memory.storeWord(services.actors.memory.user,a,v);}
 std::optional<BattleUnitEntryOutput> call(){pending=held->function;if(!current())return {};
  const auto r=services.call(services.user,*held);if(!current() || !r)return {};held.reset();pending=0;return r;}
};
BattleDisplayEntryCursor::BattleDisplayEntryCursor()=default;
BattleDisplayEntryCursor::~BattleDisplayEntryCursor()=default;
uint32_t BattleDisplayEntryCursor::pending()const{return state_?state_->pending:0;}
bool BattleDisplayEntryCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleDisplayEntry(uint32_t unit,const BattleUnitEntryContinuationServices& s,BattleDisplayEntryCursor& cursor){
 if(cursor.invoking_ || !s.lifetime || !s.current || !s.call || !s.actors.memory.word || !s.actors.memory.storeWord)return false;
 if(cursor.state_ && (cursor.state_->unit!=unit || !same(cursor.state_->services,s)))return false;
 Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<BattleDisplayEntryCursor::State>();cursor.state_->unit=unit;cursor.state_->services=s;}
 auto& c=*cursor.state_;
 for(;;){if(!c.current())return false;if(c.done)return true;
  switch(c.pc){
  case 0x8010120c:c.part=c.w(unit+0x14);c.pc=0x80101214;break;
  case 0x80101214:{
   if(!c.part){c.done=true;break;}
   const auto table=c.w(c.part+0x1bc);
   if(!table){c.w(c.part+0x1c0,0xffffffff);c.pc=0x8010126c;break;}
   c.bank=c.w(c.w(c.part+4)+8);
   if(!c.bank){c.w(c.part+0x1c0,0xffffffff);c.pc=0x8010126c;break;}
   c.pc=0x8010124c;c.held=BattleUnitEntryRequest{0x80103cd0,c.pc,{table,28,0},{}};break;}
  case 0x8010124c:{const auto r=c.call();if(!r)return false;
   c.pc=0x80101258;c.held=BattleUnitEntryRequest{0x8026d5fc,c.pc,{c.part+0x1c4,r->value,0},{}};break;}
  case 0x80101258:
   if(!c.call())return false;
   c.pc=0x80101264;c.held=BattleUnitEntryRequest{0x8004176c,c.pc,{c.bank,1,0},{}};break;
  case 0x80101264:{const auto r=c.call();if(!r)return false;c.w(c.part+0x1c0,r->value);c.pc=0x8010126c;break;}
  case 0x8010126c:c.part=c.w(c.part);c.pc=0x80101214;break;
  default:std::abort();
  }
 }
}
}
