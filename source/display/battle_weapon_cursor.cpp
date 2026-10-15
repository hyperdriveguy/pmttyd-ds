#include "battle_weapon_cursor.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t n){return n<0x80000000?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float n=a+b;return n;}
float sub(float a,float b){volatile float n=a-b;return n;}
bool same(const SourceIconGXServices&a,const SourceIconGXServices&b){return a.user==b.user&&a.word==b.word&&a.half==b.half&&a.matrix==b.matrix&&a.call==b.call&&a.quad==b.quad;}
bool same(const BattleWeaponCursorServices&a,const BattleWeaponCursorServices&b){
 const auto&m=a.actors.memory;const auto&n=b.actors.memory;const auto&i=a.icons;const auto&j=b.icons;
 return a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.interpolate==b.interpolate&&a.fma==b.fma&&
 a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&
 m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall&&
 i.user==j.user&&i.word==j.word&&i.half==j.half&&i.currentCamera==j.currentCamera&&i.matrix==j.matrix&&i.draw==j.draw&&i.gx==j.gx;
}
struct Guard{bool& flag;explicit Guard(bool&f):flag(f){flag=true;}~Guard(){flag=false;}};
}
struct BattleWeaponCursor::State {
 BattleWeaponCursorServices owner;SourceIconGXServices gx,boundGx;
 SourceIconMatrixServices icons;
 const std::array<uint32_t,3>* window=nullptr;
 uint32_t pc=0x80117a84,pending=0,battle=0,work=0,cursor=0;bool invalid=false;
 int32_t step=0;float start=0,end=0,bounce=0,a=0,b=0,c=0;
 SourceIconPositionRequest request;SourceIconPositionCursor child;
 bool valid(){if(invalid)return false;if(!owner.current||!owner.current(owner.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a){if(!valid())return 0;auto n=owner.actors.memory.word(owner.actors.memory.user,a);return valid()?n:0;}
 void store(uint32_t a,uint32_t n){if(valid()){owner.actors.memory.storeWord(owner.actors.memory.user,a,n);valid();}}
 float constant(int32_t off){return real(word(0x80427440u+uint32_t(off)));}
 template<class F>auto guarded(F f)->decltype(f()){
  using R=decltype(f());if(!valid())return R{};auto value=f();return valid()?value:R{};
 }
 void bind(){
  icons=owner.icons;icons.user=this;
  icons.word=[](void*p,uint32_t a){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.owner.icons.word?c.owner.icons.word(c.owner.icons.user,a):std::optional<uint32_t>{};});};
  icons.half=[](void*p,uint32_t a){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.owner.icons.half?c.owner.icons.half(c.owner.icons.user,a):std::optional<uint16_t>{};});};
  icons.currentCamera=[](void*p){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.owner.icons.currentCamera?c.owner.icons.currentCamera(c.owner.icons.user):std::optional<uint32_t>{};});};
  icons.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.owner.icons.matrix&&c.owner.icons.matrix(c.owner.icons.user,q);});};
  if(owner.icons.draw)icons.draw=[](void*p,const BattleActMatrix&m,const SourceMatrixIcon&i){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.owner.icons.draw(c.owner.icons.user,m,i);});};
  if(owner.icons.gx){
   gx=*owner.icons.gx;boundGx=gx;boundGx.user=this;
   boundGx.word=[](void*p,uint32_t a){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.gx.word?c.gx.word(c.gx.user,a):std::optional<uint32_t>{};});};
   boundGx.half=[](void*p,uint32_t a){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.gx.half?c.gx.half(c.gx.user,a):std::optional<uint16_t>{};});};
   boundGx.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.gx.matrix&&c.gx.matrix(c.gx.user,q);});};
   boundGx.call=[](void*p,const SourceIconGXCall&q){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.gx.call?c.gx.call(c.gx.user,q):std::optional<uint32_t>{};});};
   boundGx.quad=[](void*p,const SourceIconGXQuad&q){auto&c=*static_cast<State*>(p);return c.guarded([&](){return c.gx.quad&&c.gx.quad(c.gx.user,q);});};
   icons.gx=&boundGx;
  }
 }
 bool icon(){pending=0x8001a918;if(!valid())return false;const bool done=resumeSourceIconPosition(request,icons,child);if(!valid())return false;if(!done){if(child.pending)pending=child.pending;return false;}pending=0;return true;}
};
BattleWeaponCursor::BattleWeaponCursor()=default;BattleWeaponCursor::~BattleWeaponCursor()=default;
bool BattleWeaponCursor::active()const{return bool(state_);}bool BattleWeaponCursor::stale()const{return state_&&state_->invalid;}
uint32_t BattleWeaponCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleWeaponCursor(const std::array<uint32_t,3>&window,const BattleWeaponCursorServices&s,BattleWeaponCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.actors.memory.word||!s.actors.memory.storeWord||!s.current)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.window!=&window||!same(c.owner,s)||(s.icons.gx&&!same(c.gx,*s.icons.gx)))return false;}
 Guard guard(cursor.invoking_);
 if(!cursor.state_){if(!s.current(s.user))return false;cursor.state_=std::make_unique<BattleWeaponCursor::State>();auto&c=*cursor.state_;c.owner=s;c.bind();c.window=&window;}
 auto&c=*cursor.state_;if(!c.valid())return false;
 for(;;){if(!c.valid())return false;switch(c.pc){
 case 0x80117a84:
  // Source load order: global B, zero, W, ten, C, timer.
  c.battle=currentBattleAddress(s.actors);c.start=c.constant(-20500);c.work=c.word(c.battle+0x1c78);c.end=c.constant(-20436);c.cursor=c.word(c.work);c.step=signedWord(c.word(c.work+0x11c));c.pc=0x80117aa8;continue;
 case 0x80117aa8:{
  c.pending=0x80006c8c;if(!s.interpolate||!c.valid())return false;const auto v=s.interpolate(s.user,4,c.step,6,c.start,c.end);if(!c.valid())return false;if(!v)return false;c.bounce=*v;c.pending=0;c.pc=0x80117aac;continue;}
 case 0x80117aac:{
  c.store(c.work+0x11c,c.word(c.work+0x11c)+1);
  const auto gp=c.word(0x80415fd0),timer=c.word(c.work+0x11c),rate=c.word(gp+4);
  const int32_t duration=signedWord(rate*12)/60;
  if(signedWord(timer)>=duration)c.store(c.work+0x11c,0);
  c.pc=0x80117ae4;continue;}
 case 0x80117ae4:
  c.pc=signedWord(c.word(c.cursor+4))>0?0x80117af0:0x80117b30;continue;
 case 0x80117af0:{
  const float offset=c.constant(-20428),y=real(window[1]),zero=c.constant(-20500);
  const float top=add(offset,y),xOffset=c.constant(-20432),x=real(window[0]);
  const float outX=add(xOffset,x),scale=c.constant(-20424),outY=add(top,c.bounce);
  c.request={16,445,255,scale,{outX,outY,zero}};c.pc=0x80117b2c;continue;}
 case 0x80117b2c:if(!c.icon())return false;c.pc=0x80117b30;continue;
 case 0x80117b30:{
  const auto count=c.word(c.cursor+8),top=c.word(c.cursor+4);
  c.pc=signedWord(top)<signedWord(count-6)?0x80117b44:0x80117b8c;continue;}
 case 0x80117b44:{
  const float offset=c.constant(-20468),y=real(window[1]),zero=c.constant(-20500);
  const float top=add(offset,y),bottom=c.constant(-20420),xOffset=c.constant(-20432),x=real(window[0]);
  const float y0=sub(top,bottom),outX=add(xOffset,x),scale=c.constant(-20424),outY=sub(y0,c.bounce);
  c.request={16,446,255,scale,{outX,outY,zero}};c.pc=0x80117b88;continue;}
 case 0x80117b88:if(!c.icon())return false;c.pc=0x80117b8c;continue;
 case 0x80117b8c:{
  const auto top=c.word(c.cursor+4),selected=c.word(c.cursor);auto index=signedWord(selected-top);if(index<0)index=0;if(index>=6)index=5;
  const auto high=c.word(0x802ef780),low=c.word(0x802ef784);
  const uint64_t biasBits=uint64_t(high)<<32|low,raw=0x4330000000000000ULL|(uint32_t(index)^0x80000000u);
  double bias,number;std::memcpy(&bias,&biasBits,8);std::memcpy(&number,&raw,8);
  const float x=real(window[0]);c.a=c.constant(-20416);volatile float converted=number-bias;c.b=converted;c.c=-real(window[1]);const float zero=c.constant(-20500);
  c.request={20,504,255,0,{x,0,zero}};c.pc=0x80117bec;continue;}
 case 0x80117bec:{
  c.pending=0x80117bec;if(!s.fma||!c.valid())return false;const auto v=s.fma(s.user,c.a,c.b,c.c);if(!c.valid())return false;if(!v)return false;c.request.position[1]=-*v;c.pending=0;c.pc=0x80117bf0;continue;}
 case 0x80117bf0:c.request.scale=c.constant(-20452);c.pc=0x80117bfc;continue;
 case 0x80117bfc:if(!c.icon())return false;cursor.state_.reset();return true;
 default:return false;
 }}
}
}
