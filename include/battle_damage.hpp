#pragma once
#include <cstdint>
#include "battle_status.hpp"

namespace ttyd {
// Friendly badge labels are annotations of the documented US byte fields.
struct DamageBadges {
    uint8_t powerPlus=0, pUpDDown=0, allOrNothing=0, megaRush=0, powerRush=0;
    uint8_t pDownDUp=0, doublePain=0, lastStand=0, defendPlus=0, damageDodge=0;
    uint8_t hpDrain=0, fpDrain=0, icePower=0;
};
struct DamageUnit : BattleStatusUnit {
    int32_t extraDefense=0, attackSequence=0;
    int16_t damageSequence=0;
    uint8_t danger=0, peril=0;
    uint32_t attributes=0, heldItem=0;
    DamageBadges badges;
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
