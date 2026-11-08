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
    // Owned/external mappings reject zero size, 32-bit wrap and readable
    // live aliases. Extent probes are conservative for raw acceleration; sparse
    // semantic holes may still admit RAM through registration's byte checks.
    bool registerRam(uint32_t base,std::vector<uint8_t> bytes);
    // Caller retains stable backing storage until unregister or facade destruction.
    // Big-endian byte aliases share one process owner across separate VM lifetimes.
    bool registerExternalRam(uint32_t base,uint8_t* bytes,uint32_t size);
    bool unregisterExternalRam(uint32_t base);
    // Owner teardown must not remove a replacement registered at the same guest
    // address. This checks raw external identity, including shadowed mappings.
    bool unregisterExternalRam(uint32_t base,const uint8_t* expectedBacking);
    // Borrow stable admitted raw storage into a second VM at the SAME guest
    // address. Source storage/semantic mappings outlive the view unchanged.
    // Retire target external mapping before changing/destroying source RAM.
    bool shareRamTo(EventAddressSpace& target,uint32_t address,uint32_t size) const;
    bool sharesRamWith(const EventAddressSpace& target,uint32_t address,uint32_t size) const;
    bool unregisterRam(uint32_t base,std::vector<uint8_t>* bytes=nullptr);
    // Remove a span contained in one RAM region,retaining neighboring bytes.
    // Native owner uses this before registering a live entry view at that span.
    bool extractRam(uint32_t base,uint32_t size,std::vector<uint8_t>& bytes);
    // Copy every segment transactionally into writable original-address RAM.
    // Extracted assets remain unchanged;these are runtime-owned data copies.
    bool importScripts(const EventScriptAssets&);
    // One complete raw RAM span, excluding semantic providers. No pointer
    // escapes and callers must recheck after any mapping mutation.
    bool contiguousRam(uint32_t a,uint32_t n) const{return n && rawRamSpan(a,n);}
    // Pure identity query, also safe inside an existing synchronous raw borrow.
    // Never returns a pointer or opens a nested borrow; no mapping may mutate.
    bool hasRamBacking(uint32_t a,const uint8_t* expected,uint32_t n) const{
        return expected && n && rawRamSpan(a,n)==expected;
    }
    // Synchronous borrow only: blocks this facade's storage mapping changes
    // and nested borrows. Independent semantic registries are NOT locked; caller
    // must keep them unchanged and must not call children/yield in the callback.
    class RawRamAccess;
    template<typename Callback> bool withRawRamAccess(Callback&&);
    // Pure current mapping admission, using the same interval/semantic checks
    // as registration. No storage is reserved; recheck after mapping changes.
    bool unmappedRange(uint32_t base,uint64_t size) const;
    // BE widths alias the same live fields. A fallback store first checks
    // byte readability to avoid unmapped-prefix writes; this is not a transaction
    // against arbitrary semantic/provider side effects or concurrent mutation.
    // Fresh native ownership proof; no value cache or pointer escapes.
    bool matchesBytes(uint32_t,const uint8_t*,uint32_t) const;
    // Closed native data-copy admission, only inside a raw borrow. False has
    // no stores; owned destination must not physically alias external views.
    bool copyOwnedRam(uint32_t destination,uint32_t source,uint32_t bytes);
    bool readByte(uint32_t,uint8_t&) const;
    bool readHalf(uint32_t,uint16_t&) const;
    bool readWord(uint32_t,uint32_t&) const;
    bool writeByte(uint32_t,uint8_t);
    bool writeHalf(uint32_t,uint16_t);
    bool writeWord(uint32_t,uint32_t);
private:
    bool rawRamBorrowed_=false;
    RawRamAccess* rawRamCache_=nullptr;
    struct Region {uint32_t base;std::vector<uint8_t> bytes;};
    bool available(uint32_t,uint64_t) const;
    const uint8_t* rawRamSpan(uint32_t,unsigned) const;
    uint8_t* rawRamSpan(uint32_t,unsigned);
    bool readSpan(uint32_t,unsigned,uint32_t&) const;
    bool writeSpan(uint32_t,unsigned,uint32_t);
    EventEntryAddresses& entries_;
    EventContextAddresses& contexts_;
    EventRuntimeAddresses& runtime_;
    EventGpAddresses& gp_;
    std::vector<Region> ram_;
    struct ExternalRegion {uint32_t base,size;uint8_t* bytes;};
    std::vector<ExternalRegion> external_;
};
}
#include "event_ram_access.hpp"
