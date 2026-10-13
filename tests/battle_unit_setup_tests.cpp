#include "battle_unit_setup.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<uint32_t> entries;std::vector<std::array<int32_t,3>> equipment;std::vector<uint32_t> pouchUnits;std::vector<std::array<float,3>> positions;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x30000)));assert(vm.memory.registerRam(0x80422178,std::vector<uint8_t>{0xc3,0x0c,0,0,0x40,0xa0,0,0}));w(0x3738,0x20000);w(0x2000c,0x21000);w(0x21018,0x22000);w(0x22000,2);w(0x22004,0x23000);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 BattleUnitSetupServices services(){BattleUnitSetupServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.storeByte=[](void* p,uint32_t a,uint8_t v){assert(static_cast<Fixture*>(p)->vm.memory.writeByte(a,v));};s.pouch=[](void*){return uint32_t(0x24000);};s.entry=[](void* p,uint32_t def){auto& f=*static_cast<Fixture*>(p);f.entries.push_back(def);const auto u=0x25000+uint32_t(f.entries.size())*0x400;assert(f.vm.memory.writeHalf(u+0x108,5) && f.vm.memory.writeHalf(u+0x10e,7));return u;};s.equip=[](void* p,uint32_t u,int32_t mode,int32_t item){static_cast<Fixture*>(p)->equipment.push_back({int32_t(u),mode,item});};s.pouchParameters=[](void* p,uint32_t u){static_cast<Fixture*>(p)->pouchUnits.push_back(u);};s.basePosition=[](void*,uint32_t){return std::array<float,3>{1,12,3};};s.setBasePosition=[](void* p,uint32_t,const std::array<float,3>& pos){static_cast<Fixture*>(p)->positions.push_back(pos);};s.setPosition=s.setBasePosition;return s;}
};
int main(){Fixture f;f.w(0x2101c,42);f.w(0x2103c,43);f.w(0x21060,44);f.w(0x21040,45);assert(f.vm.memory.writeHalf(0x2438a,308) && f.vm.memory.writeHalf(0x2438c,313) && f.vm.memory.writeHalf(0x2438e,314) && f.vm.memory.writeHalf(0x24390,319));assert(packedBattleUnitSetup(0x1000,f.services())==1);assert((f.entries==std::vector<uint32_t>{0x80416a80,0x80416a50,0x23000,0x23030}));assert((f.equipment==std::vector<std::array<int32_t,3>>{{0x25400,3,0},{0x25800,3,0},{0x25c00,1,42}}));assert(f.pouchUnits==std::vector<uint32_t>{0x25800});assert(f.w(0x25f08)==42 && f.w(0x25f0c)==43 && f.w(0x26308)==44 && f.w(0x2630c)==45 && (f.w(0x2601c)&0x40000000));uint16_t hp,fp;assert(f.vm.memory.readHalf(0x25d0c,hp) && f.vm.memory.readHalf(0x25d12,fp) && hp==5 && fp==7);assert(f.w(0x1420)==0xffffffff && f.w(0x173f4)==(1|128|4096));uint8_t group;assert(f.vm.memory.readByte(0x101a,group) && group==1);
 Fixture party;party.w(0x20004,225);party.w(0x22000,0);packedBattleUnitSetup(0x1000,party.services());assert((party.entries==std::vector<uint32_t>{0x80416a80,0x80416a50,0x803637c0}));assert(party.positions.size()==2 && (party.positions[0]==std::array<float,3>{-140,12,5}) && party.positions[1]==party.positions[0]);assert((party.pouchUnits==std::vector<uint32_t>{0x25800,0x25c00}));assert((party.equipment.back()==std::array<int32_t,3>{0x25c00,5,0}));
 std::cout<<"Source mandatory system/Mario/party/group entry order, item ownership, HP/FP reset and 200 equipped-badge scan passed\n";
}
