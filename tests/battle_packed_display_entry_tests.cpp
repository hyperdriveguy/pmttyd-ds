#include "battle_packed_display_entry.hpp"
#include "battle_packed_pose_lookup.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(){EventNativeRuntime vm{0x81000000,{}};assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x5000)));auto w=[&](uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));};auto read=[&](uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;};auto m=nativeBattleActorServices(vm).memory;w(0x2000,69);w(0x2004,0x3333);w(0x2008,43);w(0x200c,0x4444);w(0x2010,70);w(0x2014,0);w(0x11bc,0x2000);assert(packedBattlePoseName(0x1000,43,m)==0x3333 && packedBattlePoseName(0x1000,70,m)==0);w(0x2000,28);assert(packedBattlePoseName(0x1000,-1,m)==0x3333 && searchPackedBattlePoseTable(0x2000,43,m)==0x3333);w(0x2000,43);assert(searchPackedBattlePoseTable(0x2000,43,m)==0x3333);for(unsigned i=0;i<29;++i){w(0x2000+8*i,99);w(0x2004+8*i,0);}w(0x20e0,43);w(0x20e4,0x4444);assert(searchPackedBattlePoseTable(0x2000,43,m)==0 && searchPackedBattlePoseTable(0,28,m)==0);
 w(0x1014,0x3000);w(0x3000,0x3500);w(0x3500,0x4000);w(0x4000,0);w(0x31bc,0);w(0x3004,0x2500);w(0x36bc,0x2000);w(0x3504,0x2600);w(0x2608,0);w(0x41bc,0x2100);w(0x4004,0x2700);w(0x2708,0x2990);w(0x2100,28);w(0x2104,0x2900);for(unsigned i=0;i<4;++i)assert(vm.memory.writeByte(0x2900+i,uint8_t("S_1"[i])));int entries=0;PackedBattleDisplayEntryServices services;services.memory=nativeBattleActorServices(vm);services.user=&entries;services.poseEntry=[](void* p,uint32_t bank,int32_t context)->int32_t{assert(bank==0x2990 && context==1);return ++*static_cast<int*>(p);};initializePackedBattleDisplay(0x1000,services);assert(entries==1 && read(0x31c0)==0xffffffff && read(0x36c0)==0xffffffff && read(0x41c0)==1);uint8_t c;for(unsigned i=0;i<4;++i)assert(vm.memory.readByte(0x41c4+i,c) && c==uint8_t("S_1"[i]));
 std::cout<<"Original ordered pose fallback/search bound and display table/bank/pose allocation branches passed\n";
}
