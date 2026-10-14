#pragma once
#include <cstdint>
namespace ttyd {
struct BattleWeaponIconServices {
    void* user=nullptr;
    const uint8_t* (*pouch)(void*)=nullptr;
};
// Complete source icon leaves: each call queries current pouch independently.
uint16_t jumpWeaponIcon(const BattleWeaponIconServices&);
uint16_t hammerWeaponIcon(const BattleWeaponIconServices&);
}
