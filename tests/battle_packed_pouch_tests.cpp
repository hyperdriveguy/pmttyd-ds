#include "battle_packed_pouch.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};unsigned allocations=0,queries=0;
 PackedPouchInitializationServices services(){PackedPouchInitializationServices s;s.memory=nativeBattleActorServices(vm).memory;s.user=this;
 s.allocate=[](void* self,int32_t heap,uint32_t size){auto& f=*static_cast<Fixture*>(self);assert(heap==0 && size==0x5d4);++f.allocations;assert(f.vm.memory.registerRam(0x1000,std::vector<uint8_t>(size,0xcd)));return uint32_t(0x1000);};
 s.message=[](void* self,const char* key){++static_cast<Fixture*>(self)->queries;assert(!std::strcmp(key,"name_party3"));return "fixture name";};return s;}
 uint16_t h(unsigned off){uint16_t v;assert(vm.memory.readHalf(0x1000+off,v));return v;}
};
int main(){
 Fixture f;assert(f.vm.memory.registerRam(0x8041eb00,std::vector<uint8_t>(4)));assert(initializePackedPouch(f.services())==0x1000 && f.allocations==1 && f.queries==1);
 uint32_t pointer=0;assert(f.vm.memory.readWord(0x8041eb00,pointer) && pointer==0x1000);
 assert(f.h(0x70)==10 && f.h(0x72)==10 && f.h(0x8e)==10 && f.h(0x74)==5 && f.h(0x76)==5 && f.h(0x90)==5 && f.h(0x92)==3 && f.h(0x94)==3 && f.h(0x8a)==1);
 assert(f.h(0)==0 && f.h(2)==0 && f.h(4)==0 && f.h(6)==0);
 for(unsigned partner=1;partner<8;++partner){assert(f.h(partner*14)==0 && f.h(partner*14+2)==10 && f.h(partner*14+4)==10 && f.h(partner*14+6)==10 && f.h(partner*14+10)==0);}
 const auto memory=nativeBattleActorServices(f.vm).memory;assert(packedPouchEquippedBadgeCount(0x1000,327,memory)==0 && packedPouchEquippedBadgeCount(0x1000,328,memory)==0 && packedPouchEquippedBadgeCount(0x1000,0,memory)==200);
 assert(f.vm.memory.writeHalf(0x138a,327) && f.vm.memory.writeHalf(0x1518,327));assert(packedPouchEquippedBadgeCount(0x1000,327,memory)==2);assert(f.vm.memory.writeHalf(0x138c,0xffff));assert(packedPouchEquippedBadgeCount(0x1000,-1,memory)==1 && packedPouchEquippedBadgeCount(0x1000,65535,memory)==0);
 for(unsigned i=0;i<200;++i)assert(f.h(0x1fa+2*i)==0);
 std::cout<<"Source fresh pouch allocation, resource/partner state and full signed equipment scan passed\n";
}
