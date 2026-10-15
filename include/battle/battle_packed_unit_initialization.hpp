#pragma once
#include "battle_actor.hpp"
// Helpers mutate source byte layouts through callbacks. Repeated reads/stores
// are observable; do not fold them into a memset or cached native struct.
namespace ttyd {
// Complete original storage helpers used by BtlUnit_Entry. Unsupported address
// returns false without mutation; display/pouch/vector services stay owner-bound.
bool initializePackedBattleUnitHelper(uint32_t sourceFunction,uint32_t unit,const BattleActorServices&);
}
