#pragma once
#include "event_heap_allocation.hpp"

namespace ttyd {
// SDK init returns aligned usable ADDRESS; Create returns heap HANDLE or-1.
// Descriptor/arena globals are original guest memory. Pools are five source
// configurations (fixed KiB or percentages of one captured remaining arena);
// caller provides valid extents and cache services, not native host malloc.
uint32_t eventSdkInitializeAllocator(uint32_t start,uint32_t end,int32_t count,const EventMemoryServices&);
int32_t eventSdkCreateHeap(uint32_t start,uint32_t end,const EventMemoryServices&);
void eventSdkDestroyHeap(int32_t handle,const EventMemoryServices&);
void initializeEventMemoryPools(const EventMemoryServices&);
void clearEventMemoryPool(int32_t index,const EventMemoryServices&);
}
