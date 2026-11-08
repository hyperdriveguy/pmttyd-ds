#include "platform.hpp"
#include <cstdio>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));
 Game game(false);const auto pouch=initializeProloguePouch("Yoshi");
 assert(game.harborTutorial.beginFromPouch(0,pouch,4));
 auto& tutorial=game.harborTutorial;const auto runtime=tutorial.nativeGameplayLifetime().lock();
 assert(runtime&&tutorial.nativeGameplayFacadeProof().current());
 Renderer renderer;renderer.tutorialActors[0].image={1,1};
 assert(!renderer.renderNativeHarborRetirement(game));
 auto copied=tutorial;
 assert(!renderer.retireNativeHarborPresentation(copied,runtime));
 auto foreign=std::make_shared<int>(1);
 assert(!renderer.retireNativeHarborPresentation(tutorial,foreign));
 std::shared_ptr<const void> forged(runtime.get(),[](const void*){});
 assert(!renderer.retireNativeHarborPresentation(tutorial,forged));
 renderer.sourceMapFrameActive_=true;
 assert(!renderer.retireNativeHarborPresentation(tutorial,runtime));
 renderer.sourceMapFrameActive_=false;renderer.allow=false;
 assert(!renderer.retireNativeHarborPresentation(tutorial,runtime));
 renderer.allow=true;renderer.c3dReady=false;
 assert(!renderer.retireNativeHarborPresentation(tutorial,runtime));
 assert(!retirementGpu.sync&&!retirementGpu.freed&&!renderer.retirements);
 renderer.c3dReady=true;
 assert(renderer.retireNativeHarborPresentation(tutorial,runtime));
 assert(retirementGpu.sync==1&&retirementGpu.freed==2&&!renderer.tutorialReady);
 assert(!renderer.harborBattleBackground&&!renderer.harborBattleStage);
 assert(renderer.nativeHarborPresentationRetired(tutorial,runtime));
 assert(!renderer.nativeHarborPresentationRetired(copied,runtime));
 assert(!renderer.nativeHarborPresentationRetired(tutorial,forged));
 // This is render routing only: source scene/result/story/trigger never change.
 const auto scene=game.scene;const auto story=game.progress.originalStory;
 const auto retirements=renderer.retirements;
 retirementGpu.begin=false;
 assert(renderer.renderNativeHarborRetirement(game)&&!retirementGpu.ended);
 retirementGpu.begin=true;
 assert(renderer.renderNativeHarborRetirement(game));
 assert(retirementGpu.ended==1&&retirementGpu.cleared==2&&retirementGpu.selected==2);
 assert((retirementGpu.targets==std::vector<unsigned>{1,2}));
 // A failed target submission must keep the barrier for a later whole frame.
 retirementGpu.lower=false;
 assert(renderer.renderNativeHarborRetirement(game));
 retirementGpu.lower=true;
 assert(renderer.renderNativeHarborRetirement(game));
 assert(retirementGpu.ended==3&&!renderer.sourceMapFrameActive_);
 assert(renderer.retirements==retirements&&!renderer.tutorialReady);
 assert(game.scene==scene&&game.progress.originalStory==story&&tutorial.running()&&tutorial.result==0);
 // Replacement at the same C++ address cannot inherit the old barrier.
 tutorial=HarborTutorial{};
 assert(!renderer.nativeHarborPresentationRetired(tutorial,runtime));
 assert(!renderer.renderNativeHarborRetirement(game));
 assert(tutorial.beginFromPouch(0,pouch,4));
 assert(!renderer.renderNativeHarborRetirement(game));
 assert(renderer.retireNativeHarborPresentation(tutorial,tutorial.nativeGameplayLifetime().lock()));
 assert(renderer.renderNativeHarborRetirement(game));
 std::puts("Actual Harbor facade/runtime retirement, foreign/copy/frame rejection and retained two-screen black routing pass; GPU boundary TEST-only");
}
