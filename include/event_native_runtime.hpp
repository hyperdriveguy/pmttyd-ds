#pragma once
#include "event_entry_storage.hpp"
#include "event_interpreter.hpp"

namespace ttyd {
struct EventNativePlatform {
    void* user=nullptr;
    uint32_t (*clock)(void*)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr;
    int32_t (*divideWord)(void*,int32_t,int32_t)=nullptr;
    int32_t (*call)(void*,EventEntry&,uint32_t function,int32_t firstCall)=nullptr;
    void (*format)(void*,const EventDiagnosticCall&)=nullptr;
    void (*flushBlock)(void*,uint32_t)=nullptr;
    void (*systemCall)(void*)=nullptr;
};
// Shared native storage/service owner. GP address and RAM/arena provisioning are
// explicit port policy. Platform callbacks are required at their call boundaries;
// no function address,clock,FPSCR or cache behavior is silently synthesized.
class EventNativeRuntime {
public:
    EventNativeRuntime(uint32_t gpBase,const EventNativePlatform&);
    EventNativeRuntime(const EventNativeRuntime&)=delete;
    EventNativeRuntime& operator=(const EventNativeRuntime&)=delete;
    EventManagerServices managerServices();
    EventCreationServices creationServices();
    EventInterpreterServices interpreterServices();
    void initializeContexts();
    void reinitializeContext();
    void runManager();

    EventGlobals gp;
    EventContext field,battle;
    EventRuntimeGlobals runtime{0,1,1,0}; // Original initialized ID/speed words.
    EventSchedule schedule;
    EventEntryAddresses entryAddresses;
    EventContextAddresses contextAddresses{entryAddresses};
    EventRuntimeAddresses runtimeAddresses{runtime,schedule};
    EventGpAddresses gpAddresses;
    EventAddressSpace memory{entryAddresses,contextAddresses,runtimeAddresses,gpAddresses};
    EventNativePlatform platform;
    EventMemoryBinding memoryBinding;
    EventEntryStorage entryStorage{entryAddresses,contextAddresses,memoryBinding};
};
}
