#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
// Complete original floor consumer. Passed battle and X/Y are intentionally
// unused: BattleStageGetPtr reloads global ownership after the wrapper capture.
float packedBattleStageFloorHeight(uint32_t battle,float x,float y,float z,const EventMemoryServices&);
// Only the two original BattleStageInit floor writes; not complete stage init.
void initializePackedBattleStageFloorFields(uint32_t battle,const EventMemoryServices&);
int32_t battleStageFloorPositionEvent(EventEntry&,const BattleUnitEventServices&,
    void* numericUser,float (*convert)(void*,int32_t));
}
