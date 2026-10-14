#include "event_lifecycle.hpp"
#include <cassert>

namespace ttyd {
namespace {
EventContext& currentContext(const EventCreationServices& s) {
    assert(s.manager.globals && s.manager.context);
    auto* gp=s.manager.globals(s.manager.user);assert(gp);
    auto* c=s.manager.context(s.manager.user,gp->battleContext!=0);assert(c);return *c;
}
EventRuntimeGlobals& runtime(const EventCreationServices& s) {assert(s.runtime);auto* g=s.runtime(s.user);assert(g);return *g;}
void copyBack(EventEntry& target,const EventEntry& source) {
    for(unsigned i=0;i<16;++i)target.locals[i]=source.locals[i];
    for(unsigned i=0;i<3;++i)target.localFlags[i]=source.localFlags[i];
    target.messageFields[1]=source.messageFields[1];target.messageFields[0]=source.messageFields[0];
    target.messageFields[2]=source.messageFields[2];
    const uint32_t a=source.messageFields[3],b=source.messageFields[4];
    target.messageFields[3]=a;target.messageFields[4]=b;target.messageFields[5]=source.messageFields[5];target.messageFields[6]=source.messageFields[6];
}
}
void deleteEvent(EventEntry& e,const EventCreationServices& s) {
    auto& c=currentContext(s);
    if(!(e.flags&1))return;
    assert(!(e.unresolvedLinkMask&2)); // Unmapped nonzero child is a native address-domain failure.
    if(e.waitingChild)deleteEvent(*e.waitingChild,s);
    auto* entries=c.entries;assert(entries || c.capacity<=0);
    for(int32_t slot=0;slot<c.capacity;++slot) {
        auto& candidate=entries[slot];
        if((candidate.flags&1) && candidate.brotherOwner==&e)deleteEvent(candidate,s);
    }
    assert(!(e.unresolvedLinkMask&1)); // Preserve raw bits until a dereference needs a mapped entry.
    if(e.waitingParent) {
        auto& parent=*e.waitingParent;parent.flags&=~uint8_t(0x10);parent.waitingChild=nullptr;parent.unresolvedLinkMask&=~uint8_t(2);copyBack(parent,e);
    }
    e.flags&=~uint8_t(1);e={};--runtime(s).allocationCount;
}
void deleteEventId(uint32_t id,const EventCreationServices& s) {
    auto& c=currentContext(s);auto* entries=c.entries;assert(entries || c.capacity<=0);
    for(int32_t slot=0;slot<c.capacity;++slot)if((entries[slot].flags&1) && entries[slot].id==id)deleteEvent(entries[slot],s);
}
EventEntry& restartEvent(EventEntry& e,const EventCreationServices& s) {
    const uint32_t script=e.restartScript;e.nextCommand=script;e.currentCommand=script;e.opcode=0;
    e.timescale=1.f;e.credit=0.f;e.loopIndex=e.switchIndex=0xff;
    e.timescale=runtime(s).speed;e.credit=0.f;e.lifetime=0;
    e.labelIds.fill(0xff);e.labelAddresses.fill(0);scanEventLabels(e,s);checkEventEntryRun(s);return e;
}
}
