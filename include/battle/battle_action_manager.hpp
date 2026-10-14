#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleActionManagerServices {
  BattleActorServices memory;
  void *user = nullptr;
  bool (*pad)(void *, uint32_t address, int32_t channel) = nullptr;
  std::optional<int32_t> (*main)(void *, uint32_t callback,
                                 uint32_t battle) = nullptr;
  bool (*display)(void *, int32_t camera, int32_t displayClass,
                  uint32_t callback, uint32_t argument,
                  float priority) = nullptr;
};
// Complete local manager. False marks an unbound provider after preceding
// effects; it is not an original return or a retry-safe continuation.
bool packedBattleActionCommandManager(uint32_t battle,
                                      const BattleActionManagerServices &);
} // namespace ttyd
