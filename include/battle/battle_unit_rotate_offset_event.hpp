#pragma once
#include "battle_unit_rotation_events.hpp"
namespace ttyd {
// Whole80110f9c/128. Atomic admitted EVT/memory/numeric domain, like the
// existing rotation owners. No rendering, allocation or asynchronous child.
int32_t battleSetUnitRotateOffsetEvent(EventEntry &,
                                       const BattleUnitRotationEventServices &);
} // namespace ttyd
