#pragma once
#include "event_memory_opcodes.hpp"
#include <array>

namespace ttyd {
enum class FukidashiBinary { Add, Subtract };
struct FukidashiServices {
 EventMemoryServices memory;
 void* user=nullptr;
 uint8_t (*byte)(void*,uint32_t)=nullptr;
 // Pure, synchronous binary32 arithmetic; no implicit host numeric substitute.
 float (*binary)(void*,FukidashiBinary,float,float)=nullptr;
 bool (*position)(void*,uint32_t payload,int32_t secondArgument)=nullptr;
 bool (*effectDelete)(void*,uint32_t effect)=nullptr;
 std::optional<float> (*depth)(void*,const std::array<float,3>&)=nullptr;
 bool (*display)(void*,int32_t camera,int32_t mode,uint32_t callback,uint32_t effect,float)=nullptr;
};
// One original effFukidashiMain80132ea8 invocation, including kind6 spin.
// Memory accesses are synchronous mapped-domain preconditions. Children hold
// their actual work; a queue acknowledgment requires a real retained display.
// Inline storage avoids a heap allocation for every effect update. The caller
// owns the effect, payload, callback context and queued consumer lifetimes.
class FukidashiMainCursor {
public:
 bool active()const{return active_;}
 uint32_t pending()const{return pending_;}
 bool reset(); // Drops continuation only, never deletes source work.
private:
 bool active_=false,invoking_=false;
 uint32_t phase_=0,pending_=0,effect_=0,payload_=0;
 int32_t mode_=0,camera_=0;
 FukidashiServices owner_;
 std::array<float,3> oldPosition_{};
 float depth_=0;
 friend bool resumeFukidashiMain(uint32_t,const FukidashiServices&,FukidashiMainCursor&);
};
// Completion is one source driver poll, not one rendered frame. Original
// table pointers/bytes and constants are read from the supplied guest memory.
bool resumeFukidashiMain(uint32_t effect,const FukidashiServices&,FukidashiMainCursor&);
}
