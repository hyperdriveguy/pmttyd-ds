#pragma once
#include "battle_candidate_choice.hpp"
#include "battle_position.hpp"

namespace ttyd {
struct SamplingPart {
    SamplingPart* next=nullptr;
    BattlePositionPart* position=nullptr;
    int8_t direction=0;
    int16_t forwardBias=0;
    std::array<JumpVector,2> samplingOffsets{}; // Original188..19c,distinct from hit170..184.
};
struct SamplingUnit {
    DamageUnit* data=nullptr;
    int32_t id=0; // Original word0,distinct from registry slot.
    const uint8_t* partCount=nullptr;
    SamplingPart* first=nullptr;
    const float* scale=nullptr;
};
struct CandidateSamplingServices {
    void* user=nullptr;
    void* (*captureWork)(void*)=nullptr;
    SamplingUnit* (*unit)(void*,void* capturedWork,int32_t registryId)=nullptr;
    SamplingPart* (*part)(void*,int32_t registryId,int32_t partId)=nullptr;
    bool (*status27)(void*,SamplingUnit&)=nullptr;
    JumpVector (*worldPosition)(void*,SamplingPart&)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr;
    const uint32_t* (*monosiri)(void*,int32_t kind)=nullptr;
};
int32_t samplingForwardLevel(float x,int8_t direction,const CandidateSamplingServices&);
void updateBattleCandidates(BattleCandidateBuffer&,const CandidateSamplingServices&);
int sampleBattleCandidates(BattleCandidateBuffer&,DamageWeapon*,int32_t actor,int32_t belong,
                          uint32_t admission,uint32_t attributes,int32_t direction,const CandidateSamplingServices&);
}
