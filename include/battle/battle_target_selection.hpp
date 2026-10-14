#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleTargetSelectionServices {
  BattleActorServices memory;
  void *user = nullptr;
  uint32_t privateCursor = 0;
  void (*cursor)(void *, uint32_t menu, uint32_t type,
                 uint32_t output) = nullptr;
  bool (*refresh)(void *, uint32_t buffer) = nullptr;
  uint32_t (*repeat)(void *, uint32_t mask) = nullptr;
  uint32_t (*trigger)(void *, uint32_t mask) = nullptr;
  void (*sound)(void *, uint32_t key) = nullptr;
  bool (*endTarget)(void *, uint32_t battle) = nullptr;
  void (*help)(void *, int32_t type) = nullptr;
  bool (*commandPose)(void *, uint32_t unit) = nullptr;
  void (*offEnemyLight)(void *) = nullptr;
  void (*offLight)(void *, int32_t, int32_t) = nullptr;
  bool (*pay)(void *, uint32_t unit, uint32_t weapon) = nullptr;
  std::optional<bool> (*audience)(void *) = nullptr;
};
// Complete local Input state11. Does not invent its outer progress P or tail.
bool processPackedBattleTargetSelection(uint32_t battle, uint32_t retainedUnit,
                                        const BattleTargetSelectionServices &);
struct BattleCommandFinalizeServices {
  BattleActorServices memory;
  void *user = nullptr;
  bool (*selectAttack)(void *, uint32_t retainedUnit, uint32_t menu) = nullptr;
  void (*offLight)(void *, int32_t, int32_t) = nullptr;
};
std::optional<int32_t>
finalizePackedBattleCommand(uint32_t battle, uint32_t retainedUnit,
                            const BattleCommandFinalizeServices &);
} // namespace ttyd
