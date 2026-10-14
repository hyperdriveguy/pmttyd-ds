#include "event_manager.hpp"
#include <cassert>
#include <utility>

namespace ttyd {
namespace {
float add(float a,float b) {return a+b;}
float sub(float a,float b) {return a-b;}
EventGlobals& globals(const EventManagerServices& s) {assert(s.globals);auto* g=s.globals(s.user);assert(g);return *g;}
EventContext& context(const EventManagerServices& s,const EventGlobals& g) {assert(s.context);auto* c=s.context(s.user,g.battleContext!=0);assert(c);return *c;}
EventEntry& entry(EventContext& c,int32_t slot) {assert(c.entries && slot>=0 && slot<c.capacity);return c.entries[slot];}
}
uint32_t eventClockRate(uint32_t clock) {
    return static_cast<uint32_t>((uint64_t(clock>>2)*0x10624dd3u)>>32)>>6;
}
uint64_t eventLifetimeDelta(uint64_t timestamp,uint64_t previous,uint32_t clock,int32_t refreshRate) {
    uint64_t delta=timestamp-previous;
    if(delta&0x8000000000000000ULL)delta=0;
    const uint32_t k=eventClockRate(clock);assert(k!=0);
    if(delta/k>500) {
        assert(refreshRate!=0);
        delta=static_cast<uint32_t>(1000/refreshRate)*k;
    }
    return delta;
}
void appendEventSchedule(EventSchedule& schedule,int32_t slot,uint32_t id) {
    assert(schedule.count>=0 && schedule.count<static_cast<int32_t>(schedule.slots.size()));
    schedule.slots[schedule.count]=slot;schedule.ids[schedule.count]=id;++schedule.count;
}
void buildEventPriorityTable(EventSchedule& schedule,const EventManagerServices& s) {
    auto& g=globals(s);auto& c=context(s,g);auto* entries=c.entries;
    assert(entries || c.capacity<=0);schedule.count=0;
    for(int32_t slot=0;slot<c.capacity;++slot)if(entries[slot].flags&1)appendEventSchedule(schedule,slot,entries[slot].id);
    for(int32_t i=0;i<schedule.count-1;++i)for(int32_t j=i+1;j<schedule.count;++j) {
        if(entry(c,schedule.slots[i]).priority<entry(c,schedule.slots[j]).priority) {
            std::swap(schedule.slots[i],schedule.slots[j]);std::swap(schedule.ids[i],schedule.ids[j]);
        }
    }
}
void runEventManager(EventSchedule& schedule,const EventManagerServices& s) {
    auto& g=globals(s);auto& c=context(s,g);schedule.managerActive=true;
    const uint64_t timestamp=g.timestamp;assert(s.clock);
    const uint32_t clock=s.clock(s.user);
    const uint64_t delta=eventLifetimeDelta(timestamp,c.previousTimestamp,clock,g.refreshRate);
    c.previousTimestamp=timestamp;
    buildEventPriorityTable(schedule,s);
    for(int32_t index=0;index<schedule.count;++index) {
        assert(index>=0 && index<static_cast<int32_t>(schedule.slots.size()));
        auto& e=entry(c,schedule.slots[index]);
        if(!(e.flags&1))continue;
        if(e.id!=schedule.ids[index])continue;
        if(e.flags&0x92)continue;
        e.lifetime+=delta;e.credit=add(e.credit,e.timescale);
        // Ordered comparison: no invented repair for NaN/infinite credit.
        while(!(e.credit<1.f)) {
            e.credit=sub(e.credit,1.f);assert(s.dispatch);
            const int32_t result=s.dispatch(s.user,e);
            if(result==1) {schedule.managerActive=false;return;}
            if(result==-1)break;
        }
    }
    schedule.managerActive=false;
}
}
