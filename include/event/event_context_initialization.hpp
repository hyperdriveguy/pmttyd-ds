#pragma once
#include "event_creation.hpp"

namespace ttyd {
struct EventContextInitializationServices {
    void* user=nullptr;
    EventManagerServices manager;
    EventContext* field=nullptr;
    EventContext* battle=nullptr;
    EventRuntimeGlobals* runtime=nullptr;
    EventSchedule* schedule=nullptr;
    // Original heap/count arguments; adapter maps allocated storage to semantic
    // entries. Null allocation still reaches clearEntries with positive bytes.
    EventEntry* (*allocate)(void*,int32_t heap,uint32_t originalBytes)=nullptr;
    void (*clearEntries)(void*,EventEntry*,uint32_t originalBytes)=nullptr;
    void (*clearBank)(void*,EventContext&,bool flags)=nullptr;
    void (*timestampHalf)(void*,EventContext&,bool high,uint32_t)=nullptr;
};
// Init allocates256 packed0x1b0 entries from heap0 for EACH context and
// clears work/flag banks. ReInit clears only selected entries/timestamp and
// allocation/active state, retaining work/flags and schedule.count. Providers
// observe low-then-high timestamp stores; neither routine retires old allocations.
void initializeEventContexts(const EventContextInitializationServices&);
void reinitializeEventContext(const EventContextInitializationServices&);
}
