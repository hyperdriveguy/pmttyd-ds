#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleAudienceInitServices {
    BattleActorServices actors;
    void* user=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
    bool (*setting)(void*)=nullptr;
    std::optional<uint32_t> (*texture)(void*,const char* path)=nullptr;
    bool (*noise)(void*)=nullptr;
    bool (*target)(void*)=nullptr;
};
bool initializePackedSetup4AudienceParameters(uint32_t config,uint32_t setup,const BattleAudienceInitServices&);
// Complete used mode0/setup4/zero-capacity population graph. Positive or
// different setup domains require their original population provider.
bool initializePackedSetup4ZeroAudience(const BattleAudienceInitServices&);
bool initializePackedBattleAudience(const BattleAudienceInitServices&);
// Full NumToTarget/Sub scan and original AddAudienceNum early-return gates.
bool updatePackedBattleAudienceZeroTarget(const BattleActorServices&);
}
