#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleUnitEventServices {
    EventMemoryServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*unit)(void*,uint32_t explicitBattle,int32_t id)=nullptr;
};
int32_t battleUnitHpEvent(EventEntry&,bool maximum,const BattleUnitEventServices&);
int32_t battleGetUnitWorkEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleSetUnitWorkEvent(EventEntry&,const BattleUnitEventServices&);
}
