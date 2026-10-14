#pragma once
#include "battle_actor.hpp"
#include "event_dispatch.hpp"
#include <optional>
namespace ttyd {
struct BattleWeaponAftereffectServices {
  BattleActorServices memory;
  EventDispatchServices values;
  void *user = nullptr;
  int32_t (*random)(void *, int32_t bound) = nullptr;
  bool (*request)(void *, uint32_t function, int32_t argument) = nullptr;
};
// Complete local dispatch. No retry-safe continuation or source success
// fallback.
std::optional<int32_t>
packedBattleWeaponAftereffectEvent(EventEntry &,
                                   const BattleWeaponAftereffectServices &);
} // namespace ttyd
