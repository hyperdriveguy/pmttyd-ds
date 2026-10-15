#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,NameRow=0x90524f10+0x5c,Model=0x86000000;
struct Fixture {
 std::unique_ptr<Game> game=std::make_unique<Game>();
 std::unique_ptr<EventNativeRuntime> vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});
 std::unique_ptr<FieldNpcRegistryBinding> binding;
 uint32_t name=0;
 Fixture(){EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));assert(vm->memory.readWord(NameRow,name));}
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 void store(uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));}
 void initialize(){binding=game->initializeOriginalFieldNpcs(vm->memory,Base);assert(binding && game->originalFieldNpcs && game->originalFieldNpcs->initialized());}
 void install(){const auto r=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(r && game->originalFieldNpcs->poses().install(r));std::vector<uint8_t> bytes(64);for(unsigned i=0;i<64;++i)bytes[i]=r->draw->byte(4+i);assert(vm->memory.registerRam(Model,std::move(bytes)));}
};
void coldAndAdmission(){
 Fixture f;assert(!FieldNpcRegistryOwner::cold(0) && !FieldNpcRegistryOwner::cold(Base+1) && !FieldNpcRegistryOwner::cold(0xfffffff0));
 assert(f.vm->memory.registerRam(Base,std::vector<uint8_t>(16,0x71)));assert(!f.game->initializeOriginalFieldNpcs(f.vm->memory,Base) && !f.game->originalFieldNpcs);uint8_t b=0;assert(!f.vm->memory.readByte(0x8041e8ac,b) && f.vm->memory.readByte(Base,b) && b==0x71);assert(f.vm->memory.unregisterRam(Base));
 auto owner=FieldNpcRegistryOwner::cold(Base);auto binding=FieldNpcRegistryBinding::bind(owner,f.vm->memory);assert(binding && !owner->initialized());assert(f.word(0x8041e8b4)==0 && f.word(0x8041e8c8)==0);assert(f.vm->memory.writeByte(Base+36*0x340,0xa5));
 assert(!binding->enter(f.name,Model,{}) && !binding->lookup(f.name));assert(binding->initialize() && !binding->initialize());assert(f.word(0x8041e8b8)==36 && f.word(0x8041e8cc)==16 && f.word(0x81000168)==Base+36*0x340+36*0x94+16*0x340);assert(f.vm->memory.readByte(Base+36*0x340,b) && b==0xa5); // npcInit did not clear auxiliary.
 assert(!FieldNpcRegistryBinding::bind(owner,f.vm->memory));assert(binding->activeActors()->empty());assert(binding->lookup(f.name)==0 && binding->lookup(f.name,PackedNpcLookup::Release)==Base+36*0x340);
 binding.reset();assert(!f.vm->memory.readByte(Base,b) && !f.vm->memory.readByte(0x8041e8ac,b));
 assert(f.vm->gpAddresses.unregisterGp(0x81000000));assert(!f.game->initializeOriginalFieldNpcs(f.vm->memory,Base) && !f.game->originalFieldNpcs);assert(!f.vm->memory.readByte(Base,b));
}
void realConstructorAndCapacity(){
 Fixture f;f.initialize();assert(!f.binding->enter(f.name,Model,{1111,2222}));assert(f.word(0x8041e8b4)==0);f.install();f.store(0x81000038,0x12345678);f.store(0x8100003c,0x9abcdef0);
 assert(f.binding->enter(f.name,Model,{1111,2222})==0);assert(f.word(Base)==3 && f.word(Base+0x104)==0 && f.word(Base+0x198)==0x12345678 && f.word(Base+0x19c)==0x9abcdef0);assert(f.word(Base+0x14c)==0x41200000 && f.word(Base+0x150)==0x41200000 && f.word(Base+0x28)==0);
 const auto* pose=f.game->originalFieldNpcs->poses().poses().pose(0);assert(pose && pose->timestamp()==1111 && pose->word(0)==9 && pose->word(0xec)==2);assert(f.binding->lookup(f.name)==Base && !f.game->originalFieldNpcs->poses().main(0,{1111,2222}));
 assert(f.binding->enter(f.name,Model,{1111,2222})==1 && f.binding->lookup(f.name)==Base);f.store(Base,0x801);assert(f.binding->lookup(f.name)==Base); // active-only exact lookup, no query/visibility gate.
 for(unsigned i=2;i<36;++i)assert(f.binding->enter(f.name,Model,{1111,2222})==int32_t(i));assert(f.binding->activeActors()->size()==36 && f.word(0x8041e8b4)==36);
 assert(!f.binding->enter(f.name,Model,{1111,2222}) && !f.game->originalFieldNpcs->poses().poses().pose(36) && f.word(0x8041e8b4)==36);
 f.store(0x81000014,7);assert(f.binding->lookup(f.name)==0 && f.binding->enter(f.name,Model,{1111,2222})==0);const auto alternate=Base+36*0x340+36*0x94;assert(f.binding->lookup(f.name)==alternate && f.word(0x8041e8c8)==1);assert(f.game->originalFieldNpcs->poses().poses().pose(36)->timestamp()==2222);
 f.store(0x81000014,0);assert(f.binding->lookup(f.name)==Base && f.word(0x8041e8b4)==36);assert(!f.game->initializeOriginalFieldNpcs(f.vm->memory,Base));
}
void retainedFacades(){
 Fixture f;f.initialize();f.install();assert(f.binding->enter(f.name,Model,{})==0);
 auto other=std::make_unique<EventNativeRuntime>(0x82000000,EventNativePlatform{});EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && other->memory.importScripts(data));auto second=FieldNpcRegistryBinding::bind(f.game->originalFieldNpcs,other->memory);assert(second && !second->initialize() && second->lookup(f.name)==Base);uint32_t v=1;assert(other->memory.readWord(0x82000168,v) && v==0); // Bind does not invent a GP lifecycle write.
 assert(other->memory.writeWord(Base+0x144,0x42b40000) && f.word(Base+0x144)==0x42b40000);assert(other->memory.writeWord(Base,0x800));assert(f.binding->lookup(f.name)==0 && second->activeActors()->empty());
 auto retained=f.game->originalFieldNpcs;f.binding.reset();assert(!f.vm->memory.readWord(Base,v) && second->lookup(f.name)==0);f.binding=FieldNpcRegistryBinding::bind(retained,f.vm->memory);assert(f.binding && !f.binding->initialize() && f.word(Base+0x144)==0x42b40000 && f.word(0x8041e8b4)==1);
 f.game->originalFieldNpcs.reset();retained.reset();assert(f.word(Base+0x144)==0x42b40000);second.reset();assert(!other->memory.readWord(Base,v));
}
void malformedInputAndAlias(){
 Fixture f;f.initialize();f.install();assert(f.vm->memory.registerRam(0x86000100,std::vector<uint8_t>(64,'a')));assert(!f.binding->enter(0x86000100,Model,{}) && !f.binding->lookup(0x86000100));
 // Cleared-slot aliases are outside this infallible native constructor domain.
 assert(f.vm->memory.writeByte(Base+8,'x') && f.vm->memory.writeByte(Base+9,0));assert(!f.binding->enter(Base+8,Model,{}) && f.word(0x8041e8b4)==0);
 f.store(0x8041e8b8,37);assert(!f.binding->activeActors() && !f.binding->enter(f.name,Model,{}) && !f.binding->lookup(f.name));f.store(0x8041e8b8,36);
 assert(f.binding->enter(f.name,Model,{})==0);for(unsigned i=0;i<32;++i)assert(f.vm->memory.writeByte(Base+8+i,'x'));assert(!f.binding->lookup(f.name) && !f.binding->enter(f.name,Model,{}) && f.word(0x8041e8b4)==1);
}
}
int main(){coldAndAdmission();realConstructorAndCapacity();retainedFacades();malformedInputAndAlias();}
