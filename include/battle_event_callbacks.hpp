#pragma once
#include "event_callback_registry.hpp"
#include "event_native_runtime.hpp"
#include "battle_data_child.hpp"

namespace ttyd {
struct BattleDataChildBackend {
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*currentBattle)(void*)=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
};
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&,const BattleDataChildBackend&);
// Registers actual RunDataEventChild at original8010a010. Service object must
// outlive registration. Registry does not stub any other Jump callback.
bool bindBattleDataChildCallback(EventCallbackRegistry&,BattleDataChildServices&);
}
