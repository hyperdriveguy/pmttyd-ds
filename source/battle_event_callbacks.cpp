#include "battle_event_callbacks.hpp"
#include <cassert>

namespace ttyd {
BattleActorServices nativeBattleActorServices(EventNativeRuntime& runtime) {
    BattleActorServices s;s.memory=runtime.memoryBinding.services();s.user=&runtime;
    s.byte=[](void* p,uint32_t a) {uint8_t v=0;const bool mapped=static_cast<EventNativeRuntime*>(p)->memory.readByte(a,v);assert(mapped);(void)mapped;return v;};return s;
}
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime& runtime) {
    BattleDataChildBackend backend;backend.user=&runtime;
    backend.translate=[](void* p,EventEntry& e,int32_t selector) {return translateBattleActor(&e,selector,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)));};
    backend.currentBattle=[](void* p) {return currentBattleAddress(nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)));};
    backend.unit=[](void* p,uint32_t b,int32_t id) {return battleRegistryUnit(b,id,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)));};
    return nativeBattleDataChildServices(runtime,backend);
}
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime& runtime,const BattleDataChildBackend& backend) {
    BattleDataChildServices s;s.creation=runtime.creationServices();s.memory=runtime.memoryBinding.services();s.schedule=&runtime.schedule;
    s.values.user=&runtime;s.values.word=s.creation.word;
    s.values.resolve=[](void* p,EventEntry& e,int32_t operand) {auto& r=*static_cast<EventNativeRuntime*>(p);return getEventValue(e,operand,r.interpreterServices().operands);};
    s.user=backend.user;s.translate=backend.translate;s.currentBattle=backend.currentBattle;s.unit=backend.unit;return s;
}
bool bindBattleDataChildCallback(EventCallbackRegistry& registry,BattleDataChildServices& services) {
    return registry.bind(0x8010a010,&services,[](void* p,EventEntry& e,int32_t) {return runBattleDataChild(e,*static_cast<BattleDataChildServices*>(p));});
}
}
