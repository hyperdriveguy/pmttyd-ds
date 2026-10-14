#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
// Semantic fields used by the recovered manager. Caller-owned entry storage and
// IDs remain separate from schedule slots; this is not the original packed ABI.
struct EventEntry {
    uint64_t lifetime=0;
    // Source bits:1 allocated,2 stopped,0x10 waiting child,0x20 append root
    // during a live manager pass,0x80 inhibited. Unknown bits stay raw.
    uint8_t flags=0,priority=0;
    uint32_t id=0;
    float timescale=0,credit=0;
    uint8_t opcode=0,argumentCount=0,initialized=0;
    uint32_t nextCommand=0,currentCommand=0,arguments=0,cachedFunction=0;
    int32_t waitCount=0;
    uint64_t waitStart=0;
    uint32_t restartScript=0,field1a4=0,field170=0,field150=0;
    int32_t owner=0,field16c=0;
    // type is an event-group mask, separate from flags. Empty stack sentinel
    // is0xff (signed -1); an eight-level stack push wraps that sentinel to0.
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
    std::array<uint8_t,8> switchStates{}; // Original128..12f,signed-byte states.
    std::array<int32_t,8> switchValues{}; // Original130..14c.
    std::array<uint32_t,7> messageFields{}; // Original178..190,retained as raw words.
    std::array<uint8_t,44> rawBytes{}; // Original unknown spans10,84,174,194,1ac.
    std::array<uint32_t,3> unresolvedLinkWords{};
    // Link bits0/1/2 refer to parent/child/brother respectively. Null host
    // pointers with a set bit retain a nonzero original address, not absent links.
    uint8_t unresolvedLinkMask=0; // Raw nonzero links awaiting address mapping.
};
struct EventContext {
    int32_t capacity=0;
    EventEntry* entries=nullptr;
    uint64_t previousTimestamp=0;
    std::array<int32_t,32> work{}; // Original context+4..83.
    std::array<uint32_t,3> flags{}; // Original context+84..8f.
    uint32_t field94=0; // Raw word,unwritten by Init/ReInit.
    uint32_t unresolvedEntriesWord=0;
    bool entriesUnresolved=false; // Nonzero original array address not yet mapped.
};
struct EventGlobals {
    int32_t battleContext=0,refreshRate=0;
    uint64_t timestamp=0;
};
// Fixed source schedule capacity256. IDs are generation identities, slots are
// storage indices; keeping both detects a recycled slot before dispatch.
struct EventSchedule {
    std::array<int32_t,256> slots{};
    std::array<uint32_t,256> ids{};
    int32_t count=0;
    uint32_t managerActive=0; // Original shared word;retain nonzero/raw byte aliases.
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
