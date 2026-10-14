#pragma once
#include <array>
namespace ttyd {
using BattleSceneMatrix = std::array<std::array<float, 4>, 4>;
// Native tilted clip coordinates: X is display vertical and Y horizontal.
// Keep the top400x240 view and crop a centered320x240 continuation below it.
BattleSceneMatrix lowerBattleSceneMatrix(const BattleSceneMatrix &top);
std::array<float, 4> lowerBattleSceneClip(const std::array<float, 4> &top);
} // namespace ttyd
