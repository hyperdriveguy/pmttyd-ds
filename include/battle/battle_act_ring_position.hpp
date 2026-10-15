#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
// Negative displacements address original float bank 0x80427440. They are
// source constants (projection offset/clamp, phase and ellipse radii), not guessed
// native screen coordinates. Numeric/projection providers remain explicit.
namespace ttyd {
struct BattleActRingPositionServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<std::array<float, 3>> (*project)(
      void *, uint32_t positionAddress) = nullptr;
  std::optional<double> (*cosine)(void *, double) = nullptr;
  std::optional<double> (*sine)(void *, double) = nullptr;
};
// Whole original GetRingCenter and GetRingOffset bodies. Projection/libm are
// linked providers; output writes preceding an unavailable provider remain.
bool packedBattleActRingCenter(std::array<float, 3> &,
                               const BattleActRingPositionServices &);
bool packedBattleActRingOffset(float angle, std::array<float, 2> &,
                               const BattleActRingPositionServices &);
} // namespace ttyd
