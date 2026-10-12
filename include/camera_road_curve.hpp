#pragma once
#include "camera_road_assets.hpp"
namespace ttyd {
enum class CameraPathStatus {Unsupported,NoHit,Found};
// collisionCurve's actual11-word result; word+4 is not written by this branch.
struct CameraCurveOutput {
    float parameter=0;
    uint32_t untouchedWord=0;
    float segmentStartY=0;
    CameraPoint anchor,tangent;
    uint32_t segment=0,flags=0;
};
static_assert(sizeof(CameraCurveOutput)==44,"Source curve result layout");
bool supportsStraightCameraPath(const CameraRoadAsset& asset,uint32_t road);
// Finite shipped mode0 straight domain, including near-normal collapse and
// exact zero-delta duplicate rows. Host projection is a numerical adaptation.
// Coordinates remain authored units; Unsupported never becomes a generic clamp.
CameraPathStatus solveStraightCameraPath(const CameraRoadAsset& asset,uint32_t road,
                                        CameraPoint player,CameraCurveOutput& output);
}
