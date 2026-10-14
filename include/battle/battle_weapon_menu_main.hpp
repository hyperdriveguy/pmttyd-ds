#pragma once
#include "battle_weapon_menu_decision.hpp"
namespace ttyd {
struct BattleWeaponMenuMainServices {
 BattleActorServices memory;void* user=nullptr;uint32_t privateCursor=0;
 uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
 bool (*keyAllowed)(void*,uint32_t battle)=nullptr;
 void (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t output)=nullptr;
 bool (*update)(void*,uint32_t battle,uint32_t type)=nullptr;
 uint32_t (*trigger)(void*,uint32_t mask)=nullptr;
 uint32_t (*repeat)(void*,uint32_t mask)=nullptr;
 void (*sound)(void*,uint32_t sourceSound)=nullptr;
 bool (*endUI)(void*,uint32_t battle)=nullptr;
 bool (*decide)(void*,uint32_t battle,uint32_t type,uint32_t index)=nullptr;
 bool (*targetSetup)(void*,uint32_t battle)=nullptr;
 void (*lightFriend)(void*)=nullptr;void (*lightEnemy)(void*)=nullptr;
 std::optional<bool> (*attackAudience)(void*)=nullptr;
 void (*display)(void*,uint32_t battle)=nullptr;
};
struct BattleMenuAudienceServices {
 BattleActorServices memory;void* user=nullptr;uint32_t privateItem=0;
 uint32_t (*trigger)(void*,uint32_t mask)=nullptr;
 uint32_t (*mario)(void*,uint32_t battle)=nullptr;
 bool (*itemOn)(void*,uint32_t output,int32_t,int32_t,int32_t,int32_t)=nullptr;
 void (*setTarget)(void*,int32_t)=nullptr;
 void (*endActMenu)(void*,uint32_t battle)=nullptr;
 void (*lightOff)(void*,int32_t,int32_t)=nullptr;
};
std::optional<bool> packedBattleMenuAudienceCheck(const BattleMenuAudienceServices&);
bool endPackedBattleWeaponMenu(uint32_t battle,const BattleWeaponMenuMainServices&);
std::optional<int32_t> mainPackedBattleWeaponMenu(uint32_t battle,uint32_t type,const BattleWeaponMenuMainServices&);
// Complete source admission leaf; UI-work pointer/readiness is caller-owned.
bool packedBattleMenuKeyAllowed(uint32_t battle,const BattleActorServices&);
}
