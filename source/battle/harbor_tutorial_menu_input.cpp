#include "harbor_tutorial_packed.hpp"
#include "battle_weapon_menu_main.hpp"
#include "battle_weapon_menu_decision.hpp"
#include <cassert>
namespace ttyd {
bool HarborTutorialPacked::openWeaponMenu(uint32_t type,HarborTutorial& scene,void* messageUser,uint32_t(*message)(void*,uint32_t),uint32_t setupFlag){
 if(type!=0 && type!=1 && type!=2 && type!=4)return false;
 struct Context{HarborTutorialPacked* p;HarborTutorial* scene;void* messageUser;uint32_t(*message)(void*,uint32_t);};Context c{this,&scene,messageUser,message};
 // Native confirmed-class bridge; the original class controller is still separate.
 word(Battle+0x171c+0x540,type);word(Battle+0x1c5c,type);
 BattleWeaponMenuDecisionServices s;s.memory=nativeBattleActorServices(vm);s.user=&c;s.privateCursor=MenuRam+0xc;
 s.build=[](void* self,uint32_t b,uint32_t t){auto& c=*static_cast<Context*>(self);assert(b==Battle);return c.p->buildWeaponMenu(t,*c.scene,c.messageUser,c.message);};
 s.cursor=[](void* self,uint32_t menu,uint32_t t,uint32_t out){auto& p=*static_cast<Context*>(self)->p;uint32_t address=0;if(findPackedCommandCursor(menu,t,address,nativeBattlePackedSelectionServices(p.vm)))p.word(out,address);};
 s.initialize=[](void* self,uint32_t b,uint32_t,uint32_t,uint32_t){auto& c=*static_cast<Context*>(self);assert(b==Battle);c.p->word(Battle+0x1c78,MenuRam+0xc00);c.p->byte(MenuRam+0xc14,0);c.scene->weaponMenuOpen=true;c.scene->weaponTargetMenuOpen=false;return true;};
 return setupPackedBattleWeaponMenu(Battle,type,setupFlag,s);
}
void HarborTutorialPacked::weaponMenuPresented(){byte(MenuRam+0xc14,1);}
std::optional<int32_t> HarborTutorialPacked::inputWeaponMenu(HarborTutorial& scene,void* messageUser,uint32_t(*message)(void*,uint32_t)){
 struct Context{HarborTutorialPacked* p;HarborTutorial* scene;void* messageUser;uint32_t(*message)(void*,uint32_t);};Context c{this,&scene,messageUser,message};
 BattleWeaponMenuMainServices s;s.memory=nativeBattleActorServices(vm);s.user=&c;s.privateCursor=MenuRam+0x10;
 s.unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<Context*>(self)->p->vm));};
 s.keyAllowed=[](void* self,uint32_t b){return packedBattleMenuKeyAllowed(b,nativeBattleActorServices(static_cast<Context*>(self)->p->vm));};
 s.cursor=[](void* self,uint32_t menu,uint32_t t,uint32_t out){auto& p=*static_cast<Context*>(self)->p;uint32_t address=0;if(findPackedCommandCursor(menu,t,address,nativeBattlePackedSelectionServices(p.vm)))p.word(out,address);};
 s.update=[](void* self,uint32_t b,uint32_t t){auto& c=*static_cast<Context*>(self);assert(b==Battle);const bool ready=c.p->updateWeaponEligibility(t);if(ready)c.p->captureWeaponRows(t,*c.scene);return ready;};
 s.trigger=[](void* self,uint32_t mask){return static_cast<Context*>(self)->p->word(Battle+0xf44)&mask;};
 s.repeat=[](void* self,uint32_t mask){return static_cast<Context*>(self)->p->word(Battle+0xfbc)&mask;};
 s.sound=[](void* self,uint32_t sound){static_cast<Context*>(self)->scene->weaponMenuSounds.push_back(sound);};
 s.endUI=[](void* self,uint32_t b){auto& c=*static_cast<Context*>(self);const auto old=c.p->word(b+0x1c78);if(old && !c.p->classUiRelease(old))return false;c.p->word(b+0x1c78,0);c.scene->weaponMenuOpen=false;return true;};
 s.decide=[](void* self,uint32_t,uint32_t type,uint32_t index){auto& c=*static_cast<Context*>(self);assert(type==c.scene->weaponMenuType);return c.p->selectWeapon(index,*c.scene,c.messageUser,c.message);};
 s.targetSetup=[](void* self,uint32_t){static_cast<Context*>(self)->scene->weaponTargetMenuOpen=true;return true;};
 s.lightFriend=[](void*){};s.lightEnemy=[](void*){};s.display=[](void*,uint32_t){}; // These exact original leaves are blr stubs.
 s.attackAudience=[](void* self)->std::optional<bool>{auto& c=*static_cast<Context*>(self);BattleMenuAudienceServices a;a.memory=nativeBattleActorServices(c.p->vm);a.user=c.p;a.privateItem=MenuRam+0x14;
  a.trigger=[](void* self,uint32_t mask){return static_cast<HarborTutorialPacked*>(self)->word(Battle+0xf44)&mask;};
  a.mario=[](void* self,uint32_t b){return battleMarioUnit(b,nativeBattleActorServices(static_cast<HarborTutorialPacked*>(self)->vm));};
  a.itemOn=[](void* self,uint32_t output,int32_t,int32_t,int32_t,int32_t){return static_cast<HarborTutorialPacked*>(self)->menuAudienceItemOn(output);}; // Whole source producer; actual interruption consumers retain their frontier.
  return packedBattleMenuAudienceCheck(a);
 };
 const auto result=mainPackedBattleWeaponMenu(Battle,scene.weaponMenuType,s);if(result)captureWeaponRows(scene.weaponMenuType,scene);return result;
}
}
