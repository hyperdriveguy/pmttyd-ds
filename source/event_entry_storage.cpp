#include "event_entry_storage.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>

namespace ttyd {
EventEntryStorage::~EventEntryStorage() {retireAll();}
EventEntry* EventEntryStorage::allocate(int32_t heap,uint32_t bytes) {
    assert(bytes!=0 && bytes%EventEntryAddresses::stride==0);
    const uint32_t base=eventHeapAllocate(heap,bytes,memory_.services());
    if(!base)return nullptr;
    const uint32_t count=bytes/EventEntryAddresses::stride;
    auto storage=std::make_unique<EventEntry[]>(count);
    std::vector<uint8_t> removed;
    const bool extracted=memory_.memory.extractRam(base,bytes,removed);assert(extracted);(void)extracted;
    const bool registered=entries_.registerEntries(base,storage.get(),count);assert(registered);(void)registered;
    // Preserve the bytes AFTER the allocator's clear/cache boundaries,including
    // any writes performed by its platform provider. Packed pointers retain
    // original tokens and resolve through the existing entry address mapper.
    for(uint32_t i=0;i<bytes;++i) {const bool copied=entries_.writeByte(base+i,removed[i]);assert(copied);(void)copied;}
    auto* result=storage.get();allocations_.push_back({base,count,std::move(storage)});
    contexts_.resolveEntryArrays();return result;
}
void EventEntryStorage::clear(EventEntry* entries,uint32_t bytes) {
    uint32_t base=0;const bool mapped=entries_.addressOf(entries,base);assert(mapped);(void)mapped;
    eventFillMemory(base,0,bytes,memory_.services()); // Additional original Init/ReInit memset;no cache flush.
}
bool EventEntryStorage::retire(uint32_t base) {
    for(size_t i=0;i<allocations_.size();++i)if(allocations_[i].base==base) {
        contexts_.detachEntryArrays(base,allocations_[i].count);
        std::vector<uint8_t> bytes;const bool removed=entries_.unregisterEntries(base,&bytes);assert(removed);(void)removed;
        const bool restored=memory_.memory.registerRam(base,std::move(bytes));assert(restored);(void)restored;
        allocations_.erase(allocations_.begin()+i);return true;
    }
    return false;
}
void EventEntryStorage::retireAll() {while(!allocations_.empty()) {const bool retired=retire(allocations_.back().base);assert(retired);(void)retired;}}
EventContextInitializationServices EventEntryStorage::initializationServices(const EventManagerServices& manager,
        EventContext& field,EventContext& battle,EventRuntimeGlobals& runtime,EventSchedule& schedule) {
    EventContextInitializationServices s;s.user=this;s.manager=manager;s.field=&field;s.battle=&battle;s.runtime=&runtime;s.schedule=&schedule;
    s.allocate=[](void* p,int32_t heap,uint32_t bytes) {return static_cast<EventEntryStorage*>(p)->allocate(heap,bytes);};
    s.clearEntries=[](void* p,EventEntry* e,uint32_t bytes) {static_cast<EventEntryStorage*>(p)->clear(e,bytes);};
    s.clearBank=[](void*,EventContext& c,bool flags) {if(flags)c.flags.fill(0);else c.work.fill(0);};
    s.timestampHalf=[](void*,EventContext& c,bool high,uint32_t v) {if(high)c.previousTimestamp=(uint64_t(v)<<32)|(c.previousTimestamp&0xffffffffu);else c.previousTimestamp=(c.previousTimestamp&0xffffffff00000000ULL)|v;};
    return s;
}
}
