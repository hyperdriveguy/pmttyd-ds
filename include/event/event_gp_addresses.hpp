#pragma once
#include "event_manager.hpp"
#include <vector>

namespace ttyd {
class EventGpAddresses {
public:
    static constexpr uint32_t size=0x13d8; // Source marioStInit bulk-clear extent.
    bool registerGp(uint32_t originalBase,EventGlobals&);
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
    struct Region {uint32_t base;EventGlobals* globals;std::vector<uint8_t> bytes;};
    struct Position {size_t region=0;uint32_t offset=0;bool valid=false;};
    Position find(uint32_t) const;
    std::vector<Region> regions_;
    uint32_t current_=0; // Original GP pointer cell80415fd0,not a host pointer.
};
}
