#pragma once
#include "camera_road_assets.hpp"
namespace ttyd {
// collisionTri_simple's finite downward-ray service. Equality replaces height;
// rejection preserves it. Coordinates are authored units (engine / 10).
bool cameraZoneTriangle(CameraPoint query,CameraPoint a,CameraPoint b,
                        CameraPoint c,float& bestHeight);
// Road index is collection1 identity, height is retained downward-ray Y.
// Select against look/contact target, not the separately shifted follow target.
struct CameraZoneSelection {uint32_t road=0;float height=0;};
// Uses the retained look/contact target, not the follow/player target.
std::optional<CameraZoneSelection> selectCameraRoadZone(const CameraRoadAsset& asset,
                                                       CameraPoint look);
}
