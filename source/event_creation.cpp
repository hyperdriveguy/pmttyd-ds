#include "event_creation.hpp"
#include <cassert>

namespace ttyd {
namespace {
EventRuntimeGlobals& runtime(const EventCreationServices& s) {assert(s.runtime);auto* g=s.runtime(s.user);assert(g);return *g;}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?static_cast<int32_t>(v):static_cast<int32_t>(v)-0x10000;}
uint32_t read(const EventCreationServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
struct Allocation {EventEntry* entry;int32_t slot;};
Allocation allocate(const EventCreationServices& s) {
    assert(s.manager.globals && s.manager.context);
    auto* gp=s.manager.globals(s.manager.user);assert(gp);
    auto* context=s.manager.context(s.manager.user,gp->battleContext!=0);assert(context && context->entries);
    const int32_t capacity=context->capacity;auto* entries=context->entries;int32_t slot=0;
    while(slot<capacity && (entries[slot].flags&1))++slot;
    assert(slot>=0 && slot<capacity);auto* chosen=&entries[slot];
    ++runtime(s).allocationCount;return {chosen,slot};
}
void append(EventSchedule& schedule,int32_t slot,EventEntry& entry) {
    const int32_t count=schedule.count;
    assert(count>=0 && count<static_cast<int32_t>(schedule.slots.size()));
    schedule.slots[count]=slot;schedule.ids[count]=entry.id;schedule.count=count+1;
}
void repairId(const EventCreationServices& s) {auto& after=runtime(s);if(after.nextId==0)after.nextId=1;}
void metadata(EventEntry& target,const EventEntry& source) {
    target.messageFields[1]=source.messageFields[1];target.messageFields[0]=source.messageFields[0];
    target.messageFields[2]=source.messageFields[2];
    const auto a=source.messageFields[3],b=source.messageFields[4];
    target.messageFields[3]=a;target.messageFields[4]=b;
    target.messageFields[5]=source.messageFields[5];target.messageFields[6]=source.messageFields[6];
}
void locals(EventEntry& target,const EventEntry& source) {
    for(unsigned i=0;i<16;++i)target.locals[i]=source.locals[i];
    for(unsigned i=0;i<3;++i)target.localFlags[i]=source.localFlags[i];
}
uint32_t blockEnd(uint32_t script,uint16_t endOpcode,const EventCreationServices& s) {
    uint32_t p=script;
    for(;;) {
        const uint32_t command=read(s,p);p+=4+static_cast<uint32_t>(4*signedHalf(command>>16));
        if((command&0xffff)==endOpcode)return p;
    }
}

}
void scanEventLabels(EventEntry& e,const EventCreationServices& s) {
    uint32_t address=e.nextCommand;unsigned labels=0;
    for(;;) {
        const uint32_t command=read(s,address),operand=read(s,address+4);
        const uint32_t opcode=command&0xffff;const int32_t count=signedHalf(command>>16);
        address+=4+static_cast<uint32_t>(4*count);
        if(opcode==1)return;
        if(opcode==3) {
            assert(labels<e.labelIds.size());const unsigned index=labels;
            e.labelIds[index]=static_cast<uint8_t>(operand);++labels;e.labelAddresses[index]=address;
        }
    }
}
void checkEventEntryRun(const EventCreationServices& s) {
    const int32_t level=runtime(s).systemLevel;uint32_t mask;
    switch(level) {
    case 1:mask=1;break;
    case 2:mask=2;break;
    case 3:mask=0x10;break;
    case 4:mask=0xef;break;
    default:return;
    }
    assert(s.stopAll);s.stopAll(s.user,mask);
}
EventEntry& createTypedRootEvent(EventSchedule& schedule,uint32_t script,int32_t priority,uint32_t flags,
                                 int32_t type,const EventCreationServices& s) {
    const auto allocated=allocate(s);auto& e=*allocated.entry;e={};
    e.flags=static_cast<uint8_t>(flags|1);e.nextCommand=script;
    e.restartScript=script;e.currentCommand=script;
    e.opcode=0;e.waitingParent=e.waitingChild=e.brotherOwner=nullptr;
    e.priority=static_cast<uint8_t>(priority);
    auto& ids=runtime(s);const uint32_t previous=ids.nextId;++ids.nextId;e.id=previous;
    e.owner=-1;e.loopIndex=0xff;e.switchIndex=0xff;e.type=static_cast<uint8_t>(type);
    e.field1a4=0;e.timescale=runtime(s).speed;e.credit=0.f;e.field16c=-1;e.field170=0;e.lifetime=0;
    e.locals.fill(0);e.localFlags.fill(0);e.labelIds.fill(0xff);e.labelAddresses.fill(0);
    scanEventLabels(e,s);
    if(schedule.managerActive && (e.flags&0x20))append(schedule,allocated.slot,e);
    checkEventEntryRun(s);repairId(s);
    return e;
}
EventEntry& createRootEvent(EventSchedule& schedule,uint32_t script,int32_t priority,uint32_t flags,
                            const EventCreationServices& s) {
    return createTypedRootEvent(schedule,script,priority,flags,0xef,s);
}

namespace {
EventEntry& related(EventSchedule& schedule,EventEntry& source,uint32_t script,uint32_t flags,bool child,const EventCreationServices& s) {
    const auto allocated=allocate(s);auto& e=*allocated.entry;
    if(child) {source.waitingChild=&e;source.flags|=0x10;}
    e={};e.flags=static_cast<uint8_t>(flags|1);
    e.nextCommand=script;e.restartScript=script;e.currentCommand=script;e.opcode=0;
    if(child) {e.waitingParent=&source;e.waitingChild=nullptr;e.brotherOwner=nullptr;}
    else {e.waitingParent=nullptr;e.brotherOwner=&source;e.waitingChild=nullptr;}
    e.priority=static_cast<uint8_t>(source.priority+(child?1:0));
    auto& ids=runtime(s);const uint32_t previous=ids.nextId;++ids.nextId;e.id=previous;
    e.owner=source.owner;e.loopIndex=e.switchIndex=0xff;e.type=source.type;e.field1a4=0;
    e.userWork=source.userWork;e.userFlags=source.userFlags;
    e.timescale=runtime(s).speed;e.credit=0.f;e.field16c=-1;e.field170=source.field170;e.lifetime=0;
    if(child)metadata(e,source);
    locals(e,source);e.labelIds.fill(0xff);e.labelAddresses.fill(0);scanEventLabels(e,s);
    if(schedule.managerActive)append(schedule,allocated.slot,e);
    if(child) {checkEventEntryRun(s);repairId(s);}else {repairId(s);checkEventEntryRun(s);}
    return e;
}
}
EventEntry& createChildEvent(EventSchedule& schedule,EventEntry& parent,uint32_t script,uint32_t flags,const EventCreationServices& s) {
    return related(schedule,parent,script,flags,true,s);
}
EventEntry& createBrotherEvent(EventSchedule& schedule,EventEntry& owner,uint32_t script,uint32_t flags,const EventCreationServices& s) {
    return related(schedule,owner,script,flags,false,s);
}
int startAttachedBrotherBlock(EventSchedule& schedule,EventEntry& owner,const EventCreationServices& s) {
    const uint32_t start=owner.nextCommand;owner.nextCommand=blockEnd(start,112,s);
    auto& e=createBrotherEvent(schedule,owner,start,0x60,s);
    e.owner=owner.owner;e.type=owner.type;e.userWork=owner.userWork;e.userFlags=owner.userFlags;
    e.field170=owner.field170;e.field150=owner.field150;return 2;
}
int startIndependentInlineBlock(EventSchedule& schedule,EventEntry& owner,const EventCreationServices& s) {
    const uint32_t start=owner.nextCommand;owner.nextCommand=blockEnd(start,109,s);
    auto& e=createTypedRootEvent(schedule,start,owner.priority,0x60,owner.type,s);
    e.owner=owner.owner;e.userWork=owner.userWork;e.userFlags=owner.userFlags;e.field170=owner.field170;
    metadata(e,owner);locals(e,owner);return 2;
}
}
