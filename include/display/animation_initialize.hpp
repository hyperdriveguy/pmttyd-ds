#pragma once
#include "battle_sequence_entry.hpp"
// The cursor owns successful child results and captured operands across holds.
// Retain the same service/guest owners until completion; false never authorizes
// restarting cold initialization or treating prior allocations as rolled back.
namespace ttyd {
struct AnimationInitializeServices {
 BattleSequenceEntryServices memory;
 // One retained lfd read; never expose a half-captured double across holds.
 std::optional<uint64_t> (*doubleWord)(void*,uint32_t)=nullptr; // memory.user
 void* tangentUser=nullptr;
 // Source tan802724e8 receives a widened binary32 angle and returns double.
 // False retains this child; native libm is an explicit numeric adaptation.
 bool (*tangent)(void*,double,double&)=nullptr;
};
struct AnimationInitializeCursor {
 bool active=false,queued=false;
 uint32_t phase=0,pending=0,work=0,index=0,allocation=0,value=0,pointer=0;
 uint32_t handle=0,holder=0,destination=0,payload=0;
 float factor=0;
 double bias=0,angle=0,tangentResult=0;
 BattleSequenceCall call;
};
// Complete animInit80042854 parent. Must precede source pose creation; it is
// not an attachment/reset API for live shared pools. Heap/file/copy children
// remain genuine services; no narrow prefix is acknowledged as full success.
bool initializeAnimation(const AnimationInitializeServices&,AnimationInitializeCursor&);
bool nativeAnimationTangent(void*,double,double&);
}
