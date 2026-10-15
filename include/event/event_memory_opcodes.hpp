#pragma once
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
struct EventMemoryServices {
    void* user=nullptr;
    uint32_t (*word)(void*,uint32_t)=nullptr;
    uint16_t (*half)(void*,uint32_t)=nullptr;
    void (*storeWord)(void*,uint32_t,uint32_t)=nullptr;
    void (*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
    void (*storeByte)(void*,uint32_t,uint8_t)=nullptr;
    void (*flushBlock)(void*,uint32_t)=nullptr; // Original dcbf address operand.
    void (*systemCall)(void*)=nullptr; // Original sc after nonzero-range flush.
};
// Original memory primitives use guest address/word wrap and source fill/
// flush ordering. Zero-length flush has no syscall; mapped extents and actual
// platform cache effects are required domains, not optional zero-filled memory.
uint32_t eventFillMemory(uint32_t address,int32_t fill,uint32_t length,const EventMemoryServices&);
void eventFlushMemory(uint32_t address,uint32_t length,const EventMemoryServices&);
uint32_t eventMapAllocate(uint32_t root,uint32_t bytes,const EventMemoryServices&);
struct EventMemoryOpcodeServices {
    EventDispatchServices values;
    EventMemoryServices memory;
};
// User buffers,bitwise,time and original-address/indirect memory74..90.
// Empty optional means unrouted opcode; an engaged handler0 is a retry.
// It does not acknowledge a missing platform memory/cache provider.
std::optional<int32_t> eventMemoryOpcode(EventEntry&,uint8_t,const EventMemoryOpcodeServices&);
}
