#include "field_npc_registry_owner.hpp"
#include "native_field_animation.hpp"
#include "field_party_pose.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_setup_binding.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Models=0x86000000;
struct NativeWorld {
 const char* root;
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();
 std::shared_ptr<NativeFieldAnimation> animation;
 bool yieldReady=true;
 std::array<int32_t,3> marioHandles{};
 std::array<std::array<uint8_t,0x170>,3> marioState{};
 std::unique_ptr<EventNativeRuntime> vm;
 std::shared_ptr<FieldNpcRegistryOwner> owner;
 std::unique_ptr<FieldNpcRegistryBinding> registry,animationRegistry;
 std::shared_ptr<EventScriptAssets> source=std::make_shared<EventScriptAssets>();
 std::unique_ptr<FieldNpcSetupBinding> setup;
 std::unique_ptr<FieldNpcMainEntryBinding> main;
 std::unique_ptr<FieldNpcClockBinding> clock;
 EventEntry* caller=nullptr;
 uint32_t name=0;
 explicit NativeWorld(const char* r):root(r){
  assert(files->yieldProvider(this,[](void* p){return static_cast<NativeWorld*>(p)->yieldReady;}));
  EventNativePlatform p;p.clock=[](void*)->uint32_t{return 243000000;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
  animation=NativeFieldAnimation::create(root,files,p);assert(animation);assert(loadFieldPlayerPoseAssets(root));
  for(unsigned n=0;n<3;++n){auto resource=fieldPlayerPoseResources()[n];auto h=animation->pool()->enter(resource,0,{},resource->modelName.c_str());assert(h);marioHandles[n]=*h;marioState[n]=animation->pool()->pose(*h)->state;}
  vm=std::make_unique<EventNativeRuntime>(0x81000000,p);
  assert(source->load((std::string(root)+"/door-data.pmevt").c_str()) && vm->memory.importScripts(*source));
  for(auto span:{std::pair<uint32_t,uint32_t>{0x8041f070,16},{0x8041e428,4},{0x82000000,0x80000},{0x8041e844,4}})assert(vm->memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
  const auto memory=vm->memoryBinding.services();const auto begin=eventSdkInitializeAllocator(0x82000000,0x82080000,1,memory);assert(eventSdkCreateHeap(begin,0x82080000,memory)==0);store(0x8041e844,0);vm->gp.refreshRate=60;vm->initializeContexts();
  // Explicit constructor fixture: reuse a genuine source name with each model.
  // No original harbor callback/story execution is claimed by these calls.
  assert(vm->memory.readWord(0x90524f10+0x5c,name));
  owner=FieldNpcRegistryOwner::cold(Base,animation->pool());
  registry=FieldNpcRegistryBinding::bind(owner,vm->memory);assert(registry && registry->initialize());
  const char* models[]={"c_pkr","c_kanbu_ibt","c_zako_n","c_gara","c_suifu_b","hoshi"};
  for(unsigned i=0;i<6;++i){
   const auto path=std::string(root)+"/field-native-"+models[i];
   auto resource=loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());
   assert(resource && owner->poses().install(resource));std::vector<uint8_t> model(64);
   for(unsigned n=0;n<64;++n)model[n]=resource->draw->byte(4+n);
   assert(vm->memory.registerRam(Models+64*i,std::move(model)));
  }
  FieldNpcSetupNativeServices setupServices;setupServices.clock=[](void*){return BattlePoseClock{};};setupServices.admitBattle=[](void*,EventAddressSpace&,uint32_t,int32_t){return false;};setupServices.random=[](void*,int32_t)->int32_t{assert(false);return 0;};
  setup=FieldNpcSetupBinding::bind(owner,*registry,*vm,source,setupServices);assert(setup);
  caller=&createRootEvent(vm->schedule,0x80415e18,0,0,vm->creationServices());caller->field170=Base;
  assert(vm->memory.registerRam(Models+0x800,{'m','e',0,0,0,0,0,0,0,0,0,0,0,0,0,0}));
  animationRegistry=FieldNpcRegistryBinding::bind(owner,animation->process().vm().memory);assert(animationRegistry);
  FieldNpcMainEntryNativeServices mainServices;mainServices.numbers={nullptr,nativeFieldNpcClockDivide,nativeFieldNpcClockConvert};mainServices.admitMario=[](void*,EventAddressSpace&,uint32_t){assert(false);return false;};
  main=FieldNpcMainEntryBinding::bind(owner,*animationRegistry,animation->process().vm(),*animation->process().animationMemory(),mainServices);assert(main);
  clock=FieldNpcClockBinding::bind(owner,*registry,*vm,mainServices.numbers);assert(clock);
 }
 ~NativeWorld(){vm->reinitializeContext();}
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 void store(uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));}
 uint32_t nativeWord(uint32_t a){uint32_t v=0;assert(animation->process().vm().memory.readWord(a,v));return v;}
 void nativeStore(uint32_t a,uint32_t v){assert(animation->process().vm().memory.writeWord(a,v));}
 uint32_t holdModel(const char* name){const auto h=files->acquire(std::string("a/")+name,5);assert(h);nativeStore(*h+0xac,1);yieldReady=false;return *h;}
 void releaseModel(uint32_t h){nativeStore(h+0xac,0);yieldReady=true;}
};
void actualNativeEntry(const char* root){
 NativeWorld w(root);FieldNpcEntryCursor cursor;const auto held=w.holdModel("c_pkr");
 assert(!w.registry->resumeEnter(w.name,Models,{},cursor));
 assert(cursor.pending() && w.owner->entryPending() && w.word(Base)==3 && !w.word(Base+0x104) && !w.word(0x8041e8b4));
 assert(w.animation->entryPending() && !w.animation->producerPending() && !w.animation->tick(false) && !w.animation->pool()->cacheMetadataReady()); // Actual partially published source model cache.
 w.store(Base+0x200,0x12345678);
 EventNativeRuntime other(0x82000000,{});EventScriptAssets data;
 assert(data.load((std::string(root)+"/door-data.pmevt").c_str()) && other.memory.importScripts(data));
 auto facade=FieldNpcRegistryBinding::bind(w.owner,other.memory);assert(facade);
 for(unsigned n=0;n<5;++n){
  assert(!w.registry->resumeEnter(w.name,Models,{},cursor) && w.word(Base+0x200)==0x12345678);
  assert(!facade->resumeEnter(w.name,Models,{},cursor) && !facade->lookup(w.name) && !facade->activeActors() && !facade->reset(0));
  assert(!w.registry->enter(w.name,Models+64,{}) && !w.registry->activeActors() && !w.registry->reset(1));
  FieldNpcMainEntryCursor scan;FieldNpcMainActorsCursor actors;FieldNpcMainCursor whole;FieldNpcDrawCursor draw;FieldNpcClockCursor clock;
  assert(w.main->advance(scan)==FieldNpcMainFrontier::Pending && w.main->advanceActors(actors)==FieldNpcActorsFrontier::Pending && !w.main->advanceMain(whole));
  assert(!w.main->advanceDraw(0x80054518,Base,draw) && !w.clock->advance(Base,clock));
  assert(!scan.active && !actors.binding && !whole.binding && !draw.active && !clock.active);
  assert(!w.setup->resolveEventActor(*w.caller,Models+0x800,true));
  w.caller->arguments=Models+0x804;w.store(Models+0x804,1);w.store(Models+0x808,Models+0x800);w.store(Models+0x80c,0x40000000);
  FieldNpcActorEventCursor leaf;assert(w.setup->invokeActorEvent(0x800500b0,*w.caller,true,leaf)==0 && !leaf.initialized);
  // Explicit already-admitted parent fixture: no callback may be reached while
  // another constructor owns the shared registry. Poison leaves prove ordering.
  FieldNpcSetupCursor parent;parent.initialized=true;parent.sourceFirst=true;parent.stage=FieldNpcSetupCursor::Stage::Metadata;
  parent.owner=w.setup.get();parent.memoryOwner=&w;parent.eventOwner=&w;parent.entry=w.caller;parent.eventId=w.caller->id;parent.actor=Base;
  FieldNpcSetupOperands operands{&w,[](void*,uint32_t)->uint32_t{assert(false);return 0;},[](void*,EventEntry&,int32_t)->int32_t{assert(false);return 0;}};
  FieldNpcSetupEvents events{&w,[](void*,uint32_t){assert(false);},[](void*,uint32_t){assert(false);return false;},[](void*,uint32_t,uint8_t,uint8_t)->EventEntry*{assert(false);return nullptr;}};
  assert(w.setup->invoke(*w.caller,false,parent,operands,events)==0 && parent.stage==FieldNpcSetupCursor::Stage::Metadata);
  assert(w.word(Base)==3 && w.word(Base+0x200)==0x12345678 && !w.word(Base+0x104));
 }
 assert(!w.registry->resumeEnter(w.name,Models+64,{},cursor));
 w.store(0x81000038,0x11223344);w.store(0x8100003c,0x55667788);w.releaseModel(held);
 assert(w.registry->resumeEnter(w.name,Models,{},cursor)==0);
 assert(cursor.complete() && !cursor.pending() && !w.owner->entryPending() && w.animation->pool()->cacheMetadataReady());
 assert(w.word(Base+0x200)==0x12345678 && w.word(Base+0x198)==0x11223344 && w.word(Base+0x19c)==0x55667788 && w.word(0x8041e8b4)==1);
 const auto handle=int32_t(w.word(Base+0x104));const auto* pose=w.animation->pool()->pose(handle);
 assert(pose && pose->sourceBuffers && pose->resource->modelName=="c_pkr" && pose->word(0)==9 && pose->word(0xec)==2);
 assert(w.registry->lookup(w.name)==Base && facade->lookup(w.name)==Base);
 assert(w.setup->resolveEventActor(*w.caller,Models+0x800,true)==Base);
 FieldNpcActorEventCursor leaf;assert(w.setup->invokeActorEvent(0x800500b0,*w.caller,true,leaf)==2 && (w.word(Base)&0x40000000));
 assert(w.registry->resumeEnter(w.name,Models,{},cursor)==0 && w.word(0x8041e8b4)==1);
 // Ordinary API also retains its implicit constructor cursor across file holds.
 const auto crump=w.holdModel("c_kanbu_ibt");assert(!w.registry->enter(w.name,Models+64,{}) && w.owner->entryPending());
 w.store(Base+0x340+0x200,0xabcdef01);
 assert(!w.registry->enter(w.name,Models+64,{}) && w.word(Base+0x340+0x200)==0xabcdef01);
 w.releaseModel(crump);assert(w.registry->enter(w.name,Models+64,{})==1 && !w.owner->entryPending());
 assert(w.word(Base+0x340+0x200)==0xabcdef01 && w.word(0x8041e8b4)==2);
 for(unsigned n=2;n<5;++n)assert(w.registry->enter(w.name,Models+64*n,{})==int32_t(n));
 assert(w.owner->poses().poses().models()[0].references()==1 && w.registry->activeActors()->size()==5);
 for(unsigned n=0;n<3;++n)assert(w.animation->pool()->pose(w.marioHandles[n])->state==w.marioState[n]);
}
void competingProducer(const char* root){
 NativeWorld w(root);const auto h=uint32_t(w.marioHandles[0]);
 AnimationFacingAddressPolicy addresses;addresses.base=0x81900000;addresses.unmapped=[&](uint32_t a,uint32_t n){return w.animation->process().vm().memory.unmappedRange(a,n);};
 assert(w.animation->process().configureMain({},addresses,[](uint64_t pass){return pass==37;}));
 assert(!w.animation->process().main(h,37,1) && w.animation->producerPending());
 FieldNpcEntryCursor cursor;assert(!w.registry->resumeEnter(w.name,Models,{},cursor) && !cursor.state);
 assert(!w.owner->entryPending() && !w.word(Base) && !w.word(0x8041e8b4));
 bool completed=false;for(unsigned n=0;n<256 && !completed;++n)completed=w.animation->process().main(h,37,8192);assert(completed);
 assert(w.registry->resumeEnter(w.name,Models,{},cursor)==0 && !w.owner->entryPending());
}
void nativeSourceFallback(const char* root){
 NativeWorld w(root);const auto file=w.files->acquire("a/c_pkr",5);assert(file);
 const auto holder=w.nativeWord(*file+0xa0),original=w.nativeWord(holder);assert(original);
 // Explicit source failure fixture: a real acquired MODEL holder has no payload.
 // Actual animPoseEntry takes its original fileFree/-2 branch, then NPC retries hoshi.
 w.nativeStore(holder,0);FieldNpcEntryCursor cursor;
 assert(w.registry->resumeEnter(w.name,Models,{},cursor)==0 && cursor.complete());
 const auto* pose=w.animation->pool()->pose(int32_t(w.word(Base+0x104)));
 assert(pose && pose->resource->modelName=="hoshi" && pose->sourceBuffers && pose->word(0)==9 && pose->word(0xec)==2);
 assert(!w.owner->entryPending() && w.word(0x8041e8b4)==1 && w.animation->pool()->cacheMetadataReady());
 w.nativeStore(holder,original);
}
void abandonedEntryStaysPending(const char* root){
 NativeWorld w(root);w.holdModel("c_pkr");
 {FieldNpcEntryCursor cursor;assert(!w.registry->resumeEnter(w.name,Models,{},cursor) && cursor.pending());}
 // Caller loss cannot acknowledge cancellation of a partially entered pose.
 assert(w.owner->entryPending() && !w.registry->lookup(w.name) && !w.registry->reset(0));
 assert(!w.registry->enter(w.name,Models,{}) && !w.word(0x8041e8b4));
}
}
int main(){competingProducer("romfs");actualNativeEntry("romfs");nativeSourceFallback("romfs");abandonedEntryStaysPending("romfs");}
