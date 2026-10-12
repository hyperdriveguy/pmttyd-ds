#pragma once
#include "event_callback_registry.hpp"
#include "event_native_runtime.hpp"
#include "battle_data_child.hpp"
#include "battle_actor.hpp"

namespace ttyd {
struct BattleDataChildBackend {
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*currentBattle)(void*)=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
};
BattleActorServices nativeBattleActorServices(EventNativeRuntime&);
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&,const BattleDataChildBackend&);
// Complete recovered actor translation and registry queries on runtime bytes.
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&);
// Registers actual RunDataEventChild at original8010a010. Service object must
// outlive registration. Registry does not stub any other Jump callback.
bool bindBattleDataChildCallback(EventCallbackRegistry&,BattleDataChildServices&);
int32_t nativeBattleJumpSetting(EventEntry&,EventNativeRuntime&);
bool bindBattleJumpSettingCallback(EventCallbackRegistry&,EventNativeRuntime&);
}
