#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
// SDK first-fit heap allocation and __memAlloc wrapper. Handle table and
// descriptor/free/allocated nodes remain original-address memory services.
// SDK descriptor stride12, node/payload offset32, first-fit signed span
// comparison; (bytes+63)&~31 includes header and rounded payload. Wrapper
// clears/flushes requested bytes only. Return0 is actual no-fit allocation.
uint32_t eventSdkAllocate(int32_t handle,uint32_t requestedBytes,const EventMemoryServices&);
uint32_t eventHeapAllocate(int32_t heapIndex,uint32_t requestedBytes,const EventMemoryServices&);
}
