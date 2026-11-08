#include "event_memory_binding.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>

// Adapt mapped memory to recovered service ABI. Missing mappings/providers
// violate required domains (assert); cache flush/system call stay platform-owned.
// Returning zeros for unsupported memory would falsely certify original behavior.
namespace ttyd {
uint32_t EventMemoryBinding::allocateHeap(int32_t heapIndex,uint32_t bytes) {
    const auto memoryServices=services();uint32_t address=0;
    const bool borrowed=memory.withRawRamAccess([&](auto&) {
        constexpr uint32_t GameHeapHandles=0x8041e844;
        const uint32_t rawHandle=memoryServices.word(memoryServices.user,GameHeapHandles+4u*uint32_t(heapIndex));
        const int32_t handle=rawHandle<=0x7fffffffu?int32_t(rawHandle):int32_t(int64_t(rawHandle)-0x100000000ll);
        address=eventSdkAllocate(handle,bytes,memoryServices);
        if(address)eventFillMemory(address,0,bytes,memoryServices);
        return true;
    });
    // A caller already borrowing RAM retains its existing cache and behavior.
    if(!borrowed)return eventHeapAllocate(heapIndex,bytes,memoryServices);
    // Platform children execute after releasing the borrow, in source order.
    if(address)eventFlushMemory(address,bytes,memoryServices);
    return address;
}
EventMemoryServices EventMemoryBinding::services() {
    EventMemoryServices s;s.user=this;
    s.word=[](void* p,uint32_t a) {uint32_t v=0;const bool ok=static_cast<EventMemoryBinding*>(p)->memory.readWord(a,v);assert(ok);(void)ok;return v;};
    s.half=[](void* p,uint32_t a) {uint16_t v=0;const bool ok=static_cast<EventMemoryBinding*>(p)->memory.readHalf(a,v);assert(ok);(void)ok;return v;};
    s.storeWord=[](void* p,uint32_t a,uint32_t v) {const bool ok=static_cast<EventMemoryBinding*>(p)->memory.writeWord(a,v);assert(ok);(void)ok;};
    s.storeHalf=[](void* p,uint32_t a,uint16_t v) {const bool ok=static_cast<EventMemoryBinding*>(p)->memory.writeHalf(a,v);assert(ok);(void)ok;};
    s.storeByte=[](void* p,uint32_t a,uint8_t v) {const bool ok=static_cast<EventMemoryBinding*>(p)->memory.writeByte(a,v);assert(ok);(void)ok;};
    s.flushBlock=[](void* p,uint32_t a) {auto& b=*static_cast<EventMemoryBinding*>(p);assert(b.flushBlock);b.flushBlock(b.platformUser,a);};
    s.systemCall=[](void* p) {auto& b=*static_cast<EventMemoryBinding*>(p);assert(b.systemCall);b.systemCall(b.platformUser);};
    return s;
}
}
