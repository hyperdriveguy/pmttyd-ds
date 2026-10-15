#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct EventTimeServices {
    EventMemoryServices memory;
    void* user=nullptr;
    uint32_t (*upper)(void*)=nullptr;
    uint32_t (*lower)(void*)=nullptr;
};
// Sample upper/lower/upper until stable across low-word wrap. Publication
// retains source GP reloads, low-then-high stores, pause/window/system gates
// and total-time cap; it is a clock prefix, not a frame advance or hardware tail.
uint64_t readEventRawTime(const EventTimeServices&);
int32_t eventWindowFreeze(const EventMemoryServices&);
// Complete viPostCallback CLOCK PREFIX only. Hardware/reset/rumble/audio tail
// and VI registration are external owners;this does not advance render frames.
void publishEventRetraceTime(uint32_t callbackArgument,const EventTimeServices&);
}
