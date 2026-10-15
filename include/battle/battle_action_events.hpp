#pragma once
#include "battle_action_help.hpp"
#include <optional>
// nullopt is unavailable native evaluation/removal; engaged0 is EVT retry,
// engaged2 completion. Prior effects persist; these immediate wrappers have no
// suspended-prefix cursor. ReadResult/ReadCount preserve raw destinations.
namespace ttyd {
struct BattleActionEventServices {
  BattleActorServices memory;
  EventDispatchServices values;
  void *user = nullptr;
  std::optional<int32_t> (*evaluate)(void *, uint32_t battle,
                                     uint32_t callback) = nullptr;
  bool (*remove)(void *, uint32_t battle, uint32_t callback) = nullptr;
};
std::optional<int32_t>
packedBattleResultActionEvent(EventEntry &, const BattleActionEventServices &);
std::optional<int32_t>
packedBattleStopActionEvent(EventEntry &, const BattleActionEventServices &);
int32_t packedBattleReadActionEvent(EventEntry &, bool count,
                                    const BattleActionEventServices &);
} // namespace ttyd
