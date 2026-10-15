#include "battle_weapon_icons.hpp"
#include <cassert>
namespace ttyd {
// Pouch98: levels 3/2/other ->169/168/167. Pouch99 Hammer ->166/165/164.
// Do not reuse class -row's signed-level/otherwise-highest icon branch.
uint16_t jumpWeaponIcon(const BattleWeaponIconServices& s){
    assert(s.pouch);const auto* pouch=s.pouch(s.user);assert(pouch);
    const auto level=pouch[0x98];return level==3?169:level==2?168:167;
}
uint16_t hammerWeaponIcon(const BattleWeaponIconServices& s){
    assert(s.pouch);const auto* pouch=s.pouch(s.user);assert(pouch);
    const auto level=pouch[0x99];return level==3?166:level==2?165:164;
}
}
