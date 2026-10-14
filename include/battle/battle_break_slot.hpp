#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleBreakSlotServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<uint32_t> (*allocate)(void*,uint32_t bytes)=nullptr;
    bool (*free)(void*,uint32_t address)=nullptr;
};
bool initializePackedBattleBreakSlot(const BattleBreakSlotServices&);
// State0 controller and original idle reel branches; admitted active states
// suspend the bridge after any earlier source stores.
bool packedBattleBreakSlotIdleMain(const BattleActorServices&);
bool endPackedBattleBreakSlot(const BattleBreakSlotServices&);
}
