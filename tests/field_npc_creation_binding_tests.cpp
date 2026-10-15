#include "field_npc_creation_binding.hpp"
#include "field_party_pose.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <map>
#include <tuple>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Root=0x90505208,Entry=0x800536e4,Tribe=0x8004e5c0,Position=0x80053158;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
struct World {
 EventNativeRuntime vm{0x81000000,platform(this)};
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();
 std::shared_ptr<NativeFieldAnimation> animation;
 std::shared_ptr<EventScriptAssets> source=std::make_shared<EventScriptAssets>();EventScriptAssets creationSource;
 std::shared_ptr<FieldNpcRegistryOwner> owner;std::unique_ptr<FieldNpcRegistryBinding> registry;
 std::unique_ptr<FieldNpcSetupBinding> setup;std::unique_ptr<FieldNpcCreationBinding> creation;
 std::map<uint32_t,FieldNpcActorEventCursor> actors;
 std::array<int32_t,3> mario{};std::array<std::array<uint8_t,0x170>,3> marioState{};
 std::vector<std::tuple<uint32_t,int32_t,unsigned>> completed;
 unsigned visits=0,polls=0,holds=0,barriers=0;bool returned=false;uint32_t root=0;
 static EventNativePlatform platform(World* w){
  EventNativePlatform p;p.user=w;p.clock=[](void*)->uint32_t{return 243000000;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){return int32_t(f);};p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
  p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){return static_cast<World*>(p)->call(e,fn,first!=0);};
  p.retiring=[](void* p,const EventEntry& e){auto& w=*static_cast<World*>(p);w.creation->retire(e);if(e.id==w.root)w.returned=e.opcode==2;};return p;
 }
 explicit World(const char* rootPath){
  auto platform=World::platform(this);animation=NativeFieldAnimation::create(rootPath,files,platform);assert(animation && loadFieldPlayerPoseAssets(rootPath));
  for(unsigned n=0;n<3;++n){auto r=fieldPlayerPoseResources()[n];auto h=animation->pool()->enter(r,0,{},r->modelName.c_str());assert(h);mario[n]=*h;marioState[n]=animation->pool()->pose(*h)->state;}
  assert(source->load((std::string(rootPath)+"/door-data.pmevt").c_str()) && vm.memory.importScripts(*source));
  assert(creationSource.load((std::string(rootPath)+"/field-npc-creation.pmevt").c_str()) && vm.memory.importScripts(creationSource));
  for(auto span:{std::pair<uint32_t,uint32_t>{0x8041f070,16},{0x8041e428,4},{0x82000000,0x80000},{0x8041e844,4}})assert(vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
  auto memory=vm.memoryBinding.services();auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,memory);assert(eventSdkCreateHeap(start,0x82080000,memory)==0 && vm.memory.writeWord(0x8041e844,0));vm.gp.refreshRate=60;vm.initializeContexts();
  owner=FieldNpcRegistryOwner::cold(Base,animation->pool());registry=FieldNpcRegistryBinding::bind(owner,vm.memory);assert(registry && registry->initialize());
  for(auto name:{"c_pkr","c_kanbu_ibt","c_zako_n","hoshi"}){const auto path=std::string(rootPath)+"/field-native-"+name;auto r=loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());assert(r && owner->poses().install(r));}
  FieldNpcSetupNativeServices services;services.clock=[](void*){return BattlePoseClock{};};services.admitBattle=[](void*,EventAddressSpace&,uint32_t,int32_t){assert(false);return false;};services.random=[](void*,int32_t)->int32_t{assert(false);return 0;};
  setup=FieldNpcSetupBinding::bind(owner,*registry,vm,source,services);assert(setup);
  creation=FieldNpcCreationBinding::bind(owner,*registry,*setup,vm,animation);assert(creation);creation->budgets(64,1);
 }
 ~World(){vm.reinitializeContext();}
 int32_t call(EventEntry& e,uint32_t fn,bool first){
  if(fieldNpcCreationEventFunction(fn)){
   auto r=creation->invoke(fn,e);if(!r){++holds;return 0;}
   if(*r==0)++polls;else{completed.emplace_back(fn,*r,visits);if(fn==Entry){const auto original=word(vm.memory,e.arguments+4);assert(animation->process().entryName()==original);}}
   return *r;
  }
  assert(fn==Position);auto& cursor=actors[e.id];auto r=setup->invokeActorEvent(fn,e,first,cursor);if(r==2){completed.emplace_back(fn,r,visits);actors.erase(e.id);}return r;
 }
 void tick(){
  ++visits;creation->pump(4096);
  if(creation->pending()){
   if(owner->entryPending()){
    ++barriers;assert(!registry->activeActors() && !registry->reset(0));
    for(unsigned n=0;n<3;++n)assert(animation->pool()->pose(mario[n])->state==marioState[n]);
   }
   if(!creation->resume())return;
  }
  vm.gp.timestamp=uint64_t(visits)*17*eventClockRate(243000000);vm.runManager();
 }
};
void original(const char* root){
 World w(root);auto& e=createRootEvent(w.vm.schedule,Root,0,0,w.vm.creationServices());w.root=e.id;
 for(unsigned n=0;n<5000 && checkEventId(w.root,w.vm.creationServices());++n)w.tick();
 assert(w.returned && !checkEventId(w.root,w.vm.creationServices()) && !w.creation->pending() && !w.creation->quarantined());assert(w.polls && w.holds && w.barriers && w.completed.size()==12);
 const uint32_t names[]={0x90401028,0x90401040,0x9040105c,0x9040106c},tribes[]={0x803316d0,0x80332690,0x803326e4,0x803326e4};
 for(unsigned n=0;n<4;++n){
  assert(std::get<0>(w.completed[2*n])==Entry && std::get<1>(w.completed[2*n])==2 && std::get<0>(w.completed[2*n+1])==Tribe && std::get<1>(w.completed[2*n+1])==1);if(n<3)assert(std::get<2>(w.completed[2*n+2])>std::get<2>(w.completed[2*n+1]));
  assert(std::get<0>(w.completed[8+n])==Position && std::get<1>(w.completed[8+n])==2);
  auto actor=w.registry->lookup(names[n]);assert(actor && *actor==Base+n*0x340);auto& m=w.vm.memory;assert(word(m,*actor+0x28)==tribes[n] && word(m,*actor)==3);
  assert(word(m,*actor+0x8c)==0 && word(m,*actor+0x90)==0xc47a0000 && word(m,*actor+0x94)==0 && word(m,*actor+0x9c)==0xc47a0000);
  assert(!word(m,*actor+0x118) && !word(m,*actor+0x11c) && !word(m,*actor+0x124));
  const auto handle=word(m,*actor+0x104);const auto* p=w.animation->pool()->pose(int32_t(handle));assert(p && handle==n+3 && p->sourceBuffers);assert(p->resource->names.at(p->word(0x14))==(n==0?"PKR_S_1":"S_1"));assert(p->word(0x3c)==0xffffffff && p->word(0x40)==0 && p->word(0x80)==0 && p->word(0x84)==0);
  assert(word(m,*actor+0x14c)==word(m,tribes[n]+0x28) && word(m,*actor+0x150)==word(m,tribes[n]+0x2c) && half(m,*actor+0x1d8)==1);
 }
 assert(word(w.vm.memory,0x8041e8b4)==4);for(unsigned n=0;n<3;++n)assert(w.animation->pool()->pose(w.mario[n])->state==w.marioState[n]);
 auto all=w.registry->activeActors();assert(all && all->size()==4);
 assert(w.owner->aliases(w.animation->process().vm().memory));
 for(auto pair:{std::pair<uint32_t,uint32_t>{0x90401038,6},{0x90401050,12},{0x90400268,9}})assert(w.vm.memory.sharesRamWith(w.animation->process().vm().memory,pair.first,pair.second));
}

void retiredSetter(const char* root){
 World w(root);auto& e=createRootEvent(w.vm.schedule,Root,0,0,w.vm.creationServices());w.root=e.id;
 for(unsigned n=0;n<2000 && !w.animation->process().selectionPending();++n)w.tick();
 assert(w.creation->pending() && w.owner->entryPending() && w.animation->process().selectionPending());
 deleteEventId(w.root,w.vm.creationServices());assert(w.creation->quarantined() && !w.creation->resume() && w.owner->entryPending() && !w.registry->reset(0));
}
}
int main(){original("romfs");retiredSetter("romfs");}
