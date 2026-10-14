#include "platform.hpp"
namespace ttyd {
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
