#pragma once
#include "battle_packed_part.hpp"
namespace ttyd {
struct BattleFacingServices {
  BattlePartPositionServices positions;
  void *user = nullptr;
  float (*signedWordFloat)(void *, int32_t) = nullptr;
  float (*subtract)(void *, float, float) = nullptr;
};
// Complete original CalculateFaceDirection wrapper; writes only the event
// output.
int32_t battleCalculateFaceDirectionEvent(EventEntry &,
                                          const BattleFacingServices &);
int32_t battleChangeFaceDirectionEvent(EventEntry &,
                                       const BattleUnitEventServices &);
} // namespace ttyd
