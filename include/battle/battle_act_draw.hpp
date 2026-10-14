#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
#include <vector>
namespace ttyd {
using BattleActMatrix = std::array<float, 12>;
struct BattleActDrawSlot {
  // Source stack elements are unwritten in states which do not produce them.
  std::optional<std::array<float, 3>> position;
  std::optional<BattleActMatrix> matrix;
  std::optional<float> parameter;
};
struct BattleActDrawServices {
  BattleActorServices memory;
  void *user = nullptr;
  bool (*camera)(void *, uint32_t) = nullptr;
  std::optional<std::array<float, 3>> (*center)(void *) = nullptr;
  std::optional<std::array<float, 2>> (*offset)(void *, float) = nullptr;
  std::optional<float> (*interpolate)(void *, int32_t, int32_t, int32_t, float,
                                      float) = nullptr;
  std::optional<BattleActMatrix> (*translation)(
      void *, const std::array<float, 3> &) = nullptr;
  std::optional<BattleActMatrix> (*rotation)(void *, float, uint32_t) = nullptr;
  std::optional<BattleActMatrix> (*scale)(void *, float, float,
                                          float) = nullptr;
  std::optional<BattleActMatrix> (*concat)(void *, const BattleActMatrix &,
                                           const BattleActMatrix &) = nullptr;
  bool (*icons)(void *, uint32_t, uint32_t,
                const std::vector<BattleActDrawSlot> &) = nullptr;
  bool (*partyButton)(void *) = nullptr;
  bool (*pinchMark)(void *) = nullptr;
};
struct BattleActIconDrawServices {
  BattleActorServices memory;
  void *user = nullptr;
  bool (*bubble)(void *, uint32_t, uint32_t, const BattleActMatrix &,
                 float) = nullptr;
  bool (*circle)(void *, uint32_t, uint32_t, const BattleActMatrix &,
                 const std::array<float, 3> &, float) = nullptr;
};
bool drawPackedBattleActIcons(uint32_t work, uint32_t rows,
                              const std::vector<BattleActDrawSlot> &,
                              const BattleActIconDrawServices &);
// Complete original DrawMainMenu local body; linked camera/math/draw providers
// retain their own readiness and lifecycle. Invoke once per simulation tick.
// False may follow memory effects; it is not a rollback/retry boundary.
bool drawPackedBattleActMenu(const BattleActDrawServices &);
} // namespace ttyd
