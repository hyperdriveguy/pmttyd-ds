#pragma once
#include "event_manager.hpp"

namespace ttyd {
struct EventDispatchServices {
    void* user=nullptr;
    uint32_t (*word)(void*,uint32_t originalAddress)=nullptr;
    int32_t (*resolve)(void*,EventEntry&,int32_t rawOperand)=nullptr;
    uint32_t (*clock)(void*)=nullptr;
    int32_t (*call)(void*,EventEntry&,uint32_t cachedFunction,int32_t firstCall)=nullptr;
    void (*remove)(void*,EventEntry&)=nullptr;
    // All other supported opcode bodies remain explicit handlers, not no-ops.
    int32_t (*handler)(void*,EventEntry&,uint8_t opcode)=nullptr;
    float (*resolveFloat)(void*,EventEntry&,int32_t rawOperand)=nullptr;
    void (*store)(void*,EventEntry&,int32_t rawDestination,int32_t)=nullptr;
    void (*storeFloat)(void*,EventEntry&,int32_t rawDestination,float)=nullptr;
};
// Handler-level outcomes: 0=retry,1=complete/yield,2=complete/continue,
// 3=redispatch,255=end entry,negative=abort manager pass. See event-vm guide.
int32_t eventWaitFrame(EventEntry&,const EventDispatchServices&);
int32_t eventWaitMilliseconds(EventEntry&,const EventDispatchServices&);
// Returns the original dispatcher codes0(yield),-1(entry end),1(manager abort).
int32_t dispatchEvent(EventEntry&,const EventDispatchServices&);
}
