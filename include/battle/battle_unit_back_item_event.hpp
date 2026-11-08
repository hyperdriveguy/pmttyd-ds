#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
// Whole8010da28/b8. Atomic admitted EVT/memory services, no async child or
// Pouch substitution. A full eight-slot table takes the source one-past store.
int32_t battleGetBackItemEvent(EventEntry &, const BattleUnitEventServices &);
} // namespace ttyd
