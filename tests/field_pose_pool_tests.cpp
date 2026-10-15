#include "field_pose_pool.hpp"
#include "field_npc_registry.hpp"
#include "event_native_runtime.hpp"
#include "animation_draw_resource.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
std::shared_ptr<const BattlePoseResource> professor(){return loadBattlePoseResource("romfs/professor.pose-directory.bin","romfs/professor.pmanim");}
void fieldOwner(){
 auto resource=professor();assert(resource && resource->draw && resource->textures);FieldPosePool field;
 assert(!field.canEnter("c_kuri_h",0) && !field.enter("c_kuri_h",0,{}));auto partial=std::make_shared<BattlePoseResource>(*resource);partial->draw.reset();assert(!field.install(partial));partial=std::make_shared<BattlePoseResource>(*resource);partial->textures.reset();assert(!field.install(partial));
 assert(field.install(resource) && field.install(resource) && field.canEnter("c_kuri_h",0));assert(!field.install(std::make_shared<BattlePoseResource>(*resource)));
 assert(field.animationReady("c_kuri_h","S_1") && field.animationReady("c_kuri_h","d_1") && !field.animationReady("unavailable","S_1"));
 auto handle=field.enter("c_kuri_h",0,{123,456});assert(handle && *handle==0);const auto* p=field.poses().pose(*handle);assert(p && p->timestamp()==123 && p->resource==resource && p->word(0x120)==0x80415dc4 && field.vivianType(*handle)==0);
 assert(resource->draw->word(0xc8)==5 && resource->draw->word(0xcc)==5 && field.height(*handle)==10.f && field.radius(*handle)==10.f);
 assert(field.paperOn(*handle) && field.materialLightFlagOn(*handle,2) && p->word(0)==9 && p->word(0xec)==2);assert(!field.main(*handle,{123,456}) && p->word(0)&8); // No invented facing completion.
 assert(field.selectName(*handle,"d_1",true,{234,456}) && p->word(0x14)==0 && field.evaluate(*handle,{234,456}));
 assert(field.selectName(*handle,"S_1",true,{300,456}) && field.evaluate(*handle,{300,456}) && !p->shape.positions.empty());
 assert(field.release(*handle) && !field.height(*handle) && !field.vivianType(*handle));const auto reused=field.enter("c_kuri_h",0,{});assert(reused && *reused==0 && field.poses().pose(0)->word(0)==1);
 for(unsigned i=1;i<256;++i){assert(field.enter("c_kuri_h",0,{}));}
 assert(!field.canEnter("c_kuri_h",0) && !field.enter("c_kuri_h",0,{}));assert(field.release(17) && field.canEnter("c_kuri_h",1) && field.enter("c_kuri_h",1,{})==17);
}
void signedMetadata(){
 const auto source=professor();std::ifstream input("romfs/professor.draw-model.bin",std::ios::binary);std::vector<char> raw{std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>()};assert(raw.size()>0x1b0);
 const int32_t values[]={0,-5,16777217,std::numeric_limits<int32_t>::min(),std::numeric_limits<int32_t>::max()};
 const uint32_t expected[]={0,0xc1200000,0x4c000000,0xcf800000,0x4f800000};
 for(unsigned i=0;i<5;++i){auto data=raw;for(unsigned j=0;j<4;++j){data[0xc8+j]=char(uint32_t(values[i])>>(24-8*j));data[0xcc+j]=data[0xc8+j];}FILE* f=std::tmpfile();assert(f && std::fwrite(data.data(),data.size(),1,f)==1);std::rewind(f);auto draw=std::make_shared<AnimationDrawResource>();assert(draw->read(f));std::fclose(f);
  auto r=std::make_shared<BattlePoseResource>(*source);r->draw=draw;FieldPosePool field;assert(field.install(r));const auto h=field.enter("c_kuri_h",0,{});assert(h && bits(*field.height(*h))==expected[i] && bits(*field.radius(*h))==expected[i]);
 }
}
// Real original source constants/name and real retained Frankly MODEL/TPL/CPU
// animation; only allocator/guest storage are explicitly test-owned.
void constructor(){
 EventNativeRuntime vm{0x81000000,{}};EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm.memory.importScripts(data));assert(vm.memory.registerRam(0x8041e8ac,std::vector<uint8_t>(0x30)));
 FieldPosePool field;const auto original=professor();assert(field.install(original));unsigned allocations=0;
 struct Owner {EventNativeRuntime* vm;FieldPosePool* field;unsigned* allocations;};Owner owner{&vm,&field,&allocations};PackedNpcRegistryServices s;s.user=&owner;s.memory.user=&owner;
 s.memory.word=[](void* p,uint32_t a){uint32_t v=0;assert(static_cast<Owner*>(p)->vm->memory.readWord(a,v));return v;};
 s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){assert(static_cast<Owner*>(p)->vm->memory.writeWord(a,v));};s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){assert(static_cast<Owner*>(p)->vm->memory.writeHalf(a,v));};s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){assert(static_cast<Owner*>(p)->vm->memory.writeByte(a,v));};
 s.allocate=[](void* p,int32_t heap,uint32_t n){auto& o=*static_cast<Owner*>(p);assert(heap==0);const uint32_t a=0x85000000+(*o.allocations)++*0x10000;assert(o.vm->memory.registerRam(a,std::vector<uint8_t>(n,0xa5)));return a;};
 s.clear=[](void* p,uint32_t a,uint32_t n){auto& m=static_cast<Owner*>(p)->vm->memory;for(unsigned i=0;i<n;++i)assert(m.writeByte(a+i,0));};
 s.compare=[](void* p,uint32_t a,uint32_t b)->int32_t{auto& m=static_cast<Owner*>(p)->vm->memory;for(;;++a,++b){uint8_t x=0,y=0;assert(m.readByte(a,x) && m.readByte(b,y));if(x!=y || !x)return int32_t(x)-int32_t(y);}};
 s.copyString=[](void* p,uint32_t a,uint32_t b){auto& m=static_cast<Owner*>(p)->vm->memory;uint8_t c;do{assert(m.readByte(b++,c) && m.writeByte(a++,c));}while(c);};
 s.poseEntry=[](void* p,uint32_t name,int32_t mode)->int32_t{auto& o=*static_cast<Owner*>(p);std::string model;for(;;++name){uint8_t c=0;assert(o.vm->memory.readByte(name,c));if(!c)break;model.push_back(char(c));}assert(o.field->canEnter(model,mode));const auto h=o.field->enter(model,mode,{333,444});assert(h);return *h;};
 s.paperOn=[](void* p,int32_t h){assert(static_cast<Owner*>(p)->field->paperOn(h));};s.heightBits=[](void* p,int32_t h){const auto v=static_cast<Owner*>(p)->field->height(h);assert(v);return bits(*v);};s.radiusBits=[](void* p,int32_t h){const auto v=static_cast<Owner*>(p)->field->radius(h);assert(v);return bits(*v);};
 s.vivianType=[](void* p,int32_t h)->int32_t{const auto v=static_cast<Owner*>(p)->field->vivianType(h);assert(v);return *v;};s.lightFlagOn=[](void* p,int32_t h,uint32_t flags){assert(static_cast<Owner*>(p)->field->materialLightFlagOn(h,flags));};
 // Source npcEnt row1 is Frankly. Its actual original name is retained; the
 // model string is in the unchanged raw MODEL header, mapped for this fixture.
 uint32_t name=0;assert(vm.memory.readWord(0x90524f10+0x5c,name));std::vector<uint8_t> modelBytes(64);for(unsigned i=0;i<64;++i)modelBytes[i]=original->draw->byte(4+i);assert(vm.memory.registerRam(0x86000000,std::move(modelBytes)));
 initializePackedNpcRegistry(s);assert(allocations==4 && enterPackedNpc(name,0x86000000,s)==0);const auto actor=packedNpcNameToPointer(name,PackedNpcLookup::NoAssert,s);assert(actor==0x85000000);
 auto word=[&](uint32_t a){uint32_t v=0;assert(vm.memory.readWord(a,v));return v;};assert(word(actor)==3 && word(actor+0x104)==0 && word(actor+0x14c)==bits(10.f) && word(actor+0x150)==bits(10.f) && word(actor+0x10c)==0x8041fd74);
 assert(field.poses().pose(0)->word(0)==9 && field.poses().pose(0)->word(0xec)==2 && field.poses().pose(0)->timestamp()==333 && !field.main(0,{333,444}));
}
}
int main(){fieldOwner();signedMetadata();constructor();}
