#include "field_map_source_owner.hpp"
#include "field_light_registry_owner.hpp"
#include "field_camera_road_owner.hpp"
#include "source_shadow_process.hpp"
#include "event_ram_access.hpp"
#include "event_memory_binding.hpp"
#include <cstring>
#include <vector>
namespace ttyd {
namespace {
using namespace FieldMapSource;
constexpr uint32_t ProcessAllocate=0x800300f0,MapAllocate=0x8002ff58,Clear=0x800050b4,Compare=0x8026d464;
constexpr unsigned MaxNameBytes=256; // Native admission bound, not source strlen.
}
FieldMapSourceOwner::FieldMapSourceOwner(EventAddressSpace& m,uint32_t w,uint32_t s,const FieldMapHeapOwner& heap):memory_(m),workAddress_(w),stackAddress_(s),heap_(heap){}
bool FieldMapSourceOwner::map(uint32_t address,uint8_t* backing,uint32_t bytes){
 if(mapped_==mappings_.size() || !memory_.registerExternalRam(address,backing,bytes))return false;
 mappings_[mapped_++]={address,bytes,backing};return true;
}
std::unique_ptr<FieldMapSourceOwner> FieldMapSourceOwner::bind(EventAddressSpace& m,uint32_t w,uint32_t s,const FieldMapHeapOwner& heap){
 // Real source allocations supply their own address only when reached. Fixed
 // addresses remain an explicit fixture policy, never a source heap receipt.
 if((heap.processAllocate?w!=0:!w) || !s || (w&(CacheLineBytes-1)) || (s&(CacheLineBytes-1)) || uint64_t(w)+WorkBytes>0x100000000ULL || uint64_t(s)+StackBytes>0x100000000ULL || !heap.backing || !heap.bytes || !heap.flushProcess || !heap.flushBlock || !heap.systemCall || (heap.current && !heap.current(heap.user)))return {};
 auto o=std::unique_ptr<FieldMapSourceOwner>(new FieldMapSourceOwner(m,w,s,heap));
 if(!o->map(Work,o->globals_.data(),uint32_t(o->globals_.size())))return {};
 if(!heap.processAllocate){if(!o->map(w,o->work_.data(),WorkBytes))return {};o->workMapped_=true;}
 if(!o->map(s,o->stack_.data(),StackBytes) || !o->map(NoFogColor,o->noFogColor_.data(),sizeof(uint32_t)) || !o->current())return {};
 return o;
}
FieldMapSourceOwner::~FieldMapSourceOwner(){while(mapped_){const auto& m=mappings_[--mapped_];memory_.unregisterExternalRam(m.address,m.backing);}}
bool FieldMapSourceOwner::current() const{
 // Cache physical RAM lookups only for this pure, synchronous identity proof.
 // Every predicate and payload/header value is still read on every call.
 const auto check=[&]() -> bool {
  if((heap_.current && !heap_.current(heap_.user)) || (heap_.shadows && !heap_.shadows->currentFor(memory_)) || mapped_!=3+unsigned(workMapped_) || (heap_.lights && !heap_.lights->readyFor(memory_)) || (heap_.cameraRoad && !heap_.cameraRoad->currentFor(memory_)))return false;
  for(unsigned i=0;i<mapped_;++i){const auto& m=mappings_[i];if(!memory_.hasRamBacking(m.address,m.backing,m.bytes))return false;}
  return memory_.hasRamBacking(heap_.address,heap_.backing,heap_.bytes);
 };
 bool checked=false;
 const bool valid=memory_.withRawRamAccess([&](auto&){checked=true;return check();});
 // An enclosing raw SDK/source borrow already supplies the same accelerator.
 // A false predicate must not be retried or mistaken for a refused borrow.
 return checked?valid:check();
}
FieldMapSourceServices FieldMapSourceOwner::services(){FieldMapSourceServices s;s.user=this;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readWord(a,v);};
 s.half=[](void* u,uint32_t a,uint16_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readHalf(a,v);};
 s.byte=[](void* u,uint32_t a,uint8_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readByte(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeWord(a,v);};
 s.storeHalf=[](void* u,uint32_t a,uint16_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeHalf(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeByte(a,v);};
 s.call=[](void* u,FieldMapSourceCall& q){
  auto& o=*static_cast<FieldMapSourceOwner*>(u);
  if(o.rawPrefix_){o.deferred_=true;return false;}
  return o.child(q);
 };return s;
}
bool FieldMapSourceOwner::child(FieldMapSourceCall& q){
 if(!current())return false;
 if(heap_.shadows && SourceShadowProcess::accepts(q.function))return heap_.shadows->service(q);
 if(heap_.lights && FieldLightRegistryOwner::accepts(q.function))return heap_.lights->service(q);
 if(heap_.cameraRoad && FieldCameraRoadOwner::accepts(q.function))return heap_.cameraRoad->service(q,services());
 auto& a=q.arguments;
 switch(q.function){
 case ProcessAllocate:
  if(a[0] || a[1]!=WorkBytes || allocated_)return false;
  if(heap_.processAllocate){
   if(!allocationAccepted_){
    uint32_t address=0;
    if(!heap_.processAllocate(heap_.user,WorkBytes,address))return false;
    workAddress_=address;allocationAccepted_=true;
   }
   if(!current())return false;
   // mapInit publishes null, then reaches its own unavailable null memset.
   if(!workAddress_){allocated_=true;q.result=0;return true;}
   if(workAddress_&(CacheLineBytes-1))return false;
   if(!allocationIsolated_){
    std::vector<uint8_t> source;
    if(!memory_.extractRam(workAddress_,WorkBytes,source))return false;
    std::memcpy(work_.data(),source.data(),WorkBytes);allocationIsolated_=true;
   }
   if(!workMapped_){if(!map(workAddress_,work_.data(),WorkBytes))return false;workMapped_=true;}
  }else if(!allocationZeroed_){work_.fill(0);allocationZeroed_=true;}
  if(!heap_.flushProcess(heap_.user,work_.data(),WorkBytes) || !current())return false;
  allocated_=true;q.result=workAddress_;return true;
 case Clear:{
  if(!memory_.contiguousRam(a[0],a[2]))return a[2]==0;
  EventMemoryBinding binding{memory_,heap_.user,heap_.flushBlock,heap_.systemCall};q.result=eventFillMemory(a[0],int32_t(a[1]),a[2],binding.services());return true;
 }
 case MapAllocate:{
  // This adapter admits an already initialized, complete contiguous primary
  // pool. Source allocation mutates its REAL headers and flushes real payload.
  if(a[0]!=heap_.address)return false;
  const uint64_t rounded=(uint64_t(a[1])+CacheLineBytes-1)&~uint64_t(CacheLineBytes-1);
  if(rounded+CacheLineBytes>heap_.bytes)return false;
  EventMemoryBinding binding{memory_,heap_.user,heap_.flushBlock,heap_.systemCall};q.result=eventMapAllocate(a[0],a[1],binding.services());return true;
 }
 case Compare:{
  for(unsigned i=0;i<MaxNameBytes;++i){uint8_t left=0,right=0;if(!memory_.readByte(a[0]+i,left) || !memory_.readByte(a[1]+i,right))return false;if(left!=right || !left){q.result=uint32_t(int32_t(left)-int32_t(right));return true;}}
  return false;
 }
 default:return heap_.sdk && heap_.sdk(heap_.user,q);
 }
}
std::optional<uint32_t> FieldMapSourceOwner::invoke(FieldMapRequest& request,uint32_t f,const std::array<uint32_t,5>& a,unsigned visits){
 if(invoking_ || !current() || (request_ && request_!=&request))return {};
 if(cursor_.active && cursor_.function!=f)return {};
 invoking_=true;struct InvocationGuard{bool& active;~InvocationGuard(){active=false;}} invocation{invoking_};
 request_=&request;
 std::optional<uint32_t> result;
 unsigned remaining=visits;
 do{
  deferred_=false;
  // Single-threaded synchronous source pass: semantic registries stay frozen.
  // The memory facade's raw cache retires before any child can mutate mappings
  // or open its own raw borrow. No pointer or admission survives this scope.
  const bool borrowed=memory_.withRawRamAccess([&](auto&){
   rawPrefix_=true;struct PrefixGuard{bool& active;~PrefixGuard(){active=false;}} prefix{rawPrefix_};
   result=resumeFieldMapSource(f,a,stackAddress_+StackBytes-StackGuardBytes,services(),cursor_,remaining);
   return true;
  });
  if(!borrowed || result || !cursor_.active)break;
  const unsigned consumed=cursor_.visitsConsumed;
  if(consumed>remaining)break; // Defensive native budget admission.
  remaining-=consumed;
  if(!deferred_ || !cursor_.queued || !child(cursor_.request))break;
  // The external instruction was already charged by the raw prefix. Consume
  // its accepted child without executing that source instruction a second time.
  if(cursor_.request.result)cursor_.registers[3]=*cursor_.request.result;
  if(cursor_.request.floatingResult)cursor_.floats[1]=*cursor_.request.floatingResult;
  cursor_.pc=cursor_.lr;cursor_.queued=false;
  if(!current())break;
 }while(remaining);
 if(!cursor_.active)request_=nullptr;
 return result;
}
}
