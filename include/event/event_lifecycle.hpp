#pragma once
#include "event_creation.hpp"

namespace ttyd {
// Valid entry/relationship/context domains are caller invariants. No callback,
// resource save, audio shutdown or immediate parent dispatch is implied.
// Delete visits child/owned brothers, copies16 locals/3 flag/7 message words
// back to waiting parent, observes retiring snapshot, then resets entry/count.
// Restart preserves identity/locals/links while resetting PC/lifetime/stacks/labels.
// Native storage retirement/heap free are different operations.
void deleteEvent(EventEntry&,const EventCreationServices&);
void deleteEventId(uint32_t id,const EventCreationServices&);
EventEntry& restartEvent(EventEntry&,const EventCreationServices&);
}
