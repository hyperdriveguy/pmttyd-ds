#include <memory>
#include "game.hpp"
#include "native_ui_startup.hpp"
#include "native_effect_process.hpp"
#include "native_shadow_process.hpp"
#include "native_camera_process.hpp"
#include "native_mario_constructor.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
int main(){
 assert(loadFieldPlayerPoseAssets("romfs"));
 EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};
 p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
 p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};
 auto game=std::make_unique<Game>(false);assert(game->initializeNativeUiFieldAnimation("romfs",NativeFileCache::create(),p));
 const auto mario=game->nativeMarioConstructor();assert(mario&&mario->ready()&&mario->posesReady());
 const auto sourceMario=mario->marioAddress(),sourcePouch=mario->pouchAddress();assert(sourceMario&&sourcePouch);
 const auto sourceHandles=mario->poseHandles();
 for(unsigned i=0;i<sourceHandles.size();++i){
  assert(sourceHandles[i]==game->fieldMarioBattleState().poseHandles[i]);
  const auto* view=game->fieldPartyState().pose(sourceHandles[i]);assert(view&&view->original());
 }
 unsigned marioCount=0;for(int32_t i=0;i<256;++i)if(game->fieldPartyState().originalPool->pose(i))++marioCount;
 assert(marioCount==3); // Source Entries are adopted, never repeated by the typed registry.
 const auto cameras=game->nativeCameraProcess();assert(cameras&&cameras->ready()&&cameras->counts().allocations==13&&cameras->roads());
 const auto bank=game->cameraManager;
 const auto shadows=game->nativeShadowProcess();assert(shadows&&shadows->ready()&&shadows->counts().heap4Allocations==1);
 const auto effects=game->nativeEffects();assert(effects&&effects->poolReady());
 assert(effects->fadeTexturesReady()&&effects->environmentTexturesReady()&&effects->texturesReady());
 const auto ui=game->nativeUiStartup();assert(ui&&ui->fontObject()&&game->nativeMarioPose());
 assert(ui->windowReady()&&ui->windowMemory()&&ui->windowObject(50)&&ui->windowCounts().callbacks==1&&ui->windowCounts().objectClears==51);
 const auto process=ui->fieldProcess();assert(process==game->fieldPartyState().nativeAnimation);
 const auto counts=ui->counts();assert(counts.gpStores==1&&counts.reads==1&&counts.heapAllocations==1&&counts.heapTransfers==1&&counts.closes==1);
 auto&vm=process->process().vm();uint32_t gp;assert(vm.memory.readWord(0x80415fd0,gp)&&vm.memory.writeWord(gp+0x16c,2));
 game->tick({});assert(ui->language()==2);
 auto candidate=std::make_unique<Game>(*game);assert(!candidate->nativeUiStartup()&&!candidate->nativeEffects());
 assert(!candidate->nativeShadowProcess()&&!candidate->nativeCameraProcess());
 assert(candidate->retainNativeFieldAnimation(*game)&&candidate->nativeUiStartup()==ui&&candidate->nativeEffects()==effects&&candidate->nativeShadowProcess()==shadows);
 assert(candidate->nativeCameraProcess()==cameras&&candidate->cameraManager==bank);
 assert(candidate->nativeMarioConstructor()==mario&&mario->marioAddress()==sourceMario&&mario->pouchAddress()==sourcePouch);
 auto saved=game->progress;game->restore(saved);assert(game->nativeUiStartup()==ui&&game->nativeEffects()==effects&&game->nativeShadowProcess()==shadows&&ui->language()==2);
 assert(game->nativeCameraProcess()==cameras&&game->cameraManager==bank);
 assert(game->nativeMarioConstructor()==mario&&mario->poseHandles()==sourceHandles);
 auto checkpoint=std::make_unique<Game>(false);checkpoint->restore(saved);
 assert(checkpoint->retainNativeFieldAnimation(*game,true)&&checkpoint->nativeUiStartup()==ui&&ui->windowReady()&&!game->nativeUiStartup());
 assert(checkpoint->nativeShadowProcess()==shadows&&!game->nativeShadowProcess());
 assert(checkpoint->nativeCameraProcess()==cameras&&checkpoint->cameraManager==bank&&!game->nativeCameraProcess());
 assert(checkpoint->nativeMarioConstructor()==mario&&!game->nativeMarioConstructor());
 assert(mario->marioAddress()==sourceMario&&mario->pouchAddress()==sourcePouch&&mario->poseHandles()==sourceHandles);
 assert(checkpoint->fieldPartyState().nativeAnimation==process&&checkpoint->nativeEffects()==effects&&!game->nativeEffects()&&ui->language()==2);
 checkpoint->tick({});assert(checkpoint->nativeMarioPose()&&ui->language()==2&&ui->counts().reads==counts.reads&&ui->counts().heapAllocations==counts.heapAllocations);
 std::cout<<"actual Game UI startup/room/checkpoint process retention PASS\n";
}
