#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleAudienceIdleServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<int32_t> (*random)(void*,int32_t bound)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    std::optional<float> (*multiplySingle)(void*,double,double)=nullptr;
    std::optional<float> (*fusedSingle)(void*,double,double,double)=nullptr;
    bool (*addTarget)(void*,float delta,float bias)=nullptr;
    // Wait's low/unordered-ratio branch does not assign callee-saved FPR29.
    // No default value or previous member's local FPR29 may substitute for it.
    std::optional<double> callerF29;
};
// Whole local Wait80192260 body. False retains earlier original stores and
// denotes an unavailable linked numeric/target service or caller register.
bool controlPackedBattleAudienceIdle(int32_t slot,int32_t front,const BattleAudienceIdleServices&);
int32_t packedBattleAudienceOffsetSeat(int32_t slot,int32_t columnOffset,int32_t rowOffset,const BattleActorServices&);
int32_t packedBattleAudienceBreakTurn(const BattleActorServices&);
}
