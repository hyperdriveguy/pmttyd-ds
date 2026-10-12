#include "battle_damage.hpp"
#include <algorithm>
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t value) {
    return value<=0x7fffffff ? int32_t(value) : int32_t(int64_t(value)-0x100000000LL);
}
int32_t add(int32_t a,int32_t b) {return signedWord(uint32_t(a)+uint32_t(b));}
int32_t sub(int32_t a,int32_t b) {return signedWord(uint32_t(a)-uint32_t(b));}
int32_t mul(int32_t a,int32_t b) {return signedWord(uint32_t(a)*uint32_t(b));}
int32_t floorZero(int32_t value) {return std::max<int32_t>(value,0);}
}
int32_t calculateHpDamage(DamageUnit* attacker,DamageUnit& defender,DamagePart& part,
                          DamageWeapon& weapon,uint32_t& code,uint32_t flags,
                          DamageBattleState& battle,const DamageStatusServices& statuses) {
    assert(statuses.check && statuses.strength);
    if(defender.flags&2)flags|=0x1000;
    uint8_t element=weapon.element;
    if(attacker && (weapon.specialFlags&0x400) && statuses.check(statuses.user,*attacker,8))element=1;
    const uint32_t defaultCode=flags|30;code=defaultCode;
    if(!weapon.hpPower || (part.attributes&0x20000000))return 0;
    if(attacker && (weapon.specialFlags&1) && attacker->badges.allOrNothing &&
       !(battle.actionResult&2) && !(flags&0x20000))return 0;
    int32_t power=weapon.hpPower(attacker,weapon,defender,part);
    // The original callback can change the weapon, actor, defender and global
    // battle state. Reload their modifier inputs after it has returned.
    const uint32_t special=weapon.specialFlags;
    if(battle.mode==1 && (special&2))power=add(power,3);
    bool defenseOnly=false;
    if(special&1) {
        assert(attacker);
        const auto& badges=attacker->badges;
        power=add(add(power,badges.powerPlus),badges.pUpDDown);
        if(badges.allOrNothing) {
            if((battle.actionResult&2) || (flags&0x20000))power=add(power,badges.allOrNothing);
            else {power=0;defenseOnly=true;}
        }
        if(!defenseOnly) {
            if(attacker->hp<=attacker->peril)power=add(power,5*badges.megaRush);
            if(attacker->hp<=attacker->danger)power=add(power,2*badges.powerRush);
            if(part.attributes&0x100)power=add(power,badges.icePower);
        }
    }
    if(!defenseOnly) {
        if(special&4) {
            assert(attacker);
            if(statuses.check(statuses.user,*attacker,16))
                power=floorZero(add(power,statuses.strength(statuses.user,*attacker,16)));
            attacker->flags|=8;
        }
        if(special&2) {
            assert(attacker);
            for(int id:{10,12})if(statuses.check(statuses.user,*attacker,id))
                power=floorZero(add(power,statuses.strength(statuses.user,*attacker,id)));
        }
        if(special&1) {
            power=sub(sub(sub(power,attacker->badges.pDownDUp),attacker->badges.hpDrain),attacker->badges.fpDrain);
        }
        if(special&2)for(int id:{11,13})if(statuses.check(statuses.user,*attacker,id))
            power=floorZero(add(power,statuses.strength(statuses.user,*attacker,id)));
        power=floorZero(power);
    }
    assert(part.defense && part.defenseAttributes);
    int32_t defense=part.defense[element];
    const int attribute=part.defenseAttributes[element];
    constexpr uint32_t elementCodes[]={23,24,26,25,27};
    code=flags|(element<5?elementCodes[element]:23);
    if(attribute!=3) {
        defense=add(defense,defender.badges.defendPlus);
        if(flags&0x40000)defense=add(defense,defender.badges.damageDodge);
        if(defender.statusFlags&0x01000000)defense=add(defense,1);
        if(statuses.check(statuses.user,defender,14))
            defense=add(defense,statuses.strength(statuses.user,defender,14));
        if(statuses.check(statuses.user,defender,15))
            defense=floorZero(add(defense,statuses.strength(statuses.user,defender,15)));
        if(battle.mode==2 && defender.kind==222)defense=add(defense,3);
        if(part.attributes&0x80)defense=floorZero(add(defense,defender.extraDefense));
        if((special&0x40) && defense>0)defense=0;
    }
    int32_t damage=attribute==3 ? power : sub(power,defense);
    damage=add(damage,defender.badges.pUpDDown);
    if(damage>0) {
        if(special&0x10)damage=std::max<int32_t>(sub(damage,defender.damageSequence),1);
        if(special&0x20) {assert(attacker);damage=std::max<int32_t>(sub(damage,attacker->attackSequence),1);}
    }
    if(flags&0x40000)damage=sub(damage,1);
    if(element==1)damage=sub(damage,defender.badges.icePower);
    damage=sub(damage,defender.badges.pDownDUp);
    if(defender.badges.doublePain)damage=mul(damage,defender.badges.doublePain+1);
    if(defender.badges.lastStand && defender.hp<=defender.danger) {
        const int32_t n=defender.badges.lastStand;
        damage=add(damage,n)/(n+1); // Signed division toward zero, after wrapped addition.
    }
    if(attribute==1)damage=add(damage,1);
    else if(attribute==2 || (attribute==4 && !(flags&0x80000000)))damage=0;
    damage=floorZero(damage);
    if(attribute==3) {code|=0x2000;return -damage;}
    if(damage==0) {code=defaultCode;return 0;}
    if((flags&0x100) && ((special&0x8000) || !(defender.attributes&0x100)))code|=0x10000;
    return damage;
}
int32_t calculateFpDamage(DamageUnit* attacker,DamageUnit& defender,DamagePart& part,
                          DamageWeapon& weapon,uint32_t& code,uint32_t flags,DamageBattleState& battle) {
    if((code&0x2000) || !weapon.fpPower || (part.attributes&0x20000000))return 0;
    if(attacker && (weapon.specialFlags&1) && attacker->badges.allOrNothing && !(battle.actionResult&2))return 0;
    int32_t damage=weapon.fpPower(attacker,weapon,defender,part);
    if(flags&0x40000)damage=sub(damage,1);
    damage=floorZero(damage);
    if(damage>0 && (code&255)==30)code=(code&0xffffff00)|23;
    return damage;
}
} // namespace ttyd
