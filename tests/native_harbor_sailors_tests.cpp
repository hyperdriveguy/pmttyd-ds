#include "native_harbor_sailors.hpp"
#include "field_npc_creation_binding.hpp"
#include <map>
#include "event_memory_bootstrap.hpp"
#include "field_party_pose.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <cstdio>
using namespace ttyd;
namespace {
struct World {
 EventNativeRuntime vm{0x81000000,platform(this)};
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();
 std::shared_ptr<NativeFieldAnimation> animation;
 std::shared_ptr<EventScriptAssets> source=std::make_shared<EventScriptAssets>();
 std::shared_ptr<FieldNpcRegistryOwner> owner;
 std::unique_ptr<FieldNpcRegistryBinding> registry;
 std::unique_ptr<NativeHarborSailors> sailors;
 std::unique_ptr<FieldNpcSetupBinding> setup;std::unique_ptr<FieldNpcCreationBinding> creation;
 std::map<uint32_t,FieldNpcActorEventCursor> actorCursors;uint32_t cutsceneRoot=0;bool returned=false;
 static EventNativePlatform platform(World* w=nullptr){EventNativePlatform p;p.user=w;p.clock=[](void*)->uint32_t{return 243000000;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){return int32_t(f);};p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};p.call=[](void* p,EventEntry&e,uint32_t fn,int32_t first){auto&w=*static_cast<World*>(p);if(w.creation&&fieldNpcCreationEventFunction(fn)){const auto result=w.creation->invoke(fn,e);return result?*result:0;}if(w.setup&&fieldNpcActorEventFunction(fn))return w.setup->invokeActorEvent(fn,e,first!=0,w.actorCursors[e.id]);return int32_t(0);};p.retiring=[](void*p,const EventEntry&e){auto&w=*static_cast<World*>(p);if(w.sailors)w.sailors->regularHistory()->retire(e);if(w.creation)w.creation->retire(e);if(e.id==w.cutsceneRoot)w.returned=e.opcode==2;};return p;}
 explicit World(const char*tags){
  animation=NativeFieldAnimation::create("romfs",files,platform(this));assert(animation);
  EventScriptAssets harbor,creationAssets;
  assert(source->load("romfs/door-data.pmevt")&&harbor.load("romfs/harbor-events.pmevt")&&creationAssets.load("romfs/field-npc-creation.pmevt"));
  assert(source->merge(harbor)&&source->merge(creationAssets)&&vm.memory.importScripts(*source));
  for(auto span:{std::pair<uint32_t,uint32_t>{0x8041f070,16},{0x8041e428,4},{0x82000000,0x80000},{0x8041e844,4}})assert(vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
  const auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,m);assert(eventSdkCreateHeap(start,0x82080000,m)==0&&vm.memory.writeWord(0x8041e844,0));vm.gp.refreshRate=60;vm.initializeContexts();
  owner=FieldNpcRegistryOwner::cold(0x85000000,animation->pool());registry=FieldNpcRegistryBinding::bind(owner,vm.memory);assert(registry&&registry->initialize());
  for(auto name:{"c_gara","c_suifu_b","c_pkr","c_kanbu_ibt","c_zako_n"}){const auto path=std::string("romfs/field-native-")+name;auto r=loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());assert(r&&owner->poses().install(r));}
  FieldMapNpcAssets map;assert(map.load(tags));
  {EventNativeRuntime foreign(0x81100000,platform());auto foreignRegistry=FieldNpcRegistryBinding::bind(owner,foreign.memory);assert(foreignRegistry);assert(!NativeHarborSailors::prepare(vm,*foreignRegistry,owner,animation,source,map,0x87000000,0x8041fa70));assert(vm.memory.unmappedRange(0x87000000,17*392));}
  sailors=NativeHarborSailors::prepare(vm,*registry,owner,animation,source,map,0x87000000,0x8041fa70);assert(sailors);
  assert(vm.memory.writeWord(0x81000014,1));assert(!sailors->advance()&&vm.runtime.allocationCount==0);assert(vm.memory.writeWord(0x81000014,0));
  FieldNpcSetupNativeServices services;services.user=this;services.clock=[](void*p){BattlePoseClock c;assert(static_cast<World*>(p)->animation->clock(c));return c;};services.admitBattle=[](void*,EventAddressSpace&,uint32_t,int32_t){return false;};services.random=[](void*,int32_t)->int32_t{assert(false);return 0;};
  setup=FieldNpcSetupBinding::bind(owner,*registry,vm,source,services);assert(setup);creation=FieldNpcCreationBinding::bind(owner,*registry,*setup,vm,animation);assert(creation);
 }
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm.memory.readWord(a,v));return v;}
};
}
int main(int argc,char**argv){
 assert(argc==2);World w(argv[1]);unsigned holds=0;
 for(unsigned i=0;i<10000&&!w.sailors->ready();++i){w.animation->files()->advanceAsync(4096);if(!w.sailors->advance())++holds;}
 assert(w.sailors->ready()&&holds);const auto count=w.vm.runtime.allocationCount;assert(count==2);
 for(unsigned i=0;i<2;++i){const auto actor=w.sailors->actor(i),id=w.sailors->regularEvent(i);assert(actor&&id);assert(w.word(*actor+0x11c)==*id);assert(w.word(*actor+0x124)==(i?0x90503040u:0x90501ca0u));assert(w.sailors->regularHistory()->owns(*id,*actor));
  uint8_t ordinal=0;assert(w.vm.memory.readByte(*actor+0x314,ordinal)&&ordinal==(i?12:2));
  FieldMapNpcAssets tags;assert(tags.load(argv[1]));const auto input=tags.input(i?12:2,0x87000000,0x8041fa70);assert(input);for(unsigned axis=0;axis<3;++axis)assert(w.word(*actor+0x8c+4*axis)==w.word(input->position+4*axis));
  for(unsigned j=0;j<200;j+=4)assert(w.word(*actor+0x230+j)==0);}
 assert(w.sailors->advance()&&w.vm.runtime.allocationCount==count);
 // The original four-actor Entry child executes alongside real held sailor
 // regular contexts. Unknown regular providers return a real hold, never END.
 auto& root=createRootEvent(w.vm.schedule,0x90505208,0,0,w.vm.creationServices());w.cutsceneRoot=root.id;
 for(unsigned i=0;i<10000&&!w.returned;++i){w.creation->pump();if(w.creation->pending()&&!w.creation->resume())continue;w.vm.gp.timestamp=uint64_t(i)*17*eventClockRate(243000000);w.vm.runManager();}
 assert(w.returned&&w.registry->activeActors()->size()==6);
 for(auto name:{0x90401028u,0x90401040u,0x9040105cu,0x9040106cu}){const auto actor=w.registry->lookup(name);assert(actor&&*actor&&w.word(*actor+0x28));}
 const auto id=*w.sailors->regularEvent(0);EventEntry* e=nullptr;for(int i=0;i<w.vm.field.capacity;++i)if(w.vm.field.entries[i].id==id&&(w.vm.field.entries[i].flags&1))e=&w.vm.field.entries[i];assert(e);w.sailors->regularHistory()->retire(*e);deleteEvent(*e,w.vm.creationServices());assert(w.sailors->regularEvent(0)==id);
 // Source-mode replacement rejects before any additional constructor work.
 assert(w.vm.memory.writeWord(0x81000014,1));assert(!w.sailors->advance()&&!w.sailors->ready());
 std::puts("Actual native source Entry/TPL waits, original sailor metadata, real regular IDs and history pass");return 0;
}
