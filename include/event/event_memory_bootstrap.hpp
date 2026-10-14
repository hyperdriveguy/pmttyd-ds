#pragma once
#include "event_heap_allocation.hpp"

namespace ttyd {
uint32_t eventSdkInitializeAllocator(uint32_t start,uint32_t end,int32_t count,const EventMemoryServices&);
int32_t eventSdkCreateHeap(uint32_t start,uint32_t end,const EventMemoryServices&);
void eventSdkDestroyHeap(int32_t handle,const EventMemoryServices&);
void initializeEventMemoryPools(const EventMemoryServices&);
void clearEventMemoryPool(int32_t index,const EventMemoryServices&);
}
