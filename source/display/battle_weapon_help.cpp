#include "battle_weapon_help.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float f=a+b;return f;}
float sub(float a,float b){volatile float f=a-b;return f;}
bool same(const BattleWeaponHelpServices&a,const BattleWeaponHelpServices&b){
 const auto&m=a.actors.memory;const auto&n=b.actors.memory;
 return a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.byte==b.byte&&a.request==b.request&&a.stackStore==b.stackStore&&
 a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;
}
struct Guard{bool&flag;explicit Guard(bool&f):flag(f){flag=true;}~Guard(){flag=false;}};
}
std::optional<uint8_t> BattleWeaponHelpBytes::byte(uint32_t offset)const{if(offset<cells.size())return cells[offset];const auto i=escaped.find(offset);return i==escaped.end()?std::nullopt:std::optional<uint8_t>(i->second);}
struct BattleWeaponHelpCursor::State {
 BattleWeaponHelpServices owner;uint32_t key=0,xBits=0,yBits=0,pc=0x80117c3c,pending=0;
 bool invalid=false,queued=false;float x=0,y=0;uint32_t source=0,destination=0,row=0,line=0;
 uint8_t value=0;BattleWeaponHelpBytes bytes;BattleWeaponHelpCall call;
 bool valid(){if(invalid)return false;if(!owner.current||!owner.current(owner.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a){if(!valid())return 0;auto n=owner.actors.memory.word(owner.actors.memory.user,a);return valid()?n:0;}
 float constant(int32_t offset){return real(word(0x80427440u+uint32_t(offset)));}
 std::optional<uint8_t> read(uint32_t address){pending=pc;if(!valid()||!owner.byte)return {};auto v=owner.byte(owner.user,address);return valid()?v:std::nullopt;}
 bool store(uint32_t address,uint8_t v){
  pending=pc;if(!valid())return false;
  if(address>=bytes.cells.size()){
   if(!owner.stackStore||!owner.stackStore(owner.user,address+12,v)||!valid())return false;
   bytes.escaped[address]=v;
  }else bytes.cells[address]=v;
  return true;
 }
 template<class F>std::optional<uint32_t> request(uint32_t function,uint32_t site,F fill){
  pending=function;if(!valid())return {};
  if(!queued){call={};call.function=function;call.callsite=site;fill(call);queued=true;}
  if(!valid()||!owner.request)return {};
  const auto v=owner.request(owner.user,call);if(!valid()||!v)return {};queued=false;return v;
 }
 std::optional<uint32_t> request(uint32_t function,uint32_t site){return request(function,site,[](auto&){});}
};
BattleWeaponHelpCursor::BattleWeaponHelpCursor()=default;BattleWeaponHelpCursor::~BattleWeaponHelpCursor()=default;
bool BattleWeaponHelpCursor::active()const{return bool(state_);}bool BattleWeaponHelpCursor::stale()const{return state_&&state_->invalid;}
uint32_t BattleWeaponHelpCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponHelpCursor::reset(){if(invoking_)return false;state_.reset();return true;}
const BattleWeaponHelpBytes* BattleWeaponHelpCursor::privateBytes()const{return state_?&state_->bytes:nullptr;}
bool resumePackedBattleWeaponHelp(uint32_t key,float x,float y,const BattleWeaponHelpServices&s,BattleWeaponHelpCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||!s.actors.memory.word)return false;
 if(cursor.state_){auto&c=*cursor.state_;if(c.key!=key||c.xBits!=bits(x)||c.yBits!=bits(y)||!same(c.owner,s))return false;}
 Guard guard(cursor.invoking_);
 if(!cursor.state_){if(!s.current(s.user))return false;cursor.state_=std::make_unique<BattleWeaponHelpCursor::State>();auto&c=*cursor.state_;c.owner=s;c.key=key;c.xBits=bits(x);c.yBits=bits(y);c.x=x;c.y=y;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.pc){
 case 0x80117c3c:{
  const auto v=c.request(0x8007b44c,0x80117c64,[&](auto&q){q.privateColor=c.word(0x80422428);q.integers={0};q.reals={c.x,c.y,c.constant(-20412),c.constant(-20408),c.constant(-20436)};});if(!v)return false;c.pc=0x80117c68;continue;}
 case 0x80117c68:if(!c.request(0x80077b4c,0x80117c68))return false;c.pc=0x80117c70;continue;
 case 0x80117c70:if(!c.request(0x80077844,0x80117c70,[](auto&q){q.integers={0};}))return false;c.pc=0x80117c78;continue;
 case 0x80117c78:if(!c.request(0x80077758,0x80117c78,[&](auto&q){q.reals={c.constant(-20404)};}))return false;c.pc=0x80117c80;continue;
 case 0x80117c80:{const auto v=c.request(0x80081b8c,0x80117c80,[&](auto&q){q.integers={c.key};});if(!v)return false;c.source=*v;c.pc=c.source?0x80117c8c:0x80117db0;continue;}
 case 0x80117c8c:for(unsigned i=0;i<4;++i)c.bytes.cells[48*i]=0;c.destination=0;c.line=0;c.pc=0x80117cb8;continue;
 case 0x80117cb8:{const auto v=c.read(c.source);if(!v)return false;if(*v==60){++c.source;c.pc=0x80117cc4;}else c.pc=0x80117cd4;continue;}
 case 0x80117cc4:{const auto v=c.read(c.source);if(!v)return false;++c.source;c.pc=*v==62?0x80117cd4:0x80117cc4;continue;}
 case 0x80117cd4:{const auto v=c.read(c.source);if(!v)return false;c.value=*v;c.pc=!c.value?0x80117ce4:c.value==10?0x80117d00:0x80117d14;continue;}
 case 0x80117ce4:if(!c.store(c.destination,0))return false;c.row=0;c.pc=0x80117d24;continue;
 case 0x80117d00:if(!c.store(c.destination,0))return false;c.line+=48;c.destination=c.line;++c.source;c.pc=0x80117cb8;continue;
 case 0x80117d14:if(!c.store(c.destination,c.value))return false;++c.source;++c.destination;c.pc=0x80117cb8;continue;
 case 0x80117d24:{const auto v=c.bytes.byte(c.row*48);if(!v){c.pending=c.row==0?0x80117d24:c.row==1?0x80117d54:0x80117d80;return false;}c.pc=*v?0x80117d34:0x80117d38;continue;}
 case 0x80117d34:{const uint32_t site=c.row==0?0x80117d34:c.row==1?0x80117d68:0x80117d94;if(!c.request(0x80077758,site,[&](auto&q){q.reals={c.constant(-20404)};}))return false;c.pc=0x80117d38;continue;}
 case 0x80117d38:{const auto v=c.request(0x800773d0,c.row==0?0x80117d50:c.row==1?0x80117d7c:0x80117da8,[&](auto&q){
   if(!c.row)c.x=add(c.constant(-20468),c.x);
   const float offset=c.constant(c.row==0?-20400:c.row==1?-20396:-20392);q.privateText=&c.bytes;q.privateOffset=c.row*48;q.reals={c.x,sub(c.y,offset)};
  });if(!v)return false;if(++c.row<3)c.pc=0x80117d24;else{cursor.state_.reset();return true;}continue;}
 case 0x80117db0:{const auto v=c.request(0x800773d0,0x80117dc8,[&](auto&q){const float offset=c.constant(-20436),bottom=c.constant(-20400);q.guestText=0x802ef788;q.reals={add(offset,c.x),sub(c.y,bottom)};});if(!v)return false;cursor.state_.reset();return true;}
 default:return false;
 }}
}
}
