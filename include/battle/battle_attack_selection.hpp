#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct BattleAttackSelectionServices {
 BattleActorServices memory;
 void* user=nullptr;
 bool (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t& cursor)=nullptr;
 void (*continueMessage)(void*,uint32_t handle)=nullptr;
};
int32_t packedWeaponIsAttack(uint32_t weapon,const BattleActorServices&);
uint32_t packedUnitData(uint32_t unit,uint32_t key,const BattleActorServices&);
// True means all invoked services were bound, not an original scalar result.
// Selection publishes source event/category/counts; it never creates a root.
bool packedSetBattleAttackEvent(uint32_t unit,uint32_t menu,const BattleAttackSelectionServices&);
}
