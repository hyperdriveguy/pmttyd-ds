#pragma once
#include "battle_actor.hpp"
#include <optional>
// Audience A=B+0x2858: 200 actors at+0x1bc/stride 0x134, 100 items at
// +0xf25c/stride 0x48. Immediate parents retain NO suffix cursor; unavailable
// children can follow stores/RNG/earlier calls and must not be blindly replayed.
namespace ttyd {
struct BattleAudienceMainServices {
    BattleActorServices actors;
    void* user=nullptr;
    bool (*request)(void*,uint32_t function)=nullptr;
};
bool packedBattleAudienceMain(const BattleAudienceMainServices&);
struct BattleAudienceControlServices {
    BattleActorServices actors;
    void* user=nullptr;
    bool (*actor)(void*,uint32_t record,int32_t index,bool motion)=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    std::optional<int32_t> (*itemOn)(void*,int32_t index)=nullptr;
};
bool packedBattleAudienceControl(const BattleAudienceControlServices&);
bool packedBattleAudienceInactiveItems(const BattleActorServices&);
bool packedBattleAudienceInactiveApSources(const BattleActorServices&);
// Actual ItemOn(index,0,1) rejection prefix. Active/type-dependent continuation
// returns nullopt, retaining the admitted original provider boundary.
std::optional<int32_t> packedBattleAudienceEmptyItemOn(int32_t index,const BattleActorServices&);
}
