#pragma once
#include "battle_packed_part.hpp"
// Calculate writes only an event result; Change stores low8 into retained
// facing189. Motion188 is a separate namespace. All raw masks/output operands
// and independently recaptured battle owners retain original order.
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
