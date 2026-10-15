#pragma once
#include <array>
#include <optional>
namespace ttyd {
class CameraManager;
// FOV in degrees; near/far in native world units, converted from source*.01.
struct BattleSceneLens {float fov,nearPlane,farPlane;};
// Read the retained perspective camera4 lens. Distances convert source units
// to native world units; screen aspect remains the handheld view adapter.
std::optional<BattleSceneLens> battleSceneLens(const CameraManager*);
using BattleSceneMatrix = std::array<std::array<float, 4>, 4>;
// Native tilted clip coordinates: X is display vertical and Y horizontal.
// Keep the top400x240 view and crop a centered320x240 continuation below it.
// Affine clip-row rewrite only: it neither reruns simulation nor changes
// original camera/visibility ownership. Scalar and matrix forms are equivalent.
BattleSceneMatrix lowerBattleSceneMatrix(const BattleSceneMatrix &top);
std::array<float, 4> lowerBattleSceneClip(const std::array<float, 4> &top);
} // namespace ttyd
