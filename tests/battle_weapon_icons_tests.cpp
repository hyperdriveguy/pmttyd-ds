#include "battle_weapon_icons.hpp"
#include <array>
#include <cassert>
using namespace ttyd;
struct Fixture {std::array<uint8_t,0x5d4> first{},second{};unsigned queries=0;bool switchOwner=false;
    BattleWeaponIconServices services(){BattleWeaponIconServices s;s.user=this;s.pouch=[](void* p)->const uint8_t*{auto& f=*static_cast<Fixture*>(p);++f.queries;return f.switchOwner && f.queries>1?f.second.data():f.first.data();};return s;}
};
int main(){Fixture f;auto s=f.services();
    for(const auto level:{0,1,2,3,4,127,128,255}){
        f.first[0x98]=uint8_t(level);f.first[0x99]=uint8_t(level);
        const std::array<uint16_t,2> expected=level==2?std::array<uint16_t,2>{168,165}:level==3?std::array<uint16_t,2>{169,166}:std::array<uint16_t,2>{167,164};
        assert(jumpWeaponIcon(s)==expected[0] && hammerWeaponIcon(s)==expected[1]);
    }
    f.queries=0;f.switchOwner=true;f.first[0x98]=3;f.first[0x99]=3;f.second[0x99]=2;
    assert(jumpWeaponIcon(s)==169 && hammerWeaponIcon(s)==165 && f.queries==2);
}
