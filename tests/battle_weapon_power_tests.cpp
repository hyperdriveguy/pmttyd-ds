#include "battle_weapon_power.hpp"
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)
struct Fixture {
    uint32_t result=0;uint8_t level=1,bonus=0;int32_t index=99,prefixWord=-7;
    DamageUnit actor;DamageWeapon weapon=ordinaryJumpWeapon();std::vector<int> order;
    bool mutate=false;MarioPowerKind expected=MarioPowerKind::Jump;const DamageUnit* bonusActor=nullptr;
    static uint32_t resultRead(void* user) {auto& f=*static_cast<Fixture*>(user);f.order.push_back(1);return f.result;}
    static uint8_t levelRead(void* user,MarioPowerKind kind) {
        auto& f=*static_cast<Fixture*>(user);CHECK(kind==f.expected);f.order.push_back(2);
        if(f.mutate){f.result^=2;f.bonus=255;}return f.level;
    }
    static uint8_t bonusRead(void* user,const DamageUnit& actor,MarioPowerKind kind) {
        auto& f=*static_cast<Fixture*>(user);CHECK(kind==f.expected);f.bonusActor=&actor;f.order.push_back(3);return f.bonus;
    }
    static int32_t wordRead(void* user,int32_t index) {
        auto& f=*static_cast<Fixture*>(user);f.order.push_back(4);f.index=index;
        return index<0?f.prefixWord:f.weapon.powerWords[size_t(index)];
    }
    WeaponPowerServices services() {return {this,resultRead,levelRead,bonusRead,wordRead};}
    int32_t power() {auto value=marioWeaponPower(actor,expected,services());CHECK((order==std::vector<int>{1,2,3,4}));order.clear();return value;}
};
void tables() {
    for(auto kind:{MarioPowerKind::Jump,MarioPowerKind::Hammer})for(int level=1;level<=4;++level)
        for(uint32_t success:{0u,2u})for(int bonus:{0,255}) {
            Fixture f;f.expected=kind;f.weapon=kind==MarioPowerKind::Jump?ordinaryJumpWeapon():ordinaryHammerWeapon();
            f.level=uint8_t(level);f.result=success;f.bonus=uint8_t(bonus);
            const int index=level>=4?5:(level-1)*2+(success?1:0);
            constexpr int jump[]={1,1,2,2,3,3},hammer[]={1,2,2,4,3,6};
            CHECK(f.power()==(kind==MarioPowerKind::Jump?jump[index]:hammer[index])+bonus && f.index==index);
            CHECK(f.weapon.fpPower==nullptr && f.weapon.element==0);
        }
    Fixture f;f.result=0xffffffff;f.level=127;CHECK(f.power()==3 && f.index==5);
    f.level=0;f.result=0;CHECK(f.power()==-7 && f.index==-2);
    f.result=2;CHECK(f.power()==-7 && f.index==-1);
    f.level=255;CHECK(f.power()==-7 && f.index==-3); // Signed-1 level, not255/upper clamp.
    f.level=128;f.result=0;CHECK(f.power()==-7 && f.index==-258);
}
void orderAndAbi() {
    Fixture f;f.mutate=true;f.result=2;f.weapon=ordinaryHammerWeapon();f.expected=MarioPowerKind::Hammer;
    CHECK(f.power()==257 && f.index==1 && f.result==0); // Capture bit before pouch access; bonus afterwards.
    f.mutate=false;f.level=1;f.bonus=1;f.result=0;f.weapon.powerWords[0]=std::numeric_limits<int32_t>::max();
    CHECK(f.power()==std::numeric_limits<int32_t>::min());
    DamageUnit actor,defender;DamagePart part;DamageWeapon boss=harborBossWeapon();
    for(int32_t value:{0,-1,150}) {boss.defaultPower=value;CHECK(boss.hpPower(nullptr,boss,defender,part)==value);}
    CHECK(boss.specialFlags==0x1087 && boss.fpPower==nullptr);
    Fixture callback;auto services=callback.services();callback.weapon.callbackData=&services;
    CHECK(callback.weapon.hpPower(&actor,callback.weapon,defender,part)==1);
    CHECK(callback.bonusActor==&actor); // Callback bonus ownership follows its actual actor argument.
    CHECK((callback.order==std::vector<int>{1,2,3,4}));
    constexpr int8_t zeros[5]={};part.defense=zeros;part.defenseAttributes=zeros;
    actor.hp=defender.hp=10;DamageBattleState battle;uint32_t code;
    callback.order.clear();CHECK(calculateHpDamage(&actor,defender,part,callback.weapon,code,0,battle,storedDamageStatusServices())==1);
    CHECK(actor.flags&8);CHECK(calculateFpDamage(&actor,defender,part,callback.weapon,code,0,battle)==0 && code==23);
}
int main() {
    try {tables();orderAndAbi();std::puts("Weapon power contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}
