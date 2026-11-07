#pragma once
#include "battle_sequence_entry.hpp"
namespace ttyd {
// One original animPoseRelease8003b2d0 invocation. Service false/empty has no
// effects; the parent can already have published stores and must be resumed.
// Raw pointers/indices are source address tokens, not native sidecar identities.
// queued captures the one child argument through holds. A changed source
// file word cannot replace that call; abandoning a cursor cannot roll back refs.
struct AnimationReleaseCursor {
 bool active=false,queued=false;
 uint32_t handle=0,phase=0,pending=0,work=0,pose=0,model=0,texture=0,index=0,value=0;
 BattleSequenceEntryServices owner;
 BattleSequenceCall call;
};
// Complete ordinary and borrowed/group graph, including both smartFree sites.
// No source ID/active/count validation is invented here. Keep the same live
// providers and request across holds; callers own lifetime admission.
bool releaseAnimationPose(int32_t,const BattleSequenceEntryServices&,AnimationReleaseCursor&);
}
