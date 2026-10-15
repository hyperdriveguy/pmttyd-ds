#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
// Map singly linked size excludes32-byte header; SDK doubly linked span
// includes it. Their coalescing arithmetic is not interchangeable. Map null free
// is a no-op; SDK free requires its source valid-payload/handle domain.
void eventMapFree(uint32_t root,uint32_t payload,const EventMemoryServices&);
uint32_t eventSdkInsertFree(uint32_t freeHead,uint32_t node,const EventMemoryServices&);
void eventSdkFree(int32_t handle,uint32_t payload,const EventMemoryServices&);
void eventHeapFree(int32_t heapIndex,uint32_t payload,const EventMemoryServices&);
// Nested battle map pool0x64000 bytes (400KiB), carved from primary map.
// These leaves change original headers/globals, not semantic entry/VM retirement.
void allocateEventBattleMapPool(const EventMemoryServices&);
void freeEventBattleMapPool(const EventMemoryServices&);
}
