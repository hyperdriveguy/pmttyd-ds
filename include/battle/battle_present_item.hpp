#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
struct BattlePresentItemServices {
  BattleActorServices actors;
  void *user = nullptr;
  uint32_t privatePosition = 0;
  std::optional<float> (*subtractDoubleSingle)(void *, double,
                                               double) = nullptr;
  std::optional<float> (*fma)(void *, float, float, float) = nullptr;
  std::optional<float> (*add)(void *, float, float) = nullptr;
  bool (*effect)(void *, uint32_t kind, int32_t amount,
                 const std::array<float, 3> &) = nullptr;
};
struct BattlePresentItemCursor {
  bool active = false;
  uint8_t phase = 0;
  EventMemoryServices owner;
  uint32_t battle = 0, unit = 0, position = 0;
  int32_t item = 0, height = 0;
  double encoded = 0, bias = 0;
  float converted = 0, scale = 0, constant = 0, y = 0, offset = 0;
  std::array<float, 3> effectPosition{};
};
// Whole _check_present_item80120024. Unavailable linked effect/math providers
// retain acknowledged recovery and private data, without replay or fake clear.
std::optional<int32_t>
resumePackedBattlePresentItem(const BattlePresentItemServices &,
                              BattlePresentItemCursor &);
} // namespace ttyd
