#pragma once
#include <cstdint>
#include "battle_status.hpp"

namespace ttyd {
// Friendly badge labels are annotations of the documented US byte fields.
struct DamageBadges {
    uint8_t powerPlus=0, pUpDDown=0, allOrNothing=0, megaRush=0, powerRush=0;
    uint8_t pDownDUp=0, doublePain=0, lastStand=0, defendPlus=0, damageDodge=0;
    uint8_t hpDrain=0, fpDrain=0, icePower=0, pityFlower=0;
};
struct DamagePart;
struct DamageUnit : BattleStatusUnit {
    int32_t extraDefense=0, attackSequence=0;
    int16_t damageSequence=0;
    uint8_t danger=0, peril=0, jumpPowerBonus=0, hammerPowerBonus=0;
    uint32_t attributes=0, heldItem=0;
    DamageBadges badges;
    int16_t baseHp=0,maxHp=0,baseFp=0,maxFp=0,fp=0;
    uint8_t hpPlusCount=0,fpPlusCount=0;
    float snapshotValue=0;
    uint16_t snapshot13c=0,snapshot13e=0;
    int32_t dealtHp=0,requestedHp=0,requestedFp=0,consumedFp=0;
    int32_t hpBefore=0,hpAfter=0,pattern=0;
    uint8_t lastHp=0,lastFp=0,lastConsumedFp=0,enemyAudienceByte=0,pattern25Gate=0;
    uint16_t damageFlags=0,field200=0,field202=0,field204=0,field206=0;
    float recoveryHeightFactor=0;
    DamagePart* lastPart=nullptr;
    std::array<const char*,4> damageSounds{}; // Original kind9c/a0/a4/a8 bindings.
};
struct DamagePart {
    uint32_t attributes=0;
    // Caller-owned original tables; each must cover the selected element index.
    const int8_t* defense=nullptr;
    const int8_t* defenseAttributes=nullptr;
};
struct DamageWeapon;
using DamagePowerCallback=int32_t (*)(DamageUnit*, DamageWeapon&, DamageUnit&, DamagePart&);
struct DamageWeapon {
    DamagePowerCallback hpPower=nullptr, fpPower=nullptr;
    uint8_t element=0;
    uint32_t specialFlags=0;
    void* callbackData=nullptr;
    int32_t defaultPower=0;
    std::array<int32_t,6> powerWords{};
};
struct DamageBattleState {
    uint32_t actionResult=0;
    uint8_t mode=0;
};
// Explicit status service boundary. The standard readers below use the recovered
// shared unit storage; callers may inject readers for separately owned unit data.
struct DamageStatusServices {
    void* user=nullptr;
    bool (*check)(void*, const DamageUnit&, int)=nullptr;
    int8_t (*strength)(void*, const DamageUnit&, int)=nullptr;
};
DamageStatusServices storedDamageStatusServices();

// US local calculators: no HP/FP mutation, resource caps, hit events or RNG.
// Defender/part/weapon/state and status readers must be valid. Attacker may be
// null only on branches that do not dereference it, as in the original bodies.
int32_t calculateHpDamage(DamageUnit*, DamageUnit&, DamagePart&, DamageWeapon&,
                          uint32_t& code, uint32_t flags, DamageBattleState&,
                          const DamageStatusServices&);
int32_t calculateFpDamage(DamageUnit*, DamageUnit&, DamagePart&, DamageWeapon&,
                          uint32_t& code, uint32_t flags, DamageBattleState&);
struct RevivalInventoryServices {
    void* user=nullptr;
    int32_t (*itemCount)(void*, int32_t)=nullptr;
};
// Marks only; no HP check, consumption, healing or event execution.
int markEmergencyRevival(const DamageUnit&,uint32_t& code,const RevivalInventoryServices&);
}
