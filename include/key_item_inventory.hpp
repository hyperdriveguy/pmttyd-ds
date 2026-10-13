#pragma once
#include "event_memory_opcodes.hpp"
#include "event_dispatch.hpp"
namespace ttyd {
// Original key-item1..120 branch,121 signed halfwords; all slots including gaps.
int32_t packedKeyItemCount(uint32_t pouch,int32_t item,const EventMemoryServices&);
int32_t removePackedKeyItem(uint32_t pouch,int32_t item,const EventMemoryServices&);
struct ItemInventoryEventServices {
    EventDispatchServices operands;
    void* user=nullptr;
    int32_t (*count)(void*,int32_t item)=nullptr;
    int32_t (*remove)(void*,int32_t item)=nullptr;
    void (*reviseMario)(void*)=nullptr;
    void (*reviseParty)(void*)=nullptr;
};
int32_t checkItemEvent(EventEntry&,const ItemInventoryEventServices&);
int32_t removeItemEvent(EventEntry&,const ItemInventoryEventServices&);
}
