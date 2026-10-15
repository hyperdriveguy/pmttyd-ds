#pragma once
#include "event_address_space.hpp"
#include "event_memory_opcodes.hpp"

namespace ttyd {
// Bind recovered memory/allocator operations to shared original-address storage.
// Unsupported accesses assert as a native domain policy. Cache operations must
// be supplied by the platform owner;original addresses are never host pointers.
struct EventMemoryBinding {
    EventAddressSpace& memory;
    void* platformUser=nullptr;
    void (*flushBlock)(void*,uint32_t originalAddress)=nullptr;
    void (*systemCall)(void*)=nullptr;
    // Returned closures borrow this binding and facade. Accesses require
    // mapped domains (assert policy), not optional retries; callers must not treat
    // disabled assertions as a supported missing-memory fallback.
    EventMemoryServices services();
};
}
