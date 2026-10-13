#include "battle_packed_unit_resources.hpp"
#include "battle_packed_pouch.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_unit_resources.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<int32_t> calls;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x20000)));assert(vm.memory.registerRam(0x8041eb00,std::vector<uint8_t>(4)));PackedPouchInitializationServices init;init.memory=nativeBattleActorServices(vm).memory;init.allocate=[](void*,int32_t heap,uint32_t bytes){assert(heap==0 && bytes==0x5d4);return uint32_t(0x20000);};init.message=[](void*,const char*){return "Yoshi";};assert(initializePackedPouch(init)==0x20000);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}int16_t h(uint32_t a){uint16_t v;assert(vm.memory.readHalf(a,v));return int16_t(v);}void byte(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}uint8_t byte(uint32_t a){uint8_t v;assert(vm.memory.readByte(a,v));return v;}
 PackedUnitResourceServices services(){PackedUnitResourceServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.pouch=[](void* p){static_cast<Fixture*>(p)->calls.push_back(-1);return uint32_t(0x20000);};s.partyIndex=[](void* p,int32_t kind)->int32_t{static_cast<Fixture*>(p)->calls.push_back(kind);return battlePartyIndex(kind);};return s;}
};
int main(){Fixture f;f.w(0x3008,222);f.h(0x20070,7);f.h(0x20074,3);f.byte(0x32f1,2);f.byte(0x32f2,1);importPackedUnitResources(0x1000,0x3000,f.services());assert(f.h(0x310a)==10 && f.h(0x3108)==20 && f.h(0x310c)==7 && f.h(0x3110)==5 && f.h(0x310e)==10 && f.h(0x3112)==3 && (f.calls==std::vector<int32_t>{-1,-1}));f.h(0x310c,30);f.h(0x3112,30);revisePackedUnitResources(0x3000,f.services());assert(f.h(0x310c)==20 && f.h(0x3112)==10);f.h(0x310c,0xffff);revisePackedUnitResources(0x3000,f.services());assert(f.h(0x310c)==-1);
 Fixture partner;partner.w(0x3008,229);partner.h(0x2002e,15);partner.h(0x20030,9);partner.byte(0x32f1,1);const auto snapshot=0x1000+0x162a0+5*0x2c;partner.w(snapshot,0x3f000000);partner.w(snapshot+0x24,0x1234);for(unsigned i=0;i<7;++i)partner.w(snapshot+4+4*i,0x01020304+i);partner.h(snapshot+0x20,0x0506);partner.h(snapshot+0x28,0x789a);partner.h(snapshot+0x2a,0xbcde);partner.byte(0x3136,0xaa);partner.byte(0x3137,0xbb);importPackedUnitResources(0x1000,0x3000,partner.services());assert(partner.h(0x310a)==15 && partner.h(0x3108)==20 && partner.h(0x310c)==9 && partner.w(0x3114)==0x3f000000 && partner.w(0x3138)==0x1234 && partner.w(0x3118)==0x01020304 && partner.h(0x3134)==0x0506 && partner.byte(0x3136)==0xaa && partner.byte(0x3137)==0xbb && uint16_t(partner.h(0x313c))==0x789a && uint16_t(partner.h(0x313e))==0xbcde);assert((partner.calls==std::vector<int32_t>{-1,229,-1,229}));partner.h(0x20030,0);importPackedUnitResources(0x1000,0x3000,partner.services());assert(partner.byte(0x3135)==1 && partner.byte(0x3134)==5);
 Fixture enemy;enemy.w(0x3008,221);enemy.h(0x310a,5);enemy.h(0x310c,5);enemy.h(0x3110,7);enemy.h(0x3112,7);enemy.byte(0x32f1,1);importPackedUnitResources(0x1000,0x3000,enemy.services());assert(enemy.h(0x3108)==0 && enemy.h(0x310c)==5 && enemy.calls==std::vector<int32_t>{-1});revisePackedUnitResources(0x3000,enemy.services());assert(enemy.h(0x3108)==10 && enemy.h(0x310e)==7 && enemy.h(0x310c)==5);
 std::cout<<"Original packed pouch import/revision, carried Mario HP, signed caps and partner snapshot ownership passed\n";
}
