#pragma once
#include <cstdint>

namespace ttyd {
// Owned views of Mario words0/4 and byte38. Zero is the source constructor's
// memset point, not a claim to implement the complete MarioInit record.
struct FieldMarioBattleState {
    uint32_t flags=0,secondaryFlags=0;
    uint8_t controlDepth=0;
};
int32_t fieldMarioControlOff(FieldMarioBattleState&);
int32_t fieldMarioControlOn(FieldMarioBattleState&);
int32_t fieldMarioControlOffCheck(const FieldMarioBattleState&);
void restoreFieldMarioAfterBattle(FieldMarioBattleState&);
// Semantic field-battle work; NPC/partner tokens belong to the adapter.
struct FieldBattleWork {
    uintptr_t npc=0;
    int32_t result=0;
};
struct FieldBattleServices {
    void* user=nullptr;
    FieldBattleWork& (*work)(void*)=nullptr;
    uintptr_t (*npc)(void*,int32_t name)=nullptr;
    void (*prepareMario)(void*)=nullptr;
    void (*marioControl)(void*,bool)=nullptr;
    int32_t (*partyId)(void*)=nullptr;
    uintptr_t (*party)(void*,int32_t)=nullptr;
    void (*partyControl)(void*,uintptr_t,bool)=nullptr;
    void (*soundFlag)(void*,uint32_t)=nullptr;
    void (*setSequence)(void*,int32_t)=nullptr;
    int32_t (*sequence)(void*)=nullptr;
    void (*stopBattleBgm)(void*)=nullptr;
    void (*postMario)(void*)=nullptr;
};
// Resolved-operand service boundary for evt_npc_battle_start (8004ef54).
// The VM's discarded read of the following word is outside this adapter.
int32_t startFieldBattle(int32_t npcName,bool firstCall,const FieldBattleServices&);
// evt_npc_wait_battle_end (8004eed8): sequence2 AND any nonzero result.
int32_t waitFieldBattleEnd(const FieldBattleServices&);
}
