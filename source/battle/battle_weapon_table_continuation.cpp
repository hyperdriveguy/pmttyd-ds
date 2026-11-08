#include "battle_weapon_table_continuation.hpp"
namespace ttyd {
namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t sb(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
int32_t sh(uint16_t n){return n<32768?int32_t(n):int32_t(n)-65536;}
bool same(const BattleWeaponTableContinuationServices& a,const BattleWeaponTableContinuationServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Invocation{bool& b;explicit Invocation(bool& v):b(v){b=true;}~Invocation(){b=false;}};
enum class Step{Unit,Pouch,Cursor,Dispatch,Level,LevelIcon,Name,Named,OverrideUnit,OverrideData,Badge,BadgeCount,Inventory,InventoryName,InventoryIcon,Empty,Special,PartyData,Party,Publish,Update,Return};
}
struct BattleWeaponTableContinuationCursor::State {
 BattleWeaponTableContinuationServices s;uint32_t battle=0,type=0,unit=0,menu=0,pouch=0,count=0;
 uint32_t level=1,index=0,badge=0,item=0,mask=0,callback=0,overrideUnit=0,overrideId=0;
 uint32_t nameSite=0,nameFallbackSite=0;Step step=Step::Unit,afterName=Step::Level;
 std::optional<uint32_t> cursor;
 std::optional<BattleWeaponTableContinuationRequest> held;
 uint32_t pending=0;bool poisoned=false;
 uint32_t word(uint32_t a)const{return s.actors.memory.word(s.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return s.actors.memory.half(s.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return s.actors.byte(s.actors.user,a);}
 void store(uint32_t a,uint32_t v)const{s.actors.memory.storeWord(s.actors.memory.user,a,v);}
 void putHalf(uint32_t a,uint16_t v)const{s.actors.memory.storeHalf(s.actors.memory.user,a,v);}
 uint32_t row()const{return menu+28*count;}
 bool current(){if(poisoned)return false;if(!s.current||!s.current(s.user)){poisoned=true;return false;}return true;}
 template<class Build>std::optional<uint32_t> call(uint32_t fn,uint32_t site,Build build){
  pending=fn;if(!held){held=BattleWeaponTableContinuationRequest{};held->function=fn;held->callsite=site;build(*held);}
  if(!s.call)return {};
  const auto result=s.call(s.user,*held);if(!current())return {};
  if(result){held.reset();pending=0;}return result;
 }
 std::optional<uint32_t> call(uint32_t fn,uint32_t site,std::array<uint32_t,3> a={},uint8_t n=0){return call(fn,site,[&](auto& q){q.arguments=a;q.count=n;});}
 void name(uint32_t direct,uint32_t fallback,Step next){nameSite=direct;nameFallbackSite=fallback;afterName=next;step=Step::Name;}
 void fromItem(uint32_t ordinal,uint32_t id){store(row()+0x90,ordinal);store(row()+0x94,id);store(row()+0x80,word(item+0x24));store(row()+0x84,0);store(row()+0x98,0);}
};
BattleWeaponTableContinuationCursor::BattleWeaponTableContinuationCursor()=default;
BattleWeaponTableContinuationCursor::~BattleWeaponTableContinuationCursor()=default;
bool BattleWeaponTableContinuationCursor::active()const{return bool(state_);}
bool BattleWeaponTableContinuationCursor::stale()const{return state_&&state_->poisoned;}
uint32_t BattleWeaponTableContinuationCursor::pending()const{return state_?state_->pending:0;}
bool BattleWeaponTableContinuationCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<int32_t> resumePackedBattleWeaponTable(uint32_t battle,uint32_t type,const BattleWeaponTableContinuationServices& s,BattleWeaponTableContinuationCursor& owner){
 const auto& m=s.actors.memory;
 if(owner.invoking_||!m.word||!m.half||!m.storeWord||!m.storeHalf||!s.actors.byte||!s.current)return {};
 if(owner.state_&&(owner.state_->battle!=battle||owner.state_->type!=type||!same(owner.state_->s,s)))return {};
 Invocation invocation(owner.invoking_);
 if(!owner.state_){owner.state_=std::make_unique<BattleWeaponTableContinuationCursor::State>();owner.state_->s=s;owner.state_->battle=battle;owner.state_->type=type;}
 auto& c=*owner.state_;
 const auto finish=[&](int32_t result){owner.state_.reset();return std::optional<int32_t>(result);};
 for(;;){
  if(!c.current())return {};
  switch(c.step){
  case Step::Unit:{const auto u=c.call(0x800f88cc,0x80123edc,[&](auto& q){q.arguments={battle,c.word(battle+0x420)};q.count=2;});if(!u)return {};c.unit=*u;c.menu=battle+0x171c;c.step=Step::Pouch;continue;}
  case Step::Pouch:{const auto p=c.call(0x800d5eb0,0x80123ee8);if(!p)return {};c.pouch=*p;if(!c.menu)return finish(0);c.step=Step::Cursor;continue;}
  case Step::Cursor:{const auto ready=c.call(0x80124cb8,0x80123f0c,[&](auto& q){q.arguments={c.menu,type};q.count=2;q.privateCursorOutput=&c.cursor;});if(!ready)return {};c.count=0;c.step=Step::Dispatch;continue;}
  case Step::Dispatch:
   c.index=0;
   if(type<=1)c.step=Step::Level;
   else if(type==2)c.step=Step::Inventory;
   else if(type==4){c.mask=c.half(c.pouch+0x8c);c.step=Step::Special;}
   else c.step=type==6?Step::PartyData:Step::Publish;
   continue;
  case Step::Level:{
   if(c.level>3){c.index=0;c.step=Step::Badge;continue;}
   if(sb(c.byte(c.pouch+(type==0?0x98:0x99)))<int32_t(c.level)){++c.level;continue;}
   constexpr uint32_t weapons[2][3]={{0x803538a8,0x80353968,0x80353ae8},{0x80353de8,0x80353ea8,0x80354028}};
   c.store(c.level==1?c.menu+0x90:c.row()+0x90,0xffffffff);c.store(c.row()+0x94,0);c.store(c.row()+0x80,weapons[type][c.level-1]);c.store(c.row()+0x84,0);c.store(c.row()+0x98,0);c.step=Step::LevelIcon;continue;
  }
  case Step::LevelIcon:{
   constexpr uint32_t sites[2][3]={{0x80123f90,0x801240b8,0x801241a8},{0x8012428c,0x801243b4,0x801244a4}};
   constexpr uint32_t names[2][3]={{0x80123fc0,0x801240e8,0x801241d8},{0x801242bc,0x801243e4,0x801244d4}};
   const auto icon=c.call(type==0?0x80124c64:0x80124c10,sites[type][c.level-1]);if(!icon)return {};
   c.putHalf(c.row()+0x8c,uint16_t(*icon));c.name(names[type][c.level-1],names[type][c.level-1]+0x30,Step::Named);continue;
  }
  case Step::Name:{
   const auto result=c.call(0x80081b8c,0,[&](auto& q){const auto weapon=c.word(c.row()+0x80);const auto key=c.word(weapon);q.callsite=key?c.nameSite:c.nameFallbackSite;q.arguments[0]=key?key:c.word(0x803108a8+40*c.word(weapon+8)+4);q.count=1;});
   if(!result)return {};
   c.store(c.row()+0x88,*result);c.step=c.afterName;continue;
  }
  case Step::Named:
   if(type<=1){
    if(c.level==1&&(c.word(battle+0xef4)&(type==0?0x09000000u:0x0a000000u))){const auto id=c.word(battle+0x424);if(sw(id)!=-1){c.overrideId=id;c.step=Step::OverrideUnit;continue;}}
    ++c.count;++c.level;c.step=Step::Level;
   }else if(type==2){++c.count;if(sw(c.count)>=21)c.index=20;else ++c.index;c.step=Step::Inventory;}
   else {++c.count;if(sw(c.count)>=21)c.index=16;else ++c.index;c.step=Step::Special;}
   continue;
  case Step::OverrideUnit:{const auto unit=c.call(0x800f88cc,type==0?0x80124020:0x8012431c,[&](auto& q){q.arguments={battle,c.overrideId};q.count=2;});if(!unit)return {};c.overrideUnit=*unit;c.step=Step::OverrideData;continue;}
  case Step::OverrideData:{const auto replacement=c.call(0x8012733c,type==0?0x80124028:0x80124324,{c.overrideUnit,type==0?68u:69u},2);if(!replacement)return {};c.store(c.row()+0x80,*replacement);++c.count;++c.level;c.step=Step::Level;continue;}
  case Step::Badge:
   if(c.index>=(type==0?8u:9u)){c.step=Step::Publish;continue;}
   c.badge=c.word((type==0?0x80416d80u:0x80416da0u)+4*c.index);c.step=Step::BadgeCount;continue;
  case Step::BadgeCount:{
   const auto equipped=c.call(0x800d3300,type==0?0x80124550:0x80124688,{c.badge},1);if(!equipped)return {};
   if(sw(*equipped)<=0){++c.index;c.step=Step::Badge;continue;}
   c.item=0x803108a8+40*c.badge;c.fromItem(c.index,c.badge);const auto weapon=c.word(c.row()+0x80);c.putHalf(c.row()+0x8c,c.word(weapon+8)?c.half(c.item+0x20):c.half(weapon+4));c.name(type==0?0x80124610:0x80124748,type==0?0x80124640:0x80124778,Step::InventoryIcon);continue;
  }
  case Step::Inventory:
   if(c.index>=20){c.step=c.count?Step::Publish:Step::Empty;continue;}
   c.badge=uint32_t(sh(c.half(c.pouch+0x192+2*c.index)));c.item=0x803108a8+40*c.badge;
   if(!c.badge||!(c.half(c.item+0x10)&2)){++c.index;continue;}
   c.fromItem(c.index,c.badge);
   if(!c.word(c.row()+0x80)){c.store(c.row()+0x80,0x80365488);c.step=Step::InventoryName;}
   else {c.putHalf(c.row()+0x8c,c.half(c.item+0x20));c.name(0x801248f8,0x80124928,Step::Named);}
   continue;
  case Step::InventoryName:{const auto name=c.call(0x80081b8c,0x801248a8,[&](auto& q){q.arguments[0]=c.word(c.item+4);q.count=1;});if(!name)return {};c.store(c.row()+0x88,*name);c.putHalf(c.row()+0x8c,c.half(c.item+0x20));c.step=Step::Named;continue;}
  // Shared post-name completion is route-specific; badge names increment both
  // count and source ordinal, without revisiting ordinary-level state.
  case Step::InventoryIcon:++c.count;++c.index;c.step=Step::Badge;continue;
  case Step::Empty:
   c.store(c.row()+0x90,0xffffffff);c.store(c.row()+0x94,0);c.store(c.row()+0x90,0);c.store(c.row()+0x80,0x80363e60);c.store(c.row()+0x84,0);c.store(c.row()+0x98,0);c.putHalf(c.row()+0x8c,362);c.name(0x80124a08,0x80124a38,Step::Return);continue;
  case Step::Special:{
   if(c.index>=16){c.step=Step::Publish;continue;}
   if(!(c.mask&(1u<<c.index))){++c.index;continue;}
   const auto at=0x80355ca8+4*c.index;if(!c.word(at)){++c.index;continue;}
   c.store(c.row()+0x90,0xffffffff);c.store(c.row()+0x94,0);c.store(c.row()+0x80,c.word(at));c.store(c.row()+0x84,0);c.putHalf(c.row()+0x8c,c.half(c.word(at)+4));c.store(c.row()+0x98,0);c.name(0x80124b1c,0x80124b4c,Step::Named);continue;
  }
  case Step::PartyData:{const auto callback=c.call(0x8012733c,0x801247d4,{c.unit,1},2);if(!callback)return {};c.callback=*callback;c.step=Step::Party;continue;}
  case Step::Party:{const auto result=c.call(c.callback,0x801247ec,[&](auto& q){q.arguments[0]=c.menu;q.count=1;q.privateCountOutput=&c.count;});if(!result)return {};c.step=Step::Publish;continue;}
  case Step::Publish:{
   if(sw(c.count)<=0)return finish(0);
   if(!c.cursor){c.pending=0x80124cb8;return {};}
   c.store(*c.cursor+8,c.count);const auto selected=*c.cursor;const auto index=sw(c.word(selected)),n=sw(c.word(selected+8));if(index>=n)c.store(selected,uint32_t(n)-1);
   for(;;){const auto pointer=*c.cursor;const auto scroll=sw(c.word(pointer+4)),n=sw(c.word(pointer+8));if(n>=sw(uint32_t(scroll)+6)||scroll<=0)break;c.store(pointer+4,c.word(pointer+4)-1);}
   c.step=Step::Update;continue;
  }
  case Step::Update:{const auto ready=c.call(0x80123db0,0x80124bf4,{battle,type},2);if(!ready)return {};return finish(sw(c.count));}
  case Step::Return:++c.count;c.step=Step::Publish;continue;
  }
 }
}
}
