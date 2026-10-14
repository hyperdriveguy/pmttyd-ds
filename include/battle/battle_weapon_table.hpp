#pragma once
#include "battle_attack_selection.hpp"
#include <optional>
namespace ttyd {
struct BattleWeaponTargetCountServices {
 BattleActorServices memory;void* user=nullptr;uint32_t privateBuffer=0;
 int32_t (*cost)(void*,uint32_t unit,uint32_t weapon)=nullptr;
 int32_t (*enemyBelong)(void*,uint32_t unit)=nullptr;
 bool (*sample)(void*,uint32_t buffer,uint32_t weapon,int32_t actor,int32_t belong,uint32_t admission,uint32_t attributes,int32_t direction)=nullptr;
};
std::optional<int32_t> packedWeaponTargetCount(uint32_t battle,uint32_t unit,uint32_t weapon,const BattleWeaponTargetCountServices&);
struct BattleWeaponEligibilityServices {
 BattleActorServices memory;void* user=nullptr;uint32_t cursorSlot=0;
 uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
 void (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t output)=nullptr;
 std::optional<int32_t> (*targetCount)(void*,uint32_t battle,uint32_t unit,uint32_t weapon)=nullptr;
};
bool updatePackedBattleWeaponTable(uint32_t battle,uint32_t type,const BattleWeaponEligibilityServices&);
struct BattleWeaponTableServices {
 BattleActorServices memory;void* user=nullptr;
 uint32_t cursorSlot=0,countSlot=0; // Separate caller-owned private RAM words.
 uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
 uint32_t (*pouch)(void*)=nullptr;
 void (*cursor)(void*,uint32_t menu,uint32_t type,uint32_t output)=nullptr;
 int32_t (*equipped)(void*,int32_t badge)=nullptr;
 uint16_t (*jumpIcon)(void*)=nullptr;
 uint16_t (*hammerIcon)(void*)=nullptr;
 uint32_t (*message)(void*,uint32_t sourceKey)=nullptr;
 bool (*party)(void*,uint32_t callback,uint32_t menu,uint32_t countAddress)=nullptr;
 bool (*update)(void*,uint32_t battle,uint32_t type)=nullptr;
};
// nullopt retains an unbound party/update service, including partial stores.
// A normal empty result returns0 and preserves prior cursor/descriptor state.
std::optional<int32_t> makePackedBattleWeaponTable(uint32_t battle,uint32_t type,const BattleWeaponTableServices&);
}
