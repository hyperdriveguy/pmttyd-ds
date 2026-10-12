#include "event_controls.hpp"
#include <cassert>

namespace ttyd {
namespace {
struct Selected {bool battle;EventContext* context;};
EventContext& context(const EventCreationServices& s,bool battle) {
    assert(s.manager.context);auto* c=s.manager.context(s.manager.user,battle);assert(c);return *c;
}
Selected selected(const EventCreationServices& s) {
    assert(s.manager.globals);auto* gp=s.manager.globals(s.manager.user);assert(gp);
    const bool battle=gp->battleContext!=0;return {battle,&context(s,battle)};
}
void mutate(EventEntry& e,uint32_t mask,bool stop) {
    if(e.type&mask) {if(stop)e.flags|=2;else e.flags&=~uint8_t(2);}
}
void visit(EventEntry&,uint32_t,bool,const EventCreationServices&);
template<class Visitor> void brothers(EventContext& c,EventEntry& owner,Visitor visitor) {
    auto* entries=c.entries;assert(entries || c.capacity<=0);
    for(int32_t slot=0;slot<c.capacity;++slot)if((entries[slot].flags&1) && entries[slot].brotherOwner==&owner)visitor(entries[slot]);
}
// Body of one compiler-inlined node: descendants use ordinary full invocations.
void body(EventEntry& e,EventContext& c,uint32_t mask,bool stop,const EventCreationServices& s) {
    if(e.waitingChild)visit(*e.waitingChild,mask,stop,s);
    brothers(c,e,[&](EventEntry& b) {visit(b,mask,stop,s);});mutate(e,mask,stop);
}
void visit(EventEntry& e,uint32_t mask,bool stop,const EventCreationServices& s) {
    const auto outer=selected(s);
    if(e.waitingChild) {
        auto& child=*e.waitingChild;auto& cc=context(s,outer.battle);
        auto* grandchild=child.waitingChild;
        if(grandchild) {
            auto& gc=context(s,outer.battle);body(*grandchild,gc,mask,stop,s);
        }
        brothers(cc,child,[&](EventEntry& b) {const auto inner=selected(s);body(b,*inner.context,mask,stop,s);});
        mutate(child,mask,stop);
    }
    brothers(*outer.context,e,[&](EventEntry& b) {
        const auto inner=selected(s);
        auto* child=b.waitingChild;
        if(child) {auto& cc=context(s,inner.battle);body(*child,cc,mask,stop,s);}
        brothers(*inner.context,b,[&](EventEntry& sibling) {const auto other=selected(s);body(sibling,*other.context,mask,stop,s);});
        mutate(b,mask,stop);
    });
    mutate(e,mask,stop);
}
void all(EventEntry* excluded,bool exclude,uint32_t mask,bool stop,const EventCreationServices& s) {
    const auto current=selected(s);auto& c=*current.context;auto* entries=c.entries;assert(entries || c.capacity<=0);
    for(int32_t slot=0;slot<c.capacity;++slot)if((entries[slot].flags&1) && (!exclude || &entries[slot]!=excluded))visit(entries[slot],mask,stop,s);
}
void byId(uint32_t id,bool stop,const EventCreationServices& s) {
    const auto current=selected(s);auto& c=*current.context;auto* entries=c.entries;assert(entries || c.capacity<=0);
    for(int32_t slot=0;slot<c.capacity;++slot)if((entries[slot].flags&1) && entries[slot].id==id)visit(entries[slot],0xef,stop,s);
}
}
void stopEvent(EventEntry& e,uint32_t mask,const EventCreationServices& s) {visit(e,mask,true,s);}
void startEvent(EventEntry& e,uint32_t mask,const EventCreationServices& s) {visit(e,mask,false,s);}
void stopAllEvents(uint32_t mask,const EventCreationServices& s) {all(nullptr,false,mask,true,s);}
void startAllEvents(uint32_t mask,const EventCreationServices& s) {all(nullptr,false,mask,false,s);}
void stopOtherEvents(EventEntry* e,uint32_t mask,const EventCreationServices& s) {all(e,true,mask,true,s);}
void startOtherEvents(EventEntry* e,uint32_t mask,const EventCreationServices& s) {all(e,true,mask,false,s);}
void stopEventId(uint32_t id,const EventCreationServices& s) {byId(id,true,s);}
void startEventId(uint32_t id,const EventCreationServices& s) {byId(id,false,s);}
int32_t checkEventId(uint32_t id,const EventCreationServices& s) {
    const auto current=selected(s);auto& c=*current.context;const int32_t capacity=c.capacity;auto* entries=c.entries;assert(entries || capacity<=0);
    for(int32_t slot=0;slot<capacity;++slot)if((entries[slot].flags&1) && entries[slot].id==id)return 1;
    return 0;
}
void setEventPriority(EventEntry& e,int32_t priority) {e.priority=static_cast<uint8_t>(priority);}
void setEventType(EventEntry& e,int32_t type) {e.type=static_cast<uint8_t>(type);}
void setEventSpeed(EventEntry& e,float speed,const EventCreationServices& s) {
    assert(s.runtime);auto* g=s.runtime(s.user);assert(g);e.timescale=speed*g->speed;
}
}
