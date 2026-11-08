#include "harbor_source_unit_construction.hpp"
#include "harbor_source_startup.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_event_callbacks.hpp"
#include "event_memory_bootstrap.hpp"
#include "prologue_pouch.hpp"
#include "fixtures/host_event_cache.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t SystemDefinition=0x80416a80,MarioDefinition=0x80416a50;
constexpr uint32_t BattleCell=0x8041eb90,Registry=0x20,PartList=0x14,PartStride=0x500;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};
 test::HostEventCache publication{vm};
 std::unique_ptr<HarborSourceMapPool> pool;
 std::shared_ptr<BattleMenuFileCache> files=std::make_shared<BattleMenuFileCache>();
 std::unique_ptr<HarborSourceStartup> startup;
 std::unique_ptr<HarborSourceUnitConstruction> construction;
 bool alive=true,probeReentry=false;unsigned reentries=0;
 uint32_t battle=0;
 Fixture(){
  EventScriptAssets source;assert(source.load("romfs/harbor-tutorial-events.pmevt") && vm.memory.importScripts(source));
  // Explicit host arithmetic over the actual source operands; no source child
  // completion, original animation or native display is manufactured.
  vm.platform.fmodDouble=[](void*,double x,double y){return std::fmod(x,y);};
  vm.platform.roundDoubleFloat=[](void*,double x){return float(x);};
  vm.platform.subtractDouble=[](void*,double x,double y){return x-y;};
  vm.platform.addFloat=[](void*,float x,float y){return x+y;};
  assert(vm.memory.registerRam(BattleCell,std::vector<uint8_t>(4)));
  constexpr uint32_t Information=0x85000000,Pouch=0x85010000,PouchCell=0x8041eb00;
  assert(vm.memory.registerRam(Information,std::vector<uint8_t>(16)));
  assert(loadProloguePouchAssets("romfs"));const auto pouch=createProloguePouch();assert(pouch);
  assert(vm.memory.registerRam(Pouch,{pouch->bytes.begin(),pouch->bytes.end()}));
  assert(vm.memory.registerRam(PouchCell,std::vector<uint8_t>(4)));put(PouchCell,Pouch);
  pool=HarborSourceMapPool::bind(vm.memory,vm.memoryBinding.services());assert(pool);publication.requireBattlePoolPublication();
  // Original EventEntryStorage allocates both contexts through heap0. Supply
  // an actual SDK heap, not detached semantic entry arrays or invented handles.
  constexpr uint32_t HeapStart=0x82000000,HeapEnd=0x82100000;
  constexpr uint32_t HeapDescriptors=0x8041f070,CurrentHeap=0x8041e428,HeapHandles=0x8041e844;
  assert(vm.memory.registerRam(HeapStart,std::vector<uint8_t>(HeapEnd-HeapStart)));
  assert(vm.memory.registerRam(HeapDescriptors,std::vector<uint8_t>(16)));
  assert(vm.memory.registerRam(CurrentHeap,std::vector<uint8_t>(4)));
  assert(vm.memory.registerRam(HeapHandles,std::vector<uint8_t>(4)));
  auto memory=vm.memoryBinding.services();const auto usable=eventSdkInitializeAllocator(HeapStart,HeapEnd,1,memory);
  const auto heap=eventSdkCreateHeap(usable,HeapEnd,memory);assert(heap==0);put(HeapHandles,uint32_t(heap));
  vm.initializeContexts();vm.gp.battleContext=1;
  assert(files->load("romfs/battle-class-common.tpl") && files->bind(vm.memory));
  BattleMenuFileBinding menu(files,nativeBattleActorServices(vm));
  HarborSourceStartupBinding cold;cold.vm=&vm;cold.pool=pool.get();cold.menu=menu.services();
  cold.lifetime=std::make_shared<const uint8_t>(0);cold.current=[this]{return alive;};
  startup=HarborSourceStartup::create(cold);assert(startup);
  // Exact real missing stage child; preceding original cold stores completed.
  assert(!startup->resumeCold(Information) && startup->pending()==0x80139780);
  battle=startup->battleAddress();assert(battle && get(BattleCell)==battle);
  HarborSourceUnitConstructionBinding binding;binding.vm=&vm;binding.pool=pool.get();binding.battle=battle;
  binding.lifetime=std::make_shared<const uint8_t>(0);
  binding.current=[this]{
   if(probeReentry && construction){probeReentry=false;++reentries;assert(!construction->resume(SystemDefinition));assert(!construction->next());}
   return alive;
  };
  construction=HarborSourceUnitConstruction::create(binding);assert(construction);
 }
 uint32_t get(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void put(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}
 std::vector<uint8_t> snapshot(){
  std::vector<uint8_t> bytes(HarborSourceMapPool::ArenaBytes);
  for(uint32_t i=0;i<bytes.size();++i)assert(vm.memory.readByte(HarborSourceMapPool::ArenaAddress+i,bytes[i]));
  return bytes;
 }
 void heldMario(){
  const auto system=construction->resume(SystemDefinition);assert(system && *system && get(*system+8)==220);
  assert(get(battle+Registry)==*system && get(*system)==0);
  for(auto part=get(*system+PartList);part;part=get(part))assert(get(part+0x1c0)==0xffffffff);
  // Actual source init event root, not a synthetic unit-id acknowledgement.
  assert(get(*system+0x288)!=0 && vm.runtime.allocationCount==1 && vm.schedule.count==0);
  const auto* entry=vm.battle.entries;assert(entry && entry->id==get(*system+0x288) && (entry->flags&1) && entry->owner==0 && entry->priority==10);
  const auto before=snapshot();assert(construction->resume(SystemDefinition)==system && snapshot()==before);
  assert(construction->next());probeReentry=true;
  assert(!construction->resume(MarioDefinition));assert(reentries==1 && construction->pending()==0x8004176c);
  const auto mario=get(battle+Registry+4);assert(mario && get(mario+8)==222 && get(mario)==1);
  const auto parts=get(mario+PartList);assert(parts && get(parts)==parts+PartStride && get(parts+PartStride)==parts+2*PartStride);
  assert(get(parts+2*PartStride)==0 && get(mario+0x288)==0 && vm.runtime.allocationCount==1 && vm.schedule.count==0);
  assert(pool->allocationLifetime(mario) && pool->allocationLifetime(parts));
  const auto held=snapshot();for(unsigned i=0;i<3;++i){assert(!construction->resume(MarioDefinition));assert(construction->pending()==0x8004176c && snapshot()==held);}
  assert(!construction->next() && !construction->resume(SystemDefinition) && snapshot()==held);
 }
};
void sharedMemory(){
 auto sourceVm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});
 auto targetVm=std::make_unique<EventNativeRuntime>(0x83000000,EventNativePlatform{});
 auto& source=sourceVm->memory;auto& target=targetVm->memory;constexpr uint32_t Name=0x82000000;
 assert(source.registerRam(Name,{'b','_','m','a','r','i','o',0}));
 assert(!source.shareRamTo(source,Name,8) && !source.shareRamTo(target,Name,0));
 assert(source.shareRamTo(target,Name,8) && source.sharesRamWith(target,Name,8));
 assert(!source.shareRamTo(target,Name,8));
 assert(target.writeByte(Name,'B'));uint8_t value;assert(source.readByte(Name,value) && value=='B');
 assert(source.writeByte(Name,'b') && target.readByte(Name,value) && value=='b');
 assert(target.unregisterExternalRam(Name));assert(target.registerRam(Name,{'b','_','m','a','r','i','o',0}));
 assert(!source.sharesRamWith(target,Name,8)); // Equal bytes do not prove backing identity.
 assert(!source.shareRamTo(target,Name,8));
 assert(!source.shareRamTo(target,UINT32_MAX,8));
}
}
int main(){
 sharedMemory();
 {auto owner=std::make_unique<Fixture>();auto& f=*owner;f.heldMario();const auto held=f.snapshot();f.alive=false;assert(!f.construction->resume(MarioDefinition));f.alive=true;assert(!f.construction->resume(MarioDefinition) && !f.construction->current() && f.snapshot()==held);}
 {auto owner=std::make_unique<Fixture>();auto& f=*owner;f.heldMario();const auto held=f.snapshot();f.put(BattleCell,f.battle+32);assert(!f.construction->resume(MarioDefinition));f.put(BattleCell,f.battle);assert(!f.construction->resume(MarioDefinition) && f.snapshot()==held);}
 {auto owner=std::make_unique<Fixture>();auto& f=*owner;f.heldMario();const auto mario=f.get(f.battle+Registry+4);assert(f.pool->free(f.pool->root(),mario));assert(!f.construction->resume(MarioDefinition) && !f.construction->current());}
 std::cout<<"actual source system220 construction/init, retained Mario animation frontier, no replay/reentry and backing/lifetime guards PASS\n";
}
