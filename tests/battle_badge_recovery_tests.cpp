#include "battle_badge_recovery.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    DamageUnit actor,mario;int work=1,currentWork=1;
    std::vector<int> calls;int32_t point=999,output=0,amount=3;
    bool nullUnit=false,skip=false,gateDuringResolve=false;
    BadgeRecoveryEventServices events() {
        BadgeRecoveryEventServices s;s.user=this;
        s.captureWork=[](void* p)->void* {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);return &f.work;};
        s.resolve=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(raw);f.currentWork=2;
            if(raw==10)return -2;if(raw==11)return 999;
            assert(raw==12);if(f.gateDuringResolve)f.actor.attributes|=0x40000000;return f.amount;};
        s.translate=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(2);assert(raw==-2);return f.skip?-1:7;};
        s.unit=[](void* p,void* work,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);assert(work==&f.work && id==7);return f.nullUnit?nullptr:&f.actor;};
        s.store=[](void* p,int32_t destination,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);f.output=destination;f.point=value;};return s;
    }
    DamageApplicationServices resources() {
        DamageApplicationServices s;s.user=this;
        s.getUnit=[](void* p,int32_t id)->DamageUnit* {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(20+id);return id==0?&f.mario:nullptr;};return s;
    }
};
int main() {
    Fixture amount;amount.actor.badges.hpDrain=4;amount.actor.badges.fpDrain=2;amount.actor.dealtHp=3;
    auto values=amount.events();values.resolve=nullptr;
    assert(badgeRecoveryAmountEvent(BadgeRecoveryResource::Hp,{{-2,-250000000}},values)==2);
    assert(amount.point==3 && amount.output==-250000000 && amount.calls==std::vector<int>({1,2,3,4}));
    amount.calls.clear();badgeRecoveryAmountEvent(BadgeRecoveryResource::Fp,{{-2,88}},values);assert(amount.point==2);
    amount.actor.dealtHp=-7;badgeRecoveryAmountEvent(BadgeRecoveryResource::Hp,{{-2,88}},values);assert(amount.point==-7);
    amount.nullUnit=true;badgeRecoveryAmountEvent(BadgeRecoveryResource::Hp,{{-2,88}},values);assert(amount.point==0);
    assert(amount.actor.dealtHp==-7 && amount.actor.badges.hpDrain==4);
    Fixture hp;hp.actor.hp=4;hp.actor.maxHp=10;hp.actor.field202=0xabcd;hp.actor.field206=0x1234;hp.actor.damageFlags=0xaaaa;
    assert(recoverHpEvent({{10,11,12}},hp.events())==2);
    assert(hp.calls==std::vector<int>({1,10,2,3,11,12}) && hp.currentWork==2);
    assert(hp.actor.hp==7 && hp.actor.hpBefore==4 && hp.actor.hpAfter==7);
    assert(hp.actor.field200==0xabcd && hp.actor.field204==0x1234 && hp.actor.damageFlags==0xaaab);
    Fixture gated;gated.actor.hp=4;gated.actor.maxHp=10;gated.actor.hpBefore=99;gated.actor.hpAfter=98;
    gated.actor.field200=97;gated.actor.field204=96;gated.actor.damageFlags=0xaaaa;gated.gateDuringResolve=true;
    recoverHpEvent({{10,11,12}},gated.events());
    assert(gated.actor.hp==7 && gated.actor.hpBefore==99 && gated.actor.hpAfter==98);
    assert(gated.actor.field200==97 && gated.actor.field204==96 && gated.actor.damageFlags==0xaaaa);
    Fixture skipped;skipped.skip=true;auto skipEvents=skipped.events();skipEvents.unit=nullptr;
    assert(recoverHpEvent({{10,11,12}},skipEvents)==2 && skipped.calls==std::vector<int>({1,10,2}));
    skipped.calls.clear();assert(recoverFpEvent({{10,11,12}},skipEvents,{})==2 && skipped.calls==std::vector<int>({1,10,2}));
    Fixture fp;fp.actor.kind=225;fp.actor.fp=99;fp.actor.hpBefore=42;fp.actor.damageFlags=0xaaaa;
    fp.mario.kind=222;fp.mario.fp=4;fp.mario.maxFp=5;
    recoverFpEvent({{10,11,12}},fp.events(),fp.resources());
    assert(fp.mario.fp==5 && fp.actor.fp==99 && fp.actor.hpBefore==42 && fp.actor.damageFlags==0xaaaa);
    assert(fp.calls==std::vector<int>({1,10,2,3,11,12,20,20}));
    Fixture overflow;overflow.actor.hp=32767;overflow.actor.maxHp=10;overflow.amount=1;
    recoverHpEvent({{10,11,12}},overflow.events());assert(overflow.actor.hp==-32768 && overflow.actor.hpAfter==-32768);
    std::cout<<"badge recovery event tests passed\n";
}
