#pragma once
#include "battle_actor.hpp"
#include <optional>
// Break-slot controller B+0x1616c (size 0x134); three reels begin+0x18,
// stride 0x5c. Idle main is a partial source bridge, not the complete active slot
// game. False can follow state/earlier-reel stores and has no saved suffix.
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
