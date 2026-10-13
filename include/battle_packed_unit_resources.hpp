#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct PackedUnitResourceServices {
 BattleActorServices memory;void* user=nullptr;
 uint32_t (*pouch)(void*)=nullptr;
 int32_t (*partyIndex)(void*,int32_t sourceKind)=nullptr;
};
// Original BtlUnit_ReviseHpFp80126fe8 and SetParamFromPouch80127134.
// Pouch and battle snapshots retain actual packed ownership and source offsets.
void revisePackedUnitResources(uint32_t unit,const PackedUnitResourceServices&);
void importPackedUnitResources(uint32_t battle,uint32_t unit,const PackedUnitResourceServices&);
}
