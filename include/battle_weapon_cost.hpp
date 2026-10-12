#pragma once
#include "battle_damage.hpp"

namespace ttyd {
struct WeaponCostServices {
    void* user=nullptr;
    int32_t (*equippedCount)(void*,int32_t item)=nullptr;
    double (*pow)(void*,double,double)=nullptr;
    int32_t (*truncate)(void*,double)=nullptr; // Original fctiwz/FPSCR dependency.
    int32_t (*getFp)(void*,DamageUnit&)=nullptr;
    void (*setFp)(void*,DamageUnit&,int32_t)=nullptr;
    int32_t (*getAp)(void*)=nullptr;
    void (*addAp)(void*,int32_t)=nullptr;
    void* (*captureWork)(void*)=nullptr;
    void (*point)(void*,void* capturedWork,uint32_t fieldOffset,int32_t)=nullptr;
    void (*count)(void*,void* capturedWork,uint32_t fieldOffset)=nullptr;
};
int32_t weaponFpCost(DamageUnit&,DamageWeapon&,const WeaponCostServices&);
bool checkWeaponCost(DamageUnit&,DamageWeapon&,const WeaponCostServices&);
void payWeaponCost(DamageUnit&,DamageWeapon&,const WeaponCostServices&);
int32_t countEquippedBadge(const uint8_t* pouch,int32_t item);
void addPouchAp(uint8_t* pouch,int32_t delta);
void addActionPoint(uint8_t& field,int32_t delta);
void addActionCount(uint8_t& field);
struct WeaponPaymentEventServices {
    void* user=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    DamageUnit* (*unit)(void*,int32_t)=nullptr;
    DamageWeapon* (*weapon)(void*,int32_t resolvedAddress)=nullptr;
};
int payWeaponCostEvent(const std::array<int32_t,2>& operands,const WeaponPaymentEventServices&,const WeaponCostServices&);
}
