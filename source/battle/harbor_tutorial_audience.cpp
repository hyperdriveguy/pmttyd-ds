#include "harbor_tutorial_packed.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_act_ui.hpp"
namespace ttyd {
std::optional<bool> HarborTutorialPacked::attackAudienceMenu(HarborTutorial& scene){
 struct Context {HarborTutorialPacked* packed;HarborTutorial* scene;} context{this,&scene};
 BattleMenuAudienceServices s;s.memory=nativeBattleActorServices(vm);s.user=&context;s.privateItem=MenuRam+0x14;
 s.trigger=[](void* p,uint32_t mask){return static_cast<Context*>(p)->packed->word(Battle+0xf44)&mask;};
 s.mario=[](void* p,uint32_t b){return battleMarioUnit(b,nativeBattleActorServices(static_cast<Context*>(p)->packed->vm));};
 s.itemOn=[](void* p,uint32_t output,int32_t,int32_t,int32_t,int32_t){return static_cast<Context*>(p)->packed->menuAudienceItemOn(output);};
 s.setTarget=[](void* p,int32_t target){packedBattleAudienceSetTarget(target,nativeBattleActorServices(static_cast<Context*>(p)->packed->vm));};
 s.endActMenuReady=[](void* p,uint32_t b){
  auto& c=*static_cast<Context*>(p);auto& packed=*c.packed;
  BattleActUiServices ui;ui.memory=nativeBattleActorServices(packed.vm);ui.user=&packed;
  ui.release=[](void* p,uint32_t at){return static_cast<HarborTutorialPacked*>(p)->classUiRelease(at);};
  if(!endPackedBattleActUi(b,ui))return false;
  packed.word(b+0x1720,packed.word(b+0x1720)&~1u);
  c.scene->classMenuOpen=false;c.scene->weaponMenuOpen=false;
  return true;
 };
 s.lightOff=[](void*,int32_t,int32_t){}; // Exact original BattleStageOffLightInCommand is blr.
 return resumePackedBattleMenuAudienceCheck(s,audienceMenuCursor);
}
}
