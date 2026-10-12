#include "battle_unit_resources.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
    std::array<uint8_t,0x92> first{},second{};
    std::array<PartnerBattleSnapshot,7> snapshots{};
    int calls=0;
    DamageUnit* mutate=nullptr;
    static uint8_t* get(void* user) {
        auto& f=*static_cast<Fixture*>(user);
        if(++f.calls==2) {
            if(f.mutate)f.mutate->kind=229;
            return f.second.data();
        }
        return f.first.data();
    }
    UnitResourceServices services() {return {this,get,&snapshots};}
};
void put(std::array<uint8_t,0x92>& p,unsigned offset,int value) {
    const auto bits=static_cast<uint16_t>(value);p[offset]=bits>>8;p[offset+1]=bits;
}
}
int main() {
    DamageUnit u;u.statusBytes.fill(0xff);u.moveBytes.fill(7);
    u.flags=0x1234;u.statusFlags=9;u.hp=3;u.zapTap=2;u.badges.powerPlus=4;
    for(auto& r:u.statusChangeInfo)r={1,2,3,4,5,0xabcdef12};
    clearUnitStatus(u);
    for(auto b:u.statusBytes)assert(b==0);
    for(auto& r:u.statusChangeInfo) {
        assert(r.type==255 && r.duration==0 && r.strength==0 && r.extra==0);
        assert(r.value==0 && r.retained==0xabcdef12);
    }
    assert(u.moveBytes[0]==7 && u.flags==0x1234 && u.statusFlags==9);
    assert(u.hp==3 && u.zapTap==2 && u.badges.powerPlus==4);
    for(auto k:{80,137,224})assert(battlePartyIndex(k)==1);
    for(auto k:{142,229})assert(battlePartyIndex(k)==3);
    assert(battlePartyIndex(228)==6 && battlePartyIndex(230)==7);
    assert(battlePartyIndex(223)==0 && battlePartyIndex(-1)==0);

    Fixture mario;u={};u.kind=222;u.hpPlusCount=2;u.fpPlusCount=1;
    put(mario.first,0x70,50);put(mario.first,0x74,-4);
    put(mario.first,0x8e,999); // Revision must request its own pouch pointer.
    put(mario.second,0x8e,10);put(mario.second,0x90,8);
    importUnitResources(u,mario.services());
    assert(mario.calls==2 && u.baseHp==10 && u.maxHp==20 && u.hp==20);
    assert(u.baseFp==8 && u.maxFp==13 && u.fp==-4);
    assert(mario.first[0x70]==0 && mario.first[0x71]==50);
    assert(!checkStatus(&u,27));
    Fixture wrap;put(wrap.first,0x8e,32767);put(wrap.first,0x90,32765);
    u.hp=10;u.fp=10;u.hpPlusCount=1;u.fpPlusCount=1;
    reviseUnitResources(u,wrap.services());
    assert(u.maxHp==-32764 && u.hp==-32764 && u.maxFp==-32766 && u.fp==-32766);

    Fixture partner;u={};u.kind=224;u.baseFp=7;u.maxFp=8;u.fp=9;
    u.hpPlusCount=1;u.fpPlusCount=255;partner.mutate=&u;
    put(partner.first,14+6,99);put(partner.second,42+4,10);
    put(partner.second,42+6,-2);
    auto& b=partner.snapshots[5]; // Reread kind229 indexes snapshot5, pouch3.
    b.value=1.25f;b.flags=0xabcdef01;b.statuses.fill(0xa5);
    b.field28=0x1234;b.field2a=0x5678;
    importUnitResources(u,partner.services());
    assert(partner.calls==2 && u.kind==229 && u.hp==-2 && u.maxHp==15 && u.baseHp==10);
    assert(u.baseFp==7 && u.maxFp==8 && u.fp==9);
    assert(u.flags==b.flags && u.snapshotValue==1.25f);
    assert(u.snapshot13c==0x1234 && u.snapshot13e==0x5678);
    for(unsigned i=0;i<29;++i)assert(u.statusBytes[i]==0xa5);
    assert(u.statusBytes[29]==1); // Dead overwrites only status27's duration.
    Fixture alive;u.kind=229;put(alive.first,42+6,10);
    put(alive.second,42+4,10);put(alive.second,42+6,10);
    alive.snapshots[5].statuses.fill(0xff);
    importUnitResources(u,alive.services());assert(u.statusBytes[29]==255);

    Fixture out;u.kind=229;u.hp=-7;u.fp=27;
    out.snapshots[5].retainedGap={0x88,0x99};out.first.fill(0xcc);
    exportUnitResources(u,out.services());
    assert(out.calls==1 && out.first[48]==0xff && out.first[49]==0xf9);
    assert(out.first[0x74]==0xcc && out.snapshots[5].statuses==u.statusBytes);
    assert(out.snapshots[5].retainedGap[0]==0x88 && out.snapshots[5].retainedGap[1]==0x99);
    // Restore direct bytes including negative durations without setter normalization.
    Fixture round;round.first=out.first;round.second=out.first;
    put(round.second,46,10);round.snapshots=out.snapshots;
    DamageUnit restored;restored.kind=229;
    importUnitResources(restored,round.services());
    for(unsigned i=0;i<29;++i)assert(restored.statusBytes[i]==u.statusBytes[i]);
    assert(restored.statusBytes[29]==1 && restored.flags==u.flags);
    assert(restored.hp==-7 && restored.snapshot13e==u.snapshot13e);
    Fixture marioOut;marioOut.first.fill(0xaa);u.kind=222;u.hp=-3;u.fp=12;
    exportUnitResources(u,marioOut.services());
    assert(marioOut.first[0x70]==255 && marioOut.first[0x71]==253);
    assert(marioOut.first[0x74]==0 && marioOut.first[0x75]==12);
    assert(marioOut.first[0x8e]==0xaa && marioOut.snapshots[0].flags==0);

    Fixture enemy;u={};u.kind=0;u.hp=30;u.fp=-10;u.baseHp=5;u.baseFp=2;
    u.hpPlusCount=2;u.fpPlusCount=3;u.statusBytes.fill(4);
    importUnitResources(u,enemy.services());
    assert(enemy.calls==1 && u.hp==30 && u.fp==-10 && u.maxHp==0 && u.statusBytes[0]==4);
    exportUnitResources(u,enemy.services());assert(enemy.calls==2 && enemy.second[0x70]==0);
    reviseUnitResources(u,enemy.services());
    assert(u.maxHp==15 && u.hp==15 && u.maxFp==17 && u.fp==-10);
    assert(u.baseHp==5 && u.baseFp==2);
    std::cout<<"unit resource tests passed\n";
}
