#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
// Synchronous original npcInit/npcEntry primitives. All guest memory and pose
// services are caller-owned. Complete capacity/name/resource admission must
// precede effects; none of these callbacks represents an asynchronous success.
struct PackedNpcRegistryServices {
 EventMemoryServices memory;
 void* user=nullptr;
 uint32_t (*allocate)(void*,int32_t heap,uint32_t bytes)=nullptr;
 void (*clear)(void*,uint32_t address,uint32_t bytes)=nullptr;
 int32_t (*compare)(void*,uint32_t left,uint32_t right)=nullptr;
 void (*copyString)(void*,uint32_t destination,uint32_t source)=nullptr;
 int32_t (*poseEntry)(void*,uint32_t model,int32_t selector)=nullptr;
 void (*paperOn)(void*,int32_t pose)=nullptr;
 uint32_t (*heightBits)(void*,int32_t pose)=nullptr;
 uint32_t (*radiusBits)(void*,int32_t pose)=nullptr;
 int32_t (*vivianType)(void*,int32_t pose)=nullptr;
 void (*lightFlagOn)(void*,int32_t pose,uint32_t flags)=nullptr;
};
enum class PackedNpcLookup {NoAssert,Release};
void initializePackedNpcRegistry(const PackedNpcRegistryServices&);
// Release matches npcNameToPtr's unchecked missing-name result (end pointer),
// not an invented assertion or null. Native callers must admit registered names.
uint32_t packedNpcNameToPointer(uint32_t name,PackedNpcLookup,const PackedNpcRegistryServices&);
// Returns the allocated slot index. Duplicate scan does not reject or reuse a
// duplicate. Requires a valid free slot after that scan and real pose effects.
int32_t enterPackedNpc(uint32_t name,uint32_t model,const PackedNpcRegistryServices&);
}
