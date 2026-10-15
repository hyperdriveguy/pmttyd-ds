#include "field_map_npc_assets.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include "game.hpp"
#include <cassert>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70;
struct Fixture {
 std::unique_ptr<Game> game=std::make_unique<Game>();
 std::unique_ptr<EventNativeRuntime> vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});
 std::unique_ptr<FieldNpcRegistryBinding> binding;
 FieldMapNpcAssets tags;
 Fixture(){EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));binding=game->initializeOriginalFieldNpcs(vm->memory,Base);assert(binding);assert(tags.load("romfs/field-npc-map-gor_02.bin") && tags.count()==15 && tags.mapTo(vm->memory,Inputs));}
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 std::string text(uint32_t a){std::string s;for(;;++a){uint8_t v=0;assert(vm->memory.readByte(a,v));if(!v)return s;s.push_back(char(v));}}
 void install(){const char* names[]={"c_deale","c_kuri_h","c_joho_b","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_f_mura","c_luigi","hoshi"};for(auto name:names){const auto base=std::string("romfs/field-native-")+name;assert(game->originalFieldNpcs->poses().load((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str()));}}
};
void originalMap(){
 Fixture f;f.install();const char* models[]={"c_deale","c_kuri_h","c_joho_b","c_monban","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_borodo_n","c_f_mura","c_luigi","c_monban"};
 for(unsigned n=0;n<15;++n){const auto input=f.tags.input(n,Inputs,Area);assert(input);const auto actor=f.binding->registerMapNpc(*input,{123,234});assert(actor && *actor==Base+n*0x340 && f.binding->lookup(input->name)==actor);const auto a=*actor,t=f.word(a+0x28);assert(t && f.text(f.word(t+4))==models[n]);
  assert(f.word(a)==(n==13?0x00800005u:5u));uint8_t ordinal=0;assert(f.vm->memory.readByte(a+0x314,ordinal) && ordinal==n);
  for(unsigned k=0;k<3;++k){assert(f.word(a+0x8c+4*k)==f.word(input->position+4*k));assert(f.word(a+0x98+4*k)==0);}
  assert(f.word(a+0x144)==f.word(input->direction) && f.word(a+0x14c)==f.word(t+0x28) && f.word(a+0x150)==f.word(t+0x2c));
  for(unsigned k:{0x120u,0x124u,0x12cu,0x130u,0x134u,0x138u,0x13cu,0x1f8u})assert(f.word(a+k)==0);
  for(unsigned k=0;k<13;++k)assert(f.word(a+0x1fc+4*k)==0);
  assert(f.word(a+0x1e0)==f.word(t+0x40) && f.word(a+0x1e4)==f.word(t+0x44) && f.word(a+0x1ec)==f.word(t+0x4c) && f.word(a+0x1f0)==f.word(t+0x50));uint16_t half=0,source=0;assert(f.vm->memory.readHalf(a+0x1e8,half) && f.vm->memory.readHalf(t+0x48,source) && half==source);assert(f.vm->memory.readHalf(a+0x1d8,half) && half==1 && f.word(a+0x1dc)==0);
  for(unsigned k=0;k<200;++k){uint8_t byte=1;assert(f.vm->memory.readByte(a+0x230+k,byte) && byte==0);} // Actual map battle-1, not later npcEnt battle0.
  assert(f.text(a+0x2c)==f.text(f.word(t+8)) && f.text(a+0x4c)==f.text(f.word(t+0x10)) && f.text(a+0x6c)==f.text(f.word(t+0x14)));
  const auto* pose=f.game->originalFieldNpcs->poses().poses().pose(int32_t(f.word(a+0x104)));assert(pose && pose->resource->modelName==models[n] && pose->word(0)==9 && pose->resource->names[pose->word(0x14)]==f.text(a+0x2c) && pose->timestamp()==123);assert(!f.game->originalFieldNpcs->poses().main(int32_t(f.word(a+0x104)),{123,234}));
 }
 assert(f.binding->activeActors()->size()==15 && f.word(0x8041e8b4)==15);
 auto duplicate=*f.tags.input(0,Inputs,Area);duplicate.ordinal=299;assert(f.binding->registerMapNpc(duplicate,{400,500})==Base && f.word(0x8041e8b4)==16);const auto latest=Base+15*0x340;assert(f.word(latest)==3 && f.word(latest+0x28)==0);uint8_t ordinal=0;assert(f.vm->memory.readByte(Base+0x314,ordinal) && ordinal==43);
}
void admissionAndLoader(){
 Fixture f;FieldMapNpcAssets unloaded;assert(!unloaded.mapTo(f.vm->memory,0x88010000));const auto input=*f.tags.input(0,Inputs,Area);assert(!f.binding->registerMapNpc(input,{}) && f.word(0x8041e8b4)==0);assert(!f.tags.input(15,Inputs,Area) && !f.tags.input(0,0xfffffff0,Area));assert(!f.tags.load("romfs/door-data.bin") && f.tags.count()==15 && !f.tags.mapTo(f.vm->memory,Inputs));f.install();
 auto malformed=input;malformed.position=0xffffffff;assert(!f.binding->registerMapNpc(malformed,{}) && f.word(0x8041e8b4)==0);
 assert(f.vm->memory.registerRam(0x88000000,{'N','O','T','_','A','_','S','E','N','T','I','N','E','L',0}));malformed=input;malformed.battle=0x88000000;assert(!f.binding->registerMapNpc(malformed,{}) && f.word(0x8041e8b4)==0); // Missing real numeric service holds before Entry.
 assert(f.vm->memory.registerRam(0x88001000,std::vector<uint8_t>(16)) && f.vm->memory.writeWord(0x88001000,0x88000000) && f.vm->memory.writeWord(0x88001004,0xffffffff) && f.vm->memory.writeWord(0x80305370,0x88001000));
 assert(f.binding->registerMapNpc(malformed,{})==Base && f.word(0x8041e8b4)==1); // Named table's actual-1 also needs no RNG provider.
 assert(f.binding->registerMapNpc(input,{})==Base);
}
}
namespace {
// Explicit service-mutation fixture checks captured versus freshly read source
// inputs. Original packs stay unchanged; only this VM's runtime copies mutate.
void orderedServices(){
 Fixture f;f.install();auto input=*f.tags.input(0,Inputs,Area);input.ordinal=0x123;
 struct Probe {Fixture* f;PackedMapNpcInput input;unsigned area=0,battle=0,animation=0;uint32_t tribe=0;};Probe p{&f,input};
 auto store=[&](uint32_t a,uint32_t v){assert(f.vm->memory.writeWord(a,v));};store(0x80338274,0x100);
 PackedMapNpcServices s;s.user=&p;s.memory.user=&p;
 s.memory.word=[](void* p,uint32_t a){return static_cast<Probe*>(p)->f->word(a);};
 s.memory.half=[](void* p,uint32_t a){uint16_t v=0;assert(static_cast<Probe*>(p)->f->vm->memory.readHalf(a,v));return v;};
 s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){assert(static_cast<Probe*>(p)->f->vm->memory.writeByte(a,v));};
 s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){assert(static_cast<Probe*>(p)->f->vm->memory.writeHalf(a,v));};
 s.memory.storeWord=[](void* raw,uint32_t a,uint32_t v){auto& p=*static_cast<Probe*>(raw);auto& m=p.f->vm->memory;assert(m.writeWord(a,v));if(a==Base+0x8c){assert(m.writeWord(p.input.position+4,0x4479c000) && m.writeWord(p.input.position+8,0x42c60000));}if(a==Base+0x120)assert(m.writeWord(0x8033827c,0x12345678));if(a==Base+0x1fc)assert(m.writeWord(0x804207b4,0x3f800000));};
 s.compare=[](void* raw,uint32_t a,uint32_t b)->int32_t{auto& p=*static_cast<Probe*>(raw);if(b==0x802c3b10)assert(p.area==1);const auto x=p.f->text(a),y=p.f->text(b);return x<y?-1:x>y?1:0;};
 s.copyString=[](void* raw,uint32_t a,uint32_t b){auto& p=*static_cast<Probe*>(raw);const auto text=p.f->text(b);for(unsigned n=0;n<=text.size();++n)assert(p.f->vm->memory.writeByte(a+n,n==text.size()?0:uint8_t(text[n])));};
 s.entry=[](void* raw,uint32_t a,uint32_t b){auto& p=*static_cast<Probe*>(raw);assert(p.f->binding->enter(a,b,{123,234})==0);};
 s.lookup=[](void* raw,uint32_t a){auto& p=*static_cast<Probe*>(raw);return *p.f->binding->lookup(a,PackedNpcLookup::Release);};
 s.area=[](void* raw,uint32_t a){auto& p=*static_cast<Probe*>(raw);assert(a==Area && p.battle==0);++p.area;return uint32_t(0x80305364);};
 s.battle=[](void* raw,uint32_t a,int32_t index){auto& p=*static_cast<Probe*>(raw);assert(a==Base && index==-1 && p.area==1);++p.battle;auto& m=p.f->vm->memory;p.tribe=p.f->word(a+0x28);assert(m.writeWord(a+0x28,0));assert(m.writeWord(p.tribe+0x10,p.f->word(p.tribe+0x18)) && m.writeWord(p.tribe+0x14,p.f->word(p.tribe+0x1c)) && m.writeWord(p.tribe+8,p.f->word(p.tribe+0xc)));const auto h=p.f->game->originalFieldNpcs->poses().enter("c_deale",0,{123,234});assert(h && *h==1 && m.writeWord(a+0x104,uint32_t(*h)));};
 s.animation=[](void* raw,int32_t h,uint32_t a,int32_t mode){auto& p=*static_cast<Probe*>(raw);assert(p.battle==1 && h==1 && a==Base+0x2c && mode==1 && p.f->text(a)=="Z_1");++p.animation;};
 assert(registerPackedMapNpc(input,s)==Base && p.area==1 && p.battle==1 && p.animation==1);assert(f.word(Base)==0x105 && f.word(Base+0x28)==0 && f.word(Base+0x90)==0 && f.word(Base+0x94)==0x42c60000 && f.word(Base+0x124)==0x12345678 && f.word(Base+0x1dc)==0);
 assert(f.text(Base+0x4c)=="W_1" && f.text(Base+0x6c)=="R_1");uint8_t ordinal=0;assert(f.vm->memory.readByte(Base+0x314,ordinal) && ordinal==0x23);
}
}
int main(){originalMap();admissionAndLoader();orderedServices();}
