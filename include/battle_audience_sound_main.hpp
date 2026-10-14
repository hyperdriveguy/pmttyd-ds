#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleAudienceSoundMainServices {
    BattleActorServices actors;
    void* user=nullptr;
    bool (*noise)(void*)=nullptr;
    std::optional<int32_t> (*check)(void*,int32_t)=nullptr;
    bool (*stop)(void*,int32_t)=nullptr;
    bool (*setVolume)(void*,int32_t,uint8_t)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
// Source SoundMain8019f97c: NoiseMain, then all24 records, without flag gating.
// False retains earlier stores/requests and stops at the unavailable provider.
bool packedBattleAudienceSoundMain(const BattleAudienceSoundMainServices&);
}
