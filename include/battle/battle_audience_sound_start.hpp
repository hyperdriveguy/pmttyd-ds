#pragma once
#include "battle_actor.hpp"
#include "battle_audience_sound.hpp"
#include <array>
#include <optional>
namespace ttyd {
struct BattleAudienceSoundInfo {uint8_t volume=0;std::array<float,3> position{};};
struct BattleAudienceSoundStartServices {
    BattleActorServices actors;void* user=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    std::optional<int32_t> (*on)(void*,uint32_t key,const std::array<float,3>&)=nullptr;
    bool (*volume)(void*,int32_t handle,uint8_t)=nullptr;
};
std::optional<BattleAudienceSoundInfo> packedBattleAudienceSoundInfo1(int32_t right,int32_t left,const BattleAudienceSoundStartServices&);
std::array<float,3> packedBattleAudienceSoundInfo2(int32_t slot,const BattleActorServices&);
std::optional<int32_t> startPackedBattleAudienceSound1(uint32_t key,int32_t right,int32_t left,const BattleAudienceSoundStartServices&);
std::optional<int32_t> startPackedBattleAudienceSound2(uint32_t key,int32_t slot,const BattleAudienceSoundStartServices&);
// Whole original SoundItemThrow801a044c, including lane selection, stop/start
// and public-handle volume initialization. No voice is fabricated.
bool playPackedBattleAudienceThrowSound(int32_t kind,int32_t slot,const BattleAudienceSoundStartServices&,const BattleAudienceSoundServices&);
}
