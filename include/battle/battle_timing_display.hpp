#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
// Packed TimingA scratch begins B+0x1f20, separate from typed TimingA state.
// Display visits can write scratch/counts; they do not advance sampled input or
// establish action-command timing. Immediate callbacks retain no retry cursor.
namespace ttyd {
struct BattleTimingIconRequest {
  uint32_t camera = 8, icon = 0, flags = 0;
  float scale = 1;
  std::array<float, 3> position{};
};
struct BattleTimingDisplayServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<float> (*interpolate)(void *, int32_t mode, int32_t step,
                                      int32_t duration, float from,
                                      float to) = nullptr;
  bool (*camera)(void *, uint32_t) = nullptr;
  bool (*icon)(void *, const BattleTimingIconRequest &) = nullptr;
};
// Full original local callbacks. False is provider readiness after preceding
// effects; callers must not retry a partially executed callback as a fresh
// draw.
bool packedBattleTimingADraw(float x, float y,
                             const BattleTimingDisplayServices &);
bool packedBattleTimingADisplay(uint32_t battle,
                                const BattleTimingDisplayServices &);
} // namespace ttyd
