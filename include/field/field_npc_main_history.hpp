#pragma once
#include "field_npc_clock.hpp"
namespace ttyd {
struct FieldNpcHistoryCursor {
  bool active = false;
  FieldNpcClockServices owner;
  uint32_t actor = 0, phase = 0, copy = 0, step = 0, x = 0, y = 0, z = 0,
           flags = 0, mainCount = 0;
};
// Source8004b10c..8004b1b4. Raw words preserve NaN payloads and signed zero.
// True reaches final state clear only, not actor advance or whole Main.
// False/empty has no effects; retain owner/mappings/cursor through holds.
bool advanceFieldNpcMainHistory(uint32_t actor, const FieldNpcClockServices &,
                                 FieldNpcHistoryCursor &);
} // namespace ttyd
