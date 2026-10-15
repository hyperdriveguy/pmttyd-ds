#include "game.hpp"
#include "native_field_animation.hpp"
namespace ttyd {
bool Game::initializeNativeFieldAnimation(const char* root,std::shared_ptr<NativeFileCache> files,const EventNativePlatform& platform){
 if(fieldParties.originalPool || nativeAnimationPending())return false;
 auto owner=NativeFieldAnimation::create(root,std::move(files),platform);
 if(!owner || !fieldParties.configureNativeAnimation(std::move(owner),fieldPlayerPoseResources()) || !fieldParties.initializeMario(fieldMarioBattle,{},0))return false;
 prepareNativeFieldAnimation();return nativeMarioPose()!=nullptr;
}
void Game::prepareNativeFieldAnimation(){
 if(!fieldParties.nativeAnimation || nativeAnimationPending())return;
 marioPresentation.update(fieldJump.motion,fieldMovement.speed,fieldMovement.headingOffsetDegrees,progress.hp,fieldCameraTickMilliseconds(fieldCameraSimulationTicks),true);
 updateNativeFieldAnimation();
}
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
bool Game::drainNativeAnimation(){return !nativeAnimationPending() || fieldParties.nativeAnimation->resumeMario(fieldParties.nativeAnimation->publicationBudget());}
ActorVector Game::nativeMarioRoot() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioRoot():ActorVector{};}
float Game::nativeMarioYaw() const{return fieldParties.nativeAnimation?fieldParties.nativeAnimation->marioYaw():0;}
bool Game::nativeMarioReflect() const{return fieldParties.nativeAnimation && fieldParties.nativeAnimation->marioReflect();}
// Candidate validation never owns the running source VM. Only after all room/
// checkpoint providers admit may its role views reconnect to the same source
// pool. Reject new/stale speculative handles instead of fabricating adoption.
bool Game::retainNativeFieldAnimation(Game& previous,bool checkpoint){
 if(nativePartyPending() || previous.nativePartyPending())return false;
 if(nativeAnimationPending() || previous.nativeAnimationPending())return false;
 auto& old=previous.fieldParties;auto& next=fieldParties;if(!old.nativeAnimation)return true;
 BattlePoseClock clock;
 if(!old.nativeAnimation->ready() || !old.nativeAnimation->clock(clock))return false;
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
 prepareNativeFieldAnimation();return true;
}
}
