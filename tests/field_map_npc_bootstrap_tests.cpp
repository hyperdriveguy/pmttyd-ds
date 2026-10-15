#include "field_area_event.hpp"
#include "event_native_runtime.hpp"
#include "game.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70,Map=0x8100010c;
void install(Game& game,const char* name){const auto base=std::string("romfs/field-native-")+name;assert(game.originalFieldNpcs->poses().load((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str()));}
FieldAreaGpState state(){FieldAreaGpState s{};std::memcpy(s.bytes.data()+0x10c-s.Begin,"gor_02",7);std::memcpy(s.bytes.data()+0x12c-s.Begin,"gor",4);std::memcpy(s.bytes.data()+0x11c-s.Begin,"w_bero",7);return s;}
void actualArea(){
 assert(loadFieldAreaEventAssets("romfs"));FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin"));auto game=std::make_unique<Game>();FieldAreaEvent area;assert(area.begin(2,0));const auto gp=state();
 assert(area.prepareMapNpcs(*game,gp,tags,Inputs,Area,Map,Base) && !area.mapNpcsReady());assert(!area.appendAreaInit(gp) && !area.appendDoorEntry(0,{}));assert(!area.advanceMapNpcs({123,234}));
 install(*game,"c_deale");assert(!area.advanceMapNpcs({123,234}));const auto* first=game->originalFieldNpcs->poses().poses().pose(0);assert(first && first->resource->modelName=="c_deale");
 // Resume at professor after holding; never allocate power a second time.
 for(const char* name:{"c_kuri_h","c_joho_b","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_f_mura","c_luigi"})install(*game,name);
 assert(area.advanceMapNpcs({456,567}) && area.mapNpcsReady() && area.advanceMapNpcs({999,999}));
 auto inspect=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});auto binding=FieldNpcRegistryBinding::bind(game->originalFieldNpcs,inspect->memory);assert(binding && binding->activeActors()->size()==15);
 uint32_t ordinal=0,field=game->originalFieldNpcs->fieldWorkAddress();uint8_t b=0;assert(inspect->memory.readByte(Base+0x314,b) && b==0);assert(inspect->memory.readWord(Base+14*0x340+0x104,ordinal) && ordinal==14);assert(!game->originalFieldNpcs->poses().poses().pose(15));
 std::string name;for(unsigned n=0;n<7;++n){assert(inspect->memory.readByte(field+0x4c+n,b));name.push_back(char(b));}assert(name==std::string("gor_02\0",7));
 auto wrong=gp;wrong.systemFlags=1;assert(!area.appendAreaInit(wrong));const auto root=area.appendAreaInit(gp);assert(root && area.eventAlive(*root));assert(game->originalFieldNpcs->poses().poses().pose(0)->timestamp()==123);
 struct RootProbe {uint32_t field;bool seen=false;} probe{field};FieldAreaEventServices services;services.user=&probe;
 services.call=[](void* p,FieldAreaNativeCall& call)->int32_t{auto& probe=*static_cast<RootProbe*>(p);uint32_t field=0;assert(call.memory.readWord(0x81000168,field) && field==probe.field);probe.seen=true;return 0;};
 area.tick(17,services);assert(probe.seen && !area.failed);
 // A root cannot silently cold-reset retained actors for a second area.
 FieldAreaEvent occupied;assert(occupied.begin(2,0));auto retained=gp;for(unsigned n=0;n<4;++n)retained.bytes[0x168-retained.Begin+n]=uint8_t(field>>(24-8*n));assert(!occupied.prepareMapNpcs(*game,retained,tags,Inputs,Area,Map,Base) && !occupied.appendAreaInit(retained));
}
void capturedGate(){
 auto game=std::make_unique<Game>();auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));auto binding=game->initializeOriginalFieldNpcs(vm->memory,Base);assert(binding);FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin"));
 assert(vm->memory.registerRam(0x88000000,{'g','o','r','_','0','2',0}));auto store=[&](uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));};store(0x8030f668,0x88000000);store(0x8030f66c,4);store(0x8030f670,1);store(0x8030f674,0);
 const auto field=game->originalFieldNpcs->fieldWorkAddress();auto cursor=FieldMapNpcBootstrap::prepare(vm->memory,*binding,tags,Inputs,Area,0x88000000,field);assert(cursor && !cursor->advance({}) && cursor->ordinal()==0);
 // Source already tested row0's unset bit before resource hold. Later writes
 // cannot retroactively skip it; row1 and row14 read their fresh bits.
 store(field+0x5c,0x4003);install(*game,"c_deale");assert(!cursor->advance({}) && cursor->ordinal()==2 && binding->activeActors()->size()==1);
 for(const char* name:{"c_joho_b","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_f_mura","c_luigi"})install(*game,name);
 assert(cursor->advance({}) && cursor->ordinal()==15 && binding->activeActors()->size()==13);uint8_t last=0;assert(vm->memory.readByte(Base+12*0x340+0x314,last) && last==13);
 cursor.reset();uint32_t v=0;assert(!vm->memory.readWord(Inputs,v));
}
}
int main(){actualArea();capturedGate();}
