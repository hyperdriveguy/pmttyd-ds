#pragma once
#include "event_creation.hpp"

namespace ttyd {
class EventRuntimeAddresses {
public:
    EventRuntimeAddresses(EventRuntimeGlobals& globals,EventSchedule& schedule):globals_(globals),schedule_(schedule) {}
    // Conservative interval probe; readable-byte checks retain sparse alias semantics.
    bool mayMapRange(uint32_t base,uint64_t size) const;
    bool readByte(uint32_t,uint8_t&) const;
    bool readWord(uint32_t,uint32_t&) const;
    bool writeByte(uint32_t,uint8_t);
    bool writeWord(uint32_t,uint32_t);
private:
    bool readField(uint32_t,uint32_t&) const;
    bool writeField(uint32_t,uint32_t);
    EventRuntimeGlobals& globals_;
    EventSchedule& schedule_;
};
}
