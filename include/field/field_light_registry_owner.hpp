#pragma once
#include "field_light_registry.hpp"
#include "field_map_light_producer.hpp"
#include "map_material_light.hpp"
#include "field_map_source.hpp"
#include <memory>
namespace ttyd {
class EventAddressSpace;
struct FieldLightHeapServices {
 void* user=nullptr;
 // Native heap0 adaptation: flush the same zeroed allocation before source
 // publication. False retains this allocation without clearing it again.
 bool (*flush)(void*,uint8_t*,uint32_t)=nullptr;
 // Optional original __memAlloc provider. False retains the source call and
 // must not consume an allocation; true returns its exact guest address,
 // including zero on exhaustion. Nonzero payloads must be owned VM RAM after
 // the original wrapper's clear/flush. The owner isolates those bytes at the
 // same address before publication and flushes the stationary native backing.
 bool (*allocate)(void*,uint32_t heap,uint32_t bytes,uint32_t& address)=nullptr;
 // Pure backend lifetime/heap identity predicate, safe inside a raw borrow.
 // It must not call this owner's readiness predicates recursively.
 bool (*current)(void*)=nullptr;
};
// One stationary native invocation. Keep it alive through unavailable children;
// another caller cannot consume that invocation, even with identical operands.
struct FieldLightRequest {
 FieldLightRequest()=default;
 FieldLightRequest(const FieldLightRequest&)=delete;
 FieldLightRequest& operator=(const FieldLightRequest&)=delete;
};
// Stationary process owner. VM and consumer lifetimes must outlive all borrowed
// records; stop source events before destroying mappings. No per-room reallocation.
class FieldLightRegistryOwner {
public:
 // allocationBase is the fixed bank base for fixture services; with allocate
 // it supplies only the private scratch address. Banks use actual heap returns.
 static std::unique_ptr<FieldLightRegistryOwner> bind(EventAddressSpace&,uint32_t allocationBase,const FieldLightHeapServices&);
 ~FieldLightRegistryOwner();
 FieldLightRegistryOwner(const FieldLightRegistryOwner&)=delete;
 FieldLightRegistryOwner& operator=(const FieldLightRegistryOwner&)=delete;
 bool initialize(FieldLightRequest&);
 bool reset(FieldLightRequest&);
 std::optional<int32_t> enter(FieldLightRequest&,FieldLightEntryArguments);
 bool loadMapLights(FieldLightRequest&,uint32_t mapEntry);
 // Bridge only the child reached by the original map parent. The same call
 // object and operands remain stationary until acknowledged. No second loop.
 bool service(FieldMapSourceCall&);
 bool readyFor(const EventAddressSpace& memory) const{return &memory_==&memory && ready();}
 static bool accepts(uint32_t function){return function==FieldLightRegistrySource::Entry || function==FieldLightRegistrySource::Reinitialize;}
 std::optional<uint32_t> find(FieldLightRequest&,uint32_t name);
 // Same physical MapLightRecord bytes exposed to source RAM; no renderer copy.
 const MapLightRecord* record(uint32_t address) const;
 const MapLightRecord* preset(unsigned sourceIndex) const;
 bool query(MapLightQuery&,uint32_t& pending) const;
 bool ready() const{return ready_ && current();}
 bool pending() const{return mapCursor_.active || cursor_.operation!=FieldLightOperation::Idle;}
 uint32_t bankAddress(unsigned group) const;
 uint32_t scratchAddress() const;
private:
 FieldLightRegistryOwner(EventAddressSpace&,uint32_t,const FieldLightHeapServices&);
 FieldLightRegistryServices services();
 std::optional<int32_t> enterImpl(FieldLightEntryArguments,bool mapChild);
 bool admitEntry(const FieldLightEntryArguments&) const;
 bool selected(unsigned&) const;
 bool current() const;
 bool map(uint32_t,uint8_t*,uint32_t);
 bool admitRequest(FieldLightRequest&);
 void releaseRequest();
 bool readString(uint32_t,char*,uint32_t,uint32_t&) const;
 bool owns(uint32_t,uint32_t) const;
 EventAddressSpace& memory_;uint32_t allocationBase_;
 FieldLightHeapServices heap_;
 std::array<uint8_t,16> descriptors_{};
 alignas(32) std::array<std::array<MapLightRecord,FieldLightRegistrySource::Capacity>,2> banks_{};
 std::array<MapLightRecord,4> presets_{};
 std::array<uint8_t,256+32> scratch_{}; // Format stack then map-loader XYZ/color locals.
 // Actual shared process ambient words, not defaults or renderer copies.
 // Source map loads publish these in source order; light reset leaves them.
 std::array<uint8_t,FieldLightRegistrySource::AmbientBytes> ambient_{};
 struct Mapping {uint32_t address=0,bytes=0;uint8_t* backing=nullptr;};
 std::array<Mapping,6> mappings_{};unsigned mapped_=0,allocations_=0;
 std::array<uint32_t,2> bankAddresses_{};
 std::array<bool,2> allocated_{},isolated_{},bankMapped_{};
 FieldLightRequest mapRequest_;
 const FieldMapSourceCall* mapCall_=nullptr;
 FieldMapSourceCall mapOperands_;
 std::array<bool,2> zeroed_{};
 const FieldLightRequest* activeRequest_=nullptr;
 FieldLightRegistryCursor cursor_;
 FieldMapLightCursor mapCursor_;
 bool ready_=false;
};
}
