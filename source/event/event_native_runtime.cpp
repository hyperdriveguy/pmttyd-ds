#include "event_native_runtime.hpp"
#include <cassert>

// Compose semantic managers with original-address memory, not a second VM.
// Original field/battle context bases0x803d8d30/0x803d8dd0; GP pointer word
// 0x80415fd0 points at the supplied GP base. Required numeric/cache/callback
// services remain explicit platform boundaries; no Gekko instruction parity implied.
namespace ttyd {
namespace {
EventNativeRuntime& owner(void* p) {return *static_cast<EventNativeRuntime*>(p);}
}
EventNativeRuntime::EventNativeRuntime(uint32_t gpBase,const EventNativePlatform& p):platform(p),
    memoryBinding{memory,this,
        [](void* p,uint32_t a) {auto& f=owner(p);assert(f.platform.flushBlock);f.platform.flushBlock(f.platform.user,a);},
        [](void* p) {auto& f=owner(p);assert(f.platform.systemCall);f.platform.systemCall(f.platform.user);}} {
    const bool fieldMapped=contextAddresses.registerContext(0x803d8d30,field);
    const bool battleMapped=contextAddresses.registerContext(0x803d8dd0,battle);
    const bool gpMapped=gpAddresses.registerGp(gpBase,gp);
    const bool rootStored=gpAddresses.writeWord(0x80415fd0,gpBase);
    assert(fieldMapped && battleMapped && gpMapped && rootStored);
    (void)fieldMapped;(void)battleMapped;(void)gpMapped;(void)rootStored;
}
EventManagerServices EventNativeRuntime::managerServices() {
    EventManagerServices s;s.user=this;
    s.globals=[](void* p) {return owner(p).gpAddresses.currentGlobals();};
    s.context=[](void* p,bool b) {auto& f=owner(p);return b?&f.battle:&f.field;};
    s.clock=[](void* p) {auto& f=owner(p);assert(f.platform.clock);return f.platform.clock(f.platform.user);};
    s.dispatch=[](void* p,EventEntry& e) {return dispatchEventInterpreter(e,owner(p).interpreterServices());};return s;
}
EventCreationServices EventNativeRuntime::creationServices() {
    EventCreationServices s;s.user=this;s.manager=managerServices();
    s.runtime=[](void* p) {return &owner(p).runtime;};
    s.word=[](void* p,uint32_t a) {auto memory=owner(p).memoryBinding.services();return memory.word(memory.user,a);};
    s.stopAll=[](void* p,uint32_t mask) {stopAllEvents(mask,owner(p).creationServices());};
    if(platform.retiring)s.retiring=[](void* p,const EventEntry& e) {auto& f=owner(p);if(f.platform.retiring)f.platform.retiring(f.platform.user,e);};
    return s;
}
EventInterpreterServices EventNativeRuntime::interpreterServices() {
    EventInterpreterServices s;s.user=this;s.schedule=&schedule;s.creation=creationServices();s.memory=memoryBinding.services();
    s.operands.user=this;
    s.operands.contextAddress=[](void* p)->uint32_t {auto* g=owner(p).gpAddresses.currentGlobals();assert(g);return g->battleContext!=0?0x803d8dd0:0x803d8d30;};
    s.operands.globalAddress=[](void* p) {return owner(p).gpAddresses.currentAddress();};
    s.operands.entryAddress=[](void* p,EventEntry& e) {uint32_t a=0;const bool mapped=owner(p).entryAddresses.addressOf(&e,a);assert(mapped);(void)mapped;return a;};
    s.operands.readWord=s.creation.word;
    s.operands.writeWord=[](void* p,uint32_t a,uint32_t v) {auto m=owner(p).memoryBinding.services();m.storeWord(m.user,a,v);};
    s.operands.readByte=[](void* p,uint32_t a) {uint8_t v=0;const bool mapped=owner(p).memory.readByte(a,v);assert(mapped);(void)mapped;return v;};
    s.operands.writeByte=[](void* p,uint32_t a,uint8_t v) {auto m=owner(p).memoryBinding.services();m.storeByte(m.user,a,v);};
    s.operands.truncate=[](void* p,float v) {auto& f=owner(p);assert(f.platform.truncate);return f.platform.truncate(f.platform.user,v);};
    s.numericUser=this;s.divideWord=[](void* p,int32_t a,int32_t b) {auto& f=owner(p);assert(f.platform.divideWord);return f.platform.divideWord(f.platform.user,a,b);};
    s.call=[](void* p,EventEntry& e,uint32_t function,int32_t first) {auto& f=owner(p);assert(f.platform.call);return f.platform.call(f.platform.user,e,function,first);};
    s.format=[](void* p,const EventDiagnosticCall& call) {auto& f=owner(p);assert(f.platform.format);f.platform.format(f.platform.user,call);};return s;
}
void EventNativeRuntime::initializeContexts() {initializeEventContexts(entryStorage.initializationServices(managerServices(),field,battle,runtime,schedule));}
void EventNativeRuntime::reinitializeContext() {reinitializeEventContext(entryStorage.initializationServices(managerServices(),field,battle,runtime,schedule));}
void EventNativeRuntime::runManager() {runEventInterpreterManager(interpreterServices());}
}
