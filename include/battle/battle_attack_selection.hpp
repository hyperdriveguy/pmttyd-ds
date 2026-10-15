#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct BattleAttackSelectionServices {
 BattleActorServices memory;
 void* user=nullptr;
 bool (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t& cursor)=nullptr;
 void (*continueMessage)(void*,uint32_t handle)=nullptr;
 bool (*continueMessageReady)(void*,uint32_t handle)=nullptr; // Native execution acknowledgement.
};
struct BattleAttackSelectionCursor {
 bool active=false;uint8_t phase=0,overrideByte=0;
 uint32_t unit=0,menu=0,battle=0,type=0,cursor=0,message=0;
 EventMemoryServices owner;void* actorOwner=nullptr;
 uint8_t (*byteReader)(void*,uint32_t)=nullptr;
};
bool resumePackedBattleAttackSelection(uint32_t unit,uint32_t menu,const BattleAttackSelectionServices&,BattleAttackSelectionCursor&);
int32_t packedWeaponIsAttack(uint32_t weapon,const BattleActorServices&);
uint32_t packedUnitData(uint32_t unit,uint32_t key,const BattleActorServices&);
// Synchronous callers only. Calls that may suspend must retain a cursor and use
// resumePackedBattleAttackSelection; retrying this wrapper starts a new selection.
// True means all invoked services were bound, not an original scalar result.
// Selection publishes source event/category/counts; it never creates a root.
bool packedSetBattleAttackEvent(uint32_t unit,uint32_t menu,const BattleAttackSelectionServices&);
}
