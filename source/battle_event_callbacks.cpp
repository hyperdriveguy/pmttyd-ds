#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include "event_memory_release.hpp"

namespace ttyd {
BattleUnitEventServices nativeBattleUnitEventServices(EventNativeRuntime& runtime) {
    BattleUnitEventServices s;s.memory=runtime.memoryBinding.services();s.user=&runtime;
    auto values=nativeBattleEventUtilityServices(runtime);s.values=values.values;
    s.translate=[](void* p,EventEntry& e,int32_t selector) {auto& r=*static_cast<EventNativeRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r));};
    s.unit=[](void* p,uint32_t battle,int32_t id) {auto& r=*static_cast<EventNativeRuntime*>(p);return battleRegistryUnit(battle,id,nativeBattleActorServices(r));};
    s.checkStatus=[](void* p,uint32_t unit,int32_t id)->int32_t {auto& r=*static_cast<EventNativeRuntime*>(p);return checkPackedBattleStatus(unit,id,nativeBattleActorServices(r))?1:0;};return s;
}
bool bindBattleCheckStatusCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80105f0c,&runtime,[](void* p,EventEntry& e,int32_t) {return battleCheckUnitStatusEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleCheckDataCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80109f88,&runtime,[](void* p,EventEntry& e,int32_t) {return battleCheckUnitDataEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetHpCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x801101c4,&runtime,[](void* p,EventEntry& e,int32_t) {return battleUnitHpEvent(e,false,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetMaxHpCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x801100cc,&runtime,[](void* p,EventEntry& e,int32_t) {return battleUnitHpEvent(e,true,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetUnitWorkCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010f828,&runtime,[](void* p,EventEntry& e,int32_t) {return battleGetUnitWorkEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleSetUnitWorkCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010f7a4,&runtime,[](void* p,EventEntry& e,int32_t) {return battleSetUnitWorkEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
EffectDeletionServices nativeEffectDeletionServices(EventNativeRuntime& runtime) {
    EffectDeletionServices s;s.memory=runtime.memoryBinding.services();s.user=&runtime;
    s.freeHeap=[](void* p,int32_t heap,uint32_t payload) {eventHeapFree(heap,payload,static_cast<EventNativeRuntime*>(p)->memoryBinding.services());};return s;
}
BattleTurnFirstServices nativeBattleTurnFirstServices(BattleTurnFirstBackend& backend) {
    assert(backend.runtime);BattleTurnFirstServices s;s.user=&backend;s.memory=backend.runtime->memoryBinding.services();
    s.byte=[](void* p,uint32_t a) {auto& b=*static_cast<BattleTurnFirstBackend*>(p);auto actors=nativeBattleActorServices(*b.runtime);return actors.byte(actors.user,a);};
    s.judgeTurnRule=[](void* p) {auto& b=*static_cast<BattleTurnFirstBackend*>(p);if(b.judgeTurnRule)b.judgeTurnRule(b.user);else judgeBattleTurnRule(nativeBattleActorServices(*b.runtime));};
    s.fortune=[](void* p) {auto& b=*static_cast<BattleTurnFirstBackend*>(p);return battleMajinaiCheck(nativeBattleMajinaiQueryServices(*b.runtime));};
    s.irand=[](void* p,int32_t bound) {auto& r=*static_cast<BattleTurnFirstBackend*>(p)->runtime;assert(r.platform.irand);return r.platform.irand(r.platform.user,bound);};
    s.resetMove=[](void* p,uint32_t unit) {auto& b=*static_cast<BattleTurnFirstBackend*>(p);if(b.resetMove)b.resetMove(b.user,unit);else resetPackedBattleMoveStatus(unit,nativeBattleActorServices(*b.runtime));};
    s.softDelete=[](void* p,uint32_t effect) {auto& b=*static_cast<BattleTurnFirstBackend*>(p);if(b.softDelete)b.softDelete(b.user,effect);else softDeleteEffect(effect,nativeEffectDeletionServices(*b.runtime));};return s;
}
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
BattleEventUtilityServices nativeBattleEventUtilityServices(EventNativeRuntime& runtime) {
    BattleEventUtilityServices s;s.memory=runtime.memoryBinding.services();s.values.user=&runtime;
    s.values.resolve=[](void* p,EventEntry& e,int32_t input) {auto& r=*static_cast<EventNativeRuntime*>(p);return getEventValue(e,input,r.interpreterServices().operands);};
    s.values.store=[](void* p,EventEntry& e,int32_t output,int32_t value) {auto& r=*static_cast<EventNativeRuntime*>(p);setEventValue(e,output,value,r.interpreterServices().operands);};return s;
}
bool bindBattleFlagQueryCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {
    return registry.bind(0x8011384c,&runtime,[](void* p,EventEntry& e,int32_t) {return battleFlagQueryEvent(e,nativeBattleEventUtilityServices(*static_cast<EventNativeRuntime*>(p)));});
}
bool bindBattleFramesToMillisecondsCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {
    return registry.bind(0x80103dc0,&runtime,[](void* p,EventEntry& e,int32_t) {return battleFramesToMillisecondsEvent(e,nativeBattleEventUtilityServices(*static_cast<EventNativeRuntime*>(p)));});
}
BattleMajinaiQueryServices nativeBattleMajinaiQueryServices(EventNativeRuntime& runtime) {
    BattleMajinaiQueryServices s;s.memory=runtime.memoryBinding.services();
    auto actors=nativeBattleActorServices(runtime);s.user=actors.user;s.byte=actors.byte;
    s.irand=[](void* p,int32_t bound) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.irand);return r.platform.irand(r.platform.user,bound);};
    s.values.resolve=[](void* p,EventEntry& e,int32_t raw) {auto& r=*static_cast<EventNativeRuntime*>(p);return getEventValue(e,raw,r.interpreterServices().operands);};
    s.values.user=&runtime;s.values.store=[](void* p,EventEntry& e,int32_t output,int32_t value) {auto& r=*static_cast<EventNativeRuntime*>(p);setEventValue(e,output,value,r.interpreterServices().operands);};return s;
}
bool bindBattleMajinaiQueryCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {
    return registry.bind(0x801146c4,&runtime,[](void* p,EventEntry& e,int32_t) {return battleMajinaiPowerQuery(e,nativeBattleMajinaiQueryServices(*static_cast<EventNativeRuntime*>(p)));});
}
bool bindPouchMajinaiSetCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {
    return registry.bind(0x800f08ac,&runtime,[](void* p,EventEntry& e,int32_t) {return pouchMajinaiSetEvent(e,nativeBattleMajinaiQueryServices(*static_cast<EventNativeRuntime*>(p)));});
}
int32_t nativeBattleJumpSetting(EventEntry& e,EventNativeRuntime& runtime) {
    const uint32_t args=e.arguments;
    auto operands=runtime.interpreterServices().operands;auto memory=runtime.memoryBinding.services();
    auto actor=nativeBattleActorServices(runtime);
    auto raw=[&](uint32_t offset) {const uint32_t v=memory.word(memory.user,args+offset);return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);};
    const int32_t id=translateBattleActor(&e,getEventValue(e,raw(0),operands),actor);
    const int32_t count=getEventValue(e,raw(4),operands);
    const float speed=getEventFloat(e,raw(8),operands),gravity=getEventFloat(e,raw(12),operands);
    const uint32_t unit=battleRegistryUnit(currentBattleAddress(actor),id,actor);
    uint32_t speedBits,gravityBits;std::memcpy(&speedBits,&speed,4);std::memcpy(&gravityBits,&gravity,4);
    memory.storeWord(memory.user,unit+0x16c,uint32_t(count));
    memory.storeWord(memory.user,unit+0x170,speedBits);memory.storeWord(memory.user,unit+0x174,gravityBits);
    return 2;
}
bool bindBattleJumpSettingCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {
    return registry.bind(0x8010d6cc,&runtime,[](void* p,EventEntry& e,int32_t) {return nativeBattleJumpSetting(e,*static_cast<EventNativeRuntime*>(p));});
}
bool bindBattleDataChildCallback(EventCallbackRegistry& registry,BattleDataChildServices& services) {
    return registry.bind(0x8010a010,&services,[](void* p,EventEntry& e,int32_t) {return runBattleDataChild(e,*static_cast<BattleDataChildServices*>(p));});
}
}
