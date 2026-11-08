#pragma once
#include "animation_facing.hpp"
namespace ttyd {
// Captured original Main registers at the evaluation boundary8003ee08.
// Guest addresses and output pointers, never native pointers. The evaluation
// child must use these captures across its own holds and linked mutations.
struct AnimationMainEvaluation {
 uint32_t work=0,pool=0,pose=0,model=0,timeline=0,positions=0,normals=0;
};
struct AnimationMainServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*gqr)(void*,uint32_t,uint32_t)=nullptr;
 // Children may return false after progress. Their own cursors/owners must
 // persist. In particular BattlePosePool::evaluate is NOT an atomic retryable
 // substitute: it can publish source state before failure.
 bool (*linked)(void*,uint32_t)=nullptr;
 bool (*evaluate)(void*,const AnimationMainEvaluation&)=nullptr;
 AnimationFacingServices facing;
};
// This retained invocation owns no VM/resource lifetime. The caller keeps the
// producer pass and each child's cursor alive across partial source effects.
struct AnimationMainCursor {
 bool active=false;uint32_t phase=0,handle=0,models=0,resource=0,index=0,entry=0,holder=0,directory=0,selector=0,linked=0,flags=0;
 AnimationMainServices owner;AnimationMainEvaluation captured;AnimationFacingCursor facing;
};
// Original Main prelude8003ed4c..ee04, explicit retained evaluation child,
// optional tail80040540..40648. No ID/active/null guards, recursion cap or
// once-per-tick filter are invented. Word/GQR unavailable means no effects;
// linked/evaluation/facing false can follow child progress. Retain this cursor
// and the SAME producer pass until completion. No automatic parent replay.
// On completion resets the cursor. GQR service owns actual quantized-load
// semantics/adaptation; a missing service holds even for a null timeline.
bool advanceAnimationMain(uint32_t handle,const AnimationMainServices&,AnimationMainCursor&);
}
