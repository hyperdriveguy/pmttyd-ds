#pragma once
#include "battle_packed_hit.hpp"
namespace ttyd {
struct BattleCameraRequestServices {
 BattlePartPositionServices positions;EventDispatchServices values;
 void* user=nullptr;
 float (*signedWordFloat)(void*,int32_t)=nullptr;
 void (*rumbleOn)(void*,int32_t channel)=nullptr;
};
// Source request writes only. Main/framing/priority producers remain external.
bool setBattleCameraMode(uint32_t battle,int32_t priority,int32_t mode,const BattleCameraRequestServices&);
bool setBattleCameraSpeed(uint32_t battle,int32_t priority,int32_t level,const BattleCameraRequestServices&);
bool setBattleCameraZoom(uint32_t battle,int32_t priority,float zoom,const BattleCameraRequestServices&);
bool setBattleCameraOffset(uint32_t battle,int32_t priority,float x,float y,float z,const BattleCameraRequestServices&);
int32_t battleCameraShakeHorizontalEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraModeEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraSpeedEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraZoomEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraHomingPartsEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraHomingEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraOffsetEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraAddZoomEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraAxisEvent(EventEntry&,uint32_t mask,const BattleCameraRequestServices&);
}
