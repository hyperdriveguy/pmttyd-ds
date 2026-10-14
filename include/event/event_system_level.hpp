#pragma once
#include "event_controls.hpp"
#include "event_memory_opcodes.hpp"

namespace ttyd {
int32_t getEventSystemLevel(const EventMemoryServices&);
int32_t getRequestedEventSystemLevel(const EventMemoryServices&);
void setEventSystemLevel(int32_t request,const EventMemoryServices&,const EventCreationServices&);
}
