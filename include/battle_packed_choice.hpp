#pragma once
#include "battle_packed_hit.hpp"
#include "battle_candidate_choice.hpp"

namespace ttyd {
struct BattleCandidateRecords {BattleActorServices bytes;uint32_t address=0;};
BattleCandidateRecords privateBattleCandidateRecords(BattleCandidateBuffer&);
struct BattlePackedChoiceServices {
    BattlePartPositionServices position;
    void* user=nullptr;
    int32_t (*status)(void*,uint32_t unit,int32_t id)=nullptr;
    int32_t (*canAct)(void*,uint32_t unit)=nullptr;
    int32_t (*maxFp)(void*,uint32_t unit)=nullptr;
    int32_t (*fp)(void*,uint32_t unit)=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    float (*convert)(void*,int32_t)=nullptr; // position.numericUser
    float (*divide)(void*,float,float)=nullptr;
    float (*subtract)(void*,float,float)=nullptr;
    float (*negativeMultiplySubtract)(void*,float,float,float)=nullptr;
};
int32_t packedUnitFp(uint32_t unit,bool maximum,const BattleActorServices&);
int32_t packedCanAct(uint32_t unit,const BattleActorServices&);
// No scalar return contract. Raw/private buffer read and ordered outputs stay
// live. Invalid private-buffer/weight indices are native provider invariants.
void choosePackedBattleCandidate(const BattleCandidateRecords&,uint32_t flags,const BattlePackedChoiceServices&,CandidateChoiceOutputs);
}
