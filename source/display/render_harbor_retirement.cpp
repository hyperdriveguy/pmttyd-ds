#include "platform.hpp"
namespace ttyd {
bool Renderer::retireNativeHarborPresentation(HarborTutorial& tutorial,
 const std::shared_ptr<const void>& runtime) {
 // A copied facade or an in-frame caller cannot retire another scene's GPU
 // ownership. Preserve the previous frame until a real outside-frame caller.
 if(sourceMapFrameActive_)return false;
 const auto proof=tutorial.nativeGameplayFacadeProof();if(!proof.current())return false;
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
    !nativeHarborSmokeMatches(runtime)||!nativeHarborStarPointMatches(runtime)||!nativeHarborRewardsMatch(runtime))return false;
 if(!c3dReady)return false;
 C3D_FrameSync();
 // Keep this barrier independently of reward/fade resources: those owners
 // are about to retire, and completion can legitimately hold for many frames.
 harborRetirement_={&tutorial,proof,runtime};
 retireNativeHarborMenu();retireNativeHarborNice();retireNativeHarborSmoke();retireNativeHarborStarPoint();retireNativeHarborRewards();
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

bool Renderer::nativeHarborPresentationRetired(const HarborTutorial& tutorial,
 const std::shared_ptr<const void>& runtime) const {
 const auto& held=harborRetirement_;
 if(!runtime||!held.proof.current()||held.facade!=&tutorial||
    !tutorial.nativeGameplayFacadeProof().current())return false;
 const auto retired=held.runtime.lock();const auto actual=tutorial.nativeGameplayLifetime().lock();
 const auto same=[](const auto&a,const auto&b){return a&&b&&a==b&&!a.owner_before(b)&&!b.owner_before(a);};
 return same(retired,runtime)&&same(actual,runtime);
}
bool Renderer::renderNativeHarborRetirement(const Game& game) {
 const auto runtime=harborRetirement_.runtime.lock();
 if(!nativeHarborPresentationRetired(game.harborTutorial,runtime))return false;
 // Submission failure retains the hold. There is no fallback to combat
 // preparation and no field/sequence receipt from clearing these targets.
 if(!c3dReady||!top||!bottom)return true;
 if(!C3D_FrameBegin(C3D_FRAME_SYNCDRAW))return true;
 sourceMapFrameActive_=true;
 constexpr uint32_t OpaqueBlack=0x000000ff;
 C3D_RenderTargetClear(top,C3D_CLEAR_ALL,OpaqueBlack,0);
 C3D_FrameDrawOn(top);
 C3D_RenderTargetClear(bottom,C3D_CLEAR_ALL,OpaqueBlack,0);
 C3D_FrameDrawOn(bottom);
 C3D_FrameEnd(0);
 sourceMapFrameActive_=false;
 return true;
}
}
