#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
// Original BattleRunHitEvent/Direct: root lifecycle only, no assumed death/win.
uint32_t packedBattleRunHitEvent(uint32_t unit,uint32_t code,const BattleUnitEventServices&);
uint32_t packedBattleRunHitEventDirect(uint32_t unit,uint32_t code,uint32_t script,
 const BattleUnitEventServices&);
}
