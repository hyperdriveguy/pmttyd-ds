#pragma once
#include "battle_damage.hpp"
#include "battle_timing_a.hpp"

namespace ttyd {
struct DamagePosition {float x=0,y=0,z=0;};
enum class DamageEffectKind {Smoke,Hit,Ice,DamageStar,Fukidashi,MissStar,Recovery};
struct DamageEffect {
    DamageEffectKind kind;
    DamagePosition position;
    std::array<int32_t,4> integers{};
    std::array<float,2> floats{};
};
enum class DamageNoticeKind {ActionCount,ActionPoint,MarioBigDamage,PartyDown,KillEnemy,FinalAttack};
struct DamageNotice {DamageNoticeKind kind;int32_t value=0;uint32_t fieldOffset=0;};
struct DamageApplicationServices {
    void* user=nullptr;
    DamageUnit* (*getUnit)(void*,int32_t)=nullptr;
    DamagePart* (*bodyPart)(void*,DamageUnit&)=nullptr;
    DamagePosition (*hitPosition)(void*,DamageUnit&,DamagePart&)=nullptr;
    DamagePosition (*unitPosition)(void*,DamageUnit&)=nullptr;
    int32_t (*height)(void*,DamageUnit&)=nullptr;
    void (*effect)(void*,const DamageEffect&)=nullptr;
    void (*sound)(void*,const char*,DamagePosition)=nullptr;
    void (*notice)(void*,DamageUnit&,DamageNotice)=nullptr;
    void (*hitEvent)(void*,DamageUnit&,uint32_t)=nullptr;
    RevivalInventoryServices inventory;
};
DamageUnit* findMarioResourceOwner(const DamageApplicationServices&);
int32_t getUnitFp(DamageUnit&,const DamageApplicationServices&);
int32_t getUnitMaxFp(DamageUnit&,const DamageApplicationServices&);
void setUnitFp(DamageUnit&,int32_t,const DamageApplicationServices&);
void recoverUnitFp(DamageUnit&,int32_t,const DamageApplicationServices&);
// Ordered US local application. All executed service callbacks must be bound;
// effects/audio/audience/hit events are real service requests, not inferred internals.
// Final hit code is passed to hitEvent; the incoming code is by value.
void applyDirectDamage(int32_t attackerId,DamageUnit&,DamagePart*,int32_t hp,int32_t fp,
                       uint32_t code,int32_t pattern,int32_t sleepRecovery,
                       BattleRandom&,const DamageApplicationServices&);
}
