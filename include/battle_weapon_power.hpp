#pragma once
#include "battle_damage.hpp"
namespace ttyd {
enum class MarioPowerKind {Jump,Hammer};
struct WeaponPowerServices {
    void* user=nullptr;
    uint32_t (*actionResult)(void*)=nullptr;
    uint8_t (*pouchLevel)(void*,MarioPowerKind)=nullptr;
    uint8_t (*bonus)(void*,MarioPowerKind)=nullptr;
    int32_t (*powerWord)(void*,int32_t)=nullptr;
};
// Negative indices reach the caller's bounded provider; no invented lower clamp.
int32_t marioWeaponPower(MarioPowerKind,const WeaponPowerServices&);
int32_t defaultWeaponPower(DamageUnit*,DamageWeapon&,DamageUnit&,DamagePart&);
int32_t jumpWeaponPower(DamageUnit*,DamageWeapon&,DamageUnit&,DamagePart&);
int32_t hammerWeaponPower(DamageUnit*,DamageWeapon&,DamageUnit&,DamagePart&);
// Minimal documented US parameters, not complete weapon records. Mario
// callbackData must point to bound WeaponPowerServices during evaluation.
DamageWeapon ordinaryJumpWeapon();
DamageWeapon ordinaryHammerWeapon();
DamageWeapon harborBossWeapon();
}
