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
struct BattleAudienceReactionServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<int32_t> (*random)(void*,int32_t bound)=nullptr;
    std::optional<int32_t> (*randomRaw)(void*)=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t mode,int32_t step,int32_t duration,float start,float end)=nullptr;
};
// Whole original JoyRaiseArm/JoyJump/JoyJump2/Booing bodies (states4..6,9).
// Missing RNG/interpolation retains stores already executed by that body.
bool controlPackedBattleAudienceReaction(uint32_t function,int32_t slot,int32_t front,const BattleAudienceReactionServices&);
}
