#pragma once
#include "event_manager.hpp"

namespace ttyd {
struct EventRuntimeGlobals {
    uint32_t allocationCount=0,nextId=0;
    float speed=0;
    int32_t systemLevel=0; // Requested enum8041e940,read by evtEntryRunCheck;distinct from GP18 mask.
};
struct EventCreationServices {
    void* user=nullptr;
    EventManagerServices manager;
    EventRuntimeGlobals* (*runtime)(void*)=nullptr;
    uint32_t (*word)(void*,uint32_t scriptAddress)=nullptr;
    void (*stopAll)(void*,uint32_t typeMask)=nullptr;
    // Optional read-only native observation after recursive deletion/copy-back,
    // before storage reset. This is not an original world callback.
    void (*retiring)(void*,const EventEntry&)=nullptr;
};
void scanEventLabels(EventEntry&,const EventCreationServices&);
void checkEventEntryRun(const EventCreationServices&);
// Available entry storage and at most16 labels are native domain invariants.
// Invalid domains assert; no recovered null/overflow repair is claimed.
EventEntry& createRootEvent(EventSchedule&,uint32_t script,int32_t priority,uint32_t flags,
                            const EventCreationServices&);
EventEntry& createTypedRootEvent(EventSchedule&,uint32_t script,int32_t priority,uint32_t flags,
                                 int32_t type,const EventCreationServices&);
EventEntry& createChildEvent(EventSchedule&,EventEntry& parent,uint32_t script,uint32_t flags,const EventCreationServices&);
EventEntry& createBrotherEvent(EventSchedule&,EventEntry& owner,uint32_t script,uint32_t flags,const EventCreationServices&);
int startAttachedBrotherBlock(EventSchedule&,EventEntry& owner,const EventCreationServices&);
int startIndependentInlineBlock(EventSchedule&,EventEntry& owner,const EventCreationServices&);
}
