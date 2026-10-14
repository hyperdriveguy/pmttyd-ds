#include "battle_scene_views.hpp"
#include "camera_manager.hpp"
#include <limits>
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
  assert(!battleSceneLens(nullptr));
  CameraManager cameras;cameras.initializeBoot(400,240);
  auto lens=battleSceneLens(&cameras);assert(lens && lens->fov==25 && std::fabs(lens->nearPlane-.01f)<1e-7f && std::fabs(lens->farPlane-327.68f)<1e-4f);
  auto* camera=cameras.camera(4);camera->fov=37;camera->nearPlane=2.5f;camera->farPlane=9000;
  lens=battleSceneLens(&cameras);assert(lens && lens->fov==37 && std::fabs(lens->nearPlane-.025f)<1e-7f && lens->farPlane==90);
  assert(camera->nearPlane==2.5f && camera->farPlane==9000 && camera->fov==37);
  camera->projectionKind=1;assert(!battleSceneLens(&cameras));camera->projectionKind=0;
  camera->fov=std::numeric_limits<float>::quiet_NaN();assert(!battleSceneLens(&cameras));camera->fov=25;
  camera->nearPlane=0;assert(!battleSceneLens(&cameras));camera->nearPlane=1;camera->farPlane=1;assert(!battleSceneLens(&cameras));
  const std::array<float, 4> top{-1.5f, .5f, .25f, 1};
  const auto lower = lowerBattleSceneClip(top);
  const float topX = (1 - top[1] / top[3]) * 200,
              topY = (1 - top[0] / top[3]) * 120;
  const float lowerX = (1 - lower[1] / lower[3]) * 160,
              lowerY = (1 - lower[0] / lower[3]) * 120;
  assert(lowerX == topX - 40 && lowerY == topY - 240 && lower[2] == top[2] &&
         lower[3] == top[3]);
  // The seam and horizontal crop have the same physical pixel scale; wider
  // top edges fall outside the lower view rather than stretching to320.
  const auto seam = lowerBattleSceneClip({-1, 0, .1f, 1});
  assert(seam[0] == 1);
  const auto left = lowerBattleSceneClip({-1.5f, 1, 0, 1});
  assert((1 - left[1]) * 160 == -40);
  BattleSceneMatrix m{
      {{{2, 3, 4, 5}}, {{6, 7, 8, 9}}, {{10, 11, 12, 13}}, {{14, 15, 16, 17}}}};
  const auto lm = lowerBattleSceneMatrix(m);
  std::array<float, 4> point{1, 2, 3, 1}, a{}, b{};
  for (unsigned row = 0; row < 4; ++row)
    for (unsigned c = 0; c < 4; ++c) {
      a[row] += m[row][c] * point[c];
      b[row] += lm[row][c] * point[c];
    }
  assert(b == lowerBattleSceneClip(a));
  assert(lm[2] == m[2] && lm[3] == m[3]);
  const auto twice = lowerBattleSceneClip({-3, 1, .5f, 2});
  assert(twice[0] / twice[3] == lower[0] && twice[1] / twice[3] == lower[1]);
}
