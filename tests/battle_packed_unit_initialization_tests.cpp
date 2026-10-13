#include "battle_packed_unit_initialization.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main(){EventNativeRuntime vm{0x81000000,{}};assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0xb34,0xab)));const auto m=nativeBattleActorServices(vm);
 assert(initializePackedBattleUnitHelper(0x8019673c,0x1000,m));uint8_t byte;assert(vm.memory.readByte(0x1000,byte) && byte==0xab);assert(!initializePackedBattleUnitHelper(0x801011fc,0x1000,m));
 assert(initializePackedBattleUnitHelper(0x80253e84,0x1000,m));assert(vm.memory.readByte(0x1347,byte) && byte==0xab);assert(vm.memory.readByte(0x1348,byte) && byte==0);assert(vm.memory.readByte(0x1ae7,byte) && byte==0);assert(vm.memory.readByte(0x1ae8,byte) && byte==0xab);
 assert(initializePackedBattleUnitHelper(0x80127b58,0x1000,m));for(unsigned i=0;i<30;++i)assert(vm.memory.readByte(0x1118+i,byte) && byte==0);for(unsigned i=0;i<6;++i){const auto r=0x1ae8+12*i;assert(vm.memory.readByte(r,byte) && byte==255);assert(vm.memory.readByte(r+8,byte) && byte==0xab);uint32_t word;assert(vm.memory.readWord(r+4,word) && word==0);}assert(vm.memory.readByte(0x1b30,byte) && byte==0xab);
 assert(vm.memory.writeHalf(0x110c,7) && vm.memory.writeHalf(0x1108,10));assert(initializePackedBattleUnitHelper(0x80125fd8,0x1000,m));uint32_t word;assert(vm.memory.readWord(0x1208,word) && word==7);assert(vm.memory.readWord(0x1210,word));float ratio;std::memcpy(&ratio,&word,4);assert(ratio==.7f);uint16_t half;assert(vm.memory.readHalf(0x1202,half) && half==20);assert(vm.memory.readHalf(0x1206,half) && half==50);
 assert(vm.memory.writeHalf(0x110c,0) && vm.memory.writeHalf(0x1108,0));assert(initializePackedBattleUnitHelper(0x80125fd8,0x1000,m));assert(vm.memory.readWord(0x1210,word));std::memcpy(&ratio,&word,4);assert(std::isnan(ratio));
 std::cout<<"Original unit helper no-op/icon extents, status gap preservation and HP gauge ratio/zero-domain passed\n";
}
