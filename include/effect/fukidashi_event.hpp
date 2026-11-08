#pragma once
#include "fukidashi_entry.hpp"

namespace ttyd {
enum class FukidashiEventBinary {Multiply,Divide};
// Whole evt_eff_fukidashi8006c3c4. Memory is the caller's actual guest domain;
// unchecked source NPC dereferences require real mapped owners. Optional child
// results distinguish native unavailability from executed zero/null returns.
struct FukidashiEventWorldServices {
 void* user=nullptr;
 std::optional<int32_t> (*compare)(void*,uint32_t left,uint32_t right)=nullptr;
 float (*binary)(void*,FukidashiEventBinary,float,float)=nullptr;
 std::optional<uint32_t> (*mario)(void*)=nullptr;
 std::optional<uint32_t> (*npc)(void*,uint32_t name)=nullptr;
 std::optional<uint32_t> (*partyId)(void*,bool extra)=nullptr;
 std::optional<uint32_t> (*partyPointer)(void*,uint32_t id)=nullptr;
 std::optional<uint32_t> (*construct)(void*,const FukidashiEntryRequest&)=nullptr;
 bool (*setName)(void*,uint32_t effect,uint32_t name)=nullptr;
};
struct FukidashiEventServices : FukidashiEventWorldServices {
 uint32_t (*word)(void*,uint32_t)=nullptr;
 uint8_t (*byte)(void*,uint32_t)=nullptr;
 std::optional<int32_t> (*value)(void*,uint32_t entry,uint32_t raw)=nullptr;
 std::optional<float> (*real)(void*,uint32_t entry,uint32_t raw)=nullptr;
};
class FukidashiEventCursor {
public:
 FukidashiEventCursor()=default;
 FukidashiEventCursor(const FukidashiEventCursor&)=delete;
 FukidashiEventCursor& operator=(const FukidashiEventCursor&)=delete;
 bool active()const{return active_;}
 uint32_t pending()const{return pending_;}
 // Caller owns real event retirement and any child's partially acquired work.
 bool reset();
private:
 enum class Phase {Arguments,Raw,Decode,Dispatch,Mario,CompareActor,Npc,Height,
                   PartyId,PartyPointer,Construct,Name,CompareName,SetName};
 Phase phase_=Phase::Arguments;
 bool active_=false,invoking_=false;
 uint32_t entry_=0,args_=0,index_=0,raw_=0,actor_=0,partyId_=0,effect_=0,pending_=0;
 int32_t mode_=0;uint32_t name_=0,actorName_=0;
 FukidashiEntryRequest request_;FukidashiEventServices owner_;
 friend std::optional<int32_t> resumeFukidashiEvent(uint32_t,const FukidashiEventServices&,FukidashiEventCursor&);
};
// Original first-call argument is unused. Completion2 follows construction and
// optional naming, never effect expiration. Holds retain every resolved operand
// and completed child; no heap allocation or effect work is repeated per poll.
std::optional<int32_t> resumeFukidashiEvent(uint32_t entry,const FukidashiEventServices&,FukidashiEventCursor&);
class EventNativeRuntime;struct EventEntry;
// Actual VM owns argument/operand/actor memory. World callbacks must resolve
// actual owners in that domain. One adapter per actual caller command/incarnation;
// retirement and partial construction cancellation remain the scheduler's job.
class NativeFukidashiEventCall {
public:
 bool step(EventEntry&,EventNativeRuntime&,const FukidashiEventWorldServices&,int32_t& result);
 uint32_t pending()const{return cursor_.pending();}
 bool complete()const{return complete_;}
private:
 EventNativeRuntime* vm_=nullptr;EventEntry* entry_=nullptr;
 uint32_t id_=0,address_=0,command_=0;bool invoking_=false,complete_=false;
 FukidashiEventWorldServices world_;FukidashiEventCursor cursor_;
 FukidashiEventServices services();
};
}
