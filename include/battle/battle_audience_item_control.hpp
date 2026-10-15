#pragma once
#include "battle_actor.hpp"
#include <optional>
// Actor item states 7/8/10/12 are separate from item-flight states 0..7.
// Item bank A+0xf25c stride 0x48; held index is SIGNED halfword+0x1c.
// Source linked sounds/penalties may hold after state/item lifecycle effects.
namespace ttyd {
struct BattleAudienceItemControlServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<int32_t> (*randomRaw)(void*)=nullptr;
    bool (*addTarget)(void*,float delta,float bias)=nullptr;
    bool (*soundAt)(void*,uint32_t key,uint32_t position)=nullptr;
    bool (*throwSound)(void*,int32_t kind,int32_t slot)=nullptr;
};
uint32_t packedBattleAudienceItemAddress(int32_t index,const BattleActorServices&);
bool packedBattleAudienceEscapeAllowed(int32_t slot,const BattleActorServices&);
// Whole ItemOn/ItemThrow/Damage/Escape bodies (states7/8/10/12).
// Unavailable linked services preserve prior source stores; other functions
// return false without stores. Item allocation/flight is a separate owner.
bool controlPackedBattleAudienceItemState(uint32_t function,int32_t slot,int32_t front,const BattleAudienceItemControlServices&);
}
