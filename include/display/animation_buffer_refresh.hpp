#pragma once
#include "battle_sequence_entry.hpp"
#include <memory>
// Nested cursors retain parent/child state, including successful allocations.
// active resumes only the same function; callers keep original memory/services
// alive. A false result leaves publications intact rather than rolling back.
namespace ttyd {
struct AnimationBufferCursor {
 bool active=false,queued=false;
 uint32_t function=0,phase=0,pending=0,work=0,base=0,value=0,result=0;
 uint32_t bytes=0,aligned=0,attempt=0,reserved=0,index=0,offset=0,pose=0,buffer=0,model=0;
 BattleSequenceCall call;
 std::unique_ptr<AnimationBufferCursor> child;
};
// Complete retained animPoseBattleInit, animPoseRefresh, testAlloc and
// initTestHeap and animPose_AllocBuffer parents. Heap0 remains a real service.
// The second argument is a source pose address for animPose_AllocBuffer.
// Recursive recovery is retained, not collapsed to ring-buffer overwrite.
bool runAnimationBufferFunction(uint32_t function,uint32_t bytes,
 const BattleSequenceEntryServices&,AnimationBufferCursor&);
uint32_t pendingAnimationBufferFunction(const AnimationBufferCursor&);
}
