#include "event_memory_binding.hpp"
#include <cassert>

namespace ttyd {
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
