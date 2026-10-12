#include "battle_event_callbacks.hpp"

namespace ttyd {
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
