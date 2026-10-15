#pragma once
#include <cstdint>
// These are WEAPON-row icons, not action-class palette IDs334..339. Current
// pouch is queried per call; unsigned level 2/3 choose upgrades, all others default.
namespace ttyd {
struct BattleWeaponIconServices {
    void* user=nullptr;
    const uint8_t* (*pouch)(void*)=nullptr;
};
// Complete source icon leaves: each call queries current pouch independently.
uint16_t jumpWeaponIcon(const BattleWeaponIconServices&);
uint16_t hammerWeaponIcon(const BattleWeaponIconServices&);
}
