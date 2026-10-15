#pragma once
#include "battle_actor.hpp"
#include <optional>
// Inline stage work is B+0x163fc, size 0xb3c; twelve entries start at+4,
// stride 0xe8, with scratch pointer+0xe4. This immediate main has no retry cursor:
// earlier child/store effects survive false and must not be blindly replayed.
namespace ttyd {
struct BattleStageWorkServices {
    BattleActorServices actors;
    void* user=nullptr;
    bool (*spotlightMain)(void*)=nullptr;
    bool (*activeEntry)(void*,uint32_t entry)=nullptr;
    bool (*freeScratch)(void*,uint32_t allocation)=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t mode,int32_t elapsed,
        int32_t duration,float start,float end)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
// StageInit's storage projection only. Child StageObjectInit and batSpotInit
// must follow in source order; this does not claim either child completed.
void initializePackedBattleStageWorkStorage(uint32_t battle,const EventMemoryServices&);
// Retains the original work owner across callbacks. False suspends at an
// unavailable provider, preserving already performed writes.
bool packedBattleStageWorkMain(const BattleStageWorkServices&);
}
