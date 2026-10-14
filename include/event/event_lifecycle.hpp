#pragma once
#include "event_creation.hpp"

namespace ttyd {
// Valid entry/relationship/context domains are caller invariants. No callback,
// resource save, audio shutdown or immediate parent dispatch is implied.
void deleteEvent(EventEntry&,const EventCreationServices&);
void deleteEventId(uint32_t id,const EventCreationServices&);
EventEntry& restartEvent(EventEntry&,const EventCreationServices&);
}
