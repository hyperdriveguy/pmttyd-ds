#include "harbor_tutorial_packed.hpp"
#include "battle_weapon_table.hpp"
#include "battle_packed_pouch.hpp"
#include "battle_weapon_icons.hpp"
#include "battle_weapon_cost.hpp"
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
bool HarborTutorialPacked::buildWeaponMenu(uint32_t type,HarborTutorial& scene,void* messageUser,uint32_t(*message)(void*,uint32_t)){
 if(!freshPouch || (type!=0 && type!=1 && type!=2 && type!=4))return false;
 word(Battle+0x420,1);readDamage(1);
 struct Context{HarborTutorialPacked* p;void* messageUser;uint32_t(*message)(void*,uint32_t);BattleWeaponEligibilityServices eligibility;bool costReady=true;};Context context{this,messageUser,message,{},true};
 auto cursor=[](void* self,uint32_t menu,uint32_t type,uint32_t output){auto& p=*static_cast<Context*>(self)->p;uint32_t address=0;if(findPackedCommandCursor(menu,type,address,nativeBattlePackedSelectionServices(p.vm)))p.word(output,address);};
 auto unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<Context*>(self)->p->vm));};
 auto targetCount=[](void* self,uint32_t b,uint32_t u,uint32_t w)->std::optional<int32_t>{
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
 };
 BattleWeaponTableServices s;s.memory=nativeBattleActorServices(vm);s.user=&context;s.cursorSlot=MenuRam;s.countSlot=MenuRam+4;s.unit=unit;s.cursor=cursor;
 s.pouch=[](void* self){return static_cast<Context*>(self)->p->word(0x8041eb00);};
 s.equipped=[](void* self,int32_t badge){auto& p=*static_cast<Context*>(self)->p;return packedPouchEquippedBadgeCount(p.word(0x8041eb00),badge,nativeBattleActorServices(p.vm).memory);};
 s.jumpIcon=[](void* self){auto& p=*static_cast<Context*>(self)->p;std::array<uint8_t,0x5d4> data{};const auto pouch=p.word(0x8041eb00);data[0x98]=p.byte(pouch+0x98);BattleWeaponIconServices i;i.user=&data;i.pouch=[](void* v)->const uint8_t*{return static_cast<std::array<uint8_t,0x5d4>*>(v)->data();};return jumpWeaponIcon(i);};
 s.hammerIcon=[](void* self){auto& p=*static_cast<Context*>(self)->p;std::array<uint8_t,0x5d4> data{};const auto pouch=p.word(0x8041eb00);data[0x99]=p.byte(pouch+0x99);BattleWeaponIconServices i;i.user=&data;i.pouch=[](void* v)->const uint8_t*{return static_cast<std::array<uint8_t,0x5d4>*>(v)->data();};return hammerWeaponIcon(i);};
 s.message=[](void* self,uint32_t key){auto& c=*static_cast<Context*>(self);assert(c.message);return c.message(c.messageUser,key);};
 // Nested updater owns a separate private cursor slot from the builder.
 auto& eligibility=context.eligibility;eligibility.memory=s.memory;eligibility.user=&context;eligibility.cursorSlot=MenuRam+8;eligibility.unit=unit;eligibility.cursor=cursor;eligibility.targetCount=targetCount;
 s.update=[](void* self,uint32_t b,uint32_t type){auto& c=*static_cast<Context*>(self);return updatePackedBattleWeaponTable(b,type,c.eligibility);};
 const auto count=makePackedBattleWeaponTable(Battle,type,s);if(!count)return false;
 uint32_t activeCursor=0;if(!findPackedCommandCursor(Battle+0x171c,type,activeCursor,nativeBattlePackedSelectionServices(vm)))return false;const auto visibleCount=int32_t(word(activeCursor+8));if(visibleCount<0 || visibleCount>64)return false;
 scene.weaponMenuType=type;scene.weaponRows.clear();
 for(int32_t n=0;n<visibleCount;++n){const auto row=Battle+0x171c+0x80+28*uint32_t(n);HarborTutorialWeaponRow r;r.weapon=word(row);r.enabled=word(row+4)!=0;r.nameAddress=word(row+8);uint16_t icon=0;if(!vm.memory.readHalf(row+12,icon))std::abort();r.icon=icon;r.ordinal=int32_t(word(row+16));r.item=int32_t(word(row+20));r.reason=int32_t(word(row+24));if(r.nameAddress)for(unsigned j=0;j<MenuRam+0x4000-r.nameAddress;++j){const auto c=byte(r.nameAddress+j);if(!c)break;r.label+=char(c);}scene.weaponRows.push_back(std::move(r));}
 return true;
}
}
