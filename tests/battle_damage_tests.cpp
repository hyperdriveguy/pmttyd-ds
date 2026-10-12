#include "battle_damage.hpp"
#include <array>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)
struct Status {bool present=false;int8_t strength=0;};
struct Fixture {
    DamageUnit a,d;DamagePart p;DamageWeapon w;DamageBattleState b;
    std::array<int8_t,256> defense{},attributes{};
    std::array<Status,28> aStatus{},dStatus{};
    std::vector<int> reads;
    int32_t hpPower=7,fpPower=3;int hpCalls=0,fpCalls=0;
    void (*mutation)(Fixture&)=nullptr;
    Fixture() {
        a.hp=d.hp=10;a.danger=d.danger=4;a.peril=d.peril=2;
        p.defense=defense.data();p.defenseAttributes=attributes.data();
        w.hpPower=hp;w.fpPower=fp;w.callbackData=this;
    }
    static int32_t hp(DamageUnit* a,DamageWeapon& w,DamageUnit& d,DamagePart& p) {
        auto& f=*static_cast<Fixture*>(w.callbackData);
        CHECK(a==nullptr || a==&f.a);CHECK(&d==&f.d && &p==&f.p);
        ++f.hpCalls;if(f.mutation)f.mutation(f);return f.hpPower;
    }
    static int32_t fp(DamageUnit*,DamageWeapon& w,DamageUnit&,DamagePart&) {
        auto& f=*static_cast<Fixture*>(w.callbackData);++f.fpCalls;return f.fpPower;
    }
    static bool check(void* user,const DamageUnit& unit,int id) {
        auto& f=*static_cast<Fixture*>(user);const bool attacker=&unit==&f.a;
        f.reads.push_back(attacker?id:-id);
        return (attacker?f.aStatus:f.dStatus)[size_t(id)].present;
    }
    static int8_t strength(void* user,const DamageUnit& unit,int id) {
        auto& f=*static_cast<Fixture*>(user);
        return (&unit==&f.a?f.aStatus:f.dStatus)[size_t(id)].strength;
    }
    DamageStatusServices services() {return {this,check,strength};}
    int32_t hp(uint32_t& code,uint32_t flags=0) {return calculateHpDamage(&a,d,p,w,code,flags,b,services());}
    int32_t fp(uint32_t& code,uint32_t flags=0) {return calculateFpDamage(&a,d,p,w,code,flags,b);}
};
void callbacksAndGates() {
    Fixture f;uint32_t code=0xffffffff;
    CHECK(f.hp(code)==7 && code==23 && f.a.hp==10 && f.d.hp==10);
    f.w.hpPower=nullptr;f.d.flags=2;CHECK(f.hp(code,0x40)==0 && code==(0x1040|30));
    CHECK(f.hpCalls==1);f.w.hpPower=Fixture::hp;f.p.attributes=0x20000000;
    CHECK(f.hp(code)==0 && code==(0x1000|30) && f.hpCalls==1);
    f.p.attributes=0;f.w.specialFlags=1;f.a.badges.allOrNothing=2;
    CHECK(f.hp(code)==0 && f.hpCalls==1);
    CHECK(f.hp(code,0x20000)==9 && f.hpCalls==2); // HP bypass.
    CHECK(f.fp(code,0x20000)==0 && f.fpCalls==0); // FP has no bypass.
    f.b.actionResult=2;CHECK(f.fp(code)==3 && f.fpCalls==1);
    f.a.badges.allOrNothing=0;f.d.flags=0;f.b.actionResult=0;
    f.mutation=[](Fixture& f) {f.a.badges.allOrNothing=1;f.w.specialFlags=5;};
    CHECK(f.hp(code)==0 && code==30 && !(f.a.flags&8)); // Second gate skips charge modifier.
    f.a.badges.allOrNothing=0;f.defense[0]=-2;
    CHECK(f.hp(code)==2 && code==23 && !(f.a.flags&8)); // Second gate still executes defense.
    f.defense[0]=0;
    f.a.badges.allOrNothing=0;f.w.specialFlags=0;
    f.mutation=[](Fixture& f) {f.w.specialFlags=4;f.w.element=1;f.d.badges.defendPlus=2;};
    CHECK(f.hp(code)==5 && code==23 && (f.a.flags&8)); // Reload flags/defender; cache element.
    f.mutation=nullptr;f.w.specialFlags=0;
    CHECK(calculateHpDamage(nullptr,f.d,f.p,f.w,code,0,f.b,f.services())==5);
}
void attackerModifiers() {
    Fixture f;uint32_t code;
    f.w.specialFlags=1;f.b.actionResult=2;f.a.hp=2;f.p.attributes=0x100;
    auto& badges=f.a.badges;badges.powerPlus=2;badges.pUpDDown=1;badges.allOrNothing=2;
    badges.megaRush=1;badges.powerRush=2;badges.icePower=3;
    badges.pDownDUp=2;badges.hpDrain=1;badges.fpDrain=2;
    CHECK(f.hp(code)==19);
    f.a.hp=3;CHECK(f.hp(code)==14);f.a.hp=4;CHECK(f.hp(code)==14);
    f.a.hp=5;CHECK(f.hp(code)==10);f.p.attributes=0;CHECK(f.hp(code)==7);
    f.w.specialFlags=0;CHECK(f.hp(code)==7); // Badge gates off.
    Fixture clamp;clamp.hpPower=1;clamp.w.specialFlags=6;
    clamp.aStatus[16]={true,-3};clamp.aStatus[10]={true,2};clamp.aStatus[13]={true,-1};
    CHECK(clamp.hp(code)==1 && (clamp.a.flags&8)); // 1-3 ->0,+2,-1 ->1.
    CHECK((clamp.reads==std::vector<int>{16,10,12,11,13,-14,-15}));
    Fixture mode;mode.b.mode=1;mode.w.specialFlags=2;CHECK(mode.hp(code)==10);
    mode.w.specialFlags=0;CHECK(mode.hp(code)==7);
    Fixture burn;burn.w.specialFlags=0x400;burn.aStatus[8].present=true;
    burn.defense[1]=2;CHECK(burn.hp(code)==5 && code==24);
    burn.aStatus[8].present=false;CHECK(burn.hp(code)==7 && code==23);
}
void defensesAndAttributes() {
    Fixture f;uint32_t code;
    f.defense[0]=-5;CHECK(f.hp(code)==12); // Negative defense is retained.
    f.dStatus[14]={true,3};CHECK(f.hp(code)==9); // Up has no immediate clamp.
    f.dStatus[14]={};f.dStatus[15]={true,-1};CHECK(f.hp(code)==7); // Down does clamp.
    f.dStatus[15]={};f.defense[0]=0;f.d.badges.defendPlus=3;
    CHECK(f.hp(code)==4);f.d.badges.damageDodge=2;CHECK(f.hp(code,0x40000)==1);
    f.d.statusFlags=0x01000000;CHECK(f.hp(code)==3);
    f.d.statusFlags=0;f.d.badges={};f.b.mode=2;f.d.kind=222;CHECK(f.hp(code)==4);
    f.d.kind=223;CHECK(f.hp(code)==7);
    f.b.mode=0;f.p.attributes=0x80;f.d.extraDefense=-3;CHECK(f.hp(code)==7);
    f.d.extraDefense=2;CHECK(f.hp(code)==5);
    f.p.attributes=0;f.defense[0]=3;f.w.specialFlags=0x40;CHECK(f.hp(code)==7);
    f.defense[0]=-3;CHECK(f.hp(code)==10);
    for(int element=0;element<6;++element) {
        Fixture e;e.w.element=uint8_t(element);constexpr uint32_t codes[]={23,24,26,25,27,23};
        CHECK(e.hp(code)==7 && code==codes[element]);
    }
    Fixture attribute;attribute.defense[0]=99;attribute.attributes[0]=3;
    attribute.d.badges.defendPlus=200;attribute.dStatus[14]={true,100};
    CHECK(attribute.hp(code)==-7 && code==(23|0x2000) && attribute.reads.empty());
    CHECK(attribute.fp(code)==0 && attribute.fpCalls==0);
    attribute.hpPower=0;CHECK(attribute.hp(code)==0 && code==(23|0x2000));
    attribute.attributes[0]=2;CHECK(attribute.hp(code)==0 && code==30);
    attribute.defense[0]=0;attribute.d.badges={};attribute.dStatus[14]={};
    attribute.hpPower=7;attribute.attributes[0]=4;
    CHECK(attribute.hp(code)==0 && code==30);
    CHECK(attribute.hp(code,0x80000000)==7 && code==(0x80000000|23));
    attribute.attributes[0]=1;attribute.hpPower=1;
    CHECK(attribute.hp(code,0x40000)==1 && code==(0x40000|23)); // +1 after guard subtraction.
    attribute.hpPower=0;CHECK(attribute.hp(code)==1 && code==23);
}
void damageTailAndFp() {
    Fixture f;uint32_t code;
    f.d.badges.doublePain=2;CHECK(f.hp(code)==21); // Linear factor3, not4.
    f.d.badges.doublePain=0;f.d.badges.lastStand=2;f.d.hp=4;
    CHECK(f.hp(code)==3);f.d.hp=5;CHECK(f.hp(code)==7);
    f.d.hp=4;f.hpPower=0;f.d.badges.pDownDUp=3;
    CHECK(f.hp(code)==0 && code==30); // Signed (-3+2)/3 truncates to0, then clamps.
    Fixture counters;counters.w.specialFlags=0x30;counters.d.damageSequence=2;counters.a.attackSequence=3;
    CHECK(counters.hp(code)==2);counters.d.damageSequence=100;CHECK(counters.hp(code)==1);
    CHECK(counters.hp(code,0x40000)==0); // No general minimum1 after guard.
    Fixture element;element.w.element=1;element.d.badges.icePower=2;element.d.badges.pDownDUp=1;
    CHECK(element.hp(code)==4);
    Fixture vulnerability;vulnerability.hpPower=0;vulnerability.d.badges.pUpDDown=1;
    CHECK(vulnerability.hp(code)==1 && code==23); // Defender vulnerability after power's clamp.
    Fixture uncapped;uncapped.hpPower=150;CHECK(uncapped.hp(code)==150); // Calculator has no99 cap.
    Fixture flags;CHECK(flags.hp(code,0x100)==7 && code==(0x100|23|0x10000));
    flags.d.attributes=0x100;CHECK(flags.hp(code,0x100)==7 && code==(0x100|23));
    flags.w.specialFlags=0x8000;CHECK(flags.hp(code,0x100)==7 && code==(0x100|23|0x10000));
    flags.w.specialFlags=0;CHECK(flags.hp(code,8)==7 && code==(8|23)); // OR, not low-byte replacement.
    Fixture fp;code=0xabcd001e;CHECK(fp.fp(code,0x40000)==2 && code==0xabcd0017);
    code=0xabcd0019;CHECK(fp.fp(code)==3 && code==0xabcd0019);
    fp.fpPower=1;code=30;CHECK(fp.fp(code,0x40000)==0 && code==30);
    fp.w.fpPower=nullptr;CHECK(fp.fp(code)==0);fp.w.fpPower=Fixture::fp;
    fp.p.attributes=0x20000000;CHECK(fp.fp(code)==0);
    Fixture wrap;wrap.hpPower=std::numeric_limits<int32_t>::max();wrap.w.specialFlags=1;
    wrap.a.badges.powerPlus=1;CHECK(wrap.hp(code)==0 && code==30);
    wrap.w.specialFlags=0;wrap.a.badges={};wrap.d.badges.doublePain=1;
    CHECK(wrap.hp(code)==0); // Signed word multiplication wraps before clamp.
    wrap.d.badges={};wrap.defense[0]=-1;
    CHECK(wrap.hp(code)==0); // INT_MAX - (-1) wraps before clamp.
}
void revivalMarker() {
    struct Inventory {
        int32_t count=0;int calls=0;
        static int32_t query(void* user,int32_t item) {
            CHECK(item==151);auto& inventory=*static_cast<Inventory*>(user);
            ++inventory.calls;return inventory.count;
        }
    } inventory;
    const RevivalInventoryServices services{&inventory,Inventory::query};
    DamageUnit unit;unit.heldItem=151;unit.hp=-1;
    for(int kind:{221,222,223,224,230,231}) {
        unit.kind=kind;const bool pouch=kind==222 || (kind>=224 && kind<=230);
        inventory.count=0;inventory.calls=0;uint32_t code=30;
        CHECK(markEmergencyRevival(unit,code,services)==(pouch?0:1));
        CHECK(code==(pouch?30u:30u|0x800));CHECK(inventory.calls==(pouch?1:0));
        inventory.count=1;inventory.calls=0;code=0x100;
        CHECK(markEmergencyRevival(unit,code,services)==1 && code==0x900);
        CHECK(inventory.count==1 && inventory.calls==(pouch?1:0) && unit.hp==-1);
    }
    unit.kind=222;inventory.count=-1;uint32_t code=0x800;
    CHECK(markEmergencyRevival(unit,code,services)==0 && code==0x800); // No held-item fallback.
    unit.kind=223;unit.heldItem=0;inventory.count=1;
    CHECK(markEmergencyRevival(unit,code,services)==0 && code==0x800); // No pouch on nonplayer path.
}
void storedStatusPipeline() {
    Fixture f;uint32_t code;
    const auto readers=storedDamageStatusServices();
    auto damage=[&] {return calculateHpDamage(&f.a,f.d,f.p,f.w,code,0,f.b,readers);};
    f.w.specialFlags=6;
    CHECK(setStatus(f.a,16,0,4)==1);CHECK(setStatus(f.a,10,2,2)==1);
    CHECK(setStatus(f.a,13,2,-1)==1);CHECK(setStatus(f.d,14,2,3)==1);
    CHECK(damage()==9 && (f.a.flags&8)); // 7+4+2-1,then defense3.
    CHECK(checkRecoveryStatus(&f.a,10)==0 && damage()==9);
    CHECK(checkRecoveryStatus(&f.a,10)==1 && damage()==7);
    CHECK(setStatus(f.d,15,2,-2)==0 && damage()==10); // Opposite defense cancels.
    CHECK(setStatus(f.d,15,2,-2)==1 && damage()==10); // Down clamps base defense0.
    CHECK(readers.strength(readers.user,f.d,15)==-2);
    f.a.zapTap=1;CHECK(readers.check(readers.user,f.a,6));
    CHECK(readers.strength(readers.user,f.a,6)==0);
    CHECK(setStatus(f.d,15,0,0)==0); // End the defense-down phase before checking Burn's element.
    f.w.specialFlags=0x400;CHECK(setStatus(f.a,8,2,0)==1);
    f.defense[1]=2;CHECK(damage()==5 && code==24);
    CHECK(setStatus(f.a,9,2,0)==0 && damage()==7 && code==23);
}
int main() {
    try {callbacksAndGates();attackerModifiers();defensesAndAttributes();damageTailAndFp();revivalMarker();storedStatusPipeline();std::puts("Damage calculator contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}
