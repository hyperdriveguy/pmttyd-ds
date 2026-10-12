#pragma once
#include "battle_damage.hpp"

namespace ttyd {
struct BattleCandidateBuffer {
    // Original record byte offsets/big endian integers, with a semantic weapon
    // pointer kept separately. Unused bytes are retained, not reinitialized.
    std::array<uint8_t,0xacc> bytes{};
    DamageWeapon* weapon=nullptr;
};
struct CandidateChoiceServices {
    void* user=nullptr;
    void* (*captureWork)(void*)=nullptr;
    DamageUnit* (*unit)(void*,void* capturedWork,int32_t)=nullptr;
    void (*output)(void*,bool unitOutput,int32_t)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    bool (*status)(void*,DamageUnit&,int32_t)=nullptr;
    bool (*canRecover)(void*,DamageUnit&)=nullptr;
    uint8_t (*danger)(void*,DamageUnit&)=nullptr;
    uint8_t (*itemByte)(void*,uint32_t originalAddress)=nullptr;
    int32_t (*maxFp)(void*,DamageUnit&)=nullptr;
    int32_t (*fp)(void*,DamageUnit&)=nullptr;
};
// Valid weight indices/permutations and conversion domains are caller/service
// invariants. Native asserts report invalid domains rather than inventing clamps.
void chooseBattleCandidate(BattleCandidateBuffer&,uint32_t flags,const CandidateChoiceServices&);
}
