#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include "event_memory_release.hpp"

namespace ttyd {
BattlePackedMarioJumpServices nativeBattleMarioJumpServices(BattleMarioJumpBackend& backend) {
    assert(backend.runtime);BattlePackedMarioJumpServices s;s.motion=nativeBattleMovePositionServices(*backend.runtime,{});s.user=&backend;
    s.eligible=[](void* p,uint32_t u)->int32_t {auto& b=*static_cast<BattleMarioJumpBackend*>(p);if(b.eligible)return b.eligible(b.user,u);return packedACPossibility(u,nativeBattleActorServices(*b.runtime));};
    s.play=[](void* p,uint32_t key,const std::array<float,3>& xyz)->uint32_t {auto& b=*static_cast<BattleMarioJumpBackend*>(p);assert(b.play);return b.play(b.user,key,xyz);};
    s.divideWord=[](void* p,int32_t a,int32_t b)->int32_t {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.divideWord);return r.platform.divideWord(r.platform.user,a,b);};
    s.add=[](void* p,double a,double b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.addDouble);return r.platform.addDouble(r.platform.user,a,b);};
    s.subtract=[](void* p,double a,double b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.subtractDouble);return r.platform.subtractDouble(r.platform.user,a,b);};
    s.multiply=[](void* p,double a,double b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.multiplyDouble);return r.platform.multiplyDouble(r.platform.user,a,b);};
    s.divide=[](void* p,double a,double b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.divideDouble);return r.platform.divideDouble(r.platform.user,a,b);};
    s.fma=[](void* p,double a,double b,double c) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.fmaDouble);return r.platform.fmaDouble(r.platform.user,a,b,c);};
    s.fmod=[](void* p,double a,double b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.fmodDouble);return r.platform.fmodDouble(r.platform.user,a,b);};return s;
}
bool bindBattleMarioJumpPositionCallback(EventCallbackRegistry& registry,BattlePackedMarioJumpServices& services) {return registry.bind(0x801093a8,&services,[](void* p,EventEntry& e,int32_t first) {return packedMarioJumpPositionEvent(e,first,*static_cast<BattlePackedMarioJumpServices*>(p));});}
bool bindBattleMarioJumpParameterCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80109c68,&runtime,[](void* p,EventEntry& e,int32_t) {return packedMarioJumpParameterEvent(e,nativeBattleMoveFrameServices(*static_cast<EventNativeRuntime*>(p)));});}
BattlePackedMovePositionServices nativeBattleMovePositionServices(EventNativeRuntime& runtime,const BattleSoundPlaybackServices& playback) {
    BattlePackedMovePositionServices s;s.query=nativeBattleMoveFrameServices(runtime);s.sound=nativeBattleMovementSoundServices(runtime,playback);
    s.heading=[](void* p,float a,float b,float c,float d) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.headingFloat);return r.platform.headingFloat(r.platform.user,a,b,c,d);};
    s.sine=[](void* p,double v) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.sineDouble);return r.platform.sineDouble(r.platform.user,v);};
    s.cosine=[](void* p,double v) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.cosineDouble);return r.platform.cosineDouble(r.platform.user,v);};
    s.roundDouble=[](void* p,double v) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.roundDoubleFloat);return r.platform.roundDoubleFloat(r.platform.user,v);};return s;
}
bool bindBattleMovePositionCallback(EventCallbackRegistry& registry,BattlePackedMovePositionServices& services) {return registry.bind(0x8010ca74,&services,[](void* p,EventEntry& e,int32_t first) {return packedMovePositionEvent(e,first,*static_cast<BattlePackedMovePositionServices*>(p));});}
BattlePackedMovementSoundServices nativeBattleMovementSoundServices(EventNativeRuntime& runtime,const BattleSoundPlaybackServices& playback) {BattlePackedMovementSoundServices s;s.position=nativeBattlePartPositionServices(runtime);s.playback=playback;return s;}
BattleMoveFrameServices nativeBattleMoveFrameServices(EventNativeRuntime& runtime) {
    BattleMoveFrameServices s;s.common=nativeBattleChoiceServices(runtime);s.user=&runtime;
    s.distance=[](void* p,float a,float b,float c,float d) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.distanceFloat);return r.platform.distanceFloat(r.platform.user,a,b,c,d);};return s;
}
bool bindBattleGetMoveFrameCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010c8c0,&runtime,[](void* p,EventEntry& e,int32_t) {return packedMoveFrameEvent(e,nativeBattleMoveFrameServices(*static_cast<EventNativeRuntime*>(p)));});}
BattlePackedChoiceServices nativeBattleChoiceServices(EventNativeRuntime& runtime) {
    BattlePackedChoiceServices s;s.position=nativeBattlePartPositionServices(runtime);s.user=&runtime;
    s.status=[](void* p,uint32_t u,int32_t id) {auto& r=*static_cast<EventNativeRuntime*>(p);return int32_t(getPackedBattleStatus(u,id,nativeBattleActorServices(r)).duration);};
    s.canAct=[](void* p,uint32_t u) {auto& r=*static_cast<EventNativeRuntime*>(p);return packedCanAct(u,nativeBattleActorServices(r));};
    s.maxFp=[](void* p,uint32_t u) {auto& r=*static_cast<EventNativeRuntime*>(p);return packedUnitFp(u,true,nativeBattleActorServices(r));};
    s.fp=[](void* p,uint32_t u) {auto& r=*static_cast<EventNativeRuntime*>(p);return packedUnitFp(u,false,nativeBattleActorServices(r));};
    s.random=[](void* p,int32_t bound) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.irand);return r.platform.irand(r.platform.user,bound);};
    s.convert=[](void* p,int32_t v) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.signedWordFloat);return r.platform.signedWordFloat(r.platform.user,v);};
    s.divide=[](void* p,float a,float b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.divideFloat);return r.platform.divideFloat(r.platform.user,a,b);};
    s.subtract=[](void* p,float a,float b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.subtractFloat);return r.platform.subtractFloat(r.platform.user,a,b);};
    s.negativeMultiplySubtract=[](void* p,float a,float b,float c) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.negativeMultiplySubtractFloat);return r.platform.negativeMultiplySubtractFloat(r.platform.user,a,b,c);};return s;
}
BattlePackedSamplingServices nativeBattleSamplingServices(EventNativeRuntime& runtime,void* user,uint32_t (*monosiri)(void*,int32_t)) {BattlePackedSamplingServices s;s.common=nativeBattleChoiceServices(runtime);s.user=user;s.monosiri=monosiri;return s;}
BattleTakeoffCoreServices nativeBattleTakeoffCoreServices(BattleTakeoffBackend& backend) {
    assert(backend.runtime);BattleTakeoffCoreServices s;s.user=&backend;
    s.sample=[](void* p,BattleCandidateBuffer& b,uint32_t w,int32_t a,int32_t side,uint32_t flags,uint32_t attrs,int32_t bias) {auto& backend=*static_cast<BattleTakeoffBackend*>(p);if(backend.sample)return backend.sample(backend.user,b,w,a,side,flags,attrs,bias);return samplePackedBattleCandidates(privateBattleCandidateRecords(b),w,a,side,flags,attrs,bias,nativeBattleSamplingServices(*backend.runtime,backend.user,backend.monosiri));};
    s.choose=[](void* p,BattleCandidateBuffer& b,uint32_t flags,int32_t& unit,int32_t& part)->int32_t {
        auto& backend=*static_cast<BattleTakeoffBackend*>(p);struct Values {int32_t* unit;int32_t* part;} values{&unit,&part};CandidateChoiceOutputs outputs;outputs.user=&values;
        outputs.write=[](void* p,bool u,int32_t v) {auto& values=*static_cast<Values*>(p);*(u?values.unit:values.part)=v;};
        choosePackedBattleCandidate(privateBattleCandidateRecords(b),flags,nativeBattleChoiceServices(*backend.runtime),outputs);return 0; // Adapter value is ignored;original has no scalar contract.
    };return s;
}
BattlePackedTakeoffServices nativeBattleTakeoffServices(EventNativeRuntime& runtime,const BattleTakeoffCoreServices& core) {
    BattlePackedTakeoffServices s;s.position=nativeBattlePartPositionServices(runtime);s.core=core;
    s.subtract=[](void* p,float a,float b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.subtractFloat);return r.platform.subtractFloat(r.platform.user,a,b);};return s;
}
bool bindBattleTakeoffCallback(EventCallbackRegistry& registry,BattlePackedTakeoffServices& services) {return registry.bind(0x80109054,&services,[](void* p,EventEntry& e,int32_t) {return packedTakeoffPositionEvent(e,*static_cast<BattlePackedTakeoffServices*>(p));});}
BattlePartPositionServices nativeBattlePartPositionServices(EventNativeRuntime& runtime) {
    BattlePartPositionServices s;s.unit=nativeBattleUnitEventServices(runtime);s.memory=nativeBattleActorServices(runtime);s.numericUser=&runtime;
    s.fma=[](void* p,float a,float b,float c) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.fmaFloat);return r.platform.fmaFloat(r.platform.user,a,b,c);};
    s.add=[](void* p,float a,float b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.addFloat);return r.platform.addFloat(r.platform.user,a,b);};
    s.multiply=[](void* p,float a,float b) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.multiplyFloat);return r.platform.multiplyFloat(r.platform.user,a,b);};return s;
}
bool bindBattleGetPartsPosCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80112d98,&runtime,[](void* p,EventEntry& e,int32_t) {return packedPartPositionEvent(e,nativeBattlePartPositionServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetHitPosCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x801120a8,&runtime,[](void* p,EventEntry& e,int32_t) {return packedHitPositionEvent(e,nativeBattlePartPositionServices(*static_cast<EventNativeRuntime*>(p)));});}
BattleFirstAttackServices nativeBattleFirstAttackServices(EventNativeRuntime& runtime) {
    BattleFirstAttackServices s;s.memory=nativeBattleActorServices(runtime);s.values=nativeBattleEventUtilityServices(runtime).values;s.user=&runtime;
    s.pair=[](void* p,uint32_t battle,uint32_t* attacker,uint32_t* target) {auto& r=*static_cast<EventNativeRuntime*>(p);return packedFirstAttackPair(battle,attacker,target,nativeBattleActorServices(r));};return s;
}
bool bindBattleFirstAttackTargetCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010b418,&runtime,[](void* p,EventEntry& e,int32_t) {return packedFirstAttackTargetEvent(e,nativeBattleFirstAttackServices(*static_cast<EventNativeRuntime*>(p)));});}
BattlePackedSelectionServices nativeBattlePackedSelectionServices(EventNativeRuntime& runtime) {
    BattlePackedSelectionServices s;s.memory=nativeBattleActorServices(runtime);s.values=nativeBattleEventUtilityServices(runtime).values;s.user=&runtime;
    s.translate=[](void* p,EventEntry& e,int32_t selector) {auto& r=*static_cast<EventNativeRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r));};return s;
}
bool bindBattleSetPosCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80112fcc,&runtime,[](void* p,EventEntry& e,int32_t) {return packedSetUnitPositionEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetBodyIdCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010ead8,&runtime,[](void* p,EventEntry& e,int32_t) {return battleGetBodyIdEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleSetMoveSpeedCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010d860,&runtime,[](void* p,EventEntry& e,int32_t) {return packedMovementSetterEvent(e,false,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleSetFallAccelCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010d770,&runtime,[](void* p,EventEntry& e,int32_t) {return packedMovementSetterEvent(e,true,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetPosCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x80113620,&runtime,[](void* p,EventEntry& e,int32_t) {return packedUnitPositionEvent(e,false,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleGetHomePosCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x801124f8,&runtime,[](void* p,EventEntry& e,int32_t) {return packedUnitPositionEvent(e,true,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleSelectedTargetCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010b540,&runtime,[](void* p,EventEntry& e,int32_t) {return packedSelectedTargetEvent(e,nativeBattlePackedSelectionServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleCommandWeaponCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x801057e8,&runtime,[](void* p,EventEntry& e,int32_t) {const auto result=packedCommandWeaponEvent(e,nativeBattlePackedSelectionServices(*static_cast<EventNativeRuntime*>(p)));if(!result)std::abort();return *result;});}
BattleUnitEventServices nativeBattleUnitEventServices(EventNativeRuntime& runtime) {
    BattleUnitEventServices s;s.memory=runtime.memoryBinding.services();s.user=&runtime;
    auto values=nativeBattleEventUtilityServices(runtime);s.values=values.values;
    s.values.resolveFloat=[](void* p,EventEntry& e,int32_t raw) {auto& r=*static_cast<EventNativeRuntime*>(p);return getEventFloat(e,raw,r.interpreterServices().operands);};
    s.translate=[](void* p,EventEntry& e,int32_t selector) {auto& r=*static_cast<EventNativeRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r));};
    s.unit=[](void* p,uint32_t battle,int32_t id) {auto& r=*static_cast<EventNativeRuntime*>(p);return battleRegistryUnit(battle,id,nativeBattleActorServices(r));};
    s.truncateFloat=[](void* p,float value) {auto& r=*static_cast<EventNativeRuntime*>(p);assert(r.platform.truncate);return r.platform.truncate(r.platform.user,value);};
    s.removeEventId=[](void* p,uint32_t id) {auto& r=*static_cast<EventNativeRuntime*>(p);deleteEventId(id,r.creationServices());};
    s.createWait=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry& {auto& r=*static_cast<EventNativeRuntime*>(p);return createRootEvent(r.schedule,script,priority,flags,r.creationServices());};
    s.checkStatus=[](void* p,uint32_t unit,int32_t id)->int32_t {auto& r=*static_cast<EventNativeRuntime*>(p);return checkPackedBattleStatus(unit,id,nativeBattleActorServices(r))?1:0;};return s;
}
bool bindBattleStartWaitCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010a26c,&runtime,[](void* p,EventEntry& e,int32_t) {return battleStartUnitWaitEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleStopWaitCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010a1fc,&runtime,[](void* p,EventEntry& e,int32_t) {return battleStopUnitWaitEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));});}
bool bindBattleWaitGuardMoveCallback(EventCallbackRegistry& registry,EventNativeRuntime& runtime) {return registry.bind(0x8010a1c8,&runtime,[](void* p,EventEntry&,int32_t) {return battleWaitGuardMove(nativeBattleEventUtilityServices(*static_cast<EventNativeRuntime*>(p)));});}
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
