#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleActUiServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*allocate)(void *, uint32_t bytes) = nullptr;
  bool (*release)(void *, uint32_t address) = nullptr;
};
// Complete original constructor/end. False marks provider readiness, not an
// original scalar return. Earlier effects remain and are not retry-safe.
bool initializePackedBattleActUi(uint32_t battle, uint32_t cursor,
                                 uint32_t rows, uint32_t flag,
                                 const BattleActUiServices &);
bool endPackedBattleActUi(uint32_t battle, const BattleActUiServices &);
} // namespace ttyd
