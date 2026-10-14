#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleAudienceActorControlServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<int32_t> (*front)(void*,int32_t slot)=nullptr;
    bool (*state)(void*,uint32_t function,int32_t slot,int32_t front)=nullptr;
    std::optional<double> (*sine)(void*,double)=nullptr;
};
// Whole 80192a0c body. An unavailable state/math provider stops before its
// dependent stores; previously executed original stores remain visible.
bool controlPackedBattleAudienceActor(int32_t slot,const BattleAudienceActorControlServices&);
// Whole original idle turn, head-tilt and jump bodies (states1..3).
// Other function addresses return false without stores.
bool controlPackedBattleAudienceIdleReaction(uint32_t function,int32_t slot,int32_t front,const BattleActorServices&);
}
