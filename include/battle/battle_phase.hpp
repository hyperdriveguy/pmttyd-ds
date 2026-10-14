#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct BattlePhaseServices {
    BattleActorServices actors;
    float (*real)(void*,uint32_t)=nullptr;
    bool (*status)(void*,uint32_t,int32_t)=nullptr;
};
void buildBattlePhaseTables(uint32_t battle,const BattlePhaseServices&);
bool battlePhaseEnded(uint32_t battle,const BattlePhaseServices&);
// Caller must establish the original valid-actor invariant before selection.
int32_t selectBattlePhaseActor(uint32_t battle,const BattlePhaseServices&);
void consumeBattleActorMove(uint32_t battle,uint32_t unit,const BattlePhaseServices&);
}
