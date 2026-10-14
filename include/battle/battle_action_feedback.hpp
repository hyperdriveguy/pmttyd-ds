#pragma once
#include "battle_actor.hpp"
#include "battle_unit_event.hpp"
#include <optional>
namespace ttyd {
struct BattleActionFeedbackServices {
  BattleActorServices memory;
  EventDispatchServices values;
  void *user = nullptr;
  float (*signedWordFloat)(void *, int32_t) = nullptr;
  float (*fma)(void *, float, float, float) = nullptr;
  bool (*audience)(void *, uint32_t weapon, bool good) = nullptr;
};
std::optional<int32_t>
packedBattleDeclareActionResultEvent(EventEntry &,
                                     const BattleActionFeedbackServices &);
struct BattleAudienceFeedbackServices {
  BattleActorServices memory;
  void *user = nullptr;
  bool (*request)(void *, uint32_t function, int32_t arg0,
                  int32_t arg1) = nullptr;
};
int32_t packedBattleAudienceWaiting(int32_t, const BattleActorServices &);
int32_t packedBattleAudienceWaitingCount(const BattleActorServices &);
bool packedBattleAudienceActionFeedback(uint32_t weapon, bool good,
                                        const BattleAudienceFeedbackServices &);
void packedBattleAudienceAcrobatNoTry(const BattleActorServices &);
} // namespace ttyd
