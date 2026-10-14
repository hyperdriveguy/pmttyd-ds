#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleWeaponMenuDecisionServices {
 BattleActorServices memory;void* user=nullptr;uint32_t privateCursor=0;
 uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
 bool (*build)(void*,uint32_t battle,uint32_t type)=nullptr;
 int32_t (*enemyBelong)(void*,uint32_t unit)=nullptr;
 int32_t (*belong)(void*,uint32_t unit)=nullptr;
 bool (*sample)(void*,uint32_t buffer,uint32_t weapon,int32_t actor,int32_t enemy,uint32_t admission,uint32_t attributes,int32_t direction)=nullptr;
 void (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t output)=nullptr;
 bool (*initialize)(void*,uint32_t battle,uint32_t cursor,uint32_t rows,uint32_t flags)=nullptr;
};
// Port readiness is separate from the original decision's scalar0.
std::optional<int32_t> decidePackedBattleWeapon(uint32_t battle,uint32_t type,uint32_t index,const BattleWeaponMenuDecisionServices&);
bool setupPackedBattleWeaponMenu(uint32_t battle,uint32_t type,uint32_t flags,const BattleWeaponMenuDecisionServices&);
}
