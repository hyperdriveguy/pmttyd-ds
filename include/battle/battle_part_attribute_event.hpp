#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
// Whole OnPartsAttribute8010e23c/84; raw masks, real linked part lookup.
// Caller admits complete synchronous EVT/memory/part domains for this call.
int32_t battleOnPartAttributeEvent(EventEntry &,
                                   const BattleUnitEventServices &);
} // namespace ttyd
