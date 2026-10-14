#pragma once
#include <array>
#include <optional>
namespace ttyd {
using CameraDepthPoint=std::array<float,3>;
struct CameraDepthSnapshot {
    std::array<std::array<float,4>,3> view{};
    std::array<std::array<float,4>,4> projection{};
};
CameraDepthPoint cameraDepthView(const CameraDepthSnapshot&,CameraDepthPoint);
CameraDepthPoint cameraDepthProject(const CameraDepthSnapshot&,CameraDepthPoint);
float cameraQueueDepth(const CameraDepthSnapshot&,CameraDepthPoint);
// SDK LookAt/perspective numeric adapter on an actual source-produced snapshot.
std::optional<CameraDepthSnapshot> makeCameraDepthSnapshot(CameraDepthPoint eye,CameraDepthPoint at,
    CameraDepthPoint up,float fov,float aspect,float nearPlane,float farPlane);
}
