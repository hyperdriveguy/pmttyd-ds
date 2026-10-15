#pragma once
#include "event_lifecycle.hpp"

namespace ttyd {
// Descendant child/brothers precede owner; repeated context/global captures
// preserve source traversal. Stop/start changes flag2 only on type-mask overlap;
// by-ID uses mask0xef. Priority/type setters narrow low8; speed multiplies the
// current global speed. None of these APIs frees native storage.
void stopEvent(EventEntry&,uint32_t mask,const EventCreationServices&);
void startEvent(EventEntry&,uint32_t mask,const EventCreationServices&);
void stopAllEvents(uint32_t mask,const EventCreationServices&);
void startAllEvents(uint32_t mask,const EventCreationServices&);
void stopOtherEvents(EventEntry* excluded,uint32_t mask,const EventCreationServices&);
void startOtherEvents(EventEntry* excluded,uint32_t mask,const EventCreationServices&);
void stopEventId(uint32_t id,const EventCreationServices&);
void startEventId(uint32_t id,const EventCreationServices&);
int32_t checkEventId(uint32_t id,const EventCreationServices&);
void setEventPriority(EventEntry&,int32_t);
void setEventType(EventEntry&,int32_t);
void setEventSpeed(EventEntry&,float,const EventCreationServices&);
}
