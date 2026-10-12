#include "event_tail_opcodes.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t index(int32_t v,int32_t base) {return signedWord(static_cast<uint32_t>(v)-static_cast<uint32_t>(base));}
uint32_t argument(const EventDispatchServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t word(const EventOperandServices& s,uint32_t a) {assert(s.readWord);return signedWord(s.readWord(s.user,a));}
uint32_t entryAddress(EventEntry& e,const EventOperandServices& s) {assert(s.entryAddress);return s.entryAddress(s.user,e);}
int32_t flag(const EventOperandServices& s,uint32_t base,int32_t i) {const int32_t q=i/32,r=i-32*q;return signedWord(static_cast<uint32_t>(word(s,base+4u*static_cast<uint32_t>(q)))&eventShiftMask(r));}
float decode(int32_t v) {const float f=static_cast<float>(signedWord(static_cast<uint32_t>(v)+230000000u));return f*.0009765625f;}
void templateAt(EventDiagnosticCall& call,uint32_t offset) {call.format=0x802bffb0+offset;}
void typed(EventDiagnosticCall& call,int32_t i,int32_t v,uint32_t hex,uint32_t floating,uint32_t integer) {
    call.integers={i,v};
    if(v<=-270000000)templateAt(call,hex);
    else if(v<=-220000000) {templateAt(call,floating);call.floating=decode(v);call.saveFloatArguments=true;}
    else templateAt(call,integer);
}
}
EventEntry* eventEntryPointer(int32_t index,const EventManagerServices& s) {
    assert(s.globals && s.context);auto* gp=s.globals(s.user);assert(gp);
    auto* c=s.context(s.user,gp->battleContext!=0);assert(c && c->entries);
    auto* e=c->entries+index;return (e->flags&1)?e:nullptr;
}
int32_t eventDebugRegister(EventEntry& e,const EventTailOpcodeServices& services) {
    const auto& s=services.operands;const uint32_t args=e.arguments;
    assert(s.contextAddress);const uint32_t c=s.contextAddress(s.user);
    const int32_t x=signedWord(argument(services.values,args));EventDiagnosticCall call;
    if(x<=-270000000) {templateAt(call,8);call.integers[0]=x;}
    else if(x<=-220000000) {templateAt(call,0x18);call.floating=decode(x);call.saveFloatArguments=true;}
    else if(x<=-200000000) {const int32_t i=index(x,-210000000);templateAt(call,0x2c);call.integers={i,flag(s,e.userFlags,i)};}
    else if(x<=-180000000) {
        const int32_t i=index(x,-190000000),v=word(s,e.userWork+4u*static_cast<uint32_t>(i));typed(call,i,v,0x3c,0x4c,0x60);
        if(v<=-270000000)call.integers[0]=v; // Original UW hex register assignment.
    } else if(x<=-140000000) {
        const bool global=x<=-160000000;const int32_t i=index(x,global?-170000000:-150000000);
        const int32_t v=readSavedSwitch(global?SavedSwitchFamily::GlobalByte:SavedSwitchFamily::LocalByte,i,s);
        typed(call,i,v,global?0x70:0xa4,global?0x80:0xb4,global?0x94:0xc8);
        if(v<=-270000000)call.integers={v,signedWord(c)}; // Original saved hex r6=C.
    } else if(x<=-100000000) {
        const bool global=x<=-120000000;const int32_t i=index(x,global?-130000000:-110000000);
        templateAt(call,global?0xd8:0xe8);call.integers={i,readSavedSwitch(global?SavedSwitchFamily::GlobalFlag:SavedSwitchFamily::LocalFlag,i,s)};
    } else if(x<=-80000000) {const int32_t i=index(x,-90000000);templateAt(call,0xf8);call.integers={i,flag(s,c+0x84,i)};}
    else if(x<=-60000000) {const int32_t i=index(x,-70000000);templateAt(call,0x108);call.integers={i,flag(s,entryAddress(e,s)+0xdc,i)};}
    else if(x<=-40000000) {const int32_t i=index(x,-50000000);typed(call,i,word(s,c+4+4u*static_cast<uint32_t>(i)),0x118,0x128,0x13c);}
    else if(x<=-20000000) {const int32_t i=index(x,-30000000);typed(call,i,word(s,entryAddress(e,s)+0x9c+4u*static_cast<uint32_t>(i)),0x14c,0x15c,0x170);}
    else {templateAt(call,0x180);call.integers[0]=x;}
    assert(services.format);services.format(services.formatUser,call);return 2;
}
std::optional<int32_t> eventTailOpcode(EventEntry& e,uint8_t op,const EventTailOpcodeServices& s) {
    switch(op) {
    case 1:case 3:case 113:case 114:case 117:return 2;
    case 11: {
        const int32_t operand=signedWord(argument(s.values,e.arguments));assert(s.values.resolve);
        return s.values.resolve(s.values.user,e,operand)==0?2:0;
    }
    case 115:return eventDebugRegister(e,s);
    case 116:e.field1a4=argument(s.values,e.arguments);return 2;
    case 118:for(int32_t i=0;i<256;++i)if(eventEntryPointer(i,s.manager)==&e)break;return 1;
    default:return {};
    }
}
}
