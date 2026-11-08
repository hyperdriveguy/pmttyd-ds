#include "battle_weapon_evaluation_continuation.hpp"
namespace ttyd {
namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t sb(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
int32_t sh(uint16_t n){return n<32768?int32_t(n):int32_t(n)-65536;}
enum class Mode{Cost,Count,Update,Decision};
bool same(const BattleWeaponEvaluationServices& a,const BattleWeaponEvaluationServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.privateBuffer==b.privateBuffer&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Invocation{bool& v;explicit Invocation(bool& b):v(b){v=true;}~Invocation(){v=false;}};
}
struct BattleWeaponEvaluationCursor::State {
 Mode mode=Mode::Cost;BattleWeaponEvaluationServices s;
 uint32_t inputs[4]{},pc=0,pending=0,b=0,u=0,w=0,type=0,index=0,buffer=0,menu=0,rawEnemy=0,availableFp=0,cost=0;
 uint32_t row=0,rowWeapon=0,candidate=0;int32_t count=0,side=0,chosen=-1;
 bool poisoned=false,sameSide=false;
 std::optional<uint32_t> cursor;
 std::optional<BattleWeaponEvaluationRequest> held;
 uint32_t word(uint32_t a)const{return s.actors.memory.word(s.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return s.actors.byte(s.actors.user,a);}
 uint16_t half(uint32_t a)const{return s.actors.memory.half(s.actors.memory.user,a);}
 void store(uint32_t a,uint32_t v)const{s.actors.memory.storeWord(s.actors.memory.user,a,v);}
 void put(uint32_t a,uint8_t v)const{s.actors.memory.storeByte(s.actors.memory.user,a,v);}
 bool current(){if(poisoned)return false;if(!s.current||!s.current(s.user)){poisoned=true;return false;}return true;}
 template<class Build>std::optional<uint32_t> call(uint32_t fn,uint32_t site,Build build){
  pending=fn;if(!held){held=BattleWeaponEvaluationRequest{};held->function=fn;held->callsite=site;build(*held);}
  if(!s.call)return {};
  const auto result=s.call(s.user,*held);if(!current())return {};
  if(result){held.reset();pending=0;}return result;
 }
 std::optional<uint32_t> call(uint32_t fn,uint32_t site,std::array<uint32_t,8> a={},uint8_t n=0){return call(fn,site,[&](auto& q){q.arguments=a;q.count=n;});}
};
BattleWeaponEvaluationCursor::BattleWeaponEvaluationCursor()=default;
BattleWeaponEvaluationCursor::~BattleWeaponEvaluationCursor()=default;
bool BattleWeaponEvaluationCursor::active()const{return bool(state_);}
bool BattleWeaponEvaluationCursor::stale()const{return state_&&state_->poisoned;}
uint32_t BattleWeaponEvaluationCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponEvaluationCursor::reset(){if(invoking_)return false;state_.reset();return true;}
struct BattleWeaponEvaluationRunner {
 static std::optional<int32_t> run(Mode mode,std::array<uint32_t,4> args,const BattleWeaponEvaluationServices& s,BattleWeaponEvaluationCursor& owner){
  const auto& m=s.actors.memory;
  if(owner.invoking_||!m.word||!m.half||!m.storeWord||!m.storeByte||!s.actors.byte||!s.current)return {};
  if(owner.state_){const auto& c=*owner.state_;if(c.mode!=mode||!same(c.s,s))return {};for(unsigned i=0;i<4;++i)if(c.inputs[i]!=args[i])return {};}
  Invocation invocation(owner.invoking_);
  if(!owner.state_){owner.state_=std::make_unique<BattleWeaponEvaluationCursor::State>();auto& c=*owner.state_;c.s=s;c.mode=mode;for(unsigned i=0;i<4;++i)c.inputs[i]=args[i];
   if(mode==Mode::Cost){c.u=args[0];c.w=args[1];c.pc=0x80126914;}
   else {c.b=args[0];if(mode==Mode::Count){c.u=args[1];c.w=args[2];c.pc=0x80123d24;}else {c.type=args[1];c.index=args[2];c.pc=mode==Mode::Update?0x80123dcc:0x80123234;}}
  }
  auto& c=*owner.state_;const auto finish=[&](int32_t result){owner.state_.reset();return std::optional<int32_t>(result);};
  for(;;){if(!c.current())return {};switch(c.pc){
  case 0x80126914:{const auto fp=c.call(0x80126c64,c.pc,{c.u},1);if(!fp)return {};c.availableFp=*fp;c.pc=0x80126924;continue;}
  case 0x80126924:{const auto cost=c.call(0x80126968,c.pc,{c.u,c.w},2);if(!cost)return {};c.cost=*cost;if(sw(c.availableFp)<sw(c.cost))return finish(0);c.pc=0x80126938;continue;}
  case 0x80126938:{const auto ap=c.call(0x800d3c44,c.pc);if(!ap)return {};return finish(sw(*ap)>=int32_t(100*c.byte(c.w+0x12))?1:0);}
  case 0x80123d24:if(!c.w||!c.word(c.w+0xb0))return finish(0);c.pc=0x80123d44;continue;
  case 0x80123d44:{const auto affordable=c.call(0x801268fc,c.pc,{c.u,c.w},2);if(!affordable)return {};if(!*affordable)return finish(-1);c.pc=0x80123d5c;continue;}
  case 0x80123d5c:{const auto side=c.call(0x80127454,c.pc,{c.u},1);if(!side)return {};c.rawEnemy=*side;c.pc=0x80123d90;continue;}
  case 0x80123d90:{
   if(!s.privateBuffer||s.privateBuffer==c.b+0x428){c.pending=0x800ff5c0;return {};}
   const auto sampled=c.call(0x800ff5c0,c.pc,[&](auto& q){const auto actor=c.word(c.u),admission=c.word(c.w+0x64),attributes=c.word(c.w+0x68);const auto direction=c.byte(c.b+c.rawEnemy*8+0xa);q.arguments={s.privateBuffer,c.w,actor,uint32_t(sb(uint8_t(c.rawEnemy))),admission,attributes,uint32_t(sb(uint8_t(-direction)))};q.count=7;});
   if(!sampled)return {};
   return finish(sb(c.byte(s.privateBuffer+0xa6c)));
  }
  case 0x80123dcc:{const auto unit=c.call(0x800f88cc,c.pc,[&](auto& q){q.arguments={c.b,c.word(c.b+0x420)};q.count=2;});if(!unit)return {};c.u=*unit;c.menu=c.b+0x171c;c.pc=0x80123de4;continue;}
  case 0x80123de4:{const auto ready=c.call(0x80124cb8,c.pc,[&](auto& q){q.arguments={c.menu,c.type};q.count=2;q.privateCursorOutput=&c.cursor;});if(!ready)return {};c.pc=0x80123de8;continue;}
  case 0x80123de8:if(!c.cursor){c.pending=0x80124cb8;return {};}c.count=sw(c.word(*c.cursor+8));c.index=0;c.pc=0x80123e00;continue;
  case 0x80123e00:
   if(sw(c.index)>=c.count)return finish(0);
   c.row=c.menu+28*c.index;c.rowWeapon=c.word(c.row+0x80);c.store(c.row+0x84,0);c.store(c.row+0x98,0);if(!c.rowWeapon){++c.index;continue;}c.pc=0x80123e20;continue;
  case 0x80123e20:{const auto targets=c.call(0x80123d00,c.pc,{c.b,c.u,c.rowWeapon},3);if(!targets)return {};const auto n=sw(*targets);if(n>0){c.store(c.row+0x84,1);c.store(c.row+0x98,0);}else c.store(c.row+0x98,n<0?(c.type==4?2:1):(c.type==0?4:c.type==1?5:3));++c.index;c.pc=0x80123e00;continue;}
  case 0x80123234:{const auto unit=c.call(0x800f88cc,c.pc,[&](auto& q){q.arguments={c.b,c.word(c.b+0x420)};q.count=2;});if(!unit)return {};c.u=*unit;c.buffer=c.b+0x428;c.pc=0x80123248;continue;}
  case 0x80123248:{const auto ready=c.call(0x80123ec0,c.pc,{c.b,c.type},2);if(!ready)return {};c.w=c.word(c.b+0x179c+28*c.index);c.pc=0x8012325c;continue;}
  case 0x8012325c:{const auto enemy=c.call(0x80127454,c.pc,{c.u},1);if(!enemy)return {};c.rawEnemy=*enemy;c.pc=0x80123290;continue;}
  case 0x80123290:{const auto sampled=c.call(0x800ff5c0,c.pc,[&](auto& q){const auto owner=sb(c.byte(c.u+0xc));const auto actor=c.word(c.b+0x420),admission=c.word(c.w+0x64),attributes=c.word(c.w+0x68);const auto direction=sb(c.byte(c.b+uint32_t(owner)*8+0xa));q.arguments={c.buffer,c.w,actor,uint32_t(sb(uint8_t(c.rawEnemy))),admission,attributes,uint32_t(direction)};q.count=7;});if(!sampled)return {};c.put(c.buffer+0xab7,0);if(!(c.word(c.w+0x68)&0x30000000))return finish(0);c.pc=0x801232b0;continue;}
  case 0x801232b0:{const auto side=c.call(0x80128144,c.pc,{c.u},1);if(!side)return {};c.side=sb(uint8_t(*side));c.chosen=-1;c.index=0;c.pc=0x8012332c;continue;}
  case 0x8012332c:
   if(sw(c.index)>=sb(c.byte(c.buffer+0xa6c))){c.pc=0x8012333c;continue;}
   // Source loads flags BEFORE permutation and captures the branch BEFORE
   // registry/side children. Later provider mutations affect the next row.
   c.sameSide=(c.word(c.w+0x68)&0x10000000)!=0;
   c.row=c.buffer+4+36*uint32_t(sb(c.byte(c.buffer+0xa6d+c.index)));c.pc=c.sameSide?0x801232e8:0x8012330c;continue;
  case 0x801232e8:case 0x8012330c:{const auto candidate=c.call(0x800f88cc,c.pc,[&](auto& q){q.arguments={c.b,uint32_t(sh(c.half(c.row)))};q.count=2;});if(!candidate)return {};c.candidate=*candidate;c.pc=c.sameSide?0x801232ec:0x80123310;continue;}
  case 0x801232ec:case 0x80123310:{const auto side=c.call(0x80128144,c.pc,{c.candidate},1);if(!side)return {};const auto equal=sb(uint8_t(*side))==c.side;if(c.sameSide){if(equal)c.chosen=sw(c.index);}else if(!equal){c.chosen=sw(c.index);c.pc=0x8012333c;continue;}++c.index;c.pc=0x8012332c;continue;}
  case 0x8012333c:if(c.chosen>=0)c.put(c.buffer+0xab7,uint8_t(c.chosen));return finish(0);
  default:return {};
  }}
 }
};
std::optional<int32_t> resumePackedBattleWeaponCostCheck(uint32_t u,uint32_t w,const BattleWeaponEvaluationServices& s,BattleWeaponEvaluationCursor& c){return BattleWeaponEvaluationRunner::run(Mode::Cost,{u,w},s,c);}
std::optional<int32_t> resumePackedBattleWeaponTargetCount(uint32_t b,uint32_t u,uint32_t w,const BattleWeaponEvaluationServices& s,BattleWeaponEvaluationCursor& c){return BattleWeaponEvaluationRunner::run(Mode::Count,{b,u,w},s,c);}
bool resumePackedBattleWeaponEligibility(uint32_t b,uint32_t t,const BattleWeaponEvaluationServices& s,BattleWeaponEvaluationCursor& c){return BattleWeaponEvaluationRunner::run(Mode::Update,{b,t},s,c).has_value();}
std::optional<int32_t> resumePackedBattleWeaponDecision(uint32_t b,uint32_t t,uint32_t index,const BattleWeaponEvaluationServices& s,BattleWeaponEvaluationCursor& c){return BattleWeaponEvaluationRunner::run(Mode::Decision,{b,t,index},s,c);}
}
