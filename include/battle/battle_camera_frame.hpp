#pragma once
#include "battle_camera_requests.hpp"
#include "camera_depth.hpp"
namespace ttyd {
struct BattleCameraFrame {CameraDepthPoint eye{},at{},up{{0,1,0}};};
// Holds the supported-mode Main tail at an unavailable interpolation, retaining
// the published baseline, prior axis effects and original temporary speeds.
struct BattleCameraTremorCursor {
    bool active=false;uint8_t axis=0,stage=0;
    uint16_t speedXY=0,speedZ=0;uint32_t phase=0;
    int32_t curve=0,step=0,duration=0;float start=0,end=0;
};
struct BattleCameraFrameServices {
    BattleCameraRequestServices requests;
    void* user=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t curve,int32_t step,int32_t duration,float start,float end)=nullptr;
    void (*rumbleOff)(void*,int32_t)=nullptr;
};
void initializeBattleCamera(uint32_t,const BattleCameraRequestServices&);
bool setBattleCameraMoveTo(uint32_t,int32_t priority,int32_t duration,int32_t curve,
    CameraDepthPoint eye,CameraDepthPoint at,const BattleCameraFrame&,const BattleCameraRequestServices&);
// Default0/14..16, timeline3, action7/10/11, victory17/18 and both tremor tails; other modes need
// their actual providers. This does not assert prior framing history/cadence.
bool updateBattleCameraFrame(uint32_t,BattleCameraFrame&,const BattleCameraFrameServices&,BattleCameraTremorCursor* continuation=nullptr);
int32_t battleCameraMoveToEvent(EventEntry&,const BattleCameraFrame&,const BattleCameraRequestServices&);
}
