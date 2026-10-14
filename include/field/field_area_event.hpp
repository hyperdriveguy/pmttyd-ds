#pragma once
#include "field_case.hpp"
#include "field_mario_preparation.hpp"
#include "field_previous_party.hpp"
#include "event_operands.hpp"
#include "event_gp_addresses.hpp"
#include <memory>
#include <vector>
namespace ttyd {
class AnimationGroupPrefetch;
class FieldDoorDataWorld;
class FieldMapFlags;
class FieldBeroInfo;
class FieldBeroExec;
class EventAddressSpace;
struct FieldAreaNativeCall {
 EventEntry& event;
 uint32_t function;
 bool first;
 const EventOperandServices& operands;
 EventAddressSpace& memory;
 EventSchedule& schedule;
 EventCreationServices creation;
};
// Explicit caller-supplied packed GP bytes40..13d7; not inferred checkpoint
// defaults. Word0 is explicit system flags; other scheduler fields below40 remain native-owned.
struct FieldAreaGpState {
 static constexpr uint32_t Begin=0x40;
 uint32_t systemFlags;
 std::array<uint8_t,EventGpAddresses::size-Begin> bytes;
};
bool loadFieldAreaEventAssets(const char* root);
struct FieldAreaEventServices {
 FieldDoorDataWorld* doors=nullptr;
 FieldMapFlags* map=nullptr;
 FieldBeroInfo* beroInfo=nullptr;
 FieldBeroExec* beroExec=nullptr;
 AnimationGroupPrefetch* prefetch=nullptr;
 FieldMarioPreparationWorld mario;
 FieldPreviousPartyWorld previousParty;
 void* user=nullptr;
 // Exact npc_hide_onoff owner; zero holds,2 completes. Not an empty-list stub.
 int32_t (*hideNpcs)(void*,uint32_t record,bool hide)=nullptr;
 // Exact world primitives may resolve/store operands through the live services.
 //0 retains,1 clears/yields,2 clears/continues. Unknown functions hold by default.
 int32_t (*call)(void*,FieldAreaNativeCall&)=nullptr;
};
struct FieldAreaEntryResult {uint32_t eventId=0;uint32_t entryIndex=0;std::array<int32_t,16> locals;bool returned=false;};
struct FieldAreaEventRuntime;
// One retained scheduler and case pool for an area. Supplied-bank roots are an
// explicit continuation adapter until the original full map-init parent is bound.
class FieldAreaEvent {
public:
 FieldAreaEvent()=default;
 FieldAreaEvent(const FieldAreaEvent&)=delete;
 FieldAreaEvent& operator=(const FieldAreaEvent&)=delete;
 FieldAreaEvent(FieldAreaEvent&&)=delete;
 FieldAreaEvent& operator=(FieldAreaEvent&&)=delete;
 bool begin(uint32_t room,uint64_t milliseconds);
 std::optional<uint32_t> appendAreaInit(const FieldAreaGpState&); // Actual cleared-root constructor, once before other roots/cases.
 std::optional<uint32_t> areaInitEventId() const;
 bool eventAlive(uint32_t) const;
 uint32_t liveEventCount() const;
 std::optional<uint32_t> appendDoorEntry(uint32_t entryIndex,const std::array<int32_t,16>&);
 void tick(uint64_t milliseconds,const FieldAreaEventServices&);
 uint32_t doorEntryCount() const;
 const FieldCasePool* cases() const;
 const std::vector<FieldAreaEntryResult>& completed() const{return completed_;}
 bool started=false,failed=false;
 uint32_t pendingFunction=0,pendingEventId=0;
private:
 std::shared_ptr<FieldAreaEventRuntime> runtime_;
 std::vector<FieldAreaEntryResult> completed_;
};
}
