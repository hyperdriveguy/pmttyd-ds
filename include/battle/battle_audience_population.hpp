#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleAudiencePopulationServices {
    BattleActorServices actors;
    void* user=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    std::optional<int32_t> (*randomRaw)(void*)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
};
// Whole original local bodies. Linked RNG/float conversion are supplied;
// nullopt/false retains prior source stores, with no synthetic successful result.
std::optional<int32_t> packedBattleAudienceFreeSeat(bool front,const BattleAudiencePopulationServices&);
std::optional<int32_t> packedBattleAudienceFreeFrontThenBack(const BattleAudiencePopulationServices&);
std::optional<int32_t> packedBattleAudienceFreeSeatUniform(const BattleAudiencePopulationServices&);
std::optional<int32_t> packedBattleAudienceEntry(int32_t slot,int32_t type,int32_t appearance,const BattleAudiencePopulationServices&);
bool packedBattleAudienceEntrySub(int32_t slot,uint8_t type,int32_t appearance,const BattleAudiencePopulationServices&);
void packedBattleAudienceSetAnimation(int32_t slot,int32_t animation,int32_t force,const BattleActorServices&);
void packedBattleAudienceChangeStatus(int32_t slot,int32_t state,const BattleActorServices&);
void packedBattleAudienceDelete(int32_t slot,const BattleActorServices&);
int32_t packedBattleAudienceAnimationEnded(int32_t slot,const BattleActorServices&);
bool initializePackedBattleAudiencePopulation(const BattleAudiencePopulationServices&);
}
