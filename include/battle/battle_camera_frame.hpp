#pragma once
#include "battle_camera_requests.hpp"
#include "camera_depth.hpp"
namespace ttyd {
struct BattleCameraFrame {CameraDepthPoint eye{},at{},up{{0,1,0}};};
struct BattleCameraFrameServices {
    BattleCameraRequestServices requests;
    void* user=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t curve,int32_t step,int32_t duration,float start,float end)=nullptr;
    void (*rumbleOff)(void*,int32_t)=nullptr;
};
void initializeBattleCamera(uint32_t,const BattleCameraRequestServices&);
bool setBattleCameraMoveTo(uint32_t,int32_t priority,int32_t duration,int32_t curve,
    CameraDepthPoint eye,CameraDepthPoint at,const BattleCameraFrame&,const BattleCameraRequestServices&);
// Init/default14..16, timeline3 and victory17/18; other modes/shakes need
// their actual providers. This does not assert prior framing history/cadence.
bool updateBattleCameraFrame(uint32_t,BattleCameraFrame&,const BattleCameraFrameServices&);
int32_t battleCameraMoveToEvent(EventEntry&,const BattleCameraFrame&,const BattleCameraRequestServices&);
}
