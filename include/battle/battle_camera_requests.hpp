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
int32_t battleCameraShakeHorizontalEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraModeEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraSpeedEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraZoomEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraHomingPartsEvent(EventEntry&,const BattleCameraRequestServices&);
int32_t battleCameraHomingEvent(EventEntry&,const BattleCameraRequestServices&);
}
