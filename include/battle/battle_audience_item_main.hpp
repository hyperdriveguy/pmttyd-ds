#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
struct BattleAudienceItemEntryServices {
    BattleActorServices actors;void* user=nullptr;
    std::optional<int32_t> (*switchValue)(void*,int32_t index)=nullptr;
};
std::optional<int32_t> createPackedBattleAudienceItem(int32_t owner,int32_t kind,int32_t limit,const BattleAudienceItemEntryServices&);
struct BattleAudienceItemMainServices {
    BattleActorServices actors;void* user=nullptr;
    std::optional<int32_t> (*random)(void*,int32_t bound)=nullptr;
    std::optional<std::array<float,3>> (*hitPosition)(void*,uint32_t unit,uint32_t part)=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t mode,int32_t step,int32_t duration,float start,float end)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
    bool (*stopSound)(void*,int32_t slot)=nullptr;
};
std::optional<uint32_t> detectPackedBattleAudiencePlayerTarget(const BattleAudienceItemMainServices&);
int32_t packedBattleAudienceEmptyItemSlots(const BattleActorServices&);
// Whole original100-item controller. Linked unavailable services retain prior
// source stores. Presentation metadata is published to its original owner;
// this function does not grant inventory or replace the presentation event.
bool processPackedBattleAudienceItems(const BattleAudienceItemMainServices&);
}
