#include "platform.hpp"
// Native presentation mute remixes all3 retained BGM slots under mutex; the
// field stream/logical envelope still exists. Motion-post binding is installed
// only when the original OLD-motion cleanup has actual stop ownership.
namespace ttyd {
void Audio::muteFieldItemPresentation(bool mute) {
 LightLock_Lock(&musicMutex);
 if(fieldItemMuted!=mute) {
  fieldItemMuted=mute;
  for(unsigned slot=0;slot<3;++slot)if(music[slot].file)musicMix(slot,music[slot].nativeVolume);
 }
 LightLock_Unlock(&musicMutex);
}
//0 requires idle+secondary Off, 10 fall Off; 1/2/3/11/15 are no-op posts.
// Readiness is checked again when called because handles/backend may change.
bool Audio::fieldMarioPostReady(unsigned old) const {
 if(!fieldMarioOwner)return false;
 const auto& handles=fieldMarioOwner->fieldMarioSoundHandles();
 // Fresh source handles are -1. Registered native voices have complete Off
 // descriptors/backend ownership; unrelated audible macro provisioning is separate.
 if(old==0)return fieldSfxBackend.canOff(handles.idle,ready) && fieldSfxBackend.canOff(handles.secondary,ready);
 if(old==10)return fieldSfxBackend.canOff(handles.fall,ready);
 return old==1 || old==2 || old==3 || old==11 || old==15;
}
void Audio::bindFieldMarioMotion(Game& game) {
 fieldMarioOwner=&game;
 if(!fieldMarioPostReady(game.fieldMarioMotion())){game.bindFieldItemMarioPost({});return;}
 game.bindFieldItemMarioPost({this,[](void* p,unsigned old){auto& audio=*static_cast<Audio*>(p);
  if(audio.fieldMarioOwner && audio.fieldMarioPostReady(old))
   audio.fieldMarioOwner->fieldMarioSoundHandles().post(old,audio.fieldSfxBackend.registry(),audio.fieldSfxBackend.services());
 }});
}
}
