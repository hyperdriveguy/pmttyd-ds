#pragma once
#include "event_callback_registry.hpp"
#include "event_native_runtime.hpp"
#include "battle_data_child.hpp"
#include "battle_actor.hpp"
#include "battle_majinai_query.hpp"
#include "battle_event_utilities.hpp"
#include "battle_turn_first.hpp"
#include "battle_packed_move.hpp"
#include "effect_deletion.hpp"
#include "battle_turn_rule.hpp"

namespace ttyd {
struct BattleDataChildBackend {
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*currentBattle)(void*)=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
};
struct BattleTurnFirstBackend {
    EventNativeRuntime* runtime=nullptr;
    void* user=nullptr;
    void (*judgeTurnRule)(void*)=nullptr; // Null selects recovered packed rule judgment.
    void (*resetMove)(void*,uint32_t unit)=nullptr; // Null selects recovered packed-byte implementation.
    void (*softDelete)(void*,uint32_t effect)=nullptr; // Null selects recovered record/heap3 implementation.
};
// Backend and runtime must outlive returned services. Closed local turn body;
// Defaults implement recovered rule/move/effect services on shared packed bytes.
EffectDeletionServices nativeEffectDeletionServices(EventNativeRuntime&);
BattleTurnFirstServices nativeBattleTurnFirstServices(BattleTurnFirstBackend&);
BattleActorServices nativeBattleActorServices(EventNativeRuntime&);
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&,const BattleDataChildBackend&);
// Complete recovered actor translation and registry queries on runtime bytes.
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&);
// Registers actual RunDataEventChild at original8010a010. Service object must
// outlive registration. Registry does not stub any other Jump callback.
bool bindBattleDataChildCallback(EventCallbackRegistry&,BattleDataChildServices&);
int32_t nativeBattleJumpSetting(EventEntry&,EventNativeRuntime&);
bool bindBattleJumpSettingCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleMajinaiQueryServices nativeBattleMajinaiQueryServices(EventNativeRuntime&);
bool bindBattleMajinaiQueryCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindPouchMajinaiSetCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleEventUtilityServices nativeBattleEventUtilityServices(EventNativeRuntime&);
bool bindBattleFlagQueryCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleFramesToMillisecondsCallback(EventCallbackRegistry&,EventNativeRuntime&);
}
