#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
int32_t battleCheckHitPatternEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleGetHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
}
