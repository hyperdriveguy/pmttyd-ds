#pragma once
#include "event_lifecycle.hpp"

namespace ttyd {
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
