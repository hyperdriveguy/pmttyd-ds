#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleUnitEventServices {
    EventMemoryServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*unit)(void*,uint32_t explicitBattle,int32_t id)=nullptr;
    int32_t (*checkStatus)(void*,uint32_t unit,int32_t id)=nullptr;
    void (*removeEventId)(void*,uint32_t)=nullptr;
    EventEntry& (*createWait)(void*,uint32_t script,int32_t priority,uint32_t flags)=nullptr;
};
int32_t runBattleUnitWait(uint32_t unit,const BattleUnitEventServices&);
int32_t battleStartUnitWaitEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleStopUnitWaitEvent(EventEntry&,const BattleUnitEventServices&);
bool battleUnitHasData(uint32_t unit,int32_t key,const EventMemoryServices&);
int32_t battleCheckUnitDataEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckUnitStatusEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleUnitHpEvent(EventEntry&,bool maximum,const BattleUnitEventServices&);
int32_t battleGetUnitWorkEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleSetUnitWorkEvent(EventEntry&,const BattleUnitEventServices&);
}
