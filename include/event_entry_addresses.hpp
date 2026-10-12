#pragma once
#include "event_manager.hpp"
#include <vector>

namespace ttyd {
class EventEntryAddresses {
public:
    static constexpr uint32_t stride=0x1b0;
    // Registration is native provider policy:nonzero,nonoverlapping original
    // extents and unique live host entries. It is not an original VM bounds check.
    bool registerEntries(uint32_t originalBase,EventEntry*,uint32_t count);
    // Call before host storage is released/replaced. Incoming relationships
    // retain raw addresses;optional bytes let the address-space owner preserve
    // memory contents independently of the old host objects.
    bool unregisterEntries(uint32_t originalBase,std::vector<uint8_t>* bytes=nullptr);
    bool addressOf(const EventEntry*,uint32_t& address) const;
    EventEntry* entryAt(uint32_t originalAddress) const;
    bool readByte(uint32_t address,uint8_t&) const;
    bool readWord(uint32_t address,uint32_t&) const;
    bool writeByte(uint32_t address,uint8_t);
    bool writeWord(uint32_t address,uint32_t);
    bool linksResolved(const EventEntry&) const;
private:
    struct Region {uint32_t base,count;EventEntry* entries;};
    struct Position {EventEntry* entry=nullptr;uint32_t offset=0;};
    Position find(uint32_t) const;
    bool readFieldWord(EventEntry&,uint32_t offset,uint32_t&) const;
    bool writeFieldWord(EventEntry&,uint32_t offset,uint32_t);
    std::vector<Region> regions_;
};
}
