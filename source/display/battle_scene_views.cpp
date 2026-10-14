#include "battle_scene_views.hpp"
namespace ttyd {
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
