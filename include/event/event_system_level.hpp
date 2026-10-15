#pragma once
#include "event_controls.hpp"
#include "event_memory_opcodes.hpp"

namespace ttyd {
// Live getter returns GP+0x18 mask, requested getter scalar0..4 at0x8041e940.
// Setter ignores requests outside0..4, updates low-nibble inhibit bits and invokes
// source type-mask stop/start; level and event flags are separate namespaces.
int32_t getEventSystemLevel(const EventMemoryServices&);
int32_t getRequestedEventSystemLevel(const EventMemoryServices&);
void setEventSystemLevel(int32_t request,const EventMemoryServices&,const EventCreationServices&);
}
