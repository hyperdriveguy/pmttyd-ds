#pragma once
#include "battle_packed_hit.hpp"
#include "battle_candidate_choice.hpp"

namespace ttyd {
// Providers own the recovered sampling/choice bodies and source domains.
// Each invocation supplies its own buffer. No shared attack buffer or fixed
// candidate fallback. These callbacks must not retain the private references.
struct BattleTakeoffCoreServices {
    void* user=nullptr;
    int32_t (*sample)(void*,BattleCandidateBuffer&,uint32_t weapon,int32_t actor,int32_t belong,uint32_t flags,uint32_t attributes,int32_t bias)=nullptr;
    int32_t (*choose)(void*,BattleCandidateBuffer&,uint32_t mode,int32_t& unit,int32_t& part)=nullptr;
};
struct BattlePackedTakeoffServices {
    BattlePartPositionServices position;
    BattleTakeoffCoreServices core;
    float (*subtract)(void*,float,float)=nullptr; // position.numericUser
};
int32_t packedEnemyBelong(uint32_t unit,const BattleActorServices&);
int32_t packedTakeoffPositionEvent(EventEntry&,const BattlePackedTakeoffServices&);
}
