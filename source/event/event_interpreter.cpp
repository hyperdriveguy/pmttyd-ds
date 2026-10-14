#include "event_interpreter.hpp"
#include "event_if_opcodes.hpp"
#include "event_switch_opcodes.hpp"
#include "event_loop_opcodes.hpp"
#include <cassert>

namespace ttyd {
namespace {
const EventInterpreterServices& owner(void* p) {assert(p);return *static_cast<const EventInterpreterServices*>(p);}
uint32_t scriptWord(void* p,uint32_t a) {const auto& s=owner(p);assert(s.creation.word);return s.creation.word(s.creation.user,a);}
EventDispatchServices values(const EventInterpreterServices& s);
int32_t handler(void* p,EventEntry& e,uint8_t op) {
    const auto& s=owner(p);const auto v=values(s);
    if(op>=4 && op<=8)return *eventLoopOpcode(e,op,v);
    if(op>=12 && op<=33) {
        EventIfOpcodeServices f;f.values=v;f.byte=[](void* p,uint32_t a) {const auto& s=owner(p);assert(s.operands.readByte);return s.operands.readByte(s.operands.user,a);};
        return *eventIfOpcode(e,op,f);
    }
    if(op>=34 && op<=49)return *eventSwitchOpcode(e,op,v);
    if(op>=50 && op<=73) {
        EventDataOpcodeServices d;d.values=v;
        // MOD uses the same original float-to-word instruction service as operands.
        d.truncate=[](void* p,float f) {const auto& s=owner(p);assert(s.operands.truncate);return s.operands.truncate(s.operands.user,f);};
        // Division and MOD have distinct owners,so use a local adapter for both.
        d.numericUser=p;d.divideWord=[](void* p,int32_t a,int32_t b) {const auto& s=owner(p);assert(s.divideWord);return s.divideWord(s.numericUser,a,b);};
        return *eventDataOpcode(e,op,d);
    }
    if(op>=74 && op<=90) {EventMemoryOpcodeServices m;m.values=v;m.memory=s.memory;return *eventMemoryOpcode(e,op,m);}
    if(op>=92 && op<=112) {EventControlOpcodeServices c;c.schedule=s.schedule;c.creation=s.creation;c.values=v;return *eventControlOpcode(e,op,c);}
    EventTailOpcodeServices t;t.values=v;t.operands=s.operands;t.manager=s.creation.manager;t.formatUser=s.user;t.format=s.format;
    const auto result=eventTailOpcode(e,op,t);assert(result);return *result;
}
EventDispatchServices values(const EventInterpreterServices& s) {
    EventDispatchServices v;v.user=const_cast<EventInterpreterServices*>(&s);v.word=scriptWord;v.handler=handler;
    v.resolve=[](void* p,EventEntry& e,int32_t op) {return getEventValue(e,op,owner(p).operands);};
    v.resolveFloat=[](void* p,EventEntry& e,int32_t op) {return getEventFloat(e,op,owner(p).operands);};
    v.store=[](void* p,EventEntry& e,int32_t d,int32_t value) {setEventValue(e,d,value,owner(p).operands);};
    v.storeFloat=[](void* p,EventEntry& e,int32_t d,float value) {setEventFloat(e,d,value,owner(p).operands);};
    v.clock=[](void* p) {const auto& s=owner(p).creation.manager;assert(s.clock);return s.clock(s.user);};
    v.call=[](void* p,EventEntry& e,uint32_t function,int32_t first) {const auto& s=owner(p);assert(s.call);return s.call(s.user,e,function,first);};
    v.remove=[](void* p,EventEntry& e) {deleteEvent(e,owner(p).creation);};return v;
}
}
int32_t dispatchEventInterpreter(EventEntry& e,const EventInterpreterServices& s) {assert(s.schedule);return dispatchEvent(e,values(s));}
void runEventInterpreterManager(const EventInterpreterServices& s) {
    assert(s.schedule);EventManagerServices m;m.user=const_cast<EventInterpreterServices*>(&s);
    m.globals=[](void* p) {const auto& m=owner(p).creation.manager;assert(m.globals);return m.globals(m.user);};
    m.context=[](void* p,bool battle) {const auto& m=owner(p).creation.manager;assert(m.context);return m.context(m.user,battle);};
    m.clock=[](void* p) {const auto& m=owner(p).creation.manager;assert(m.clock);return m.clock(m.user);};
    m.dispatch=[](void* p,EventEntry& e) {return dispatchEventInterpreter(e,owner(p));};
    runEventManager(*s.schedule,m);
}
}
