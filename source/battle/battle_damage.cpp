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
bool storedStatusCheck(void*,const DamageUnit& unit,int id) {return checkStatus(&unit,id);}
int8_t storedStatusStrength(void*,const DamageUnit& unit,int id) {
    const uint8_t value=getStatus(unit,id).strength;
    return value<128?int8_t(value):int8_t(int(value)-256);
}
}
DamageStatusServices storedDamageStatusServices() {return {nullptr,storedStatusCheck,storedStatusStrength};}
// Capture element BEFORE hpPower; read special flags/modifiers AFTER it.
// Default code=flags|30. Defender flags 2 adds0x1000; part0x20000000/no callback
// and pre-callback AllOrNothing failure return0. Callback-added AllOrNothing
// failure instead takes defenseOnly, preserving later defense/output modifiers.
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
    // Original attack modifier order: mode 1+3, additive badges, independent
    // peril 5*MegaRush and danger 2*PowerRush, IcePower, status 16, statuses 10/12,
    // drain/defense-trade penalties, statuses 11/13. Each status-stage floor 0
    // remains separate; status 16 path also ORs attacker flag 8.
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
    // Source element indices0..4 map codes23/24/26/25/27; unknown element
    // code defaults23 but table bounds are STILL caller-owned. Defense attribute
    //3 skips all defense modifiers; others retain badge/status/mode/extra-defense
    // order and positive-defense piercing mask 0x40.
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
    // Defender trade modifier precedes sequence decay (minimum 1 only while
    // initially positive), guard-1, elemental IcePower, defense trade, DoublePain
    // multiplier(n+1), LastStand wrapped(damage+n)/(n+1), then attributes.
    // Integer arithmetic wraps at each helper, not merely the final assignment.
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
    // Attribute1 adds1, 2 nullifies, 4 nullifies unless incoming high bit set.
    // Attribute3 returns NEGATIVE floored damage and code mask 0x2000 even at0;
    // ordinary zero instead restores default code 30. No HP write occurs here.
    if(attribute==1)damage=add(damage,1);
    else if(attribute==2 || (attribute==4 && !(flags&0x80000000)))damage=0;
    damage=floorZero(damage);
    if(attribute==3) {code|=0x2000;return -damage;}
    if(damage==0) {code=defaultCode;return 0;}
    if((flags&0x100) && ((special&0x8000) || !(defender.attributes&0x100)))code|=0x10000;
    return damage;
}
// FP skips code 0x2000 and unlike HP has NO incoming0x20000 AllOrNothing
// bypass. After callback only guard subtraction/floor 0 apply. Positive FP can
// replace low code 30 with 23 while retaining higher bits.
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
// Mario222/partners224..230 query inventory151; other kinds check held151.
// Eligibility only ORs code 0x800 and returns1: no HP test, consumption or healing.
int markEmergencyRevival(const DamageUnit& unit,uint32_t& code,const RevivalInventoryServices& inventory) {
    bool eligible;
    if(unit.kind==222 || (unit.kind>=224 && unit.kind<=230)) {
        assert(inventory.itemCount);
        eligible=inventory.itemCount(inventory.user,151)>0;
    } else eligible=unit.heldItem==151;
    if(!eligible)return 0;
    code|=0x800;return 1;
}
} // namespace ttyd
