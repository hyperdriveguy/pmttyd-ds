#include "battle_weapon_icons.hpp"
#include <cassert>
namespace ttyd {
uint16_t jumpWeaponIcon(const BattleWeaponIconServices& s){
    assert(s.pouch);const auto* pouch=s.pouch(s.user);assert(pouch);
    const auto level=pouch[0x98];return level==3?169:level==2?168:167;
}
uint16_t hammerWeaponIcon(const BattleWeaponIconServices& s){
    assert(s.pouch);const auto* pouch=s.pouch(s.user);assert(pouch);
    const auto level=pouch[0x99];return level==3?166:level==2?165:164;
}
}
