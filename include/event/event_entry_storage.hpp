#pragma once
#include "event_memory_binding.hpp"
#include "event_context_initialization.hpp"
#include <memory>

namespace ttyd {
// Platform owner for SDK-allocated event entry arrays. Referenced mappings and
// memory binding must outlive this object. Destruction retires live views to raw
// RAM;it does not call original heap free or synthesize event deletion.
class EventEntryStorage {
public:
    EventEntryStorage(EventEntryAddresses& e,EventContextAddresses& c,EventMemoryBinding& m):entries_(e),contexts_(c),memory_(m) {}
    ~EventEntryStorage();
    EventEntryStorage(const EventEntryStorage&)=delete;
    EventEntryStorage& operator=(const EventEntryStorage&)=delete;
    // Bytes are multiples of packed0x1b0, never host sizeof(EventEntry).
    // Allocate source heap first, extract raw RAM, then import its actual cleared/
    // provider-written bytes into semantic entries and resolve pending arrays.
    EventEntry* allocate(int32_t heap,uint32_t bytes);
    void clear(EventEntry*,uint32_t bytes);
    // Detach contexts, snapshot/unregister entries, restore raw RAM, then drop
    // host storage. Original heap allocation remains live until its separate free.
    bool retire(uint32_t originalBase);
    void retireAll();
    EventContextInitializationServices initializationServices(const EventManagerServices&,
        EventContext& field,EventContext& battle,EventRuntimeGlobals&,EventSchedule&);
private:
    struct Allocation {uint32_t base,count;std::unique_ptr<EventEntry[]> entries;};
    EventEntryAddresses& entries_;
    EventContextAddresses& contexts_;
    EventMemoryBinding& memory_;
    std::vector<Allocation> allocations_;
};
}
