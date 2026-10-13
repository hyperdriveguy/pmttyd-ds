#include "prologue_pouch.hpp"
#include "battle_unit_resources.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main(){
    auto pouch=initializeProloguePouch("fixture name");
    assert(pouch.half(0x70)==10 && pouch.half(0x72)==10 && pouch.half(0x74)==5 && pouch.half(0x76)==5);
    assert(pouch.half(0x8e)==10 && pouch.half(0x90)==5 && pouch.half(0x92)==3 && pouch.half(0x94)==3 && pouch.half(0x8a)==1);
    assert(pouch.half(0x78)==100 && pouch.bytes[0x98]==1 && pouch.bytes[0x99]==1);
    assert(pouch.half(0xa0)==9 && pouch.half(0xa2)==0x32 && pouch.half(0xa4)==6);
    for(unsigned i=3;i<121;++i)assert(pouch.half(0xa0+2*i)==0);
    assert(std::strcmp(reinterpret_cast<const char*>(pouch.bytes.data()+0x5c0),"fixture name")==0);
    assert(pouch.half(6)==0);for(unsigned i=1;i<=7;++i)assert(pouch.half(14*i+6)==10);
    // Damage/FP spending supplied by the field remains current on entry.
    pouch.half(0x70,7);pouch.half(0x74,2);pouch.half(14+6,6);
    std::array<PartnerBattleSnapshot,7> snapshots{};snapshots[0].value=1;
    UnitResourceServices services;services.user=&pouch;services.snapshots=&snapshots;services.pouch=[](void* p){return static_cast<ProloguePouch*>(p)->bytes.data();};
    DamageUnit mario;mario.kind=222;importUnitResources(mario,services);assert(mario.hp==7 && mario.maxHp==10 && mario.fp==2 && mario.maxFp==5);
    DamageUnit goombella;goombella.kind=224;goombella.fp=5;goombella.baseFp=5;goombella.maxFp=5;importUnitResources(goombella,services);assert(goombella.hp==6 && goombella.maxHp==10 && goombella.fp==5 && goombella.maxFp==5 && goombella.snapshotValue==1);
    assert(pouch.half(0x70)==7 && pouch.half(0x74)==2 && pouch.half(14+6)==6); // Import does not export or heal.
    std::cout<<"Original initial-stage pouch fields/grant order and retained live entry HP/FP passed\n";
}
