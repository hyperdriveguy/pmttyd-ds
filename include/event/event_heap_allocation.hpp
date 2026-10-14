#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
// SDK first-fit heap allocation and __memAlloc wrapper. Handle table and
// descriptor/free/allocated nodes remain original-address memory services.
uint32_t eventSdkAllocate(int32_t handle,uint32_t requestedBytes,const EventMemoryServices&);
uint32_t eventHeapAllocate(int32_t heapIndex,uint32_t requestedBytes,const EventMemoryServices&);
}
