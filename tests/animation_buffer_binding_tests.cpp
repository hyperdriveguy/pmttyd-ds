#include "battle_sequence_binding.hpp"
#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
using namespace ttyd;
void runAnimationBindingFixture(){
 unsigned flushes=0;EventNativePlatform platform;platform.user=&flushes;
 platform.flushBlock=[](void* p,uint32_t){++*static_cast<unsigned*>(p);};platform.systemCall=[](void*){};
 auto game=std::make_unique<Game>(false);auto vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);
 EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
 auto registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry);
 auto resource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(resource && game->originalFieldNpcs->poses().install(resource));
 std::vector<uint8_t> nameBytes(64);for(unsigned i=0;i<64;++i)nameBytes[i]=resource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(nameBytes)));
 uint32_t name=0;assert(vm->memory.readWord(0x90524f10+0x5c,name) && registry->enter(name,0x86000000,{})==0);
 auto n=*registry->lookup(name);uint32_t gp=0,f=0;assert(vm->memory.readWord(0x80415fd0,gp) && vm->memory.readWord(gp+0x168,f) && vm->memory.writeWord(f+8,n));
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,vm->interpreterServices().operands);
 assert(loadProloguePouchAssets("romfs"));game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
 auto binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
 auto storage=BattleSequenceStorage::cold(game->progress.originalPouch,0x88000000,0x100000,0x89000000);assert(storage && binding->attachStorage(storage));
 // Explicit native fixture views, not original animInit/file-manager history.
 // Both poses use real CPU original MODEL/TPL resources. Mode2 here is a real
 // extra typed pose, not a claim that Mario/partner have migrated namespaces.
 auto& field=game->originalFieldNpcs->poses();auto second=field.enter(resource->modelName,2,{});assert(second && *second==1);
 auto* p0=const_cast<BattlePose*>(field.poses().slot(0));auto* p1=const_cast<BattlePose*>(field.poses().slot(1));assert(p0 && p1);
 assert(vm->memory.registerExternalRam(0x8a010000,p0->state.data(),0x170) && vm->memory.registerExternalRam(0x8a010170,p1->state.data(),0x170));
 for(auto span:std::array<std::pair<uint32_t,uint32_t>,5>{{{0x80415dc0,4},{0x8a000000,0x110},{0x8a020000,16},{0x8a030000,0xa4},{0x8a040000,4}}})assert(vm->memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
 std::vector<uint8_t> model(0x1b0);for(unsigned i=0;i<model.size();++i)model[i]=resource->draw->byte(i);assert(vm->memory.registerRam(0x8a050000,std::move(model)));
 assert(vm->memory.writeWord(0x80415dc0,0x8a000000) && vm->memory.writeWord(0x8a000000,0x8a020000) && vm->memory.writeWord(0x8a000010,0x8a010000) && vm->memory.writeWord(0x8a000014,2));
 assert(vm->memory.writeWord(0x8a020008,0x8a030000) && vm->memory.writeWord(0x8a0300a0,0x8a040000) && vm->memory.writeWord(0x8a040000,0x8a050000));
 struct Backend {EventNativeRuntime& vm;unsigned requests=0;bool admit=false;} backend{*vm};
 BattleSequenceEntryServices services;services.user=&backend;services.call=[](void* p,BattleSequenceCall& c){auto& b=*static_cast<Backend*>(p);
  if(c.function==0x800300f0){++b.requests;if(!b.admit)return false;assert(c.args[0]==0 && c.args[1]==0x180000);assert(b.vm.memory.registerRam(0x8b000000,std::vector<uint8_t>(0x180000)));c.result=0x8b000000;return true;}
  return c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28;};
 auto& work=game->originalSequence->state.work;assert(binding->callback(0x80073f20,work,services));
 work[1]=4;assert(!binding->callback(0x80073b24,work,services) && binding->pending()==0x800300f0 && work[1]==4);
 uint32_t value=0;assert(vm->memory.readWord(0x8a00010c,value) && value==1 && p1->word(0x48)==0 && p0->word(0x48)==0);
 assert(!binding->callback(0x80073b24,work,services) && backend.requests==2);
 backend.admit=true;assert(!binding->callback(0x80073b24,work,services) && binding->pending()==0x8004ce58 && work[1]==4 && backend.requests==3);
 assert(p0->word(0x48)==0 && p1->word(0x48)==0x8b000000 && p1->word(0x58)==p1->word(0x5c) && p1->word(0x68)==p1->word(0x6c) && p1->word(0x3c)==0xffffffff);
 auto before=p1->word(0x48);assert(!binding->callback(0x80073b24,work,services) && backend.requests==3 && p1->word(0x48)==before);
 // The exact same cache word is read by typed evaluate after source refresh.
 assert(field.evaluate(1,{0,17}) && p1->word(0x3c)!=0xffffffff);
 binding.reset();assert(vm->memory.unregisterExternalRam(0x8a010000) && vm->memory.unregisterExternalRam(0x8a010170));
}
int main(){runAnimationBindingFixture();}
