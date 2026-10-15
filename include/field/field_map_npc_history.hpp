#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
class EventAddressSpace;
struct PackedMapNpcHistoryServices {
 EventMemoryServices memory;
 void* user=nullptr;
 int32_t (*compare)(void*,uint32_t,uint32_t)=nullptr;
 void (*copyString)(void*,uint32_t,uint32_t)=nullptr;
 void (*clear)(void*,uint32_t,uint32_t)=nullptr;
};
struct PackedMapNpcHistoryGate {uint32_t head=0;bool enabled=false;};
// Source first match or actual null-name terminator (stride12), not a default.
uint32_t packedMapNpcMapDefinition(uint32_t map,const PackedMapNpcHistoryServices&);
// Admitted setupDataBase84f44..85214. F is the captured fbatGetPointer result.
// Full64/no-match holds before the source's out-of-history promotion domain.
std::optional<PackedMapNpcHistoryGate> preparePackedMapNpcHistory(uint32_t field,uint32_t map,const PackedMapNpcHistoryServices&);
uint32_t packedMapNpcOrdinalMask(uint32_t ordinal);
bool skipPackedMapNpc(const PackedMapNpcHistoryGate&,uint32_t ordinal,const EventMemoryServices&);
// Bounded native admission before any history stores. Actual retained F and
// source table/string bytes must already exist; never seeds an empty history.
std::optional<PackedMapNpcHistoryGate> prepareFieldMapNpcHistory(EventAddressSpace&,uint32_t field,uint32_t map);
std::optional<bool> skipFieldMapNpc(EventAddressSpace&,const PackedMapNpcHistoryGate&,uint32_t ordinal);
}
