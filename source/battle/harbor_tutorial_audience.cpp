#include "harbor_tutorial_packed.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_act_ui.hpp"
namespace ttyd {
// optional distinguishes provider hold from the source false/true result.
// The retained audience cursor owns pending item/UI work; stack Context is
// rebuilt each visit. MenuRam+14 is native private output scratch, not an ISO
// address; Battle+f44 is the source pad-trigger word, not repeat/held input.
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
  // Clear act-menu bit1 only AFTER actual UI release acknowledges. Scene
  // booleans mirror the completed close; they cannot substitute for release.
  packed.word(b+0x1720,packed.word(b+0x1720)&~1u);
  c.scene->classMenuOpen=false;c.scene->weaponMenuOpen=false;
  return true;
 };
 s.lightOff=[](void*,int32_t,int32_t){}; // Exact original BattleStageOffLightInCommand is blr.
 return resumePackedBattleMenuAudienceCheck(s,audienceMenuCursor);
}
}
