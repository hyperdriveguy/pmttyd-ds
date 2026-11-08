#pragma once
#include "battle_actor.hpp"
#include "battle_unit_event.hpp"
namespace ttyd {
// Complete80113304/b8. Both services address the same admitted actual VM.
// ClearStatus is the existing whole source helper; status27 writes its real byte.
int32_t battleKillUnitEvent(EventEntry&, const BattleUnitEventServices&,
                           const BattleActorServices&);
}
