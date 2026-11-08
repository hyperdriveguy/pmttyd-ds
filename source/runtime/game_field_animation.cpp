#include "game.hpp"
#include "native_field_animation.hpp"
#include "native_ui_startup.hpp"
#include "native_effect_process.hpp"
#include "native_shadow_process.hpp"
#include "native_camera_process.hpp"
#include "native_map_process.hpp"
#include "native_mario_constructor.hpp"
namespace ttyd {
// Native bootstrap requires a fresh pose namespace and no held producer.
// Creation/configuration/Mario Entries are ordered admissions, not a rollback
// transaction: false can leave earlier owner work installed for caller handling.
bool Game::initializeNativeFieldAnimation(const char* root,std::shared_ptr<NativeFileCache> files,const EventNativePlatform& platform){
 if(fieldParties.originalPool || nativeAnimationPending())return false;
 auto owner=NativeFieldAnimation::create(root,std::move(files),platform);
 if(!owner || !fieldParties.configureNativeAnimation(std::move(owner),fieldPlayerPoseResources()) || !fieldParties.initializeMario(fieldMarioBattle,{},0))return false;
 prepareNativeFieldAnimation();return nativeMarioPose()!=nullptr;
}
// Simulation owns presenter/selection changes. A pending Main/publication
// freezes this prefix; the runtime drains it before advancing another tick.
// Camera simulation ticks, converted by the shared clock helper, drive poses.
bool Game::initializeNativeUiFieldAnimation(const char* root,std::shared_ptr<NativeFileCache> files,const EventNativePlatform& platform){
 if(fieldParties.originalPool)return false;
 if(!nativeUiStartup_){
  auto owner=NativeFieldAnimation::create(root,std::move(files),platform);
  if(!owner)return false;
  nativeUiStartup_=NativeUiStartup::create(std::move(owner),root);
  if(!nativeUiStartup_)return false;
 }
 // Held allocator/delivery/SDK children retain this exact actual process and
 // prefix. No Mario Entry may interleave or authorize incomplete UI history.
 if(!nativeUiStartup_->resumeIcons())return false;
 // Attach to this actual process after UI's shared DVD root/language exists.
 // Pool initialization is separate from texture setup and driver/draw admission.
 if(!nativeEffects_)nativeEffects_=NativeEffectProcess::create(nativeUiStartup_,root);
 if(!nativeEffects_ || !nativeEffects_->initializeFade())return false;
 if(!nativeEffects_ || !nativeEffects_->initialize())return false;
 // Whole original marioInit publishes actual SDK Mario/pouch records in this
 // same process. Constructor polling does not stand in for group completion.
 if(!nativeMarioConstructor_)nativeMarioConstructor_=NativeMarioConstructor::create(nativeUiStartup_,root);
 if(!nativeMarioConstructor_ || !nativeMarioConstructor_->initialize())return false;
 // Explicit native saved-state handoff after fresh Init, never rerun pouchInit.
 // Later typed field/battle services still require their shared raw-owner bridge.
 if(!nativeMarioConstructor_->posesReady() && !nativeMarioConstructor_->busy() && progress.originalPouch &&
    !nativeMarioConstructor_->adoptPouchSnapshot(*progress.originalPouch))return false;
 // Same source VM/heap4 and existing UI SDK bytes; complete CPU shadow Init
 // before Mario Entry. This does not publish a GPU shadow/camera/map receipt.
 if(!nativeShadowProcess_)nativeShadowProcess_=NativeShadowProcess::create(nativeUiStartup_,root);
 if(!nativeShadowProcess_ || !nativeShadowProcess_->initialize())return false;
 // Explicit texture-startup contribution: original Fade -> Environment ->
 // Effect/N64 setup, then independent callbacks, before native Mario Entries.
 // The raw Mario pose wrapper follows; whole Logo platform/file chronology
 // still belongs to its separately retained source owner.
 if(!nativeEffects_->initializeEnvironment() || !nativeEffects_->beginFadeTextures() ||
    !nativeEffects_->beginEnvironmentTextures() || !nativeEffects_->beginTextures())return false;
 // Cold bootstrap drains supported ROMFS banks synchronously. This native IO
 // budget is unlimited for startup; it supplies no source DVD timing/frame tick.
 constexpr uint32_t ColdStartupIoBudget=UINT32_MAX;
 if(!nativeEffects_->pumpFadeTextures(ColdStartupIoBudget) ||
    !nativeEffects_->pumpEnvironmentTextures(ColdStartupIoBudget) ||
    !nativeEffects_->pumpTextures(ColdStartupIoBudget))return false;
 // Native gfx owns the console; original selected NTSC CPU publication and
 // all13 source camera/matrix constructors run against this same live heap.
 if(!nativeCameraProcess_)nativeCameraProcess_=NativeCameraProcess::create(nativeShadowProcess_,root,nativeCameraLibmNumerics());
 if(!nativeCameraProcess_||!nativeCameraProcess_->initialize()||!nativeCameraProcess_->publishBoot(cameraManager))return false;
 auto owner=nativeUiStartup_->fieldProcess();
 if(!owner || !owner->files()->advanceAsync(ColdStartupIoBudget))return false;
 const auto resources=fieldPlayerPoseResources();
 if(!nativeMarioConstructor_->initializePoses({resources[0],resources[1],resources[2]}))return false;
 if(!fieldParties.configureNativeAnimation(owner,resources) ||
    !fieldParties.adoptMarioSourcePoses(fieldMarioBattle,nativeMarioConstructor_->poseHandles()))return false;
 const auto sharedPouch=nativeMarioConstructor_->sharedPouch();
 if(!sharedPouch)return false;
 // Only an explicitly supplied packed inventory becomes canonical. Legacy
 // absence stays absent; source Init is not a native saved-inventory receipt.
 if(progress.originalPouch)progress.originalPouch=sharedPouch;
 prepareNativeFieldAnimation();return nativeMarioPose()!=nullptr;
}
std::shared_ptr<NativeUiStartup> Game::nativeUiStartup() const{
 return nativeUiStartup_ && nativeUiStartup_->current() && nativeUiStartup_->fontObject() && nativeUiStartup_->windowReady() && nativeUiStartup_->fieldProcess()==fieldParties.nativeAnimation?nativeUiStartup_:nullptr;
}
std::shared_ptr<NativeEffectProcess> Game::nativeEffects() const{
 return nativeEffects_ && nativeEffects_->poolReady() && nativeEffects_->ui()==nativeUiStartup()?nativeEffects_:nullptr;
}
std::shared_ptr<NativeShadowProcess> Game::nativeShadowProcess()const{
 return nativeShadowProcess_ && nativeShadowProcess_->ready() && nativeShadowProcess_->ui()==nativeUiStartup_ && nativeUiStartup_ && nativeUiStartup_->fieldProcess()==fieldParties.nativeAnimation?nativeShadowProcess_:nullptr;
}
std::shared_ptr<NativeMarioConstructor> Game::nativeMarioConstructor()const{
 return nativeMarioConstructor_ && nativeMarioConstructor_->posesReady() &&
   nativeMarioConstructor_->ui()==nativeUiStartup()?nativeMarioConstructor_:nullptr;
}
std::shared_ptr<NativeCameraProcess> Game::nativeCameraProcess()const{
 return nativeCameraProcess_&&nativeCameraProcess_->ready()&&nativeCameraProcess_->shadows()==nativeShadowProcess()&&nativeCameraProcess_->bootPublishedFor(cameraManager)?nativeCameraProcess_:nullptr;
}
bool Game::initializeNativeMapProcess(const char* root,const NativeMapCpuServices& cpu){
 if(nativeAnimationPending()||nativePartyPending())return false;
 if(!nativeMapProcess_){auto camera=nativeCameraProcess();if(!camera)return false;
  nativeMapProcess_=NativeMapProcess::create(camera,root,cpu,nativeCameraLibmNumerics().reciprocalSquareRoot);
 }
 return nativeMapProcess_&&nativeMapProcess_->initialize();
}
std::shared_ptr<NativeMapProcess> Game::nativeMapProcess()const{
 return nativeMapProcess_&&nativeMapProcess_->current()&&nativeMapProcess_->cameras()==nativeCameraProcess()?nativeMapProcess_:nullptr;
}
bool Game::beginNativeHarborMapLoad(){
 if(nativeAnimationPending()||nativePartyPending()||!nativeMapProcess())return false;
 return nativeMapProcess_->loadHarbor();
}
bool Game::nativeMapPending()const{return nativeMapProcess_&&nativeMapProcess_->busy();}
bool Game::drainNativeMap(){return !nativeMapProcess_||nativeMapProcess_->resume();}
void Game::prepareNativeFieldAnimation(){
 if(!fieldParties.nativeAnimation || nativeAnimationPending())return;
 marioPresentation.update(fieldJump.motion,fieldMovement.speed,fieldMovement.headingOffsetDegrees,progress.hp,fieldCameraTickMilliseconds(fieldCameraSimulationTicks),true);
 updateNativeFieldAnimation();
}
// Root remains in native world units; water supplies absolute Y while ordinary
// motion uses floor+jump. Configure the actual camera owner before submission.
// Only field Explore/Dialogue outside battle/story locks starts Mario work;
// publicationBudget is a native visit budget, not measured source frame timing.
void Game::updateNativeFieldAnimation(){
 if(!fieldParties.nativeAnimation || nativeAnimationPending())return;
 auto& animation=*fieldParties.nativeAnimation;
 if(!animation.configurePresentation(cameraManager))return;
 animation.setMarioRoot({progress.position.x,water.active()?water.position[1]:groundHeight+jumpHeight,progress.position.z});
 // WORD4's active-handle bits share the existing retained presenter authority.
 fieldMarioBattle.secondaryFlags=(fieldMarioBattle.secondaryFlags&~0x90000000u)|(marioPresentation.displayFlags&0x90000000u);
 if(!trioBattleActive && !harborEncounter.locked() && !professorStory.locked() && (scene==Scene::Explore || scene==Scene::Dialogue))
  animation.mario(marioPresentation,fieldMarioBattle.poseHandles,animation.publicationBudget());
}
const BattlePose* Game::nativeMarioPose() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioPose():nullptr;}
unsigned Game::nativeMarioBank() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioBank():0;}
bool Game::nativeAnimationPending() const{return fieldParties.nativeAnimation && fieldParties.nativeAnimation->producerPending();}
// Resume the existing request with its captured pass/selection/root. This is
// drain work only; no presenter update, new input edge or simulation tick here.
bool Game::drainNativeAnimation(){return !nativeAnimationPending() || fieldParties.nativeAnimation->resumeMario(fieldParties.nativeAnimation->publicationBudget());}
ActorVector Game::nativeMarioRoot() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioRoot():ActorVector{};}
float Game::nativeMarioYaw() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioYaw():0;}
bool Game::nativeMarioReflect() const{return fieldParties.nativeAnimation && fieldParties.nativeAnimation->marioReflect();}
// Candidate validation never owns the running source VM. Only after all room/
// checkpoint providers admit may its role views reconnect to the same source
// pool. Reject new/stale speculative handles instead of fabricating adoption.
bool Game::retainNativeFieldAnimation(Game& previous,bool checkpoint){
 if((nativeEffects_ && nativeEffects_->busy()) || (previous.nativeEffects_ && previous.nativeEffects_->busy()))return false;
 if(nativePartyPending() || previous.nativePartyPending())return false;
 if(nativeAnimationPending() || previous.nativeAnimationPending())return false;
 if((nativeShadowProcess_ && nativeShadowProcess_->busy()) || (previous.nativeShadowProcess_ && previous.nativeShadowProcess_->busy()))return false;
 if((nativeCameraProcess_&&nativeCameraProcess_->busy())||(previous.nativeCameraProcess_&&previous.nativeCameraProcess_->busy()))return false;
 if(nativeMapPending()||previous.nativeMapPending())return false;
 if((nativeMarioConstructor_ && nativeMarioConstructor_->busy()) || (previous.nativeMarioConstructor_ && previous.nativeMarioConstructor_->busy()))return false;
 const auto retainedNativeMario=previous.nativeMarioConstructor();
 const auto retainedNativePouch=retainedNativeMario?retainedNativeMario->sharedPouch():nullptr;
 if(previous.nativeMarioConstructor_ && !retainedNativePouch)return false;
 // Room staging borrows canonical inventory; it may not replace it with a
 // detached candidate snapshot. Checkpoint commit is the explicit copy path.
 if(retainedNativePouch && previous.progress.originalPouch && previous.progress.originalPouch!=retainedNativePouch)return false;
 if(!checkpoint && retainedNativePouch && progress.originalPouch!=previous.progress.originalPouch)return false;
 const auto retainedNativeCamera=previous.nativeCameraProcess();
 const auto retainedNativeMap=previous.nativeMapProcess();
 const auto retainedNativeShadow=previous.nativeShadowProcess();
 const auto retainedUi=previous.nativeUiStartup();
 const auto retainedEffects=previous.nativeEffects();
 auto& old=previous.fieldParties;auto& next=fieldParties;if(!old.nativeAnimation)return true;
 BattlePoseClock clock;
 if(!old.nativeAnimation->ready() || !old.nativeAnimation->clock(clock))return false;
 // Room candidate admission checks every present pose's incarnation/resource
 // and full source bytes. For Mario handles only, allow exactly the documented
 // reinit patch: material word e8=0 and light word ec OR2 (bytes e8..ef).
 if(!checkpoint){
  if(!next.originalPool)return false;
  for(unsigned i=0;i<next.poses.size();++i)if(next.poses[i]){
   const auto* detached=next.poses[i]->original();const auto* live=old.originalPool->pose(int32_t(i));
   if(!detached || !live || detached->nativeGeneration!=live->nativeGeneration || detached->resource!=live->resource)return false;
   auto admitted=live->state;
   for(const auto h:fieldMarioBattle.poseHandles)if(h==int32_t(i)){
    // Candidate restore owns precisely these source reinit stores. Other pose
    // bytes still must match; speculative generation/selection is rejected.
    if(detached->word(0xe8)==0 && detached->word(0xec)==(live->word(0xec)|2)){
     for(unsigned j=0xe8;j<0xf0;++j)admitted[j]=detached->state[j];
    }
   }
   if(detached->state!=admitted)return false;
  }
  // Actors hold candidate room-placement values; role views target the actual
  // retained process. Only the admitted reinit below publishes source stores.
  auto source=old.originalPool;
  for(unsigned i=0;i<next.poses.size();++i)if(next.poses[i])next.poses[i]=std::make_unique<FieldPartyPose>(source,int32_t(i),next.poses[i]->sourceResource);
  next.originalPool=std::move(source);next.nativeAnimation=old.nativeAnimation;
  fieldParties.reinitializeMario(fieldMarioBattle);
 // Checkpoint handoff MOVES the live pool/process instead of adopting detached
 // views. Retain handle/party identities, source clock, upload serial and NPC
 // owner, then perform Mario reinit. This does not restart the cold source VM.
 }else{
  next=std::move(old);fieldMarioBattle.poseHandles=previous.fieldMarioBattle.poseHandles;
  fieldMarioBattle.partyIndices=previous.fieldMarioBattle.partyIndices;fieldMarioBattle.savedPartyKinds=previous.fieldMarioBattle.savedPartyKinds;
  fieldParties.reinitializeMario(fieldMarioBattle);
  fieldCameraSimulationTicks=previous.fieldCameraSimulationTicks;
  marioPresentation.poseUploadSerial=previous.marioPresentation.poseUploadSerial;
  originalFieldNpcs=previous.originalFieldNpcs;
 }
 // Ownership admission is complete. A later source sampler hold is reported
 // by the ordinary renderer; it is not a failed/rolled-back room transaction.
 nativeUiStartup_=retainedUi;nativeEffects_=retainedEffects;nativeShadowProcess_=retainedNativeShadow;nativeCameraProcess_=retainedNativeCamera;nativeMapProcess_=retainedNativeMap;nativeMarioConstructor_=retainedNativeMario;
 if(retainedNativeCamera)cameraManager=previous.cameraManager; // Source camera bank survives room/checkpoint ownership handoff.
 if(retainedNativePouch && progress.originalPouch){
  // All fallible candidate/pose admission is complete. Copy bytes into the
  // same source allocation, then publish the shared typed view; never Init.
  if(checkpoint && progress.originalPouch!=retainedNativePouch)*retainedNativePouch=*progress.originalPouch;
  progress.originalPouch=retainedNativePouch;
 }
 prepareNativeFieldAnimation();return true;
}
}

namespace ttyd {
bool Game::beginHarborTutorialFromField(){
 if(!harborEncounter.battleRequested||harborEncounter.battleSetup!=2||harborTutorial.triggered)return false;
 const auto owner=nativeMarioConstructor();
 if(nativeMarioConstructor_&&!owner)return false; // Detached/incomplete native field.
 const auto time=ticks*1000/60;const auto story=progress.originalStory.value_or(0);
 if(!owner||!progress.originalPouch)return progress.originalPouch?
  harborTutorial.beginFromPouch(time,*progress.originalPouch,story):harborTutorial.beginFresh(time,progress.hp,10,story);
 NativeHarborFieldPouchBinding binding;binding.owner=owner;binding.pouch=progress.originalPouch;
 const auto proof=battleSequenceFacade_.proof();
 const std::weak_ptr<NativeMarioConstructor> weakOwner=owner;const std::weak_ptr<ProloguePouch> weakPouch=progress.originalPouch;
 binding.current=[this,proof,weakOwner,weakPouch]{
  if(!proof.current())return false;
  const auto owner=weakOwner.lock();const auto pouch=weakPouch.lock();
  const auto actual=nativeMarioConstructor();const auto& canonical=progress.originalPouch;
  return owner&&pouch&&actual==owner&&!actual.owner_before(owner)&&!owner.owner_before(actual)&&
   canonical==pouch&&!canonical.owner_before(pouch)&&!pouch.owner_before(canonical);
 };
 return harborTutorial.beginFromNativePouch(time,story,std::move(binding));
}
bool Game::commitHarborPouchReturn(const NativeHarborCompletionReceipt& receipt){
 const auto proof=battleSequenceFacade_.proof();
 const auto owner=nativeMarioConstructor();if(!owner||!progress.originalPouch)return false;
 const auto pouch=harborTutorial.commitNativePouchReturn(receipt,owner,progress.originalPouch);
 if(!pouch||!proof.current())return false;
 progress.hp=pouch->currentHP();return true;
}
}
