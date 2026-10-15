#pragma once
#include "battle_actor.hpp"
#include <optional>
// Twenty-four records at A+0x133fc, stride 0x24: flags 0, handle4, category 8,
// life+c, fade10, elapsed14, duration 18, volume bytes1c..20 (hex). Public backend
// handle -1 is a real failed request; nullopt is unavailable provider work.
namespace ttyd {
struct BattleAudienceSoundServices {
    BattleActorServices actors;
    void* user=nullptr;
    // Assigned=false preserves the source's uninitialized zero-count key
    // boundary. nullopt means no native provider; -1 is an actual failed request.
    std::optional<int32_t> (*play)(void*,uint32_t key,bool assigned,int32_t right,int32_t left)=nullptr;
    bool (*stop)(void*,int32_t handle)=nullptr;
    std::optional<uint8_t> (*volume)(void*,int32_t handle)=nullptr;
};
uint32_t battleAudienceSoundRecord(int32_t,const BattleActorServices&);
int32_t battleAudienceHalfCount(bool right,const BattleActorServices&);
bool stopBattleAudienceSound(int32_t,const BattleAudienceSoundServices&);
bool clapBattleAudienceSound(int32_t lifetime,int32_t fade,const BattleAudienceSoundServices&);
bool noiseAlwaysBattleAudienceSound(const BattleAudienceSoundServices&);
bool noiseMainBattleAudienceSound(const BattleAudienceSoundServices&);
void setBattleAudienceVolume(int32_t slot,int32_t target,int32_t duration,const BattleActorServices&);
void setAllBattleAudienceVolumes(int32_t target,int32_t duration,const BattleActorServices&);
}
