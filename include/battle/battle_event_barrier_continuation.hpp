#pragma once
#include "battle_actor.hpp"
#include <memory>
#include <optional>
namespace ttyd {
struct BattleEventBarrierServices {
  BattleActorServices actors;
  void *user = nullptr;
  std::optional<int32_t> (*eventExists)(void *, uint32_t id) = nullptr;
  // Source return is ignored, including failure0. nullopt alone means wait.
  std::optional<int32_t> (*removeItem)(void *, uint32_t item) = nullptr;
  std::optional<int32_t> (*concluded)(void *, uint32_t battle) = nullptr;
  // Actual evtEntry guest address; value0 is completed allocation failure.
  std::optional<uint32_t> (*create)(void *, uint32_t script, int32_t priority,
                                    uint32_t flags) = nullptr;
};
// One entry kind and passed battle pointer per invocation. Pin all provider
// users/callbacks, including the audience lifecycle observer. Reset abandons
// native state only, never guest writes/queue shifts/item removals already done.
class BattleEventBarrierCursor {
public:
  BattleEventBarrierCursor();
  ~BattleEventBarrierCursor();
  BattleEventBarrierCursor(const BattleEventBarrierCursor &) = delete;
  BattleEventBarrierCursor &
  operator=(const BattleEventBarrierCursor &) = delete;
  bool active() const;
  bool reset();
  uint32_t pending() const;

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend struct BattleEventBarrierRunner;
};
// Both barrier APIs scan all 64 unit slots; Unit omits the five outer battle
// event fields. Neither name promises single-unit scope. Reaction's engaged1
// instead means attempted reaction dispatch, including event allocation failure.
// Original int0/1 is complete readiness, distinct from nullopt provider wait.
std::optional<int32_t>
resumeBattleAllEventBarrier(uint32_t, const BattleEventBarrierServices &,
                            BattleEventBarrierCursor &);
std::optional<int32_t>
resumeBattleUnitEventBarrier(uint32_t, const BattleEventBarrierServices &,
                             BattleEventBarrierCursor &);
std::optional<int32_t>
resumeBattleAfterReaction(const BattleEventBarrierServices &,
                          BattleEventBarrierCursor &);
// Native completion of the source void consumer, not an invented item result.
bool resumeBattleConsumeReservedItems(const BattleEventBarrierServices &,
                                      BattleEventBarrierCursor &);
// Exact immediate Release800f79e4; recaptures global B, removes all matching
// pairs and compacts each source word in order. No external child can wait.
void releasePackedBattleAfterReaction(int32_t unit, uint32_t key,
                                      const BattleActorServices &);
} // namespace ttyd
