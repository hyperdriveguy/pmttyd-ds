#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleApReportServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*allocate)(void *, uint32_t bytes) = nullptr;
  uint32_t (*mario)(void *, uint32_t battle) = nullptr;
  uint32_t (*party)(void *, uint32_t battle) = nullptr;
  int32_t (*kindCount)(void *, int32_t) = nullptr;
  float (*wordFloat)(void *, int32_t) = nullptr;
  float (*fma)(void *, float, float, float) = nullptr;
  float (*add)(void *, float, float) = nullptr;
  float (*multiply)(void *, float, float) = nullptr;
  double (*squareRoot)(void *, double) = nullptr;
  float (*roundFloat)(void *, double) = nullptr;
  int32_t (*truncate)(void *, float) = nullptr;
  int32_t (*breakTurn)(void *) = nullptr;
  uint32_t (*breakSlot)(void *) = nullptr;
  int32_t (*random)(void *, int32_t bound) = nullptr;
  bool (*pointIncrement)(void *) = nullptr;
};
int32_t packedBattleAudiencePpKindCount(int32_t, const BattleActorServices &);
bool packedBattleBuildApReport(uint32_t input, const BattleApReportServices &);
// Complete caller: clear retained twelve-byte input only after build readiness.
bool packedBattleInviteApReport(const BattleApReportServices &);
} // namespace ttyd
