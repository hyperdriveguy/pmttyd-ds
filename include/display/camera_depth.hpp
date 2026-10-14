#pragma once
#include <array>
#include <optional>
#include <cstdint>
namespace ttyd {
using CameraDepthPoint=std::array<float,3>;
using CameraViewMatrix=std::array<std::array<float,4>,3>;
using CameraProjectionMatrix=std::array<std::array<float,4>,4>;
struct CameraDepthSnapshot {
    CameraViewMatrix view{};
    CameraProjectionMatrix projection{};
};
// Persistent roll/translation and one-use projection request. The caller owns
// camera history; this does not construct a camera or run its callback.
struct CameraMatrixPublication {
    float rollDegrees=0;
    CameraDepthPoint translation{};
    uint16_t flags=0;
    std::array<std::array<float,4>,4> extra{};
};
// Native double trig adapter; source concat order and float stores are retained.
CameraDepthSnapshot publishCameraMatrices(CameraDepthSnapshot,CameraMatrixPublication&);
void publishCameraExtra(CameraProjectionMatrix&,CameraMatrixPublication&);
CameraDepthPoint cameraDepthView(const CameraDepthSnapshot&,CameraDepthPoint);
CameraDepthPoint cameraDepthProject(const CameraDepthSnapshot&,CameraDepthPoint);
float cameraQueueDepth(const CameraDepthSnapshot&,CameraDepthPoint);
// Source projection arithmetic; native double tan is an explicit numeric adapter.
CameraProjectionMatrix cameraPerspective(float fov,float aspect,float nearPlane,float farPlane);
CameraProjectionMatrix cameraOrthographic(float top,float bottom,float left,float right,float nearPlane,float farPlane);
// Finite-domain native normalization adapter, separate from projection/roll.
std::optional<CameraViewMatrix> cameraLookAtAdapter(CameraDepthPoint eye,CameraDepthPoint at,CameraDepthPoint up);
// SDK LookAt/perspective numeric adapter on an actual source-produced snapshot.
std::optional<CameraDepthSnapshot> makeCameraDepthSnapshot(CameraDepthPoint eye,CameraDepthPoint at,
    CameraDepthPoint up,float fov,float aspect,float nearPlane,float farPlane);
}
