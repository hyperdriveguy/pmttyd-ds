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
    uint32_t restartScript=0,field1a4=0,field170=0,field150=0;
    int32_t owner=0,field16c=0;
    uint8_t type=0,loopIndex=0,switchIndex=0;
    EventEntry* waitingParent=nullptr;
    EventEntry* waitingChild=nullptr;
    EventEntry* brotherOwner=nullptr;
    uint32_t userWork=0,userFlags=0;
    std::array<int32_t,16> locals{};
    std::array<uint32_t,3> localFlags{};
    std::array<uint8_t,16> labelIds{};
    std::array<uint32_t,16> labelAddresses{};
    std::array<uint32_t,8> loopBodies{}; // Original e8..104,valid stack domain0..7.
    std::array<int32_t,8> loopCounts{}; // Original108..124;raw count operands.
    std::array<uint32_t,7> messageFields{}; // Original178..190,retained as raw words.
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
