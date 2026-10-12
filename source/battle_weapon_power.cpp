#include "battle_weapon_power.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t word(uint32_t x) {return x<=0x7fffffff?int32_t(x):int32_t(int64_t(x)-0x100000000LL);}
int32_t marioCallback(DamageUnit* actor,MarioPowerKind kind,DamageWeapon& weapon) {
    assert(actor && weapon.callbackData);
    return marioWeaponPower(*actor,kind,*static_cast<const WeaponPowerServices*>(weapon.callbackData));
}
}
int32_t marioWeaponPower(const DamageUnit& actor,MarioPowerKind kind,const WeaponPowerServices& s) {
    assert(s.actionResult && s.pouchLevel && s.bonus && s.powerWord);
    const uint32_t success=(s.actionResult(s.user)>>1)&1;
    const uint8_t raw=s.pouchLevel(s.user,kind);
    const int32_t level=raw<128?int32_t(raw):int32_t(int(raw)-256);
    int32_t index=word((uint32_t(level)-1)*2+success);
    if(index>=6)index=5;
    const uint8_t bonus=s.bonus(s.user,actor,kind);
    const int32_t power=s.powerWord(s.user,index);
    return bonus?word(uint32_t(power)+bonus):power;
}
int32_t defaultWeaponPower(DamageUnit*,DamageWeapon& w,DamageUnit&,DamagePart&) {return w.defaultPower;}
int32_t jumpWeaponPower(DamageUnit* a,DamageWeapon& w,DamageUnit&,DamagePart&) {return marioCallback(a,MarioPowerKind::Jump,w);}
int32_t hammerWeaponPower(DamageUnit* a,DamageWeapon& w,DamageUnit&,DamagePart&) {return marioCallback(a,MarioPowerKind::Hammer,w);}
DamageWeapon ordinaryJumpWeapon() {
    DamageWeapon w;w.hpPower=jumpWeaponPower;w.powerWords={1,1,2,2,3,3};w.specialFlags=0x35807;return w;
}
DamageWeapon ordinaryHammerWeapon() {
    DamageWeapon w;w.hpPower=hammerWeaponPower;w.powerWords={1,2,2,4,3,6};w.specialFlags=0x30887;return w;
}
DamageWeapon harborBossWeapon() {
    DamageWeapon w;w.hpPower=defaultWeaponPower;w.defaultPower=1;w.specialFlags=0x1087;return w;
}
}
