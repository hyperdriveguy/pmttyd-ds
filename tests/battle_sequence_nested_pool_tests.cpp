#include "battle_sequence_binding.hpp"
#include "battle_sequence_nested_pool.hpp"
#include "harbor_source_map_pool.hpp"
#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_release.hpp"
#include "animation_initialize.hpp"
#include "animation_pool_memory.hpp"
#include <cassert>
#include <iostream>
#include <functional>
using namespace ttyd;
namespace {
// TEST source-parent position. This is not normal encounter activation: real
// Init and FieldInitialize execute, then the genuine unbound animation child holds.
constexpr uint32_t Work=0x803d9470;
struct MemoryWorld {
 std::shared_ptr<BattlePosePool> pool;
 std::unique_ptr<EventNativeRuntime> vm;
 std::unique_ptr<AnimationPoolMemory> view;

 uint32_t next=0x8a000000,models=0,textures=0,poses=0;
 explicit MemoryWorld(std::shared_ptr<BattlePosePool> p=std::make_shared<BattlePosePool>()):pool(std::move(p)){
  EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);auto& m=vm->memory;
  assert(m.registerRam(0x80415dc0,std::vector<uint8_t>(4)) && m.writeWord(0x80415dc0,Work));
  assert(m.registerRam(Work,std::vector<uint8_t>(0x110+178*4)) && m.registerRam(0x8041fbb4,std::vector<uint8_t>(4)) && m.writeWord(0x8041fbb4,0x3c8efa33));
  assert(m.registerRam(0x802c1070,std::vector<uint8_t>(8)) && m.writeWord(0x802c1070,0x43300000) && m.writeWord(0x802c1074,0x80000000));
  AnimationInitializeServices s;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;return static_cast<MemoryWorld*>(p)->vm->memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
  s.memory.store=[](void* p,uint32_t a,uint32_t v){return static_cast<MemoryWorld*>(p)->vm->memory.writeWord(a,v);};
  s.memory.call=[](void* p,BattleSequenceCall& c){auto& w=*static_cast<MemoryWorld*>(p);
   if(c.function==0x800300f0){assert(c.args[0]==0);c.result=w.next;w.next+=c.args[1]+0x100;return w.vm->memory.registerRam(c.result,std::vector<uint8_t>(c.args[1]));}
   if(c.function==0x8018a61c){c.result=0;return true;} // Explicit fixture null-file branch, not real file startup.
   return c.function==0x80189d38;};
  s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{uint32_t hi=0,lo=0;auto& m=static_cast<MemoryWorld*>(p)->vm->memory;return m.readWord(a,hi) && m.readWord(a+4,lo)?std::optional<uint64_t>(uint64_t(hi)<<32|lo):std::nullopt;};
  s.tangent=nativeAnimationTangent;AnimationInitializeCursor cursor;assert(initializeAnimation(s,cursor));
  models=word(Work);textures=word(Work+8);poses=word(Work+0x10);
 }
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}

};

struct Fixture {
 struct Cache {unsigned lines=0,barriers=0;std::function<void()> duringFlush;} cache;
 std::unique_ptr<Game> game;
 std::unique_ptr<EventNativeRuntime> vm;
 std::shared_ptr<BattleSequenceStorage> storage;
 std::unique_ptr<BattleSequenceBinding> binding;
 std::shared_ptr<const BattleSequenceNestedPoolReceipt> receipt;
 std::unique_ptr<HarborSourceMapPool> adopted;
 Fixture(){

 EventNativePlatform platform;platform.user=&cache;platform.flushBlock=[](void* p,uint32_t){auto& c=*static_cast<Cache*>(p);++c.lines;if(c.duringFlush)c.duringFlush();};platform.systemCall=[](void* p){++static_cast<Cache*>(p)->barriers;};
 game=std::make_unique<Game>(false);vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);
 EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
 auto registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry);
 auto resource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(resource && game->originalFieldNpcs->poses().install(resource));
 std::vector<uint8_t> model(64);for(unsigned i=0;i<64;++i)model[i]=resource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(model)));
 uint32_t name=0;assert(vm->memory.readWord(0x90524f10+0x5c,name) && registry->enter(name,0x86000000,{})==0);
 auto n=*registry->lookup(name);uint32_t gp=0,f=0;assert(vm->memory.readWord(0x80415fd0,gp) && vm->memory.readWord(gp+0x168,f));
 // Explicit caller fixture, not the authored tik_01 encounter bridge.
 assert(vm->memory.writeWord(f+8,n));writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,vm->interpreterServices().operands);
 assert(loadProloguePouchAssets("romfs"));game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
 storage=BattleSequenceStorage::cold(game->progress.originalPouch,0x88000000,0x100000,0x89000000);assert(storage);
 assert(!BattleSequenceStorage::cold({},0x88000000,0x100000,0x89000000));
 assert(!BattleSequenceStorage::cold(game->progress.originalPouch,0x88000001,0x100000,0x89000000));
 binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
 assert(vm->memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
 assert(!binding->attachStorage(storage));uint32_t absent=0;assert(!vm->memory.readWord(0x8041e808,absent) && !game->battleSequenceProcess->storage);
 assert(vm->memory.unregisterRam(0x8041eb90) && binding->attachStorage(storage));
 assert(!binding->attachStorage(storage));
 assert(vm->memory.writeHalf(0x89000070,7) && game->progress.originalPouch->half(0x70)==7);

  assert(!binding->nestedPoolReceipt());
  BattleSequenceEntryServices services;services.call=[](void*,BattleSequenceCall& c){return c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28;};
  auto& work=game->originalSequence->state.work;
  assert(binding->callback(0x80073f20,work,services));
  work[1]=4;
  cache.duringFlush=[&]{assert(!binding->nestedPoolReceipt());assert(!binding->callback(0x80073b24,work,services));};
  assert(!binding->callback(0x80073b24,work,services)&&binding->pending()==0x80041c58);
  cache.duringFlush={};
  receipt=binding->nestedPoolReceipt();assert(receipt&&receipt->current()&&receipt->root()==0x88000020);
  assert(cache.lines==0x64000/32&&cache.barriers==1);
  assert(!binding->callback(0x80073b24,work,services)&&cache.barriers==1);
  auto wrong=std::make_unique<EventNativeRuntime>(0x81000000,platform);
  assert(!HarborSourceMapPool::adopt(*wrong,receipt));
  uint32_t before=0;assert(vm->memory.readWord(receipt->root()+4,before));
  adopted=HarborSourceMapPool::adopt(*vm,receipt);assert(adopted&&adopted->current());
  assert(!HarborSourceMapPool::adopt(*vm,receipt));
  uint32_t after=0;assert(vm->memory.readWord(receipt->root()+4,after)&&before==after&&cache.barriers==1);
 }
 void stale(){assert(!receipt->current()&&!adopted->current()&&!adopted->root());assert(!adopted->allocate(0x88000020,32));assert(!adopted->free(0x88000020,0));}
};
void allocationAndSourceContext(){
 Fixture f;const auto root=f.adopted->root();
 assert(!f.adopted->allocationLifetime(root+32));
 // Explicit TEST external birth: adoption may not invent its incarnation.
 const auto external=eventMapAllocate(root,64,f.vm->memoryBinding.services());assert(external);
 assert(!f.adopted->allocationLifetime(external)&&!f.adopted->free(root,external));
 eventMapFree(root,external,f.vm->memoryBinding.services());
 assert(!f.binding->allocateMap(root,32,f.vm->memory)); // Exclusive nested lease.
 f.cache.duringFlush=[&]{assert(!f.adopted->allocate(root,32));assert(!f.adopted->free(root,0));};
 const auto a=f.adopted->allocate(root,128),b=f.adopted->allocate(root,128);
 f.cache.duringFlush={};assert(a&&*a&&b&&*b);
 auto old=f.adopted->allocationLifetime(*a);assert(old);
 assert(f.vm->memory.writeWord(*a,0x12345678));
 assert(f.adopted->free(root,*b)&&f.adopted->free(root,*a)&&!f.adopted->allocationLifetime(*a));
 const auto reused=f.adopted->allocate(root,128);assert(reused&&*reused==*a);
 assert(f.adopted->allocationLifetime(*reused)!=old&&f.receipt->current());
 f.vm->reinitializeContext();assert(f.receipt->current()&&f.adopted->current());
 const auto oldUser=f.vm->platform.user;f.vm->platform.user=nullptr;f.stale();f.vm->platform.user=oldUser;assert(f.receipt->current());
 // A retired receipt does not authorize another root producer or adoption.
 const_cast<BattleSequenceNestedPoolReceipt*>(f.receipt.get())->retire();f.stale();assert(!HarborSourceMapPool::adopt(*f.vm,f.receipt));
}
void facadeLifetimes(){
 {Fixture f;auto copy=std::make_unique<Game>(*f.game);assert(f.receipt->current());copy.reset();assert(f.receipt->current());}
 {Fixture f;auto moved=std::make_unique<Game>(std::move(*f.game));f.stale();}
 {Fixture f;auto replacement=std::make_unique<Game>(false);*f.game=*replacement;f.stale();}
 {Fixture f;auto replacement=std::make_unique<Game>(false);*f.game=std::move(*replacement);f.stale();}
 {Fixture f;f.game->restore(f.game->progress);f.stale();}
 {Fixture f;f.game.reset();f.stale();f.binding.reset();}
 {Fixture f;f.vm.reset();f.stale();f.binding.reset();f.adopted.reset();}
 {Fixture f;f.binding.reset();f.stale();}
 {Fixture f;f.game->progress.originalPouch=createProloguePouch();f.stale();}
 {Fixture f;f.vm->memoryBinding.platformUser=nullptr;f.stale();}
}
void attachedAnimationVmDeath(){
 assert(loadFieldPlayerPoseAssets("romfs"));auto game=std::make_unique<Game>(false);
 auto& parties=const_cast<FieldPartyRegistry&>(game->fieldPartyState());assert(parties.configureOriginalPool(fieldPlayerPoseResources()));
 MemoryWorld w(parties.originalPool);
 assert(w.vm->memory.unregisterRam(0x802c1070));
 EventScriptAssets data;assert(data.load("romfs/door-data.pmevt")&&w.vm->memory.importScripts(data));
 assert(game->initializeOriginalFieldNpcs(w.vm->memory,0x85000000));
 auto binding=game->bindOriginalBattleSequence(*w.vm,"romfs");assert(binding&&binding->attachAnimationPool());
 auto retained=w.pool;assert(retained->sourceArraysCurrent());
 w.vm.reset();assert(!retained->sourceArraysCurrent()); // Guard must reject before its first VM read.
 binding.reset();assert(retained->sourceArraysCurrent()); // Metadata detached, no stale guard/userdata.
}
void replacementMappings(){
 Fixture f;assert(f.vm->memory.unregisterExternalRam(0x8041e808));
 std::vector<uint8_t> replacement(20);assert(f.vm->memory.registerRam(0x8041e808,std::move(replacement)));f.stale();f.binding.reset();
 uint32_t value=1;assert(f.vm->memory.readWord(0x8041e808,value)&&value==0);
}
}
int main(){allocationAndSourceContext();facadeLifetimes();attachedAnimationVmDeath();replacementMappings();std::cout<<"source nested-pool adoption tests PASS\n";return 0;}
