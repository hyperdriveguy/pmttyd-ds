#include "battle_sub_icon_continuation.hpp"
#include <cstring>
// Field offsets and masks in these notes are hex; counts, strides, states
// and icon IDs are decimal. Original addresses remain32-bit source identities.
namespace ttyd {namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
// Original signed-int conversion: construct43300000:(n XOR80000000),
// subtract source double(2^52+2^31),then round to float. Keep operation order.
float converted(uint32_t n,double bias){const uint64_t raw=(uint64_t(0x43300000)<<32)|(n^0x80000000u);double v;std::memcpy(&v,&raw,8);return float(v-bias);}
template<class T>bool equal(const std::optional<T>&a,const std::optional<T>&b){return bool(a)==bool(b)&&(!a||std::memcmp(&*a,&*b,sizeof(T))==0);}
bool equal(const BattleWeaponDrawOutput&a,const BattleWeaponDrawOutput&b){
 if(!equal(a.helpX,b.helpX)||!equal(a.helpY,b.helpY)||a.classes.size()!=b.classes.size())return false;
 for(size_t i=0;i<a.classes.size();++i){const auto&x=a.classes[i];const auto&y=b.classes[i];if(!equal(x.position,y.position)||!equal(x.matrix,y.matrix)||!equal(x.parameter,y.parameter))return false;}
 for(size_t i=0;i<7;++i){const auto&x=a.weapons[i];const auto&y=b.weapons[i];if(!equal(x.matrix,y.matrix)||!equal(x.label,y.label)||!equal(x.icon,y.icon)||!equal(x.enabled,y.enabled)||!equal(x.cost,y.cost)||!equal(x.bp,y.bp))return false;}
 return true;
}
bool same(const BattleSubIconServices&a,const BattleSubIconServices&b){const auto&x=a.math;const auto&y=b.math;const auto&m=x.memory;const auto&n=y.memory;
 return a.user==b.user&&a.current==b.current&&a.truncate==b.truncate&&a.fma==b.fma&&a.request==b.request&&x.user==y.user&&x.camera==y.camera&&x.center==y.center&&x.offset==y.offset&&x.interpolate==y.interpolate&&x.translation==y.translation&&x.rotation==y.rotation&&x.scale==y.scale&&x.concat==y.concat&&x.icons==y.icons&&x.partyButton==y.partyButton&&x.pinchMark==y.pinchMark&&m.user==n.user&&m.byte==n.byte&&m.audienceLifecycle.user==n.audienceLifecycle.user&&m.audienceLifecycle.event==n.audienceLifecycle.event&&m.memory.user==n.memory.user&&m.memory.word==n.memory.word&&m.memory.half==n.memory.half&&m.memory.storeWord==n.memory.storeWord&&m.memory.storeHalf==n.memory.storeHalf&&m.memory.storeByte==n.memory.storeByte&&m.memory.flushBlock==n.memory.flushBlock&&m.memory.systemCall==n.memory.systemCall;
}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
// work98 scroll; work0 captured cursor; seven physical rows at worka4 flags.
// First fctiwz computes a fixed fractional offset. Each row separately reloads
// scroll/count and performs its own fctiwz gate; do not reuse first conversion.
struct BattleSubIconCursor::State {
 BattleSubIconServices services;const BattleWeaponDrawOutput* borrowed=nullptr;BattleWeaponDrawOutput input;
 uint32_t work=0,flag=0,stackFlag=0,pc=0x8011903c,pending=0,cursor=0,row=0,count=0;
 float scroll=0,fraction=0,a=0,b=0,c=0;std::array<float,3> xyz{};BattleActMatrix translation{},matrix{};
 bool invalid=false;std::optional<BattleSubIconRasterRequest> request;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t address)const{return services.math.memory.memory.word(services.math.memory.memory.user,address);}
 // r2=80427440; source offsets -20416/-20284/-20296/-20500 hold
 // spacing24/base35/X100/Z0. Bias reads hi then lo at802ef780/784.
 float constant(int32_t offset)const{return scalar(word(0x80427440u+uint32_t(offset)));}
 double bias()const{const auto hi=word(0x802ef780),lo=word(0x802ef784);const uint64_t raw=(uint64_t(hi)<<32)|lo;double d;std::memcpy(&d,&raw,8);return d;}
};
BattleSubIconCursor::BattleSubIconCursor()=default;
BattleSubIconCursor::~BattleSubIconCursor()=default;
bool BattleSubIconCursor::active()const{return bool(state_);}
bool BattleSubIconCursor::stale()const{return state_&&state_->invalid;}
uint32_t BattleSubIconCursor::pending()const{return state_?state_->pending:0;}
bool BattleSubIconCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Pin borrowed output ADDRESS and optional-value BIT patterns,not just row
// count. Actual current predicate guards source lifetime; caller freezes mapped
// storage inside synchronous read groups. Missing private cells hold at use.
bool resumePackedBattleSubIcon(uint32_t work,const BattleWeaponDrawOutput& input,uint32_t flag,uint32_t stackFlag,const BattleSubIconServices&s,BattleSubIconCursor&cursor){
 if(cursor.invoking_||!s.current||!s.math.memory.memory.word)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.work!=work||c.borrowed!=&input||c.flag!=flag||c.stackFlag!=stackFlag||!same(c.services,s)||!equal(c.input,input))return false;}
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleSubIconCursor::State>();auto&c=*cursor.state_;c.services=s;c.borrowed=&input;c.input=input;c.work=work;c.flag=flag;c.stackFlag=stackFlag;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.pc){
 // Preserve actual current-battle/registry lookup even though its unit result
 // is unused here. Initial scroll is captured before the first truncation hold.
 case 0x8011903c:{const auto battle=currentBattleAddress(s.math.memory);const auto id=c.word(battle+0x420);(void)battleRegistryUnit(battle,sw(id),s.math.memory);c.scroll=scalar(c.word(c.work+0x98));c.pc=0x80119080;continue;}
 case 0x80119080:{c.pending=c.pc;if(!s.truncate)return false;const auto v=s.truncate(s.user,c.scroll);if(!c.valid()||!v)return false;
  const auto d=c.bias();const auto spacing=c.constant(-20416);c.cursor=c.word(c.work);c.fraction=mul(spacing,sub(c.scroll,converted(uint32_t(*v),d)));c.pc=0x801190c0;continue;}
 case 0x801190c0:
  if(c.row>=7){cursor.state_.reset();return true;}
  c.scroll=scalar(c.word(c.work+0x98));c.count=c.word(c.cursor+8);c.pc=0x801190c8;continue;
 // Signed wrapped(truncated fresh scroll + physical row) must be<count,
 // then worka4[row] bit1. Source Y is -fma(24,row,-35)+initial fraction,
 // then translation CONCAT supplied row matrix; do not algebraically fold it.
 case 0x801190c8:{c.pending=c.pc;if(!s.truncate)return false;const auto v=s.truncate(s.user,c.scroll);if(!c.valid()||!v)return false;
  if(sw(uint32_t(*v)+c.row)>=sw(c.count)){++c.row;c.pc=0x801190c0;continue;}
  if(!(c.word(c.work+0xa4+4*c.row)&1)){++c.row;c.pc=0x801190c0;continue;}
  const auto d=c.bias();c.a=c.constant(-20416);const auto base=c.constant(-20284);c.xyz[0]=c.constant(-20296);c.xyz[2]=c.constant(-20500);c.b=converted(c.row,d);c.c=-base;c.pc=0x80119124;continue;}
 case 0x80119124:{c.pending=c.pc;if(!s.fma)return false;const auto v=s.fma(s.user,c.a,c.b,c.c);if(!c.valid()||!v)return false;c.xyz[1]=add(-*v,c.fraction);c.pc=0x8011912c;continue;}
 case 0x8011912c:{c.pending=0x8029e230;if(!s.math.translation)return false;const auto v=s.math.translation(s.math.user,c.xyz);if(!c.valid()||!v)return false;c.translation=*v;c.pc=0x80119144;continue;}
 case 0x80119144:{c.pending=0x8029dd64;const auto& matrix=c.input.weapons[c.row].matrix;if(!matrix||!s.math.concat)return false;const auto v=s.math.concat(s.math.user,c.translation,*matrix);if(!c.valid()||!v)return false;c.matrix=*v;c.pc=0x80119170;continue;}
 // Only reached rows require actual label/icon/enabled/cost/BP optionals.
 // Copy the whole request once; leaf waits retain it and never repeat math.
 case 0x80119170:{c.pending=0x8011847c;if(!c.request){const auto&slot=c.input.weapons[c.row];if(!slot.label||!slot.icon||!slot.enabled||!slot.cost||!slot.bp)return false;
  c.request=BattleSubIconRasterRequest{};auto&q=*c.request;q.work=c.work;q.row=c.row;q.matrix=c.matrix;q.label=*slot.label;q.icon=*slot.icon;q.enabled=*slot.enabled;q.cost=*slot.cost;q.bp=*slot.bp;q.flag=c.flag;q.stackFlag=c.stackFlag;}
  if(!s.request)return false;
  const auto v=s.request(s.user,*c.request);if(!c.valid()||!v)return false;c.request.reset();++c.row;c.pc=0x801190c0;continue;}
 default:return false;
 }}
}
}
