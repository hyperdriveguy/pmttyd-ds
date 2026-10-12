#pragma once
#include "event_dispatch.hpp"
#include "event_creation.hpp"
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleDataChildServices {
    EventDispatchServices values;
    EventCreationServices creation;
    EventMemoryServices memory;
    EventSchedule* schedule=nullptr;
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*currentBattle)(void*)=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
};
uint32_t battleUnitData(uint32_t unit,int32_t key,const EventMemoryServices&);
// RunDataEventChild's firstCall argument is unused. Return2 for zero data;
// nonzero data creates a waiting child and returns255 WITHOUT parent deletion.
int32_t runBattleDataChild(EventEntry&,const BattleDataChildServices&);
}
