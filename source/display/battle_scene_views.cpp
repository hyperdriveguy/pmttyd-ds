#include "battle_scene_views.hpp"
#include "camera_manager.hpp"
#include <cmath>
namespace ttyd {
std::optional<BattleSceneLens> battleSceneLens(const CameraManager* owner) {
  const auto* camera=owner?owner->camera(4):nullptr;
  if(!camera || camera->projectionKind!=0 || !std::isfinite(camera->fov) || camera->fov<=0 || camera->fov>=180 ||
     !std::isfinite(camera->nearPlane) || !std::isfinite(camera->farPlane) || camera->nearPlane<=0 || camera->farPlane<=camera->nearPlane)return {};
  return BattleSceneLens{camera->fov,camera->nearPlane*.01f,camera->farPlane*.01f};
}
// Tilted clip X is screen vertical: adding2W continues one full top-view
// height below it. Clip Y is horizontal: 400/320=1.25 crops the same center
// into the narrower bottom screen. Z/W stay unchanged; this is native framing.
std::array<float, 4> lowerBattleSceneClip(const std::array<float, 4> &top) {
  return {top[0] + 2 * top[3], top[1] * 1.25f, top[2], top[3]};
}
BattleSceneMatrix lowerBattleSceneMatrix(const BattleSceneMatrix &top) {
  auto lower = top;
  for (unsigned c = 0; c < 4; ++c) {
    lower[0][c] = top[0][c] + 2 * top[3][c];
    lower[1][c] = top[1][c] * 1.25f;
  }
  return lower;
}
} // namespace ttyd
