#pragma once
#include "field_case.hpp"
#include <memory>
#include <vector>
namespace ttyd {
class FieldDoorDataWorld;
class FieldMapFlags;
bool loadFieldAreaEventAssets(const char* root);
struct FieldAreaEventServices {
 FieldDoorDataWorld* doors=nullptr;
 FieldMapFlags* map=nullptr;
 void* user=nullptr;
 // Exact npc_hide_onoff owner; zero holds,2 completes. Not an empty-list stub.
 int32_t (*hideNpcs)(void*,uint32_t record,bool hide)=nullptr;
};
struct FieldAreaEntryResult {uint32_t eventId=0;uint32_t entryIndex=0;std::array<int32_t,16> locals;};
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
 std::optional<uint32_t> appendDoorEntry(uint32_t entryIndex,const std::array<int32_t,16>&);
 void tick(uint64_t milliseconds,const FieldAreaEventServices&);
 uint32_t doorEntryCount() const;
 const FieldCasePool* cases() const;
 const std::vector<FieldAreaEntryResult>& completed() const{return completed_;}
 bool started=false,failed=false;
 uint32_t pendingFunction=0;
private:
 std::shared_ptr<FieldAreaEventRuntime> runtime_;
 std::vector<FieldAreaEntryResult> completed_;
};
}
