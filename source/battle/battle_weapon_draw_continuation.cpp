#include "battle_weapon_draw_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
uint32_t bits(float n){uint32_t v;std::memcpy(&v,&n,4);return v;}
float sourceAdd(float a,float b){volatile float v=a+b;return v;}
float sourceSub(float a,float b){volatile float v=a-b;return v;}
float sourceMul(float a,float b){volatile float v=a*b;return v;}
float sourceDiv(float a,float b){volatile float v=a/b;return v;}
float converted(uint32_t n,double bias){const uint64_t raw=(uint64_t(0x43300000)<<32)|(n^0x80000000u);double value;std::memcpy(&value,&raw,8);return float(value-bias);}
struct Running{bool& f;explicit Running(bool& x):f(x){f=true;}~Running(){f=false;}};
bool same(const BattleWeaponDrawServices& a,const BattleWeaponDrawServices& b){
 const auto& x=a.math;const auto& y=b.math;const auto& m=x.memory;const auto& n=y.memory;
 return a.user==b.user&&a.current==b.current&&a.request==b.request&&a.truncate==b.truncate&&a.fma==b.fma&&a.identity==b.identity&&x.user==y.user&&x.camera==y.camera&&x.center==y.center&&x.offset==y.offset&&x.interpolate==y.interpolate&&x.translation==y.translation&&x.rotation==y.rotation&&x.scale==y.scale&&x.concat==y.concat&&x.icons==y.icons&&x.partyButton==y.partyButton&&x.pinchMark==y.pinchMark&&
 m.user==n.user&&m.byte==n.byte&&m.audienceLifecycle.user==n.audienceLifecycle.user&&m.audienceLifecycle.event==n.audienceLifecycle.event&&m.memory.user==n.memory.user&&m.memory.word==n.memory.word&&m.memory.half==n.memory.half&&m.memory.storeWord==n.memory.storeWord&&m.memory.storeHalf==n.memory.storeHalf&&m.memory.storeByte==n.memory.storeByte&&m.memory.flushBlock==n.memory.flushBlock&&m.memory.systemCall==n.memory.systemCall;
}
struct MathInput{uint32_t fn=0;int32_t mode=0,step=0,duration=0;float a=0,b=0,c=0;uint32_t axis=0;std::array<float,3> xyz{};BattleActMatrix left{},right{};};
}
struct BattleWeaponDrawCursor::State {
 BattleWeaponDrawServices services;uint32_t camera=0,passedBattle=0,pc=0,battle=0,work=0,weaponCursor=0,unit=0,selectedWeapon=0;
 uint32_t commonB=0,w=0,cursor=0,classCursor=0,index=0,row=0,rowState=0,visible=0,oldTop=0,count=0,pending=0;
 uint32_t subIndex=0,subRow=0,subState=0,commonMode=0,rowOrdinal=0;
 float angle=0,factor=0,zero=0;uint32_t subTimer=0,slotDuration=0,metadataCount=0,rowOffset=0,costWeapon=0,helpKey=0;std::array<float,3> vector{};std::array<float,2> offset{};
 BattleActMatrix scale{},translation{};bool invalid=false;
 BattleWeaponDrawOutput output;std::optional<MathInput> math;std::optional<BattleWeaponDrawRequest> held;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return services.math.memory.memory.word(services.math.memory.memory.user,a);}
 uint16_t half(uint32_t a)const{return services.math.memory.memory.half(services.math.memory.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.math.memory.byte(services.math.memory.user,a);}
 void store(uint32_t a,uint32_t n)const{services.math.memory.memory.storeWord(services.math.memory.memory.user,a,n);}
 void put(uint32_t a,uint8_t n)const{services.math.memory.memory.storeByte(services.math.memory.memory.user,a,n);}
 float real(uint32_t a)const{return scalar(word(a));}
 float constant(int32_t n)const{return real(0x80427440u+uint32_t(n));}
 double bias()const{const auto hi=word(0x802ef780),lo=word(0x802ef784);const uint64_t n=(uint64_t(hi)<<32)|lo;double d;std::memcpy(&d,&n,8);return d;}
 int32_t duration(uint32_t n)const{return sw(word(word(0x80415fd0)+4)*n)/60;}
 template<class Build>void prepare(uint32_t fn,Build build){pending=fn;if(!math){math=MathInput{};math->fn=fn;build(*math);}}
 template<class Build>std::optional<float> interpolate(Build build){prepare(0x80006c8c,build);const auto& q=*math;const auto& p=services.math;if(!p.interpolate)return {};auto v=p.interpolate(p.user,q.mode,q.step,q.duration,q.a,q.b);if(!valid()||!v)return {};
  math.reset();return v;}
 template<class Build>std::optional<BattleActMatrix> matrix(uint32_t fn,Build build){prepare(fn,build);const auto& q=*math;const auto& p=services.math;std::optional<BattleActMatrix> v;
  if(fn==0x8029e230&&p.translation)v=p.translation(p.user,q.xyz);
  else if(fn==0x8029e2b0&&p.scale)v=p.scale(p.user,q.a,q.b,q.c);
  else if(fn==0x8029dd64&&p.concat)v=p.concat(p.user,q.left,q.right);
  else if(fn==0x8029dff0&&p.rotation)v=p.rotation(p.user,q.a,q.axis);
  else if(fn==0x8029dd04&&services.identity)v=services.identity(services.user);
  if(!valid()||!v)return {};
  math.reset();return v;
 }
 std::optional<std::array<float,3>> center(){pending=0x80119cf0;const auto& p=services.math;if(!p.center)return {};const auto v=p.center(p.user);if(!valid())return {};return v;}
 std::optional<std::array<float,2>> ringOffset(){prepare(0x80119c7c,[&](auto&q){q.a=angle;});const auto& p=services.math;if(!p.offset)return {};const auto v=p.offset(p.user,math->a);if(!valid()||!v)return {};
  math.reset();return v;}
 template<class Build>std::optional<uint32_t> request(uint32_t fn,Build build){pending=fn;if(!held){held=BattleWeaponDrawRequest{};held->function=fn;held->callsite=pc;build(*held);}if(!services.request)return {};const auto v=services.request(services.user,*held);if(!valid()||!v)return {};held.reset();return v;}
 std::optional<int32_t> truncate(uint32_t site){prepare(site,[&](auto&q){q.a=real(work+0x98);metadataCount=word(weaponCursor+8);});if(!services.truncate)return {};const auto v=services.truncate(services.user,math->a);if(!valid()||!v)return {};
  math.reset();return v;}
 void subStates(uint32_t state){for(uint32_t i=0;i<7;++i){store(w+0xc0+4*i,state);store(w+0xdc+4*i,0);}}
 void sub1(){
  if(visible==6){const auto n=word(cursor+8);
   if(sw(n)>6&&oldTop==n-6&&word(cursor+4)==0&&word(cursor)!=1){
    store(w+0xc,30);const auto d=bias();store(w+0x10,0);const auto zero=constant(-20500);
    const auto count=word(cursor+8);store(w+0x9c,bits(converted(count-6,d)));store(w+0xa0,bits(zero));subStates(30);return;
   }
  }
  if(visible==6){const auto n=word(cursor+8);
   if(sw(n)>6&&oldTop==0&&word(cursor+4)==n-6&&word(cursor)!=5){
    store(w+0xc,40);const auto zero=constant(-20500);store(w+0x10,0);const auto d=bias();
    store(w+0x9c,bits(zero));const auto count=word(cursor+8);store(w+0xa0,bits(converted(count-6,d)));subStates(40);return;
   }
  }
  const auto top=word(cursor+4);
  if(top==oldTop+1){const auto d=bias();store(w+0xc,10);store(w+0x10,0);store(w+0x9c,bits(converted(oldTop,d)));store(w+0xa0,bits(converted(oldTop+1,d)));subStates(10);return;}
  if(top==oldTop-1){const auto d=bias();store(w+0xc,20);store(w+0x10,0);store(w+0x9c,bits(converted(oldTop,d)));store(w+0xa0,bits(converted(oldTop-1,d)));subStates(20);}
 }
 void nextClass(){++index;pc=0x8011741c;}
 void nextSub(){++subIndex;pc=0x801175f4;}
};
BattleWeaponDrawCursor::BattleWeaponDrawCursor()=default;
BattleWeaponDrawCursor::~BattleWeaponDrawCursor()=default;
bool BattleWeaponDrawCursor::active()const{return bool(state_);}
bool BattleWeaponDrawCursor::stale()const{return state_&&state_->invalid;}
uint32_t BattleWeaponDrawCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponDrawCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleWeaponQueuedDraw(uint32_t camera,uint32_t battle,const BattleWeaponDrawServices& s,BattleWeaponDrawCursor& cursor){
 const auto& m=s.math.memory.memory;if(cursor.invoking_||!s.current||!m.word||!m.half||!m.storeWord||!m.storeByte||!s.math.memory.byte)return false;
 if(cursor.state_&&(cursor.state_->camera!=camera||cursor.state_->passedBattle!=battle||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleWeaponDrawCursor::State>();auto&c=*cursor.state_;c.services=s;c.camera=camera;c.passedBattle=battle;c.pc=0x80115dbc;}
 auto& c=*cursor.state_;const auto finish=[&](){if(!c.valid())return false;cursor.state_.reset();return true;};
 for(;;){if(!c.valid())return false;switch(c.pc){
 case 0x80115dbc:{
  c.battle=currentBattleAddress(s.math.memory);c.work=c.word(c.battle+0x1c78);const auto actor=c.word(c.battle+0x420);c.weaponCursor=c.word(c.work);
  c.unit=battleRegistryUnit(c.battle,sw(actor),s.math.memory);
  const auto selection=c.word(c.weaponCursor),rows=c.word(c.work+8);c.selectedWeapon=c.word(rows+28*selection);c.pc=0x80116e98;continue;
 }
 case 0x80116e98:{
  c.commonB=currentBattleAddress(s.math.memory);c.w=c.word(c.commonB+0x1c78);const auto actor=c.word(c.commonB+0x420);c.cursor=c.word(c.w);
  const auto u=battleRegistryUnit(c.commonB,sw(actor),s.math.memory);
  c.classCursor=c.commonB+(c.word(u+8)==222?0x1bb4:0x1bc0);
  const auto count=c.word(c.cursor+8);c.visible=sw(count)<6?count:6;
  c.oldTop=c.word(c.cursor+4);const auto selected=c.word(c.cursor);
  if(sw(selected)<=sw(c.oldTop))c.store(c.cursor+4,selected-1);
  else if(sw(selected)>=sw(c.oldTop+c.visible-1))c.store(c.cursor+4,selected-c.visible+2);
  if(sw(c.word(c.cursor+4))<0)c.store(c.cursor+4,0);
  const auto liveCount=c.word(c.cursor+8),top=c.word(c.cursor+4);
  if(sw(top)>=sw(liveCount-c.visible))c.store(c.cursor+4,liveCount-c.visible);
  const auto x=c.constant(-20448),y=c.constant(-20444);c.output.helpX=x;c.output.helpY=y;
  c.commonMode=c.word(c.w+0xc);
  const auto target=c.commonMode<=40?c.word(0x803636e8+4*c.commonMode):0x80117124;
  if(target==0x80116f94){c.put(c.w+0x14,1);const auto top2=c.word(c.cursor+4);const auto d=c.bias();c.store(c.w+0x98,bits(converted(top2,d)));
   const auto n=c.word(c.classCursor+8);uint32_t done=0;
   for(uint32_t i=0;sw(i)<sw(n);++i){const auto state=c.word(c.w+0x34+4*i);if(state==1||state==11)++done;}
   uint32_t count2=0;for(uint32_t i=0;i<7;++i)if(c.word(c.w+0xc0+4*i)==2)++count2;
   if(done==n&&count2==7)c.store(c.w+0xc,2);
   c.sub1();
  }else if(target==0x80117058){c.put(c.w+0x14,1);const auto top2=c.word(c.cursor+4);const auto d=c.bias();c.store(c.w+0x98,bits(converted(top2,d)));c.sub1();}
  else if(target==0x8011709c){
   if(c.commonMode==10||c.commonMode==20)c.put(c.w+0x14,1);
   uint32_t ready=0;for(uint32_t i=0;i<7;++i)if(c.word(c.w+0xc0+4*i)==2)++ready;
   if(ready==7){c.store(c.w+0xc,2);const auto d=c.bias();const auto top2=c.word(c.cursor+4);c.store(c.w+0x98,bits(converted(top2,d)));}c.sub1();
  }
  c.store(c.w+0x1c,0);c.index=0;c.pc=0x8011741c;continue;
 }
 case 0x8011741c:
  c.count=c.word(c.classCursor+8);if(sw(c.index)>=sw(c.count)){c.subIndex=0;c.pc=0x801175f4;continue;}
  // Original parent private arrays contain five class entries. An invalid
  // overflow count is outside this typed native private-output domain.
  if(c.index>=5){c.pending=0x80116e80;return false;}
  if(c.output.classes.size()<=c.index)c.output.classes.resize(c.index+1);
  c.row=c.w+4*c.index;c.rowState=c.word(c.row+0x34);
  if(c.rowState==0){c.store(c.row+0x20,c.word(c.row+0x20)|1);c.store(c.row+0x20,c.word(c.row+0x20)|2);c.pc=0x80117194;continue;}
  if(c.rowState==1){const auto f=c.word(c.row+0x20);const auto x=c.constant(-20440),y=c.constant(-20460);c.store(c.row+0x20,f|1);const auto z=c.constant(-20500);c.store(c.row+0x20,c.word(c.row+0x20)|2);c.prepare(0x8029e230,[&](auto&q){q.xyz={x,y,z};});c.pc=0x801172c4;continue;}
  if(c.rowState==10){const auto f=c.word(c.row+0x20);const auto radius=c.constant(-20492);c.store(c.row+0x20,f|1);const auto period=c.constant(-20488);c.store(c.row+0x20,c.word(c.row+0x20)&~2u);
   const auto n=c.word(c.classCursor+8),ordinal=c.word(c.row+0x84);
   c.angle=sourceDiv(sourceMul(radius,c.real(0x803635d8+20*n+4*ordinal-60)),period);c.pc=0x80117330;continue;}
  if(c.rowState==11)c.store(c.row+0x20,c.word(c.row+0x20)&~1u);
  c.nextClass();continue;
 case 0x80117194:{const auto v=c.center();if(!v)return false;c.vector=*v;c.pc=0x801171bc;continue;}
 case 0x801171bc:{const auto v=c.interpolate([&](auto&q){q.mode=4;const auto gp=c.word(0x80415fd0);q.a=c.vector[0];q.duration=sw(c.word(gp+4)*12)/60;q.b=c.constant(-20464);q.step=sw(c.word(c.row+0x48));});if(!v)return false;c.vector[0]=*v;c.pc=0x801171e8;continue;}
 case 0x801171e8:{const auto v=c.interpolate([&](auto&q){q.mode=4;q.b=c.constant(-20460);q.a=c.vector[1];q.duration=c.duration(12);q.step=sw(c.word(c.row+0x48));});if(!v)return false;c.vector[1]=*v;c.pc=0x80117208;continue;}
 case 0x80117208:{const auto v=c.matrix(0x8029e230,[&](auto&q){q.xyz={sourceAdd(c.constant(-20468),c.vector[0]),c.vector[1],c.vector[2]};});if(!v)return false;c.output.classes[c.index].matrix=*v;c.pc=0x80117230;continue;}
 case 0x80117230:{const auto v=c.interpolate([&](auto&q){q.mode=0;const auto gp=c.word(0x80415fd0);q.a=c.constant(-20500);q.duration=sw(c.word(gp+4)*12)/60;q.b=c.constant(-20476);q.step=sw(c.word(c.row+0x48));});if(!v)return false;
  c.output.classes[c.index].parameter=*v;const auto period=c.constant(-20476);if(*v>period)c.output.classes[c.index].parameter=sourceSub(*v,period);
  if(sw(c.word(c.row+0x48))>=3)c.store(c.w+0x1c,1);
  c.store(c.row+0x48,c.word(c.row+0x48)+1);const auto gp=c.word(0x80415fd0),step=c.word(c.row+0x48);const auto duration=sw(c.word(gp+4)*12)/60;
  if(sw(step)>duration)c.store(c.row+0x34,1);
  c.output.classes[c.index].position=c.vector;c.nextClass();continue;
 }
 case 0x801172c4:{const auto v=c.matrix(0x8029e230,[&](auto&q){q.xyz={c.constant(-20440),c.constant(-20460),c.constant(-20500)};});if(!v)return false;c.output.classes[c.index].matrix=*v;c.output.classes[c.index].parameter=c.constant(-20476);c.store(c.w+0x1c,1);c.nextClass();continue;}
 case 0x80117330:{const auto v=c.center();if(!v)return false;c.vector=*v;c.pc=0x80117340;continue;}
 case 0x80117340:{const auto v=c.ringOffset();if(!v)return false;c.offset=*v;
  c.vector[0]=sourceAdd(c.vector[0],c.offset[0]);const auto d=c.bias();const auto slope=c.constant(-20456),one=c.constant(-20452);c.zero=c.constant(-20500);
  c.vector[1]=sourceAdd(c.vector[1],c.offset[1]);c.vector[2]=c.offset[1];
  const auto timer=converted(c.word(c.row+0x48),d);
  c.prepare(0x8011739c,[&](auto&q){q.a=slope;q.b=timer;q.c=-one;});c.pc=0x8011739c;continue;
 }
 case 0x8011739c:{if(!s.fma)return false;const auto& q=*c.math;const auto v=s.fma(s.user,q.a,q.b,q.c);if(!c.valid()||!v)return false;c.factor=-*v;c.math.reset();if(c.factor<=c.zero)c.store(c.row+0x34,11);c.pc=0x801173c0;continue;}
 case 0x801173c0:{const auto v=c.matrix(0x8029e2b0,[&](auto&q){q.a=c.factor;q.b=c.factor;q.c=c.constant(-20452);});if(!v)return false;c.scale=*v;c.pc=0x801173d4;continue;}
 case 0x801173d4:{const auto v=c.matrix(0x8029e230,[&](auto&q){q.xyz=c.vector;});if(!v)return false;c.translation=*v;c.pc=0x801173e4;continue;}
 case 0x801173e4:{const auto v=c.matrix(0x8029dd64,[&](auto&q){q.left=c.translation;q.right=c.scale;});if(!v)return false;c.output.classes[c.index].matrix=*v;c.output.classes[c.index].position=c.vector;c.output.classes[c.index].parameter=c.constant(-20500);c.store(c.row+0x48,c.word(c.row+0x48)+1);c.nextClass();continue;}
 case 0x801175f4:
  if(c.subIndex>=7){c.rowOrdinal=0;c.pc=0x80115e1c;continue;}
  c.subRow=c.w+4*c.subIndex;c.subState=c.word(c.subRow+0xc0);
  {const auto target=c.subState<=40?c.word(0x80363644+4*c.subState):0x801175ec;
  if(target==0x8011745c){c.store(c.subRow+0xa4,c.word(c.subRow+0xa4)&~1u);c.put(c.w+0x114+c.subIndex,255);c.nextSub();continue;}
  if(target==0x80117478){c.store(c.subRow+0xa4,c.word(c.subRow+0xa4)&~1u);c.put(c.w+0x114+c.subIndex,255);c.subTimer=c.word(c.subRow+0xdc);
   c.pc=sw(c.subTimer)>=0&&c.subIndex!=6?0x801174c4:0x801174e8;continue;}
  if(target==0x80117520){c.put(c.w+0x114+c.subIndex,255);
   if(c.subIndex==6){c.store(c.subRow+0xa4,c.word(c.subRow+0xa4)&~1u);c.nextSub();continue;}
   c.store(c.subRow+0xa4,c.word(c.subRow+0xa4)|1);c.pc=0x80117544;continue;}
  if(target==0x8011755c){
   c.slotDuration=12;if(c.subState==30||c.subState==40)c.slotDuration=12+uint32_t(sw(c.word(c.cursor+8)-6)/2);
   c.store(c.subRow+0xa4,c.word(c.subRow+0xa4)|1);c.pc=0x801175a4;continue;
  }
  c.nextSub();continue;}
 case 0x801174c4:{const auto v=c.interpolate([&](auto&q){const auto gp=c.word(0x80415fd0);q.mode=0;q.a=c.constant(-20480);q.duration=sw(c.word(gp+4)*12)/60;q.b=c.constant(-20500);q.step=sw(c.subTimer);});if(!v)return false;
  const auto flags=c.word(c.subRow+0xa4);const auto scale=c.constant(-20472);const auto angle=sourceMul(scale,*v);c.store(c.subRow+0xa4,flags|1);
  c.prepare(0x8029dff0,[&](auto&q){q.a=angle;q.axis=121;});c.pc=0x801174e4;continue;
 }
 case 0x801174e4:{const auto v=c.matrix(0x8029dff0,[](auto&){});if(!v)return false;c.output.weapons[c.subIndex].matrix=*v;c.pc=0x801174e8;continue;}
 case 0x801174e8:{c.store(c.subRow+0xdc,c.word(c.subRow+0xdc)+1);const auto gp=c.word(0x80415fd0),timer=c.word(c.subRow+0xdc);const auto duration=sw(c.word(gp+4)*12)/60;if(sw(timer)>duration)c.store(c.subRow+0xc0,2);c.nextSub();continue;}
 case 0x80117544:{const auto v=c.matrix(0x8029dd04,[](auto&){});if(!v)return false;c.output.weapons[c.subIndex].matrix=*v;c.nextSub();continue;}
 case 0x801175a4:{const auto v=c.matrix(0x8029dd04,[](auto&){});if(!v)return false;c.output.weapons[c.subIndex].matrix=*v;c.pc=0x801175bc;continue;}
 case 0x801175bc:{const auto v=c.interpolate([&](auto&q){q.a=c.real(c.w+0x9c);q.b=c.real(c.w+0xa0);q.mode=4;q.duration=sw(c.slotDuration);q.step=sw(c.word(c.subRow+0xdc));});if(!v)return false;
  c.store(c.w+0x98,bits(*v));c.store(c.subRow+0xdc,c.word(c.subRow+0xdc)+1);if(sw(c.word(c.subRow+0xdc))>sw(c.slotDuration)){c.store(c.subRow+0xc0,2);c.store(c.subRow+0xdc,0);}c.nextSub();continue;
 }
 case 0x80115e1c:{
  if(c.rowOrdinal>=7){c.pc=0x80115ef4;continue;}
  const auto scroll=c.truncate(0x80115e1c);if(!scroll)return false;
  const auto count=c.metadataCount,rank=uint32_t(*scroll)+c.rowOrdinal;
  if(sw(rank)>=sw(count)){++c.rowOrdinal;continue;}
  c.rowOffset=28*rank;auto& slot=c.output.weapons[c.rowOrdinal];
  slot.label=c.word(c.word(c.work+8)+c.rowOffset+4);slot.icon=c.half(c.battle+0x171c+c.rowOffset+0x8c);
  slot.enabled=c.word(c.word(c.work+8)+c.rowOffset+8);
  c.costWeapon=c.word(c.word(c.work+8)+c.rowOffset);c.pc=c.costWeapon?0x80115e84:0x80115e94;continue;
 }
 case 0x80115e84:{const auto v=c.request(0x80126968,[&](auto&q){q.arguments[q.count++]=c.unit;q.arguments[q.count++]=c.costWeapon;});if(!v)return false;c.output.weapons[c.rowOrdinal].cost=*v;c.pc=0x80115ea0;continue;}
 case 0x80115e94:c.output.weapons[c.rowOrdinal].cost=0;c.pc=0x80115ea0;continue;
 case 0x80115ea0:{const auto weapon=c.word(c.word(c.work+8)+c.rowOffset);c.output.weapons[c.rowOrdinal].bp=weapon?c.byte(weapon+0x12):0;++c.rowOrdinal;c.pc=0x80115e1c;continue;}
 case 0x80115ef4:{const auto v=c.request(0x80119abc,[&](auto&q){q.arguments[q.count++]=c.work;q.arguments[q.count++]=c.battle+0x1724;q.output=&c.output;});if(!v)return false;c.pc=0x80115f20;continue;}
 case 0x80115f20:{const auto v=c.request(0x80119024,[&](auto&q){q.arguments[q.count++]=c.work;q.arguments[q.count++]=0;q.arguments[q.count++]=0;q.output=&c.output;});if(!v)return false;c.pc=0x80115f48;continue;}
 case 0x80115f48:{const auto v=c.request(0x80117a6c,[&](auto&q){q.cursorWindow=std::array<uint32_t,3>{c.word(0x802ef750),c.word(0x802ef754),c.word(0x802ef758)};});if(!v)return false;c.pc=0x80115f4c;continue;}
 case 0x80115f4c:{
  if(c.selectedWeapon){const auto item=c.word(c.selectedWeapon+8);
   if(item)c.helpKey=c.word(0x803108a8+40*item+8);
   else if(c.selectedWeapon==0x80365488){const auto index=c.word(c.weaponCursor);c.helpKey=c.word(0x803108a8+40*c.word(c.word(c.work+8)+28*index+0x14)+8);}
   else c.helpKey=c.word(c.selectedWeapon+0xc);
  }else{const auto index=c.word(c.weaponCursor);c.helpKey=c.word(0x803108a8+40*c.word(c.word(c.work+8)+28*index+0x14)+8);}
  if(!c.helpKey)return finish();
  c.pc=0x80115ff4;continue;
 }
 case 0x80115ff4:{const auto v=c.request(0x80117c1c,[&](auto&q){q.arguments[q.count++]=c.helpKey;q.x=c.output.helpX;q.y=c.output.helpY;});if(!v)return false;return finish();}
 default:return false;
 }}
}
}
