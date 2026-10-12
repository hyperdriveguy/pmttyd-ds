#include "event_creation.hpp"
#include <cassert>

namespace ttyd {
namespace {
EventRuntimeGlobals& runtime(const EventCreationServices& s) {assert(s.runtime);auto* g=s.runtime(s.user);assert(g);return *g;}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?static_cast<int32_t>(v):static_cast<int32_t>(v)-0x10000;}
uint32_t read(const EventCreationServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
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
    assert(s.manager.globals && s.manager.context);
    auto* gp=s.manager.globals(s.manager.user);assert(gp);
    auto* context=s.manager.context(s.manager.user,gp->battleContext!=0);assert(context && context->entries);
    int32_t slot=0;
    while(slot<context->capacity && (context->entries[slot].flags&1))++slot;
    assert(slot>=0 && slot<context->capacity);
    auto& e=context->entries[slot];++runtime(s).allocationCount;e={};
    e.flags=static_cast<uint8_t>(flags|1);e.nextCommand=script;
    e.restartScript=script;e.currentCommand=script;
    e.opcode=0;e.waitingParent=e.waitingChild=e.brotherOwner=nullptr;
    e.priority=static_cast<uint8_t>(priority);
    auto& ids=runtime(s);const uint32_t previous=ids.nextId;++ids.nextId;e.id=previous;
    e.owner=-1;e.loopIndex=0xff;e.switchIndex=0xff;e.type=static_cast<uint8_t>(type);
    e.field1a4=0;e.timescale=runtime(s).speed;e.credit=0.f;e.field16c=-1;e.field170=0;e.lifetime=0;
    e.locals.fill(0);e.localFlags.fill(0);e.labelIds.fill(0xff);e.labelAddresses.fill(0);
    scanEventLabels(e,s);
    if(schedule.managerActive && (e.flags&0x20)) {
        const int32_t count=schedule.count;
        assert(count>=0 && count<static_cast<int32_t>(schedule.slots.size()));
        schedule.slots[count]=slot;schedule.ids[count]=e.id;schedule.count=count+1;
    }
    checkEventEntryRun(s);
    auto& after=runtime(s);if(after.nextId==0)after.nextId=1;
    return e;
}
EventEntry& createRootEvent(EventSchedule& schedule,uint32_t script,int32_t priority,uint32_t flags,
                            const EventCreationServices& s) {
    return createTypedRootEvent(schedule,script,priority,flags,0xef,s);
}
}
