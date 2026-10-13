#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct BattlePackedHitPrecheckServices {
 BattleActorServices memory;
 void* user=nullptr;
 bool (*status)(void*,uint32_t unit,int32_t)=nullptr;
 int32_t (*random)(void*,int32_t bound)=nullptr;
 float (*multiply)(void*,float,float)=nullptr;
 int32_t (*truncate)(void*,float)=nullptr;
};
// Complete recovered BattlePreCheckDamage decision tree. No HP application.
int32_t packedBattleHitPrecheck(uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t flags,const BattlePackedHitPrecheckServices&);
}
