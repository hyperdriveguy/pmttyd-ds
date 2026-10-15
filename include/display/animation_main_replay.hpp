#pragma once
#include "animation_main_timeline.hpp"
namespace ttyd {
struct AnimationMainReplayServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 bool (*storeByte)(void*,uint32_t,uint8_t)=nullptr;
 // Actual GQR4 W1 source load. A native decoder must prove its platform state;
 // a missing/wrong quantization owner is unavailable, never assumed signed8.
 std::optional<float> (*quantized)(void*,uint32_t)=nullptr;
};
struct AnimationReplayCopy {
 bool active=false,backward=false,loaded=false;uint32_t function=0,destination=0,source=0,bytes=0,remaining=0,value=0;
};
struct AnimationMainReplayResult {
 uint32_t current=0,next=0;float fraction=0;std::array<uint32_t,5> counts{};
};
struct AnimationMainReplayCursor {
 bool active=false,rebuild=false;uint32_t phase=0,old=0,key=0,end=0,channel=0,offset=0,count=0,stream=0,source=0,destination=0,bulkLimit=0,entryIndex=0,lane=0,word=0,deltaIndex=0,increment=0;
 uint64_t remaining=0;float scale=0,oldValue=0,delta=0,newValue=0;std::array<float,3> deltas{};
 AnimationMainEvaluation input;AnimationMainReplayServices owner;AnimationMainReplayResult result;AnimationReplayCopy copy;
};
// Full original cache region8003f0f4..80040084, including both replay paths,
// five base copies, all stream operations and same-key fraction reset. Copy
// helpers retain exact forward-word/backward-or-forward-byte semantics, even
// do/while shifted-count0 and texture-derived-count0 wrap domains.
// False may follow stores or cooperative entry/copy/key yields. Primitive
// unavailable results have no effects. Retain the SAME cursor/input/services/
// producer pass. Output publishes at completion; final R3c/R40 publication
// belongs AFTER interpolation and is not acknowledged by this child.
bool advanceAnimationMainReplay(const AnimationMainEvaluation&,const AnimationMainReplayServices&,AnimationMainReplayCursor&,AnimationMainReplayResult&);
struct AnimationMainCacheCursor {
 bool active=false;uint32_t phase=0;AnimationMainEvaluation input;
 AnimationMainTimeServices timeOwner;AnimationMainReplayServices replayOwner;
 AnimationMainScalarCursor scalar;AnimationMainReplayCursor replay;
};
// Contiguous nonnull Main through40084. Holds in replay never re-run scalar
// evaluation. Remaining interpolation and final R3c/R40 stores stay separate.
bool advanceAnimationMainCache(const AnimationMainEvaluation&,const AnimationMainTimeServices&,const AnimationMainReplayServices&,AnimationMainCacheCursor&,AnimationMainReplayResult&);

}
