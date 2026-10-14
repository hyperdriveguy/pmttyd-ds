#pragma once
#include "battle_damage_application.hpp"

namespace ttyd {
enum class BadgeRecoveryResource {Hp,Fp};
struct BadgeRecoveryEventServices {
    void* user=nullptr;
    void* (*captureWork)(void*)=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    DamageUnit* (*unit)(void*,void* capturedWork,int32_t)=nullptr;
    void (*store)(void*,int32_t rawDestination,int32_t)=nullptr;
};
int badgeRecoveryAmountEvent(BadgeRecoveryResource,const std::array<int32_t,2>&,
                             const BadgeRecoveryEventServices&);
int recoverHpEvent(const std::array<int32_t,3>&,const BadgeRecoveryEventServices&);
int recoverFpEvent(const std::array<int32_t,3>&,const BadgeRecoveryEventServices&,
                   const DamageApplicationServices& resourceServices);
}
