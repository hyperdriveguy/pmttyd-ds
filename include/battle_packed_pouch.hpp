#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
struct PackedPouchInitializationServices {
 EventMemoryServices memory;
 void* user=nullptr;
 uint32_t (*allocate)(void*,int32_t heap,uint32_t bytes)=nullptr;
 // Source msgSearch("name_party3"); caller retains actual localized text.
 const char* (*message)(void*,const char* key)=nullptr;
};
// Original fresh pouchInit allocation/field initialization. Save loading and
// later field item/equipment changes belong to the owning lifecycle.
uint32_t initializePackedPouch(const PackedPouchInitializationServices&);
// Original pouchCheckEquipBadge800d3300, including ID0 matching empty slots.
int32_t packedPouchEquippedBadgeCount(uint32_t pouch,int32_t badge,
 const EventMemoryServices&);
}
