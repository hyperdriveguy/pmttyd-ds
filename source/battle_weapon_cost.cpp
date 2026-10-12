#include "battle_weapon_cost.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t half(const uint8_t* p) {const uint16_t v=uint16_t(p[0])<<8|p[1];return v<0x8000?v:int32_t(v)-0x10000;}
void half(uint8_t* p,int32_t value) {const uint16_t v=static_cast<uint16_t>(value);p[0]=v>>8;p[1]=v;}
bool exponential(int32_t id) {
    return (id>=240 && id<=243)||(id>=247 && id<=249)||id==252||id==256||id==257||
           (id>=332 && id<=334)||id==337||id==338;
}
bool linear(int32_t id) {return (id>=244 && id<=246)||id==251||id==253;}
}
int32_t weaponFpCost(DamageUnit& u,DamageWeapon& v,const WeaponCostServices& s) {
    const int32_t item=v.itemId;const uint8_t base=v.fpCost;int32_t cost=base;
    if(item!=0) {
        assert(s.equippedCount);const int32_t copies=s.equippedCount(s.user,item);
        const int32_t reloadedItem=v.itemId;
        if(exponential(reloadedItem)) {
            assert(s.pow && s.truncate);
            const int32_t exponent=signedWord(static_cast<uint32_t>(copies)-1);
            const double factor=s.pow(s.user,2.0,static_cast<double>(exponent));
            cost=s.truncate(s.user,factor*static_cast<double>(base));
        } else if(linear(reloadedItem))cost=signedWord(uint32_t(base)*static_cast<uint32_t>(copies));
    }
    if(cost!=0) {
        cost=signedWord(static_cast<uint32_t>(cost)-u.fpCostReduction);
        if(cost<=0)cost=1;
    }
    if(cost>=1000)cost=999;
    return cost;
}
bool checkWeaponCost(DamageUnit& u,DamageWeapon& v,const WeaponCostServices& s) {
    assert(s.getFp);const int32_t available=s.getFp(s.user,u);
    const int32_t cost=weaponFpCost(u,v,s);if(available<cost)return false;
    assert(s.getAp);const int32_t ap=s.getAp(s.user);return ap>=100*v.apCost;
}
void payWeaponCost(DamageUnit& u,DamageWeapon& v,const WeaponCostServices& s) {
    assert(s.captureWork && s.getFp && s.setFp && s.addAp);
    void* work=s.captureWork(s.user);
    const int32_t fp=s.getFp(s.user,u);const int32_t cost=weaponFpCost(u,v,s);
    s.setFp(s.user,u,signedWord(static_cast<uint32_t>(fp)-static_cast<uint32_t>(cost)));
    const int32_t kind=u.kind;
    if(kind==222 || (kind>=224 && kind<=230)) {
        assert(s.point && s.count);const uint32_t field=kind==222?0x16f4b:0x16f4d;
        s.point(s.user,work,field,static_cast<uint8_t>(cost));s.count(s.user,work,field+1);
    }
    s.addAp(s.user,-100*v.apCost);
}
int32_t countEquippedBadge(const uint8_t* pouch,int32_t item) {
    assert(pouch);int32_t count=0;
    for(unsigned i=0;i<200;++i)if(half(pouch+0x38a+2*i)==item)++count;
    return count;
}
void addPouchAp(uint8_t* pouch,int32_t delta) {
    assert(pouch);half(pouch+0x7a,signedWord(static_cast<uint32_t>(half(pouch+0x7a))+static_cast<uint32_t>(delta)));
    if(half(pouch+0x7a)<0)half(pouch+0x7a,0);
    if(half(pouch+0x7a)>half(pouch+0x7c))half(pouch+0x7a,half(pouch+0x7c));
}
void addActionPoint(uint8_t& field,int32_t delta) {
    field=static_cast<uint8_t>(static_cast<uint32_t>(field)+static_cast<uint32_t>(delta));
    if(field>100)field=100;
}
void addActionCount(uint8_t& field) {if(field<100)++field;}
int payWeaponCostEvent(const std::array<int32_t,2>& operands,const WeaponPaymentEventServices& event,const WeaponCostServices& cost) {
    assert(event.resolve && event.translate && event.unit && event.weapon);
    const int32_t actor=event.resolve(event.user,operands[0]);
    const int32_t weapon=event.resolve(event.user,operands[1]);
    auto* u=event.unit(event.user,event.translate(event.user,actor));assert(u);
    auto* v=event.weapon(event.user,weapon);assert(v);payWeaponCost(*u,*v,cost);return 2;
}
}
