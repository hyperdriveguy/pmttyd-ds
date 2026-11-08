#include "field_map_source_owner.hpp"
#include "field_light_registry_owner.hpp"
#include "field_camera_road_owner.hpp"
#include "source_shadow_process.hpp"
#include "event_ram_access.hpp"
#include "event_memory_binding.hpp"
#include <cstring>
namespace ttyd {
namespace {
using namespace FieldMapSource;
constexpr uint32_t ProcessAllocate=0x800300f0,MapAllocate=0x8002ff58,Clear=0x800050b4,Compare=0x8026d464;
constexpr unsigned MaxNameBytes=256; // Native admission bound, not source strlen.
}
FieldMapSourceOwner::FieldMapSourceOwner(EventAddressSpace& m,uint32_t w,uint32_t s,const FieldMapHeapOwner& heap):memory_(m),workAddress_(w),stackAddress_(s),heap_(heap){}
std::unique_ptr<FieldMapSourceOwner> FieldMapSourceOwner::bind(EventAddressSpace& m,uint32_t w,uint32_t s,const FieldMapHeapOwner& heap){
 if(!w || !s || (w&(CacheLineBytes-1)) || (s&(CacheLineBytes-1)) || uint64_t(w)+WorkBytes>0x100000000ULL || uint64_t(s)+StackBytes>0x100000000ULL || !heap.backing || !heap.bytes || !heap.flushProcess || !heap.flushBlock || !heap.systemCall)return {};
 auto o=std::unique_ptr<FieldMapSourceOwner>(new FieldMapSourceOwner(m,w,s,heap));
 auto map=[&](uint32_t a,uint8_t* b,uint32_t n){if(!m.registerExternalRam(a,b,n))return false;o->mappings_[o->mapped_++]=a;return true;};
 if(!map(Work,o->globals_.data(),uint32_t(o->globals_.size())) || !map(w,o->work_.data(),WorkBytes) || !map(s,o->stack_.data(),StackBytes) || !map(NoFogColor,o->noFogColor_.data(),sizeof(uint32_t)) || !o->current())return {};
 return o;
}
FieldMapSourceOwner::~FieldMapSourceOwner(){const std::array<const uint8_t*,4> b{{globals_.data(),work_.data(),stack_.data(),noFogColor_.data()}};while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],b[mapped_]);}}
bool FieldMapSourceOwner::current() const{
 if((heap_.shadows && !heap_.shadows->currentFor(memory_)) || mapped_!=mappings_.size() || (heap_.lights && !heap_.lights->readyFor(memory_)) || (heap_.cameraRoad && !heap_.cameraRoad->currentFor(memory_)))return false;
 return memory_.hasRamBacking(Work,globals_.data(),uint32_t(globals_.size())) && memory_.hasRamBacking(workAddress_,work_.data(),WorkBytes) && memory_.hasRamBacking(stackAddress_,stack_.data(),StackBytes) && memory_.hasRamBacking(heap_.address,heap_.backing,heap_.bytes) && memory_.hasRamBacking(NoFogColor,noFogColor_.data(),sizeof(uint32_t));
}
FieldMapSourceServices FieldMapSourceOwner::services(){FieldMapSourceServices s;s.user=this;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readWord(a,v);};
 s.half=[](void* u,uint32_t a,uint16_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readHalf(a,v);};
 s.byte=[](void* u,uint32_t a,uint8_t& v){return static_cast<FieldMapSourceOwner*>(u)->memory_.readByte(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeWord(a,v);};
 s.storeHalf=[](void* u,uint32_t a,uint16_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeHalf(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<FieldMapSourceOwner*>(u)->memory_.writeByte(a,v);};
 s.call=[](void* u,FieldMapSourceCall& q){return static_cast<FieldMapSourceOwner*>(u)->child(q);};return s;
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
  if(!allocationZeroed_){work_.fill(0);allocationZeroed_=true;}
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
 if(!current() || (request_ && request_!=&request))return {};
 if(cursor_.active && cursor_.function!=f)return {};
 request_=&request;auto result=resumeFieldMapSource(f,a,stackAddress_+StackBytes-StackGuardBytes,services(),cursor_,visits);
 if(!cursor_.active)request_=nullptr;
 return result;
}
}
