#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleActionClassServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*unit)(void *, uint32_t battle,
                                  int32_t id) = nullptr;
  std::optional<uint32_t> (*pouch)(void *) = nullptr;
  std::optional<uint32_t> (*message)(void *, uint32_t originalKey) = nullptr;
};
// Full original producer. Result is raw ABI count (pouch pointer on
// wrapped-M0). Missing readiness can follow stores; this is not a retry-safe
// continuation.
std::optional<uint32_t>
makePackedBattleActionClassTable(uint32_t battle,
                                 const BattleActionClassServices &);
} // namespace ttyd
