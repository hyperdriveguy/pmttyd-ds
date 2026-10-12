#pragma once
#include "battle_packed_choice.hpp"
namespace ttyd {
struct BattlePackedSamplingServices {
    BattlePackedChoiceServices common;
    void* user=nullptr;
    uint32_t (*monosiri)(void*,int32_t kind)=nullptr;
};
int32_t packedSamplingForwardLevel(float x,int32_t direction,const BattlePackedSamplingServices&);
int32_t samplePackedBattleCandidates(const BattleCandidateRecords&,uint32_t weapon,int32_t actor,int32_t belong,uint32_t admission,uint32_t attributes,int32_t direction,const BattlePackedSamplingServices&);
void updatePackedBattleCandidates(const BattleCandidateRecords&,const BattlePackedSamplingServices&);
}
