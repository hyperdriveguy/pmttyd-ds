#include "field_map_source_owner.hpp"
#include "field_map_sdk.hpp"
#include "event_native_runtime.hpp"
#include "event_heap_allocation.hpp"
#include "native_field_animation.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
using namespace FieldMapSource;
constexpr uint32_t ProcessWork=0x86000000,Stack=0x87000000,Heap=0x88000000,Inputs=0x8b000000;
constexpr uint32_t OriginalScene=0x8a0d2b00,SceneCount=540,OriginalShipOrdinal=52;
constexpr uint32_t Parent=Inputs,LookupName=Inputs+0x100;
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t w=0;assert(m.readWord(a,w));return w;}
void text(EventAddressSpace& m,uint32_t a,const char* s){do{assert(m.writeByte(a++,uint8_t(*s)));}while(*s++);}
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};
 std::vector<uint8_t> heap=std::vector<uint8_t>(512*1024);
 bool processFlush=false,holdSdk=false;unsigned flushes=0,blocks=0,syscalls=0,sdkCalls=0;
 FieldMapSdk sdk{vm.memory,[this]{return owner && owner->current();}};
 std::unique_ptr<FieldMapSourceOwner> owner;
 explicit Fixture(const char* root){
  EventScriptAssets source;assert(source.load((std::string(root)+"/field-harbor-map-source.pmevt").c_str()) && vm.memory.importScripts(source));
  assert(vm.memory.registerExternalRam(Heap,heap.data(),uint32_t(heap.size())));
  assert(vm.memory.registerRam(PrimaryHeap,{0x88,0,0,0}));
  assert(vm.memory.writeWord(Heap,0) && vm.memory.writeWord(Heap+4,uint32_t(heap.size())) && vm.memory.writeHalf(Heap+8,0));
  assert(vm.memory.registerRam(Inputs,std::vector<uint8_t>(0x200)));
  FieldMapHeapOwner children;children.user=this;children.address=Heap;children.bytes=uint32_t(heap.size());children.backing=heap.data();
  children.flushProcess=[](void* u,uint8_t*,uint32_t size){auto& f=*static_cast<Fixture*>(u);assert(size==WorkBytes);++f.flushes;return f.processFlush;};
  children.flushBlock=[](void* u,uint32_t address){auto& f=*static_cast<Fixture*>(u);assert(address>=Heap && address<Heap+f.heap.size());++f.blocks;};
  children.systemCall=[](void* u){++static_cast<Fixture*>(u)->syscalls;};
  children.sdk=[](void* u,FieldMapSourceCall& q){auto& f=*static_cast<Fixture*>(u);++f.sdkCalls;return !f.holdSdk && f.sdk.advance(q);};
  owner=FieldMapSourceOwner::bind(vm.memory,ProcessWork,Stack,children);assert(owner);
 }
 std::optional<uint32_t> run(FieldMapRequest& request,uint32_t f,const std::array<uint32_t,5>& a={}){for(unsigned tries=0;tries<100000;++tries){auto result=owner->invoke(request,f,a,4096);if(result)return result;}assert(false);return {};}
};
}
void originalMapOwner(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapRequest request,other;
 assert(!f.owner->invoke(request,Init) && f.owner->pending());assert(word(m,Work)==0 && f.flushes==1);
 f.processFlush=true;assert(!f.owner->invoke(other,Init) && f.flushes==1);
 assert(f.run(request,Init) && word(m,Work)==ProcessWork && word(m,ActiveGroup)==0 && word(m,Auxiliary)==0 && f.flushes==2);
 FieldMapSourceCall scale;scale.function=0x8029e2b0;scale.arguments[0]=Parent;scale.scalars.fill(bits(10));assert(f.sdk.advance(scale));
 f.holdSdk=true;for(unsigned i=0;!f.sdkCalls && i<1000;++i)assert(!f.owner->invoke(request,Entry,{OriginalScene,Parent,0}));assert(f.sdkCalls && f.owner->pending());assert(word(m,ProcessWork+ObjectCount)==SceneCount && word(m,ProcessWork)==0);
 assert(word(m,ProcessWork+ObjectArray)==Heap+32 && f.blocks==0x289c0/32 && f.syscalls==1);
 assert(!f.owner->invoke(other,Entry,{OriginalScene,Parent,0}));assert(!f.owner->invoke(request,Find,{LookupName}));
 f.holdSdk=false;auto result=f.run(request,Entry,{0,0,99});assert(result && *result==Heap+32 && !f.owner->pending());assert(word(m,ProcessWork)==0);assert(f.blocks==0x289c0/32 && f.syscalls==1);
 const auto array=word(m,ProcessWork+ObjectArray);std::map<uint32_t,uint32_t> objects;
 for(unsigned i=0;i<SceneCount;++i){const auto object=array+i*ObjectBytes;const auto joint=word(m,object+Joint);assert(joint && objects.emplace(joint,object).second);}
 assert(objects.size()==SceneCount && objects.at(OriginalScene)==array);
 for(const auto& item:objects){const auto joint=item.first,object=item.second;const auto parent=word(m,joint+8),child=word(m,joint+0x0c),sibling=word(m,joint+0x10);assert(word(m,object+0xe0)==(joint==OriginalScene?0:objects.at(parent)));assert(word(m,object+Child)==(child?objects.at(child):0));assert(word(m,object+Sibling)==(joint!=OriginalScene && sibling?objects.at(sibling):0));}
 const auto ship=array+OriginalShipOrdinal*ObjectBytes;
 assert(word(m,ship+World)==bits(10) && word(m,ship+World+0x14)==bits(10) && word(m,ship+World+0x28)==bits(10));
 assert(word(m,ship+World+0x0c)==bits(210) && word(m,ship+World+0x1c)==bits(0) && word(m,ship+World+0x2c)==bits(45));
 assert(word(m,ship+Additional)==bits(1) && word(m,ship+Additional+0x14)==bits(1) && word(m,ship+Additional+0x28)==bits(1));
 text(m,LookupName,"S_fune_b");assert(f.run(request,Find,{LookupName})==0); // Full loader has not published loaded-map count.
 assert(m.writeWord(ProcessWork+Root,*result) && m.writeWord(ProcessWork,1)); // Explicit fixture of the later source loader publication, not part of mapEntry.
 assert(f.run(request,Find,{LookupName})==ship);
 assert(f.run(request,Recalculate,{*result,Parent,0}));assert(!(word(m,ship)&Dirty));
 assert(m.writeWord(ship+Additional+0x0c,bits(2)) && m.writeWord(ship,word(m,ship)|Dirty|HasAdditional));
 assert(f.run(request,Recalculate,{*result,Parent,0}));assert(word(m,ship+World+0x0c)==bits(230) && !(word(m,ship)&Dirty));
 auto square=[&](float x,float y,float z){for(unsigned i=0;i<3;++i)assert(m.writeWord(Inputs+0x80+i*4,bits(i==0?x:i==1?y:z)) && m.writeWord(Inputs+0x90+i*4,0));FieldMapSourceCall q;q.function=0x8029ee8c;q.arguments[0]=Inputs+0x80;q.arguments[1]=Inputs+0x90;assert(f.sdk.advance(q) && q.floatingResult);return *q.floatingResult;};
 assert(square(3,4,12)==0x43290000 && square(100,100,0)==0x469c4000 && square(90,100,0)==0x468d6800 && square(100,100,100)==0x46ea6000);
 std::vector<uint8_t> replacement(f.heap);assert(m.unregisterExternalRam(Heap) && m.registerExternalRam(Heap,replacement.data(),uint32_t(replacement.size())));assert(!f.owner->current() && !f.owner->invoke(request,Find,{LookupName}));f.owner.reset();assert(m.unregisterExternalRam(Heap,replacement.data()));
}

void mapBoundingThresholds(const char* root){
 // Synthetic single-joint inputs isolate the complete source constructor's
 // flag02 comparison. Actual 540-object original harbor coverage is above.
 for(const auto test:std::array<std::array<float,4>,3>{{{{10,10,0,0}},{{9,10,0,1}},{{10,10,10,0}}}}){
  Fixture f(root);f.processFlush=true;FieldMapRequest request;assert(f.run(request,Init));auto& m=f.vm.memory;
  assert(m.writeWord(OriginalScene+0x0c,0) && m.writeWord(OriginalScene+0x58,Inputs+0x180) && m.writeWord(OriginalScene+0x5c,1));
  assert(m.writeByte(Inputs+0x180,1) && m.writeByte(Inputs+0x182,0));
  for(unsigned i=0;i<3;++i)assert(m.writeWord(OriginalScene+0x3c+i*4,bits(-test[i])) && m.writeWord(OriginalScene+0x48+i*4,bits(test[i])));
  FieldMapSourceCall scale;scale.function=0x8029e2b0;scale.arguments[0]=Parent;scale.scalars.fill(bits(10));assert(f.sdk.advance(scale));
  auto result=f.run(request,Entry,{OriginalScene,Parent,0});assert(result && word(m,ProcessWork+ObjectCount)==1);
  assert(bool(word(m,*result)&2)==bool(test[3]));
 }
}
// Real process Work allocation uses the actual cold source heap0. The small
// independent map pool is unused here; these checks stop at original mapInit.
void sourceHeapMapWork(const char* root){
 constexpr uint32_t MapPool=0x88000000,PrivateStack=0x8d100000;
 constexpr uint32_t HeapHandles=0x8041e844,SdkDescriptors=0x8041f070;
 constexpr uint32_t DescriptorBytes=12,HeaderBytes=32,NodeNext=4,NodeSize=8;
 constexpr unsigned SdkListVisitLimit=4096;
 struct Backend {
  std::shared_ptr<NativeFieldAnimation> field;
  std::vector<uint8_t> pool=std::vector<uint8_t>(4096);
  uint32_t address=0;unsigned allocations=0,flushes=0;
  bool available=false,flush=false,alive=true,retireOnAllocation=false;
  EventNativeRuntime& vm(){return field->process().vm();}
  bool current(){
   if(!alive || !field->process().heapGlobalsCurrent())return false;
   if(!address)return true;
   auto& m=vm().memory;const auto descriptor=word(m,SdkDescriptors)+DescriptorBytes*word(m,HeapHandles);
   auto node=word(m,descriptor+NodeSize);
   for(unsigned budget=0;node && budget<SdkListVisitLimit;++budget){
    if(node+HeaderBytes==address)return word(m,node+NodeSize)>=WorkBytes+HeaderBytes;
    node=word(m,node+NodeNext);
   }
   return false;
  }
  FieldMapHeapOwner services(){
   FieldMapHeapOwner h;h.user=this;h.address=MapPool;h.bytes=uint32_t(pool.size());h.backing=pool.data();
   h.current=[](void* p){return static_cast<Backend*>(p)->current();};
   h.processAllocate=[](void* p,uint32_t size,uint32_t& out){
    auto& b=*static_cast<Backend*>(p);assert(size==WorkBytes && b.allocations==0);
    if(!b.available)return false;
    // Deferred native child is outside the source raw prefix: both a nested
    // service borrow and actual SDK allocation/mapping changes remain legal.
    assert(b.vm().memory.withRawRamAccess([](auto&){return true;}));
    out=b.address=eventHeapAllocate(0,size,b.vm().memoryBinding.services());++b.allocations;
    if(b.retireOnAllocation){b.alive=false;b.retireOnAllocation=false;}
    return true;
   };
   h.flushProcess=[](void* p,uint8_t* bytes,uint32_t size){
    auto& b=*static_cast<Backend*>(p);assert(bytes && size==WorkBytes && reinterpret_cast<uintptr_t>(bytes)%32==0);
    ++b.flushes;return b.flush;
   };
   h.flushBlock=[](void*,uint32_t){};h.systemCall=[](void*){};return h;
  }
 } backend;
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 platform.truncate=[](void*,float f){return int32_t(f);};
 platform.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
 platform.roundDoubleFloat=[](void*,double d){volatile float v=float(d);return v;};
 backend.field=NativeFieldAnimation::create(root,NativeFileCache::create(),platform);assert(backend.field);
 auto& m=backend.vm().memory;assert(m.registerExternalRam(MapPool,backend.pool.data(),uint32_t(backend.pool.size())));
 assert(!FieldMapSourceOwner::bind(m,0,PrivateStack,FieldMapHeapOwner{}));
 assert(!FieldMapSourceOwner::bind(m,MapPool,PrivateStack,backend.services()));
 auto owner=FieldMapSourceOwner::bind(m,0,PrivateStack,backend.services());FieldMapRequest request,foreign;
 assert(owner && owner->current() && !owner->workAddress());
 assert(!owner->invoke(request,Init) && owner->pending() && backend.allocations==0 && word(m,Work)==0);
 backend.available=true;
 assert(!owner->invoke(request,Init) && backend.allocations==1 && backend.flushes==1 && word(m,Work)==0);
 const auto address=owner->workAddress();assert(address && address==backend.address);
 assert(word(m,address-HeaderBytes+NodeSize)>=WorkBytes+HeaderBytes);
 assert(m.writeByte(address+7,0xed));
 assert(!owner->invoke(foreign,Init) && backend.flushes==1);
 assert(!owner->invoke(request,Init) && backend.allocations==1 && backend.flushes==2);
 uint8_t marker=0;assert(m.readByte(address+7,marker) && marker==0xed);
 backend.flush=true;
 // One instruction acknowledges allocation; budget then stops before pointer
 // publication and before the original second memset.
 assert(!owner->invoke(request,Init,{},1) && owner->pendingInstruction()==0x8002af30 && word(m,Work)==0);
 assert(m.readByte(address+7,marker) && marker==0xed);
 assert(!owner->invoke(request,Init,{},1) && word(m,Work)==address);
 for(unsigned i=0;owner->pending() && i<100;++i)owner->invoke(request,Init);
 assert(!owner->pending() && word(m,ActiveGroup)==0 && word(m,Auxiliary)==0);
 assert(m.readByte(address+7,marker) && marker==0); // Actual explicit source clear.
 const auto descriptor=word(m,SdkDescriptors)+DescriptorBytes*word(m,HeapHandles),head=word(m,descriptor+NodeSize);
 assert(m.writeWord(descriptor+NodeSize,0) && !owner->current());
 assert(m.writeWord(descriptor+NodeSize,head) && owner->current());
 std::vector<uint8_t> replacement(WorkBytes);
 for(uint32_t i=0;i<WorkBytes;++i)assert(m.readByte(address+i,replacement[i]));
 assert(m.unregisterExternalRam(address) && m.registerRam(address,replacement));
 assert(!owner->current());owner.reset();
 assert(m.matchesBytes(address,replacement.data(),WorkBytes));
 // Actual exhaustion completed while current() is unavailable must retain the
 // accepted null, then publish0 and hold at original memset without replay.
 backend.address=0;backend.allocations=0;backend.flushes=0;
 const auto freeHead=word(m,descriptor+NodeNext);assert(m.writeWord(descriptor+NodeNext,0));
 owner=FieldMapSourceOwner::bind(m,0,PrivateStack,backend.services());assert(owner);
 backend.retireOnAllocation=true;
 assert(!owner->invoke(request,Init) && backend.allocations==1 && !owner->current());
 backend.alive=true;
 assert(!owner->invoke(request,Init) && owner->current() && backend.allocations==1 && backend.flushes==0);
 assert(word(m,Work)==0 && !owner->workAddress() && owner->pendingInstruction()==0x8002af3c);
 assert(!owner->invoke(request,Init) && backend.allocations==1 && owner->pendingInstruction()==0x8002af3c);
 assert(m.writeWord(descriptor+NodeNext,freeHead));
}
void rawPrefixBudgets(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapRequest request,foreign;
 f.processFlush=true;
 // Five exact source instructions precede the allocation call instruction.
 assert(!f.owner->invoke(request,Init,{},5));
 assert(f.owner->pendingInstruction()==0x8002af2c && f.flushes==0 && word(m,Work)==0);
 assert(!f.owner->invoke(foreign,Init,{},1) && f.flushes==0);
 // An unrelated outer borrow cannot execute a prefix or consume its child.
 assert(m.withRawRamAccess([&](auto&){
  assert(!f.owner->invoke(request,Init,{},1));
  assert(f.owner->pendingInstruction()==0x8002af2c && f.flushes==0);
  return true;
 }));
 assert(!f.owner->invoke(request,Init,{},1));
 assert(f.owner->pendingInstruction()==0x8002af30 && f.flushes==1 && word(m,Work)==0);
 // Child acknowledgement uses no second source-case visit; the very next
 // visit is the original pointer store, not a replay of allocation.
 assert(!f.owner->invoke(request,Init,{},1));
 assert(f.owner->pendingInstruction()==0x8002af34 && f.flushes==1 && word(m,Work)==ProcessWork);
 assert(f.run(request,Init) && f.flushes==1 && !f.owner->pending());
}
int main(){rawPrefixBudgets("romfs");sourceHeapMapWork("romfs");originalMapOwner("romfs");mapBoundingThresholds("romfs");}
