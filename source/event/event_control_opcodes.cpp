#include "event_control_opcodes.hpp"
#include <cassert>

// Control opcode names/results: docs/code/event-vm.md. Four-byte argument
// strides and16/3 local-bank sizes are the source script/entry ABI.
// Read destination operands RAW so output writes do not resolve their old values.
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?static_cast<int32_t>(v):static_cast<int32_t>(v)-0x10000;}
uint32_t word(const EventDispatchServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
int32_t raw(const EventDispatchServices& s,uint32_t args,unsigned index) {return signedWord(word(s,args+4*index));}
int32_t resolve(EventEntry& e,const EventDispatchServices& s,uint32_t args,unsigned index=0) {
    const int32_t value=raw(s,args,index);assert(s.resolve);return s.resolve(s.user,e,value);
}
void output(EventEntry& e,const EventDispatchServices& s,int32_t destination,uint32_t id) {assert(s.store);s.store(s.user,e,destination,signedWord(id));}
void locals(EventEntry& target,const EventEntry& source) {
    for(unsigned i=0;i<16;++i)target.locals[i]=source.locals[i];
    for(unsigned i=0;i<3;++i)target.localFlags[i]=source.localFlags[i];
}
void metadata(EventEntry& target,const EventEntry& source) {
    target.messageFields[1]=source.messageFields[1];target.messageFields[0]=source.messageFields[0];target.messageFields[2]=source.messageFields[2];
    const auto a=source.messageFields[3],b=source.messageFields[4];target.messageFields[3]=a;target.messageFields[4]=b;
    target.messageFields[5]=source.messageFields[5];target.messageFields[6]=source.messageFields[6];
}
uint32_t afterBlock(uint32_t start,uint16_t end,const EventCreationServices& s) {
    assert(s.word);uint32_t p=start;
    for(;;) {const uint32_t command=s.word(s.user,p);p+=4+static_cast<uint32_t>(4*signedHalf(command>>16));if((command&0xffff)==end)return p;}
}
}
std::optional<int32_t> eventControlOpcode(EventEntry& e,uint8_t opcode,const EventControlOpcodeServices& s) {
    if(opcode<92 || opcode>112)return std::nullopt;
    const uint32_t args=e.arguments;const auto& values=s.values;const auto& creation=s.creation;
    switch(opcode) {
    case 92:case 93: {
        const uint32_t script=static_cast<uint32_t>(resolve(e,values,args));
        const int32_t destination=opcode==93?raw(values,args,1):0;
        assert(s.schedule);auto& newEntry=createTypedRootEvent(*s.schedule,script,e.priority,0,e.type,creation);
        newEntry.owner=e.owner;newEntry.field170=e.field170;locals(newEntry,e);newEntry.userWork=e.userWork;newEntry.userFlags=e.userFlags;
        if(opcode==93)output(e,values,destination,newEntry.id);
        return 2;
    }
    // RUN_CHILD suspends the parent and returns handler255 (dispatcher -1).
    // This stops its visit without deleting it; child deletion later wakes it.
    case 94: {
        const uint32_t script=static_cast<uint32_t>(resolve(e,values,args));assert(s.schedule);
        createChildEvent(*s.schedule,e,script,0,creation);e.opcode=0;return 255;
    }
    case 95:deleteEventId(static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 96:e.restartScript=static_cast<uint32_t>(resolve(e,values,args));restartEvent(e,creation);return 2;
    case 97:setEventPriority(e,resolve(e,values,args));return 2;
    case 98: {
        const int32_t operand=raw(values,args,0);assert(values.resolveFloat);
        const float speed=values.resolveFloat(values.user,e,operand);setEventSpeed(e,speed,creation);return 2;
    }
    case 99:setEventType(e,resolve(e,values,args));return 2;
    case 100:stopAllEvents(static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 101:startAllEvents(static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 102:stopOtherEvents(&e,static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 103:startOtherEvents(&e,static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 104:stopEventId(static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 105:startEventId(static_cast<uint32_t>(resolve(e,values,args)),creation);return 2;
    case 106: {
        const uint32_t id=static_cast<uint32_t>(resolve(e,values,args));const int32_t destination=raw(values,args,1);
        const int32_t result=checkEventId(id,creation);assert(values.store);values.store(values.user,e,destination,result);return 2;
    }
    case 107:assert(s.schedule);return startIndependentInlineBlock(*s.schedule,e,creation);
    // ID-returning inline/brother variants skip to109/112, construct with
    // source flags0x60, then publish the new generation ID to raw destination.
    case 108:case 111: {
        const int32_t destination=raw(values,args,0);const uint32_t start=e.nextCommand;
        e.nextCommand=afterBlock(start,opcode==108?109:112,creation);assert(s.schedule);
        auto& newEntry=opcode==108?createTypedRootEvent(*s.schedule,start,e.priority,0x60,e.type,creation):createBrotherEvent(*s.schedule,e,start,0x60,creation);
        newEntry.owner=e.owner;
        if(opcode==111)newEntry.type=e.type;
        newEntry.userWork=e.userWork;newEntry.userFlags=e.userFlags;newEntry.field170=e.field170;
        if(opcode==108) {metadata(newEntry,e);locals(newEntry,e);}
        output(e,values,destination,newEntry.id);return 2;
    }
    // Independent end109 deletes/returns255; brother end112 deletes/returns0.
    // These distinct result paths are intentional source behavior.
    case 109:deleteEvent(e,creation);return 255;
    case 110:assert(s.schedule);return startAttachedBrotherBlock(*s.schedule,e,creation);
    case 112:deleteEvent(e,creation);return 0;
    default:return std::nullopt;
    }
}
}
