#pragma once
#include "event_lifecycle.hpp"
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleAttackRegistrationServices {
    EventMemoryServices memory;
    EventCreationServices events;
    EventSchedule* schedule=nullptr;
    void* user=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
    int32_t (*status)(void*,uint32_t unit,int32_t status)=nullptr;
    int32_t (*irand)(void*,int32_t bound)=nullptr;
    void (*windowOff)(void*)=nullptr;
    void (*advance)(void*,uint32_t battle,int32_t channel)=nullptr;
    // Optional original creation boundary;default uses the recovered ROOT
    // constructor. No fallback script is supplied for a null returned entry.
    EventEntry* (*create)(void*,uint32_t script,int32_t priority,uint32_t flags)=nullptr;
};
// Complete btlseqAct STATE6 region,not other action-controller states.
void registerBattleAttackState6(uint32_t battle,const BattleAttackRegistrationServices&);
}
