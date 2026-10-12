#pragma once
#include "battle_unit_event.hpp"

namespace ttyd {
// Original leaf loads/stores each axis in order. Output0 is not optional.
void copyPackedUnitPosition(uint32_t unit,bool home,const std::array<uint32_t,3>& outputs,const EventMemoryServices&);
// Event wrappers snapshot three source floats into private temporaries before
// conversion/output calls. Only visible GetPos skips raw sentinel outputs.
int32_t packedUnitPositionEvent(EventEntry&,bool home,const BattleUnitEventServices&);
}
