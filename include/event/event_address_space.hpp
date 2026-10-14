#pragma once
#include "event_context_addresses.hpp"
#include "event_gp_addresses.hpp"
#include "event_runtime_addresses.hpp"
#include "event_script_assets.hpp"

namespace ttyd {
// Native owner-facing storage adapter. Register live views before RAM; introducing
// overlapping live mappings afterward is outside this adapter's domain.
class EventAddressSpace {
public:
    EventAddressSpace(EventEntryAddresses& e,EventContextAddresses& c,
                      EventRuntimeAddresses& r,EventGpAddresses& g):entries_(e),contexts_(c),runtime_(r),gp_(g) {}
    bool registerRam(uint32_t base,std::vector<uint8_t> bytes);
    bool unregisterRam(uint32_t base,std::vector<uint8_t>* bytes=nullptr);
    // Remove a span contained in one RAM region,retaining neighboring bytes.
    // Native owner uses this before registering a live entry view at that span.
    bool extractRam(uint32_t base,uint32_t size,std::vector<uint8_t>& bytes);
    // Copy every segment transactionally into writable original-address RAM.
    // Extracted assets remain unchanged;these are runtime-owned data copies.
    bool importScripts(const EventScriptAssets&);
    bool readByte(uint32_t,uint8_t&) const;
    bool readHalf(uint32_t,uint16_t&) const;
    bool readWord(uint32_t,uint32_t&) const;
    bool writeByte(uint32_t,uint8_t);
    bool writeHalf(uint32_t,uint16_t);
    bool writeWord(uint32_t,uint32_t);
private:
    struct Region {uint32_t base;std::vector<uint8_t> bytes;};
    bool available(uint32_t,uint64_t) const;
    bool readSpan(uint32_t,unsigned,uint32_t&) const;
    bool writeSpan(uint32_t,unsigned,uint32_t);
    EventEntryAddresses& entries_;
    EventContextAddresses& contexts_;
    EventRuntimeAddresses& runtime_;
    EventGpAddresses& gp_;
    std::vector<Region> ram_;
};
}
