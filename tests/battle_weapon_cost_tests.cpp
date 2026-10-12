#include "battle_weapon_cost.hpp"
#include "battle_damage_application.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    DamageUnit actor,mario;DamageWeapon weapon;
    std::array<DamageUnit*,64> registry{};std::vector<int> order;
    int32_t copies=1,ap=0,apDelta=9999,pointValue=-1;
    uint32_t pointField=0,countField=0;int work=0;
    bool mutateItem=false,mutateKind=false,mutateAp=false;
    DamageApplicationServices resources() {
        DamageApplicationServices s;s.user=this;
        s.getUnit=[](void* user,int32_t id) {return static_cast<Fixture*>(user)->registry[id];};return s;
    }
    WeaponCostServices services() {
        WeaponCostServices s;s.user=this;
        s.equippedCount=[](void* p,int32_t item)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(item==f.weapon.itemId);f.order.push_back(1);
            if(f.mutateItem) {f.weapon.itemId=244;f.weapon.fpCost=100;}return f.copies;};
        s.pow=[](void* p,double b,double e) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);return std::pow(b,e);};
        s.truncate=[](void* p,double v)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(3);assert(std::isfinite(v) && v>=-2147483648.0 && v<2147483648.0);return static_cast<int32_t>(v);};
        s.getFp=[](void* p,DamageUnit& u)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);return getUnitFp(u,f.resources());};
        s.setFp=[](void* p,DamageUnit& u,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(5);setUnitFp(u,value,f.resources());if(f.mutateKind)u.kind=225;};
        s.getAp=[](void* p)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(6);if(f.mutateAp)f.weapon.apCost=2;return f.ap;};
        s.addAp=[](void* p,int32_t delta) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(7);f.apDelta=delta;};
        s.captureWork=[](void* p)->void* {auto& f=*static_cast<Fixture*>(p);f.order.push_back(8);return &f.work;};
        s.point=[](void* p,void* work,uint32_t field,int32_t value) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);f.order.push_back(9);f.pointField=field;f.pointValue=value;};
        s.count=[](void* p,void* work,uint32_t field) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);f.order.push_back(10);f.countField=field;};return s;
    }
};
int main() {
    Fixture f;f.actor.fpCostReduction=255;
    assert(weaponFpCost(f.actor,f.weapon,f.services())==0 && f.order.empty());
    f.actor.fpCostReduction=0;f.weapon.fpCost=3;f.weapon.itemId=240;
    for(int n=0;n<4;++n) {f.copies=n;assert(weaponFpCost(f.actor,f.weapon,f.services())==(n==0?1:n==1?3:n==2?6:12));}
    f.weapon.fpCost=1;f.copies=0;assert(weaponFpCost(f.actor,f.weapon,f.services())==0);
    f.weapon.itemId=244;f.weapon.fpCost=3;f.copies=0;assert(weaponFpCost(f.actor,f.weapon,f.services())==0);
    f.copies=2;assert(weaponFpCost(f.actor,f.weapon,f.services())==6);
    f.actor.fpCostReduction=6;assert(weaponFpCost(f.actor,f.weapon,f.services())==1);
    f.actor.fpCostReduction=255;assert(weaponFpCost(f.actor,f.weapon,f.services())==1);
    f.actor.fpCostReduction=0;f.weapon.itemId=240;f.weapon.fpCost=3;f.mutateItem=true;f.order.clear();
    assert(weaponFpCost(f.actor,f.weapon,f.services())==6 && f.order==std::vector<int>({1})); // Reloaded linear item, captured base3.
    f.mutateItem=false;f.weapon.itemId=999;f.weapon.fpCost=9;assert(weaponFpCost(f.actor,f.weapon,f.services())==9);
    f.weapon.itemId=244;f.weapon.fpCost=255;f.copies=200;assert(weaponFpCost(f.actor,f.weapon,f.services())==999);
    for(int id:{240,243,247,249,252,256,257,332,334,337,338}) {f.weapon.itemId=id;f.weapon.fpCost=2;f.copies=2;assert(weaponFpCost(f.actor,f.weapon,f.services())==4);}
    for(int id:{244,245,246,251,253}) {f.weapon.itemId=id;assert(weaponFpCost(f.actor,f.weapon,f.services())==4);}

    f.weapon.itemId=0;f.weapon.fpCost=3;f.weapon.apCost=1;f.actor.fp=2;f.order.clear();
    assert(!checkWeaponCost(f.actor,f.weapon,f.services()) && f.order==std::vector<int>({4}));
    f.actor.fp=3;f.ap=99;assert(!checkWeaponCost(f.actor,f.weapon,f.services()));
    f.ap=100;assert(checkWeaponCost(f.actor,f.weapon,f.services()));
    f.mutateAp=true;assert(!checkWeaponCost(f.actor,f.weapon,f.services())); // AP byte reread after GetAP.
    f.mutateAp=false;f.actor.kind=222;f.actor.fp=1;f.order.clear();payWeaponCost(f.actor,f.weapon,f.services());
    assert(f.actor.fp==-2 && f.apDelta==-200 && f.pointValue==3 && f.pointField==0x16f4b && f.countField==0x16f4c);
    assert(f.order==std::vector<int>({8,4,5,9,10,7})); // Payment does not query affordability.
    Fixture partner;partner.actor.kind=229;partner.actor.fp=50;partner.mario.kind=222;partner.mario.belong=0;partner.mario.fp=4;
    partner.registry[37]=&partner.mario;partner.weapon.fpCost=6;payWeaponCost(partner.actor,partner.weapon,partner.services());
    assert(partner.mario.fp==-2 && partner.actor.fp==50 && partner.pointField==0x16f4d && partner.countField==0x16f4e);
    Fixture zero;zero.actor.kind=222;zero.actor.fp=8;payWeaponCost(zero.actor,zero.weapon,zero.services());
    assert(zero.actor.fp==8 && zero.pointValue==0 && zero.countField==0x16f4c && zero.apDelta==0);
    Fixture enemy;enemy.actor.kind=0;enemy.actor.fp=-32768;enemy.weapon.fpCost=1;
    payWeaponCost(enemy.actor,enemy.weapon,enemy.services());assert(enemy.actor.fp==32767 && enemy.order==std::vector<int>({8,4,5,7}));
    Fixture mutated;mutated.actor.kind=222;mutated.mutateKind=true;payWeaponCost(mutated.actor,mutated.weapon,mutated.services());
    assert(mutated.pointField==0x16f4d); // Kind is reread after SetFP.
    f.weapon.itemId=244;f.weapon.fpCost=255;f.copies=200;payWeaponCost(f.actor,f.weapon,f.services());assert(f.pointValue==231);

    std::array<uint8_t,0x51a> pouch{};
    auto put=[&](unsigned offset,int value) {const uint16_t v=static_cast<uint16_t>(value);pouch[offset]=v>>8;pouch[offset+1]=v;};
    assert(countEquippedBadge(pouch.data(),0)==200);
    put(0x38a,240);put(0x38a+398,240);put(0x38a+2,-1);
    assert(countEquippedBadge(pouch.data(),240)==2 && countEquippedBadge(pouch.data(),-1)==1 && countEquippedBadge(pouch.data(),65535)==0);
    put(0x7a,32767);put(0x7c,1000);addPouchAp(pouch.data(),1);assert(pouch[0x7a]==0 && pouch[0x7b]==0);
    put(0x7a,500);addPouchAp(pouch.data(),2000);assert(pouch[0x7a]==3 && pouch[0x7b]==232);
    put(0x7c,-5);addPouchAp(pouch.data(),-2000);assert(pouch[0x7a]==255 && pouch[0x7b]==251);
    uint8_t point=100;addActionPoint(point,200);assert(point==44);
    point=0;addActionPoint(point,-1);assert(point==100);
    uint8_t count=101;addActionCount(count);assert(count==101);count=99;addActionCount(count);assert(count==100);
    Fixture event;event.actor.kind=222;
    WeaponPaymentEventServices wrapper;wrapper.user=&event;
    wrapper.resolve=[](void* p,int32_t raw)->int32_t {
        auto& f=*static_cast<Fixture*>(p);f.order.push_back(raw==10?20:21);return raw==10?-2:0x1234;
    };
    wrapper.translate=[](void* p,int32_t id)->int32_t {
        auto& f=*static_cast<Fixture*>(p);assert(id==-2);f.order.push_back(22);return 7;
    };
    wrapper.unit=[](void* p,int32_t id) {
        auto& f=*static_cast<Fixture*>(p);assert(id==7);f.order.push_back(23);return &f.actor;
    };
    wrapper.weapon=[](void* p,int32_t address) {
        auto& f=*static_cast<Fixture*>(p);assert(address==0x1234);f.order.push_back(24);return &f.weapon;
    };
    assert(payWeaponCostEvent({{10,11}},wrapper,event.services())==2);
    assert(event.order==std::vector<int>({20,21,22,23,24,8,4,5,9,10,7}) && event.apDelta==0);
    std::cout<<"weapon cost tests passed\n";
}
