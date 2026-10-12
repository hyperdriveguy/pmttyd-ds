#pragma once
#include "battle_actor.hpp"

namespace ttyd {
struct BattleTurnFirstServices {
    EventMemoryServices memory;
    void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    void (*judgeTurnRule)(void*)=nullptr;
    int32_t (*fortune)(void*)=nullptr;
    void (*resetMove)(void*,uint32_t unit)=nullptr;
    int32_t (*irand)(void*,int32_t bound)=nullptr;
    void (*softDelete)(void*,uint32_t effect)=nullptr;
};
uint32_t battleUnitStatusFlags(uint32_t unit,uint32_t mask,const BattleTurnFirstServices&);
void clearBattleUnitStatusFlags(uint32_t unit,uint32_t mask,const BattleTurnFirstServices&);
int32_t battleFogEndCheck(const BattleTurnFirstServices&);
void battleTurnFirstProcess(uint32_t explicitBattle,const BattleTurnFirstServices&);
}
