#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleConclusionServices {
    BattleActorServices actors;
    void* user=nullptr;
    int32_t (*checkStatus)(void*,uint32_t,int32_t)=nullptr;
    bool (*eventExists)(void*,uint32_t)=nullptr;
    int32_t (*afterReaction)(void*)=nullptr;
    int32_t (*enemyBelong)(void*,uint32_t marioUnit)=nullptr;
};
// Original group results are refreshed even when flags force conclusion.
int32_t battleCheckConcluded(uint32_t battle,const BattleConclusionServices&);
// Checked unit IDs are cleaned throughout the scan, including after a live ID.
int32_t battleUnitEventBarrier(uint32_t battle,const BattleConclusionServices&);
int32_t battleAllEventBarrier(uint32_t battle,const BattleConclusionServices&);
// Only the result-selection region of btlseqEnd state07000000. The caller owns
// admission to that state and the original field-info setter/end graph. This
// function publishes no victory, sequence completion or field return itself.
std::optional<int32_t> battleInitialEndResult(uint32_t battle,const BattleConclusionServices&);
}
