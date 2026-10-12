#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
// Semantic fields used by the recovered manager. Caller-owned entry storage and
// IDs remain separate from schedule slots; this is not the original packed ABI.
struct EventEntry {
    uint64_t lifetime=0;
    uint8_t flags=0,priority=0;
    uint32_t id=0;
    float timescale=0,credit=0;
    uint8_t opcode=0,argumentCount=0,initialized=0;
    uint32_t nextCommand=0,currentCommand=0,arguments=0,cachedFunction=0;
    int32_t waitCount=0;
    uint64_t waitStart=0;
};
struct EventContext {
    int32_t capacity=0;
    EventEntry* entries=nullptr;
    uint64_t previousTimestamp=0;
};
struct EventGlobals {
    int32_t battleContext=0,refreshRate=0;
    uint64_t timestamp=0;
};
struct EventSchedule {
    std::array<int32_t,256> slots{};
    std::array<uint32_t,256> ids{};
    int32_t count=0;
    bool managerActive=false;
};
struct EventManagerServices {
    void* user=nullptr;
    EventGlobals* (*globals)(void*)=nullptr;
    EventContext* (*context)(void*,bool battle)=nullptr;
    uint32_t (*clock)(void*)=nullptr;
    int32_t (*dispatch)(void*,EventEntry&)=nullptr;
};
uint32_t eventClockRate(uint32_t clock);
uint64_t eventLifetimeDelta(uint64_t timestamp,uint64_t previous,uint32_t clock,int32_t refreshRate);
void buildEventPriorityTable(EventSchedule&,const EventManagerServices&);
void appendEventSchedule(EventSchedule&,int32_t slot,uint32_t id);
void runEventManager(EventSchedule&,const EventManagerServices&);
}
