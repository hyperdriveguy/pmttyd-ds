#include "harbor_tutorial_packed.hpp"
#include "battle_weapon_table.hpp"
#include "battle_weapon_menu_decision.hpp"
#include "battle_packed_pouch.hpp"
#include "battle_weapon_icons.hpp"
#include "battle_weapon_cost.hpp"
#include "battle_attack_selection.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <climits>
namespace ttyd {
uint32_t HarborTutorialPacked::cacheMenuLabel(uint32_t key,const char* text){
 if(!text)return 0;
 const auto found=menuText.find(key);if(found!=menuText.end())return found->second;
 const auto length=std::strlen(text)+1;const auto size=(length+3)&~size_t(3);if(size>MenuRam+0x4000-menuTextAt)std::abort();
 const auto address=menuTextAt;menuTextAt+=uint32_t(size);for(size_t n=0;n<length;++n)byte(address+uint32_t(n),uint8_t(text[n]));menuText.emplace(key,address);return address;
}
std::optional<int32_t> HarborTutorialPacked::countWeaponTargets(uint32_t b,uint32_t u,uint32_t w){
 struct Context{HarborTutorialPacked* p;bool costReady=true;};Context context{this,true};void* self=&context;
  auto& p=*static_cast<Context*>(self)->p;BattleWeaponTargetCountServices t;t.memory=nativeBattleActorServices(p.vm);t.user=self;t.privateBuffer=MenuRam+0x40;
  t.cost=[](void* self,uint32_t u,uint32_t w)->int32_t{auto& p=*static_cast<Context*>(self)->p;const auto id=p.word(u);assert(id==1);p.readDamage(id);DamageWeapon weapon;weapon.itemId=int32_t(p.word(w+8));weapon.fpCost=p.byte(w+0x11);weapon.apCost=p.byte(w+0x12);
   struct Cost{HarborTutorialPacked* p;uint32_t unit,weapon;DamageWeapon* value;Context* context;};Cost cost{&p,u,w,&weapon,static_cast<Context*>(self)};WeaponCostServices s;s.user=&cost;
   s.equippedCount=[](void* self,int32_t badge){auto& c=*static_cast<Cost*>(self);return packedPouchEquippedBadgeCount(c.p->word(0x8041eb00),badge,nativeBattleActorServices(c.p->vm).memory);};
   s.pow=[](void*,double a,double b){return std::pow(a,b);};s.truncate=[](void* self,double v){if(!std::isfinite(v) || v<INT32_MIN || v>INT32_MAX){static_cast<Cost*>(self)->context->costReady=false;return int32_t(0);}return int32_t(v);};
   s.getFp=[](void* self,DamageUnit&){auto& c=*static_cast<Cost*>(self);return packedUnitFp(c.unit,false,nativeBattleActorServices(c.p->vm));};
   s.getAp=[](void* self){auto& c=*static_cast<Cost*>(self);c.value->apCost=c.p->byte(c.weapon+0x12);uint16_t value=0;if(!c.p->vm.memory.readHalf(c.p->word(0x8041eb00)+0x7a,value))std::abort();return value<32768?int32_t(value):int32_t(value)-65536;};
   const bool affordable=checkWeaponCost(p.damageUnits[id],weapon,s);return cost.context->costReady && affordable?1:0;
  };
  t.enemyBelong=[](void* self,uint32_t u){const auto v=static_cast<Context*>(self)->p->byte(u+0xc);const auto belong=v<128?int32_t(v):int32_t(v)-256;return belong==0?1:belong==1?0:belong;};
  t.sample=[](void* self,uint32_t buffer,uint32_t w,int32_t actor,int32_t belong,uint32_t admission,uint32_t attributes,int32_t direction){auto& p=*static_cast<Context*>(self)->p;samplePackedBattleCandidates({nativeBattleActorServices(p.vm),buffer},w,actor,belong,admission,attributes,direction,nativeBattleSamplingServices(p.vm));return true;};
  const auto result=packedWeaponTargetCount(b,u,w,t);if(!static_cast<Context*>(self)->costReady)return std::nullopt;return result;
}
bool HarborTutorialPacked::updateWeaponEligibility(uint32_t type){
 BattleWeaponEligibilityServices e;e.memory=nativeBattleActorServices(vm);e.user=this;e.cursorSlot=MenuRam+8;
 e.unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<HarborTutorialPacked*>(self)->vm));};
 e.cursor=[](void* self,uint32_t menu,uint32_t t,uint32_t out){auto& p=*static_cast<HarborTutorialPacked*>(self);uint32_t c=0;if(findPackedCommandCursor(menu,t,c,nativeBattlePackedSelectionServices(p.vm)))p.word(out,c);};
 e.targetCount=[](void* self,uint32_t b,uint32_t u,uint32_t w){return static_cast<HarborTutorialPacked*>(self)->countWeaponTargets(b,u,w);};return updatePackedBattleWeaponTable(Battle,type,e);
}
void HarborTutorialPacked::captureWeaponRows(uint32_t type,HarborTutorial& scene){
 uint32_t activeCursor=0;if(!findPackedCommandCursor(Battle+0x171c,type,activeCursor,nativeBattlePackedSelectionServices(vm)))std::abort();const auto visibleCount=int32_t(word(activeCursor+8));if(visibleCount<0 || visibleCount>64)std::abort();
 scene.weaponMenuType=type;scene.weaponRows.clear();
 for(int32_t n=0;n<visibleCount;++n){const auto row=Battle+0x171c+0x80+28*uint32_t(n);HarborTutorialWeaponRow r;r.weapon=word(row);r.enabled=word(row+4)!=0;r.nameAddress=word(row+8);uint16_t icon=0;if(!vm.memory.readHalf(row+12,icon))std::abort();r.icon=icon;r.ordinal=int32_t(word(row+16));r.item=int32_t(word(row+20));r.reason=int32_t(word(row+24));if(r.nameAddress)for(unsigned j=0;j<MenuRam+0x4000-r.nameAddress;++j){const auto c=byte(r.nameAddress+j);if(!c)break;r.label+=char(c);}scene.weaponRows.push_back(std::move(r));}
 scene.weaponRowIndex=int32_t(word(activeCursor));
}
bool HarborTutorialPacked::buildWeaponMenu(uint32_t type,HarborTutorial& scene,void* messageUser,uint32_t(*message)(void*,uint32_t)){
 weaponDecided=false;commandFinalized=false;commandUnbound=0;scene.commandInputPending=0;
 if(!freshPouch || (type!=0 && type!=1 && type!=2 && type!=4))return false;
 word(Battle+0x420,1);readDamage(1);
 struct Context{HarborTutorialPacked* p;void* messageUser;uint32_t(*message)(void*,uint32_t);};Context context{this,messageUser,message};
 auto cursor=[](void* self,uint32_t menu,uint32_t type,uint32_t output){auto& p=*static_cast<Context*>(self)->p;uint32_t address=0;if(findPackedCommandCursor(menu,type,address,nativeBattlePackedSelectionServices(p.vm)))p.word(output,address);};
 auto unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<Context*>(self)->p->vm));};
 BattleWeaponTableServices s;s.memory=nativeBattleActorServices(vm);s.user=&context;s.cursorSlot=MenuRam;s.countSlot=MenuRam+4;s.unit=unit;s.cursor=cursor;
 s.pouch=[](void* self){return static_cast<Context*>(self)->p->word(0x8041eb00);};
 s.equipped=[](void* self,int32_t badge){auto& p=*static_cast<Context*>(self)->p;return packedPouchEquippedBadgeCount(p.word(0x8041eb00),badge,nativeBattleActorServices(p.vm).memory);};
 s.jumpIcon=[](void* self){auto& p=*static_cast<Context*>(self)->p;std::array<uint8_t,0x5d4> data{};const auto pouch=p.word(0x8041eb00);data[0x98]=p.byte(pouch+0x98);BattleWeaponIconServices i;i.user=&data;i.pouch=[](void* v)->const uint8_t*{return static_cast<std::array<uint8_t,0x5d4>*>(v)->data();};return jumpWeaponIcon(i);};
 s.hammerIcon=[](void* self){auto& p=*static_cast<Context*>(self)->p;std::array<uint8_t,0x5d4> data{};const auto pouch=p.word(0x8041eb00);data[0x99]=p.byte(pouch+0x99);BattleWeaponIconServices i;i.user=&data;i.pouch=[](void* v)->const uint8_t*{return static_cast<std::array<uint8_t,0x5d4>*>(v)->data();};return hammerWeaponIcon(i);};
 s.message=[](void* self,uint32_t key){auto& c=*static_cast<Context*>(self);assert(c.message);return c.message(c.messageUser,key);};
 s.update=[](void* self,uint32_t b,uint32_t type){auto& c=*static_cast<Context*>(self);assert(b==Battle);return c.p->updateWeaponEligibility(type);};
 const auto count=makePackedBattleWeaponTable(Battle,type,s);if(!count)return false;
 captureWeaponRows(type,scene);
 return true;
}
bool HarborTutorialPacked::selectWeapon(unsigned index,HarborTutorial& scene,void* messageUser,uint32_t(*message)(void*,uint32_t)){
 struct Context{HarborTutorialPacked* p;HarborTutorial* scene;void* messageUser;uint32_t(*message)(void*,uint32_t);};Context context{this,&scene,messageUser,message};
 BattleWeaponMenuDecisionServices s;s.memory=nativeBattleActorServices(vm);s.user=&context;
 s.unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<Context*>(self)->p->vm));};
 s.build=[](void* self,uint32_t b,uint32_t type){auto& c=*static_cast<Context*>(self);assert(b==Battle);return c.p->buildWeaponMenu(type,*c.scene,c.messageUser,c.message);};
 s.enemyBelong=[](void* self,uint32_t u){const auto raw=static_cast<Context*>(self)->p->byte(u+0xc);const auto side=raw<128?int32_t(raw):int32_t(raw)-256;return side==0?1:side==1?0:side;};
 s.belong=[](void* self,uint32_t u){return int32_t(static_cast<Context*>(self)->p->byte(u+0xc));};
 s.sample=[](void* self,uint32_t buffer,uint32_t w,int32_t actor,int32_t enemy,uint32_t admission,uint32_t attributes,int32_t direction){auto& p=*static_cast<Context*>(self)->p;samplePackedBattleCandidates({nativeBattleActorServices(p.vm),buffer},w,actor,enemy,admission,attributes,direction,nativeBattleSamplingServices(p.vm));return true;};
 const auto decided=decidePackedBattleWeapon(Battle,scene.weaponMenuType,index,s);if(!decided)return false;
 scene.weaponTargets.clear();const auto buffer=Battle+0x428;const auto rawCount=byte(buffer+0xa6c);const int32_t count=rawCount<128?int32_t(rawCount):int32_t(rawCount)-256;
 for(int32_t j=0;j<count;++j){const auto raw=byte(buffer+0xa6d+uint32_t(j));const int32_t permutation=raw<128?int32_t(raw):int32_t(raw)-256;const auto row=buffer+4+36*uint32_t(permutation);uint16_t unit=0,part=0;if(!vm.memory.readHalf(row,unit) || !vm.memory.readHalf(row+2,part))std::abort();scene.weaponTargets.push_back({unit<32768?int32_t(unit):int32_t(unit)-65536,part<32768?int32_t(part):int32_t(part)-65536});}
 weaponDecided=true;
 return true;
}
bool HarborTutorialPacked::selectAttackEvent(){
 if(!weaponDecided)return false;
 BattleAttackSelectionServices selection;selection.memory=nativeBattleActorServices(vm);selection.user=this;
 selection.cursor=[](void* self,uint32_t menu,uint32_t type,uint32_t& cursor){auto& p=*static_cast<HarborTutorialPacked*>(self);return findPackedCommandCursor(menu,type,cursor,nativeBattlePackedSelectionServices(p.vm));};
 const bool selected=packedSetBattleAttackEvent(Units+0x1000,Battle+0x171c,selection);
 if(selected)weaponDecided=false;
 return selected;
}

}
