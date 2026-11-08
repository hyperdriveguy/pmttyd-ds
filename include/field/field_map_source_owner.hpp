#pragma once
#include "field_map_source.hpp"
#include "event_address_space.hpp"
#include <memory>
namespace ttyd {
class FieldLightRegistryOwner;
class FieldCameraRoadOwner;
class SourceShadowProcess;
struct FieldMapRequest {
 FieldMapRequest()=default;FieldMapRequest(const FieldMapRequest&)=delete;
 FieldMapRequest& operator=(const FieldMapRequest&)=delete;
};
struct FieldMapHeapOwner {
 void* user=nullptr;
 uint32_t address=0,bytes=0;
 uint8_t* backing=nullptr; // Actual primary source map-pool bytes, caller-owned.
 bool (*flushProcess)(void*,uint8_t*,uint32_t)=nullptr;
 void (*flushBlock)(void*,uint32_t)=nullptr;
 void (*systemCall)(void*)=nullptr;
 bool (*sdk)(void*,FieldMapSourceCall&)=nullptr;
 // Initialized process owner in this exact VM; borrowed through all holds.
 FieldLightRegistryOwner* lights=nullptr;
 FieldCameraRoadOwner* cameraRoad=nullptr;
 SourceShadowProcess* shadows=nullptr;
};
// Owns actual process Work/global/private-stack backing. The original primary
// map heap remains its existing owner's allocation; constructed MapObjs live
// in that exact heap through the complete original map allocator operation.
// VM, heap, callbacks and each request outlive all holds/borrows. Freeze users
// during retained unload; stop users before destroying any owner.
class FieldMapSourceOwner {
public:
 static constexpr uint32_t CacheLineBytes=32,StackBytes=64*1024,StackGuardBytes=CacheLineBytes;
 static std::unique_ptr<FieldMapSourceOwner> bind(EventAddressSpace&,uint32_t workAddress,uint32_t stackAddress,const FieldMapHeapOwner&);
 ~FieldMapSourceOwner();
 FieldMapSourceOwner(const FieldMapSourceOwner&)=delete;FieldMapSourceOwner& operator=(const FieldMapSourceOwner&)=delete;
 std::optional<uint32_t> invoke(FieldMapRequest&,uint32_t function,const std::array<uint32_t,5>& arguments={},unsigned visits=4096);
 bool current() const;
 bool pending() const{return cursor_.active;}
 uint32_t pendingInstruction() const{return cursor_.pc;}
 uint32_t workAddress() const{return workAddress_;}
private:
 FieldMapSourceOwner(EventAddressSpace&,uint32_t,uint32_t,const FieldMapHeapOwner&);
 FieldMapSourceServices services();bool child(FieldMapSourceCall&);
 EventAddressSpace& memory_;uint32_t workAddress_,stackAddress_;
 FieldMapHeapOwner heap_;
 alignas(CacheLineBytes) std::array<uint8_t,FieldMapSource::WorkBytes> work_{};
 std::array<uint8_t,FieldMapSource::GlobalsBytes> globals_{};
 std::array<uint8_t,StackBytes> stack_{};
 std::array<uint8_t,sizeof(uint32_t)> noFogColor_{}; // Source startup BSS; survives room load/unload.
 std::array<uint32_t,4> mappings_{};unsigned mapped_=0;
 bool allocationZeroed_=false,allocated_=false;
 const FieldMapRequest* request_=nullptr;
 FieldMapSourceCursor cursor_;
};
}
