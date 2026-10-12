#include "battle_damage_application.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace ttyd {
namespace {
int32_t word(uint32_t x) {return x<=0x7fffffff?int32_t(x):int32_t(int64_t(x)-0x100000000LL);}
int32_t add(int32_t a,int32_t b) {return word(uint32_t(a)+uint32_t(b));}
int32_t sub(int32_t a,int32_t b) {return word(uint32_t(a)-uint32_t(b));}
int16_t half(int32_t x) {const uint16_t v=uint16_t(x);return v<32768?int16_t(v):int16_t(int(v)-65536);}
int byte(uint8_t x) {return x<128?int(x):int(x)-256;}
DamageUnit& fpOwner(DamageUnit& unit,const DamageApplicationServices& s) {
    if(unit.kind<224 || unit.kind>230)return unit;
    auto* owner=findMarioResourceOwner(s);assert(owner);return *owner;
}
void emit(const DamageApplicationServices& s,DamageEffectKind kind,DamagePosition p,
          std::array<int32_t,4> integers={},std::array<float,2> floats={}) {
    assert(s.effect);s.effect(s.user,{kind,p,integers,floats});
}
void sound(const DamageApplicationServices& s,const char* key,DamagePosition p) {
    assert(s.sound);s.sound(s.user,key,p);
}
void notice(const DamageApplicationServices& s,DamageUnit& unit,DamageNoticeKind kind,int32_t value=0,uint32_t field=0) {
    assert(s.notice);s.notice(s.user,unit,{kind,value,field});
}
}
DamageUnit* findMarioResourceOwner(const DamageApplicationServices& s) {
    assert(s.getUnit);
    for(int i=0;i<64;++i) {auto* unit=s.getUnit(s.user,i);if(unit && unit->kind==222 && unit->belong==0)return unit;}
    return nullptr;
}
int32_t getUnitFp(DamageUnit& unit,const DamageApplicationServices& s) {return fpOwner(unit,s).fp;}
int32_t getUnitMaxFp(DamageUnit& unit,const DamageApplicationServices& s) {return fpOwner(unit,s).maxFp;}
void setUnitFp(DamageUnit& unit,int32_t value,const DamageApplicationServices& s) {fpOwner(unit,s).fp=half(value);}
void recoverUnitFp(DamageUnit& unit,int32_t amount,const DamageApplicationServices& s) {
    auto& owner=fpOwner(unit,s);owner.fp=half(add(owner.fp,amount));
    if(owner.fp>owner.maxFp)owner.fp=owner.maxFp;
}
void applyDirectDamage(int32_t attackerId,DamageUnit& d,DamagePart* part,int32_t hp,int32_t fp,
                       uint32_t code,int32_t pattern,int32_t sleepRecovery,BattleRandom& random,
                       const DamageApplicationServices& s) {
    DamageUnit* attacker=nullptr;
    if(attackerId!=-5) {assert(s.getUnit);attacker=s.getUnit(s.user,attackerId);}
    hp=std::clamp<int32_t>(hp,-99,99);
    if(attacker)attacker->dealtHp=add(attacker->dealtHp,hp);
    if(!part) {assert(s.bodyPart);part=s.bodyPart(s.user,d);}
    assert(part && s.hitPosition);d.lastPart=part;d.pattern=pattern;
    if((d.attributes&0x200000) && pattern>=21 && pattern<=24)d.pattern=0;
    const auto initial=s.hitPosition(s.user,d,*part);
    if(code&0x2000) {
        emit(s,DamageEffectKind::Recovery,s.hitPosition(s.user,d,*part),{0,std::abs(hp)});
    } else if(hp!=0 || fp!=0) {
        const auto position=s.hitPosition(s.user,d,*part);const uint32_t low=code&255;
        if(low==24 || low==25)emit(s,DamageEffectKind::Smoke,position,{0,30},{1});
        if(low==26)emit(s,DamageEffectKind::Ice,position,{0});
        const int type=low==24 || low==25?1:low==26?3:low==27?2:0;
        emit(s,DamageEffectKind::Hit,position,{type,3},{1});
        auto star=position;star.x+=d.belong==0?-8.f:0.f;star.y+=d.belong==0?5.f:10.f;
        if(hp!=0 && !(d.attributes&0x400000))
            emit(s,DamageEffectKind::DamageStar,star,{0,hp},{10,d.belong==0?30.f:-30.f});
        if(fp!=0)emit(s,DamageEffectKind::DamageStar,star,{1,fp},
            {10,d.belong==0?(hp!=0?60.f:30.f):(hp!=0?-60.f:-30.f)});
        if(attacker && attacker->badges.megaRush && attacker->hp<=attacker->peril)
            emit(s,DamageEffectKind::Fukidashi,s.hitPosition(s.user,d,*part),{0,0,0,60},{10,0});
    } else {
        const auto position=s.hitPosition(s.user,d,*part);
        const bool skip=pattern==2 || pattern==3 || pattern==7 || pattern==19 || pattern==20 ||
            pattern==21 || pattern==22 || pattern==27 || (code&255)==22;
        if(!skip)emit(s,DamageEffectKind::MissStar,position,{(code&0x20000)?1:0,1,d.belong==0?-1:1});
    }
    if(!(code&0x2000) && hp>0) {
        int successes=0;const int copies=d.badges.pityFlower;
        for(int i=0;i<copies;++i)if(random.bounded(100)<30)++successes;
        if(successes>0) {
            recoverUnitFp(d,successes,s);assert(s.unitPosition && s.height);
            auto position=s.unitPosition(s.user,d);const int32_t height=s.height(s.user,d);
            position.y+=std::fma(d.recoveryHeightFactor,float(height),5.f);
            emit(s,DamageEffectKind::Recovery,position,{1,successes});
        }
    }
    if(pattern!=19 && pattern!=27) {
        constexpr uint32_t masks[]={0x01000000,0x02000000,0x04000000,0x08000000,0x10000000};
        constexpr const char* keys[]={"SFX_MARIO_HAMMER_PIKKYO_R1","SFX_MARIO_HAMMER_PIKKYO_Y1",
            "SFX_MARIO_HAMMER_PIKKYO_B1","SFX_MARIO_HAMMER_PIKKYO_G1","SFX_MARIO_HAMMER_PIKKYO_P1"};
        bool override=false;
        for(int i=0;i<5;++i)if(code&masks[i]) {sound(s,keys[i],initial);override=true;break;}
        if(!override && (code&0x40000) && d.belong==0) {sound(s,"SFX_MARIO_GUARD1",initial);override=true;}
        if(!override) {
            if(hp>0) {
                const uint32_t low=code&255;
                if(low==24 || low==26 || low==27)sound(s,d.damageSounds[low==24?1:low==26?2:3],initial);
                sound(s,d.damageSounds[0],initial);
            }
            if(fp>0 && d.belong==0)sound(s,"SFX_MARIO_DAMAGE_FP1",initial);
        }
    }
    d.lastHp=uint8_t(hp);d.requestedHp=add(d.requestedHp,byte(d.lastHp));
    d.lastFp=d.lastConsumedFp=uint8_t(fp);d.requestedFp=add(d.requestedFp,byte(d.lastFp));
    if(!(d.attributes&0x40000000) && !(part->attributes&0x800000)) {
        d.hpBefore=d.hp;d.hp=half(sub(d.hp,byte(d.lastHp)));
        if(d.hp>d.maxHp)d.hp=d.maxHp;
        if(d.hp<0)d.hp=0;
        d.hpAfter=d.hp;
        if(byte(d.lastHp)!=0) {d.field200=d.field202;d.field204=d.field206;}
        d.damageFlags=(d.damageFlags&~1u)|((code&0x100)?1u:0u);
    }
    if(byte(d.lastFp)!=0) {
        const int32_t current=getUnitFp(d,s);
        d.lastConsumedFp=uint8_t(current<fp?current:fp);
        setUnitFp(d,std::max<int32_t>(sub(current,fp),0),s);
        d.consumedFp=add(d.consumedFp,byte(d.lastConsumedFp));
    }
    if(hp>0 && (d.kind==222 || (d.kind>=224 && d.kind<=230))) {
        const bool mario=d.kind==222;
        notice(s,d,DamageNoticeKind::ActionCount,0,mario?0x16f3e:0x16f3f);
        notice(s,d,DamageNoticeKind::ActionPoint,uint8_t(hp),mario?0x16f3c:0x16f3d);
    }
    if(sleepRecovery && (hp>0 || fp>0) && checkStatus(&d,1) && random.bounded(100)<50) {
        d.statusByte(0x118)=1;d.statusByte(0x119)=100;checkRecoveryStatus(&d,1);
        assert(s.unitPosition && s.height);auto position=s.unitPosition(s.user,d);
        position.y+=float(s.height(s.user,d)/2);
        emit(s,DamageEffectKind::Fukidashi,position,{0,0,0,60},{10,0});
    }
    if(d.kind==222)notice(s,d,DamageNoticeKind::MarioBigDamage,byte(d.lastHp));
    if(d.hp>0) {if(d.pattern==25)d.pattern=0;}
    else {
        d.hp=0;
        if(code&0x100) {
            code|=0x200;
            if(d.attributes&0x10000) {
                const uint32_t low=code&255;
                if(low==24 || low==25 || low==46 || low==49 || low==50 || low==54)code|=0x400;
            } else {code|=0x400;markEmergencyRevival(d,code,s.inventory);}
            if(d.pattern==25 && (!(code&0x400) || (code&0x800) || !d.pattern25Gate))d.pattern=0;
            if(code&0x400) {
                if(d.kind>=224 && d.kind<=230)notice(s,d,DamageNoticeKind::PartyDown);
                if(d.kind>=1 && d.kind<=215) {
                    notice(s,d,DamageNoticeKind::KillEnemy,d.enemyAudienceByte);notice(s,d,DamageNoticeKind::FinalAttack);
                }
            }
        }
    }
    d.flags|=1;assert(s.hitEvent);s.hitEvent(s.user,d,code);
}
}
