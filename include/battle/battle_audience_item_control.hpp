#pragma once
#include "battle_actor.hpp"
#include <optional>
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
