#pragma once
#include "event_manager.hpp"
#include <vector>
#include <memory>

namespace ttyd {
struct PhysicalInputProducer;
class EventGpLanguageAlias;
class EventGpAddresses {
public:
    static constexpr uint32_t size=0x13d8; // Source marioStInit bulk-clear extent.
    // Borrow globals and retain other packed GP bytes. Registration neither
    // selects the new GP nor follows aliases; currentAddress is the raw original
    // pointer word and currentGlobals may be null for an unresolved token.
    bool registerGp(uint32_t originalBase,EventGlobals&);
    // One explicit live process owner for original GP170/172. Backing must
    // remain stable until GP retirement; no per-battle dimension reseeding.
    bool bindRenderDimensions(uint32_t originalBase,std::array<uint8_t,4>&);
    // Borrow actual input producer storage, not a copied pad snapshot. Caller
    // keeps producer/dimension backing alive; GP retirement drops these views.
    // Input/dimensions take precedence over ordinary packed bytes at their lanes.
    bool bindPhysicalInput(uint32_t originalBase,PhysicalInputProducer&);
    // Native process-language alias only; no other GP bytes are redirected.
    bool bindLanguageAlias(uint32_t originalBase,std::shared_ptr<EventGpLanguageAlias>);
    // Identity only; caller separately proves the actual source alias current.
    bool languageAliasIs(uint32_t originalBase,const std::shared_ptr<EventGpLanguageAlias>&)const;
    std::shared_ptr<const uint8_t> languageRegistration(uint32_t originalBase)const;
    bool languageSourceCurrent(uint32_t originalBase,const std::shared_ptr<const uint8_t>&)const;
    bool languageAliasSpan(uint32_t base,uint64_t bytes)const;
    bool readAliasSpan(uint32_t address,unsigned bytes,uint32_t&)const;
    bool writeAliasSpan(uint32_t address,unsigned bytes,uint32_t);
    bool unregisterGp(uint32_t originalBase,std::vector<uint8_t>* bytes=nullptr);
    EventGlobals* currentGlobals() const;
    uint32_t currentAddress() const {return current_;}
    // Conservative interval probe; readable-byte checks retain sparse alias semantics.
    bool mayMapRange(uint32_t base,uint64_t size) const;
    bool readByte(uint32_t,uint8_t&) const;
    bool readWord(uint32_t,uint32_t&) const;
    bool writeByte(uint32_t,uint8_t);
    bool writeWord(uint32_t,uint32_t);
private:
    struct Region {uint32_t base;EventGlobals* globals;std::vector<uint8_t> bytes;uint8_t* renderDimensions=nullptr;PhysicalInputProducer* input=nullptr;std::shared_ptr<const uint8_t> registration=std::make_shared<const uint8_t>(0);std::shared_ptr<EventGpLanguageAlias> language={};};
    struct Position {size_t region=0;uint32_t offset=0;bool valid=false;};
    Position find(uint32_t) const;
    bool readByteLocal(uint32_t,uint8_t&)const;
    bool writeByteLocal(uint32_t,uint8_t);
    mutable bool languageInvoking_=false;
    std::vector<Region> regions_;
    uint32_t current_=0; // Original GP pointer cell80415fd0,not a host pointer.
};
}
