#pragma once
#include "battle_actor.hpp"
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
struct BattlePackedSelectionServices {
    BattleActorServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
};
int32_t packedSelectedTargetEvent(EventEntry&,const BattlePackedSelectionServices&);
bool findPackedCommandCursor(uint32_t menu,uint32_t type,uint32_t& cursor,const BattlePackedSelectionServices&);
std::optional<uint32_t> packedSelectedCommandWeapon(uint32_t capturedBattle,const BattlePackedSelectionServices&);
// nullopt denotes unsupported native cursor domain,not original fallback success.
std::optional<int32_t> packedCommandWeaponEvent(EventEntry&,const BattlePackedSelectionServices&);
}
