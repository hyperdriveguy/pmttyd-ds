#include "battle_packed_unit_vectors.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_jump_motion.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main(){EventNativeRuntime vm{0x81000000,{}};assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x500)));assert(vm.memory.registerRam(0x804226a4,std::vector<uint8_t>(0x28)));auto word=[&](uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));};auto store=[&](uint32_t a,float v){uint32_t bits;std::memcpy(&bits,&v,4);word(a,bits);};auto read=[&](uint32_t a){uint32_t bits;assert(vm.memory.readWord(a,bits));float v;std::memcpy(&v,&bits,4);return v;};store(0x804226c4,-250000000.f);store(0x804226c8,360);PackedUnitVectorServices services;services.memory=nativeBattleActorServices(vm).memory;services.reviseAngle=[](void*,float v){JumpMotionServices js;js.fmod=[](void*,double a,double b){return std::fmod(a,b);};return reviseJumpAngle(v,js);};
 const std::array<std::array<uint32_t,2>,13> mapping={{{0x80128c6c,0x3c},{0x801289a0,0x30},{0x801283d8,0x48},{0x80128680,0x60},{0x801288b4,0x6c},{0x80128604,0x78},{0x80128524,0x90},{0x80128574,0x9c},{0x801284a4,0x6c},{0x80128470,0x60},{0x80128654,0x3c},{0x801287b4,0x48},{0x801285d0,0x54}}};for(auto pair:mapping){assert(setPackedUnitVector(pair[0],0x1000,{10,20,30},services));assert(read(0x1000+pair[1])==10 && read(0x1004+pair[1])==20 && read(0x1008+pair[1])==30);}
 assert(setPackedUnitVector(0x80128524,0x1000,{-250000000.f,3,-250000000.f},services));assert(read(0x1090)==10 && read(0x1094)==3 && read(0x1098)==30);assert(setPackedUnitVector(0x80128680,0x1000,{-10,720,NAN},services));assert(read(0x1060)==350 && read(0x1064)==0 && read(0x1068)==0);services.reviseAngle=[](void*,float v){return v;};assert(setPackedUnitVector(0x801288b4,0x1000,{360,361,359},services));assert(read(0x106c)==0 && read(0x1070)==0 && read(0x1074)==359);assert(!setPackedUnitVector(0xdead,0x1000,{1,2,3},services));
 std::cout<<"Original thirteen unit/part vector targets, sentinel retention and rotate normalization/clamp passed\n";
}
