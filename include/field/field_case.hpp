#pragma once
#include "event_creation.hpp"
#include <array>
#include <optional>
#include <string>
namespace ttyd {
// Semantic native owner of caseInit's64 original160-byte records. Source RAM
// aliasing/heap addresses are separate; names>=64 and full pools hold admission.
// Flags1 active,2 marked for dispatch,4 derives from descriptor type8000.
// Event ID is an owned scheduler identity, separate from pool index/callback.
struct FieldCaseRecord {
 uint16_t flags=0,padding=0;
 uint32_t type=0;
 std::array<char,64> name{};
 int32_t index=0;
 uint32_t argument=0,callback=0;
 std::array<int32_t,16> locals{};
 uint32_t eventId=0,script=0;
 int32_t priority=0;
};
struct FieldCaseDescriptor {
 uint16_t type=0;
 std::string name;
 uint32_t argument=0,callback=0,script=0;
 // Bero cases request priority20 and NPC stop; ordinary cases priority0.
 // Local bank is copied at registration, before returning the case index.
 int32_t priority=0;
 std::array<int32_t,16> locals{};
};
struct FieldCaseDispatchServices {
 EventSchedule* schedule=nullptr;
 EventCreationServices creation;
 void* user=nullptr;
 void (*stopNpcs)(void*)=nullptr; // Required only at actual priority20 stop call.
};
class FieldCasePool {
public:
 static constexpr unsigned Capacity=64;
 std::optional<int32_t> enter(const FieldCaseDescriptor&);
 const FieldCaseRecord* active(int32_t) const;
 const FieldCaseRecord* record(int32_t) const;
 bool erase(int32_t); // caseDelete: active1 only; does not delete an event.
 void reinitialize(); // caseReInit: same operation across all64 records.
 bool mark(int32_t); // Called by actual admitted case-type predicate.
 // Separate source second pass. No event execution here; manager runs later.
 // Requires source constructor/free-slot/stop providers before entry.
 bool dispatch(const FieldCaseDispatchServices&);
 bool eraseEvent(int32_t mode,int32_t index,const EventCreationServices&);
private:
 std::array<FieldCaseRecord,Capacity> records_{};
};
// Wrappers retain captured args, ordered value/number reads, caller local
// snapshot BEFORE index publication, and a fresh raw destination read afterward.
struct FieldCaseEventServices {
 void* user=nullptr;
 uint32_t (*word)(void*,uint32_t)=nullptr;
 int32_t (*value)(void*,EventEntry&,int32_t)=nullptr;
 void (*setValue)(void*,EventEntry&,int32_t,int32_t)=nullptr;
 bool (*text)(void*,uint32_t,std::string&)=nullptr;
 FieldCasePool* (*owner)(void*)=nullptr;
};
int32_t fieldCaseNumber(int32_t raw); // evtGetNumber800310fc; no dereference.
int32_t fieldRunCaseEvent(EventEntry&,bool bero,const FieldCaseEventServices&);
}
