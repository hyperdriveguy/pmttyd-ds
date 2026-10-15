#include "field_npc_reset.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_map_npc_assets.hpp"
#include "event_native_runtime.hpp"
#include "game.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
namespace {
void orderedSource(){
 struct Probe {std::map<uint32_t,uint32_t> words;std::vector<uint32_t> writes;uint32_t array=0,size=0,half=0;bool forbidGp=false;};Probe p;
 auto setup=[&]{p={};p.words={{0x8041e8b8,2},{0x8041e8c0,0x1000},{0x8041e8cc,3},{0x8041e8d4,0x2000},{0x80415fd0,0x9000},{0xa168,0x3000},{0xb168,0x4000}};};setup();
 PackedNpcResetServices s;s.user=s.memory.user=&p;
 s.memory.word=[](void* raw,uint32_t a){auto& p=*static_cast<Probe*>(raw);assert(!p.forbidGp || a!=0x80415fd0);assert(p.words.count(a));return p.words[a];};
 s.memory.storeWord=[](void* raw,uint32_t a,uint32_t v){auto& p=*static_cast<Probe*>(raw);assert(v==0);p.words[a]=v;p.writes.push_back(a);if(a==0x8041e8b4)p.words[0x80415fd0]=0xb000;};
 s.memory.storeHalf=[](void* raw,uint32_t a,uint16_t v){auto& p=*static_cast<Probe*>(raw);assert(v==0);p.half=a;p.writes.push_back(a);};
 s.clear=[](void* raw,uint32_t a,uint32_t size){auto& p=*static_cast<Probe*>(raw);p.array=a;p.size=size;p.words[0x80415fd0]=0xa000;p.words[0x8041e8b8]=999;p.writes.push_back(0xc1ea);};
 resetPackedNpcRegistry(0,s);assert(p.array==0x1000 && p.size==2*0x340 && p.half==0x4000);assert((p.writes==std::vector<uint32_t>{0xc1ea,0x8041e8b4,0x4000,0x8041e8ac}));
 for(int selector:{1,-1,27}){setup();p.forbidGp=true;resetPackedNpcRegistry(selector,s);assert(p.array==0x2000 && p.size==3*0x340 && !p.half && (p.writes==std::vector<uint32_t>{0xc1ea,0x8041e8c8,0x8041e8ac}));}
 setup();p.words[0x8041e8cc]=0;resetPackedNpcRegistry(1,s);assert(p.size==0 && p.writes.front()==0xc1ea);
}
void retainedNative(){
 constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70;
 auto game=std::make_unique<Game>();auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));auto binding=game->initializeOriginalFieldNpcs(vm->memory,Base);assert(binding);
 auto store=[&](uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));};auto word=[&](uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;};
 for(const char* name:{"c_deale","c_monban"}){const auto base=std::string("romfs/field-native-")+name;assert(game->originalFieldNpcs->poses().load((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str()));}
 FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin") && tags.mapTo(vm->memory,Inputs));auto input=*tags.input(0,Inputs,Area);assert(binding->registerMapNpc(input,{123,234})==Base);
 const auto field=game->originalFieldNpcs->fieldWorkAddress(),aux=word(0x8041e8b0),alternate=word(0x8041e8d4);store(field,0x12345678);store(field+0x5c,0xdeadbeef);store(field+0x20,0xabcdef01);store(aux,0x11223344);store(0x8041e8ac,9);
 store(0x81000014,1);input=*tags.input(3,Inputs,Area);assert(binding->registerMapNpc(input,{123,234})==alternate && word(0x8041e8c8)==1);
 // Selector0 must reset ordinary despite live GP battle context1. Pose1 and
 // alternate actor survive, and pose0 is intentionally retained until the
 // separate original AutoRelease stage; npcReset has no pose/event effects.
 assert(binding->reset(0) && word(0x8041e8b4)==0 && word(0x8041e8b8)==36 && word(0x8041e8c0)==Base && word(0x8041e8ac)==0 && word(field)==0x5678 && word(field+0x5c)==0xdeadbeef && word(field+0x20)==0xabcdef01 && word(aux)==0x11223344 && word(alternate)==5);
 for(unsigned n=0;n<36*0x340;n+=4)assert(word(Base+n)==0);
 assert(game->originalFieldNpcs->poses().poses().pose(0) && game->originalFieldNpcs->poses().poses().pose(1) && binding->activeActors()->size()==1);
 store(field,0x87654321);store(0x8041e8ac,3);assert(binding->reset(-5) && word(0x8041e8c8)==0 && word(0x8041e8cc)==16 && word(field)==0x87654321 && word(0x8041e8ac)==0);
 for(unsigned n=0;n<16*0x340;n+=4)assert(word(alternate+n)==0);
 assert(game->originalFieldNpcs->poses().poses().pose(1));
 // Native admission checks every destination before the source array clear.
 store(Base,0xfeed1234);store(0x81000168,0);assert(!binding->reset(0) && word(Base)==0xfeed1234);store(0x81000168,field);store(0x8041e8b8,35);assert(!binding->reset(0) && word(Base)==0xfeed1234);store(0x8041e8b8,36);store(0x8041e8c0,Base+4);assert(!binding->reset(0) && word(Base)==0xfeed1234);store(0x8041e8c0,Base);assert(binding->reset(0));
 // A new facade can observe the same cleared actor storage/history without a
 // cold Init or a new F publication. Existing pose handles remain owned.
 auto next=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});assert(next->memory.importScripts(data));auto rebound=FieldNpcRegistryBinding::bind(game->originalFieldNpcs,next->memory);assert(rebound && !rebound->initialize() && rebound->activeActors()->empty());assert(next->memory.writeWord(0x81000168,field));assert(rebound->reset(0));uint32_t mask=0;assert(next->memory.readWord(field+0x5c,mask) && mask==0xdeadbeef);
}
}
int main(){orderedSource();retainedNative();}
