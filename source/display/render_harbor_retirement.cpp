#include "platform.hpp"
namespace ttyd {
bool Renderer::retireNativeHarborPresentation(HarborTutorial& tutorial,
 const std::shared_ptr<const void>& runtime) {
 const auto actual=tutorial.nativeGameplayLifetime().lock();
 if(!runtime||!actual||actual!=runtime||actual.owner_before(runtime)||runtime.owner_before(actual))return false;
 auto same=[&](HarborTutorial* owner){
  if(!owner)return true;
  const auto token=owner->nativeGameplayLifetime().lock();
  return token&&token==runtime&&!token.owner_before(runtime)&&!runtime.owner_before(token);
 };
 // Validate all retained identities before the first native retirement.
 if(!same(tutorialStartupOwner)||!same(tutorialFadeOwner)||
    !nativeHarborMenuMatches(runtime)||!nativeHarborNiceMatches(runtime)||
    !nativeHarborSmokeMatches(runtime))return false;
 if(!c3dReady)return false;
 C3D_FrameSync();
 retireNativeHarborMenu();retireNativeHarborNice();retireNativeHarborSmoke();
 releaseClassMenuDrawing();releaseWeaponMenuSurfaces();releaseAudienceDrawing();
 releaseTutorialStartupResources();releaseTutorialCurtain();
 freeRoom(harborBattleBackground);freeRoom(harborBattleStage);
 for(auto& actor:tutorialActors){
  if(actor.image.imported){Tex3DS_TextureFree(actor.image.imported);C3D_TexDelete(&actor.image.texture);}
  actor=CineActor{};
 }
 tutorialReady=false;tutorialDrawRanges={};
 return true;
}
}
