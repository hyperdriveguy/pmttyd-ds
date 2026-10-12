#pragma once
#include "battle_damage.hpp"
#include <optional>

namespace ttyd {
// Original address arithmetic is retained as uint32_t tokens. Providers map
// these to owned records; this API never dereferences original RAM addresses.
struct BattleSelectionServices {
    void* user=nullptr;
    uint32_t (*currentWork)(void*)=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    uint16_t (*half)(void*,uint32_t)=nullptr;
    uint32_t (*word)(void*,uint32_t)=nullptr;
    DamageUnit* (*unit)(void*,uint32_t work,int32_t id)=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    void (*store)(void*,int32_t rawDestination,int32_t)=nullptr;
};
int selectedTargetEvent(const std::array<int32_t,2>& outputs,const BattleSelectionServices&);
// bool/optional report unsupported native cursor domains; they are host policy,
// not the unknown original scalar return or a fallback weapon. Cursor remains
// untouched when no original output write occurs.
bool findCommandCursor(uint32_t menu,uint32_t type,uint32_t& cursor,const BattleSelectionServices&);
std::optional<uint32_t> selectedCommandWeapon(uint32_t capturedWork,const BattleSelectionServices&);
std::optional<int> commandWeaponAddressEvent(const std::array<int32_t,2>& operands,const BattleSelectionServices&);
}
