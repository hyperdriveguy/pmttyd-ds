#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
void eventMapFree(uint32_t root,uint32_t payload,const EventMemoryServices&);
uint32_t eventSdkInsertFree(uint32_t freeHead,uint32_t node,const EventMemoryServices&);
void eventSdkFree(int32_t handle,uint32_t payload,const EventMemoryServices&);
void eventHeapFree(int32_t heapIndex,uint32_t payload,const EventMemoryServices&);
void allocateEventBattleMapPool(const EventMemoryServices&);
void freeEventBattleMapPool(const EventMemoryServices&);
}
