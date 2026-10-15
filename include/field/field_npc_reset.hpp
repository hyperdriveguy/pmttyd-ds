#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
struct PackedNpcResetServices {
 EventMemoryServices memory;
 void* user=nullptr;
 void (*clear)(void*,uint32_t address,uint32_t size)=nullptr;
};
// Exact npcReset8004bafc. Argument zero selects ordinary work; every nonzero
// value selects alternate work independently of GP+14. Does not release poses,
// delete events, clear auxiliary work/history or perform complete map unload.
// Reset captures capacity/array, clears only selected actor bytes/count,
// then ordinary mode halfword and global npcMainCount. It preserves F history.
void resetPackedNpcRegistry(int32_t selector,const PackedNpcResetServices&);
}
