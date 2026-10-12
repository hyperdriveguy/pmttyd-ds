#include "battle_damage_application.hpp"
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)
struct Fixture {
    DamageUnit a,d,mario,partner;DamagePart part;BattleRandom random{1};
    std::array<DamageUnit*,64> units{};
    std::vector<DamageEffect> effects;std::vector<std::string> sounds,order;
    std::vector<DamageNotice> notices;uint32_t hitCode=0;int hits=0,inventoryCalls=0,hitPositions=0;
    int32_t inventoryCount=0;
    Fixture() {
        a.hp=a.maxHp=d.hp=d.maxHp=10;d.fp=5;d.maxFp=5;d.belong=1;d.kind=1;
        d.damageSounds={"damage","fire","ice","electric"};
        units[0]=&a;units[1]=&d;mario.kind=222;mario.fp=3;mario.maxFp=5;partner.kind=224;
    }
    static Fixture& self(void* user) {return *static_cast<Fixture*>(user);}
    static DamageUnit* lookup(void* user,int32_t id) {auto& f=self(user);CHECK(id>=0 && id<64);return f.units[size_t(id)];}
    static DamagePart* body(void* user,DamageUnit&) {self(user).order.push_back("body");return &self(user).part;}
    static DamagePosition hit(void* user,DamageUnit&,DamagePart&) {auto& f=self(user);++f.hitPositions;return {20,30,40};}
    static DamagePosition position(void*,DamageUnit&) {return {10,20,30};}
    static int32_t height(void*,DamageUnit&) {return 11;}
    static void effect(void* user,const DamageEffect& e) {auto& f=self(user);f.effects.push_back(e);f.order.push_back("effect");}
    static void sound(void* user,const char* key,DamagePosition p) {
        auto& f=self(user);CHECK(key);CHECK(p.x==20 && p.y==30 && p.z==40);
        f.sounds.emplace_back(key);f.order.push_back("sound");
    }
    static void notice(void* user,DamageUnit&,DamageNotice n) {auto& f=self(user);f.notices.push_back(n);f.order.push_back("notice");}
    static void event(void* user,DamageUnit& d,uint32_t code) {auto& f=self(user);CHECK(d.flags&1);f.hitCode=code;++f.hits;f.order.push_back("event");}
    static int32_t inventory(void* user,int32_t item) {auto& f=self(user);CHECK(item==151);++f.inventoryCalls;return f.inventoryCount;}
    DamageApplicationServices services() {return {this,lookup,body,hit,position,height,effect,sound,notice,event,{this,inventory}};}
    void apply(int32_t hp,int32_t fp=0,uint32_t code=23,int32_t pattern=0,int32_t wake=0,int32_t attacker=0) {
        applyDirectDamage(attacker,d,nullptr,hp,fp,code,pattern,wake,random,services());
        CHECK(hits==1 && order.back()=="event" && d.lastPart==&part);
    }
};
uint32_t seedFor(int firstLow,int firstHigh,int secondLow=0,int secondHigh=99) {
    for(uint32_t seed=0;seed<100000;++seed) {
        BattleRandom random(seed);const int a=random.bounded(100),b=random.bounded(100);
        if(a>=firstLow && a<=firstHigh && b>=secondLow && b<=secondHigh)return seed;
    }
    throw std::runtime_error("No derived RNG fixture seed");
}
void resourcesAndAccounting() {
    Fixture f;f.apply(150);CHECK(f.d.hp==0 && f.d.hpBefore==10 && f.d.hpAfter==0);
    CHECK(f.a.dealtHp==99 && f.d.requestedHp==99 && f.d.lastHp==99 && f.hitCode==23);
    CHECK(f.d.field200==0 && f.d.damageFlags==0);
    Fixture suppressed;suppressed.d.attributes=0x40000000;suppressed.d.hpBefore=42;
    suppressed.d.field200=7;suppressed.d.field202=11;suppressed.apply(4,2,0x100|23);
    CHECK(suppressed.d.hp==10 && suppressed.d.fp==3 && suppressed.a.dealtHp==4);
    CHECK(suppressed.d.requestedHp==4 && suppressed.d.hpBefore==42 && suppressed.d.field200==7);
    CHECK(suppressed.d.damageFlags==0 && !suppressed.effects.empty());
    Fixture part;part.part.attributes=0x800000;part.apply(2);CHECK(part.d.hp==10 && part.d.requestedHp==2);
    Fixture healing;healing.d.hp=3;healing.apply(-150,0,0x2000|23);
    CHECK(healing.d.hp==10 && healing.a.dealtHp==-99 && healing.d.requestedHp==-99);
    CHECK(healing.effects.size()==1 && healing.effects[0].integers[1]==99 && healing.sounds.empty());
    Fixture signedHp;signedHp.d.hp=signedHp.d.maxHp=32767;signedHp.apply(-1);
    CHECK(signedHp.d.hp==0 && signedHp.d.hpBefore==32767 && signedHp.d.hpAfter==0);
    Fixture fields;fields.d.field202=0xabcd;fields.d.field206=0x1234;fields.d.damageFlags=0xaaaa;
    fields.apply(1,0,0x100|23);CHECK(fields.d.field200==0xabcd && fields.d.field204==0x1234 && fields.d.damageFlags==0xaaab);
    Fixture fp;fp.apply(0,128);CHECK(fp.d.fp==0 && fp.d.requestedFp==-128 && fp.d.consumedFp==5);
    Fixture fp256;fp256.apply(0,256);CHECK(fp256.d.fp==5 && fp256.d.requestedFp==0 && fp256.d.consumedFp==0);
    Fixture negative;negative.apply(0,-4);CHECK(negative.d.fp==9 && negative.d.requestedFp==-4 && negative.d.consumedFp==-4);
    Fixture pool;pool.units[5]=&pool.mario;pool.partner.fp=99;
    CHECK(getUnitFp(pool.partner,pool.services())==3 && getUnitMaxFp(pool.partner,pool.services())==5);
    setUnitFp(pool.partner,65535,pool.services());CHECK(pool.mario.fp==-1 && pool.partner.fp==99);
    recoverUnitFp(pool.partner,20,pool.services());CHECK(pool.mario.fp==5);
    recoverUnitFp(pool.partner,-10,pool.services());CHECK(pool.mario.fp==-5);
    pool.mario.fp=32767;recoverUnitFp(pool.partner,1,pool.services());CHECK(pool.mario.fp==-32768);
    pool.units[2]=&pool.partner;pool.units[3]=&pool.a;pool.a.kind=222;pool.a.belong=1;
    CHECK(findMarioResourceOwner(pool.services())==&pool.mario); // Belong matters, first qualifying slot wins.
}
void presentationOrder() {
    Fixture fire;fire.d.belong=0;fire.apply(2,3,24);
    CHECK(fire.effects.size()==4 && fire.effects[0].kind==DamageEffectKind::Smoke);
    CHECK(fire.effects[1].kind==DamageEffectKind::Hit && fire.effects[1].integers[0]==1);
    CHECK(fire.effects[2].position.x==12 && fire.effects[2].position.y==35);
    CHECK(fire.effects[2].floats[1]==30 && fire.effects[3].floats[1]==60);
    CHECK((fire.sounds==std::vector<std::string>{"fire","damage","SFX_MARIO_DAMAGE_FP1"}));
    Fixture code25;code25.apply(2,0,25);CHECK(code25.effects[0].kind==DamageEffectKind::Smoke);
    CHECK((code25.sounds==std::vector<std::string>{"damage"}));
    Fixture ice;ice.apply(1,0,26);CHECK(ice.effects[0].kind==DamageEffectKind::Ice && ice.effects[1].integers[0]==3);
    Fixture override;override.d.belong=0;override.apply(1,1,0x01000000|0x02000000|0x40000|23);
    CHECK((override.sounds==std::vector<std::string>{"SFX_MARIO_HAMMER_PIKKYO_R1"}));
    Fixture normalized;normalized.d.attributes=0x200000;normalized.apply(0,0,23,21);
    CHECK(normalized.d.pattern==0 && normalized.effects.empty()); // Original pattern suppresses miss.
    Fixture miss;miss.apply(0,0,0x20000|23);CHECK(miss.effects.size()==1);
    CHECK(miss.effects[0].kind==DamageEffectKind::MissStar && miss.effects[0].integers[0]==1 && miss.effects[0].integers[2]==1);
    Fixture starSuppressed;starSuppressed.d.attributes=0x400000;starSuppressed.apply(2,3);
    CHECK(starSuppressed.effects.size()==2 && starSuppressed.effects[1].integers[0]==1);
    for(int pattern:{19,27}) {Fixture silent;silent.apply(1,1,24,pattern);CHECK(silent.sounds.empty());}
}
void randomAndWake() {
    Fixture pity;pity.random.state=seedFor(29,29,30,30);const uint32_t seed=pity.random.state;
    pity.d.badges.pityFlower=2;pity.d.recoveryHeightFactor=.5f;pity.apply(1,2);
    BattleRandom expected(seed);expected.bounded(100);expected.bounded(100);
    CHECK(pity.random.state==expected.state && pity.d.fp==3); // Recovery at cap discarded before incoming2.
    CHECK(pity.effects.back().kind==DamageEffectKind::Recovery && pity.effects.back().integers[1]==1);
    CHECK(pity.effects.back().position.y==30.5f);
    for(bool lethal:{false,true}) {
        Fixture wake;wake.random.state=seedFor(49,49);wake.d.hp=lethal?1:10;
        setStatus(wake.d,1,3,0);wake.apply(1,0,0x100|23,0,1);
        CHECK(wake.d.statusByte(0x118)==(lethal?1:0) && wake.d.statusByte(0x119)==100);
        CHECK(wake.effects.back().kind==DamageEffectKind::Fukidashi && wake.effects.back().position.y==25);
    }
    Fixture noWake;noWake.random.state=seedFor(50,50);setStatus(noWake.d,1,3,0);noWake.apply(1,0,23,0,1);
    CHECK(noWake.d.statusByte(0x118)==3);
    Fixture instant;instant.random.state=seedFor(49,49);setStatus(instant.d,1,3,0);setStatus(instant.d,27,1,0);
    instant.apply(1,0,23,0,1);CHECK(instant.d.statusByte(0x118)==1 && instant.d.statusByte(0x119)==100);
    CHECK(instant.effects.back().kind==DamageEffectKind::Fukidashi);
    Fixture both;both.random.state=seedFor(29,29,49,49);const auto start=both.random.state;
    both.d.badges.pityFlower=1;setStatus(both.d,1,3,0);both.apply(1,1,23,0,1);
    BattleRandom two(start);two.bounded(100);two.bounded(100);
    CHECK(both.random.state==two.state && both.d.statusByte(0x118)==0);
    Fixture suppressed;suppressed.d.attributes=0x40000000;suppressed.d.badges.pityFlower=1;
    suppressed.random.state=seedFor(29,29);const auto before=suppressed.random.state;suppressed.apply(1);
    CHECK(suppressed.random.state!=before && suppressed.d.hp==10);
    Fixture absorption;absorption.d.badges.pityFlower=2;const auto initial=absorption.random.state;
    absorption.apply(1,0,0x2000|23);CHECK(absorption.random.state==initial && absorption.d.hp==9);
    CHECK(absorption.effects.size()==1 && absorption.effects[0].kind==DamageEffectKind::Recovery);
}
void lethalAndEvents() {
    for(int kind:{1,215,216,222,223,224,230,231}) {
        Fixture f;f.d.kind=kind;f.d.hp=1;f.d.enemyAudienceByte=17;f.apply(2,0,0x100|23,25);
        CHECK((f.hitCode&0x600)==0x600 && f.d.pattern==0);
        bool kill=false,party=false;
        for(auto notice:f.notices) {kill|=notice.kind==DamageNoticeKind::KillEnemy;party|=notice.kind==DamageNoticeKind::PartyDown;}
        CHECK(kill==(kind>=1 && kind<=215) && party==(kind>=224 && kind<=230));
        CHECK(f.inventoryCalls==((kind==222 || (kind>=224 && kind<=230))?1:0));
    }
    Fixture revival;revival.d.kind=222;revival.d.hp=1;revival.inventoryCount=1;
    revival.apply(2,0,0x100|23,25);CHECK((revival.hitCode&0xe00)==0xe00 && revival.d.hp==0);
    CHECK(revival.inventoryCount==1 && revival.d.pattern==0); // Marker does not consume/heal.
    Fixture special;special.d.hp=1;special.d.attributes=0x10000;special.d.pattern25Gate=1;
    special.apply(2,0,0x100|24,25);CHECK((special.hitCode&0x600)==0x600 && special.d.pattern==25 && special.inventoryCalls==0);
    Fixture nondeath;nondeath.d.hp=1;nondeath.d.attributes=0x10000;nondeath.apply(2,0,0x100|23,25);
    CHECK((nondeath.hitCode&0x400)==0 && nondeath.d.pattern==0);
    Fixture suppressed; suppressed.d.kind=222;suppressed.d.attributes=0x40000000;suppressed.apply(3);
    CHECK(suppressed.d.hp==10 && suppressed.notices.size()==3);
    CHECK(suppressed.notices[0].kind==DamageNoticeKind::ActionCount && suppressed.notices[1].value==3);
    CHECK(suppressed.notices[0].fieldOffset==0x16f3e && suppressed.notices[1].fieldOffset==0x16f3c);
    CHECK(suppressed.notices[2].kind==DamageNoticeKind::MarioBigDamage && suppressed.notices[2].value==3);
}
int main() {
    try {resourcesAndAccounting();presentationOrder();randomAndWake();lethalAndEvents();std::puts("Damage application contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}
