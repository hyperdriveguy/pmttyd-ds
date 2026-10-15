#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct EventEntry;
struct FieldPreviousPartyWorld {
 void* user=nullptr;
 uintptr_t (*mario)(void*)=nullptr;
 uint8_t (*byte)(void*,uintptr_t)=nullptr;
 void (*storeByte)(void*,uintptr_t,uint8_t)=nullptr;
 // nullopt is unavailable native admission with no effects; zero is an actual null lookup.
 std::optional<uintptr_t> (*party)(void*,int32_t index)=nullptr;
 // A nullopt result must have no constructor effects. A real result is the
 // full signed source Entry/Entry2 return, not a boolean or allocated kind.
 std::optional<int32_t> (*enter)(void*,int32_t kind,bool secondary)=nullptr;
};
// Cursor retains raw selector, captured Mario and full constructor return
// across native holds. It is bound to w.user, not reusable across world owners.
struct FieldPreviousPartyCursor {
 enum class Stage {Start,Constructor,Tail,Lookup,Complete,Unsupported};
 Stage stage=Stage::Start;
 bool initialized=false,sourceFirst=false;
 void* owner=nullptr;
 uintptr_t mario=0;
 uint32_t selector=0;
 int32_t kind=0,index=0;
 void firstVisit(bool first){if(!initialized){initialized=true;sourceFirst=first;}}
};
// Native holding preserves a required synchronous source first phase. The area
// owner must discard the cursor on completion/actual event retirement.
int32_t fieldPreviousPartyEvent(EventEntry&,bool first,FieldPreviousPartyCursor&,
 void* operands,uint32_t (*word)(void*,uint32_t),const FieldPreviousPartyWorld&);
}
