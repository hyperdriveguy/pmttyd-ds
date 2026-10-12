#include "event_context_initialization.hpp"
#include <cassert>

namespace ttyd {
namespace {
EventGlobals& globals(const EventContextInitializationServices& s) {assert(s.manager.globals);auto* g=s.manager.globals(s.manager.user);assert(g);return *g;}
void timestamp(EventContext& c,EventGlobals& gp,const EventContextInitializationServices& s) {
    const uint32_t high=static_cast<uint32_t>(gp.timestamp>>32),low=static_cast<uint32_t>(gp.timestamp);
    assert(s.timestampHalf);s.timestampHalf(s.user,c,false,low);s.timestampHalf(s.user,c,true,high);
}
void initialize(EventContext& c,const EventContextInitializationServices& s) {
    c.capacity=256;assert(s.allocate);auto* allocated=s.allocate(s.user,0,0x1b000);
    c.entries=allocated;c.entriesUnresolved=false;timestamp(c,globals(s),s);
    assert(s.clearEntries && s.clearBank);s.clearEntries(s.user,allocated,static_cast<uint32_t>(c.capacity)*0x1b0u);
    s.clearBank(s.user,c,false);s.clearBank(s.user,c,true);
}
}
void initializeEventContexts(const EventContextInitializationServices& s) {
    assert(s.field && s.battle && s.runtime && s.schedule);
    initialize(*s.field,s);initialize(*s.battle,s);
    s.runtime->allocationCount=0;s.schedule->count=0;s.schedule->managerActive=false;
    // evt_msg_init is a complete no-op;no invented message/resource reset.
}
void reinitializeEventContext(const EventContextInitializationServices& s) {
    assert(s.field && s.battle && s.runtime && s.schedule);
    auto& gp=globals(s);auto& c=gp.battleContext!=0?*s.battle:*s.field;timestamp(c,gp,s);
    const int32_t capacity=c.capacity;auto* entries=c.entries;assert(s.clearEntries);
    s.clearEntries(s.user,entries,static_cast<uint32_t>(capacity)*0x1b0u);
    s.runtime->allocationCount=0;s.schedule->managerActive=false;
}
}
