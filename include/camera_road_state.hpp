#pragma once
#include "camera_road_curve.hpp"
#include "camera_road_zone.hpp"
namespace ttyd {
// Logical source state. File-relative road index replaces the relocated pointer;
// clock adapter supplies independently quantized GP40/44 milliseconds.
struct CameraRoadState {
    bool active=false,zone=false;
    uint32_t flags=0;
    CameraPoint player,look;
    std::optional<uint32_t> road;
    CameraPoint anchor,direction;
    float distance=0,pitch=0,secondary=0,segment=0;
    uint64_t milliseconds=0;
    float shift=0,shiftRate=0;
};
uint32_t compareCameraRoad(const CameraRoadAsset&,const CameraRoadState& a,
                           const CameraRoadState& b,float& rate);
void blendCameraRoad(bool anchorBlend,const CameraRoadState& origin,
                     const CameraRoadState& destination,CameraRoadState& current,float progress);
struct CameraRoadWork {
    CameraRoadState current,candidate,destination,origin;
    bool transitioning=false,anchorBlend=false;
    float progress=0,rate=0,tangentWeight=1,normalWeight=1;
    uint32_t changeMask=0;
    uint64_t transitionMilliseconds=0;
    CameraPoint failedPlayer,failedLook;
    // False leaves work unchanged for unsupported finite/math domains.
    bool update(const CameraRoadAsset&,CameraPoint player,CameraPoint look,
                float shift,uint64_t milliseconds);
};
}
