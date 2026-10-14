#pragma once
#include "event_entry_addresses.hpp"

namespace ttyd {
class EventContextAddresses {
public:
    explicit EventContextAddresses(EventEntryAddresses& entries):entries_(entries) {}
    bool registerContext(uint32_t originalBase,EventContext&);
    // Conservative interval probe; readable-byte checks retain sparse alias semantics.
    bool mayMapRange(uint32_t base,uint64_t size) const;
    bool readByte(uint32_t,uint8_t&) const;
    bool readWord(uint32_t,uint32_t&) const;
    bool writeByte(uint32_t,uint8_t);
    bool writeWord(uint32_t,uint32_t);
    void resolveEntryArrays();
    // Owner calls this BEFORE entry unregister/free;array address bits survive.
    void detachEntryArrays(uint32_t base,uint32_t count);
private:
    struct Region {uint32_t base;EventContext* context;};
    struct Position {EventContext* context=nullptr;uint32_t offset=0;};
    Position find(uint32_t) const;
    bool readField(EventContext&,uint32_t,uint32_t&) const;
    bool writeField(EventContext&,uint32_t,uint32_t);
    EventEntryAddresses& entries_;
    std::vector<Region> regions_;
};
}
