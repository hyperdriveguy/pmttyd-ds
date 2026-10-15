#pragma once
#include "battle_sequence_entry.hpp"
#include <memory>
namespace ttyd {
struct AnimationBufferCursor {
 bool active=false,queued=false;
 uint32_t function=0,phase=0,pending=0,work=0,base=0,value=0,result=0;
 uint32_t bytes=0,aligned=0,attempt=0,reserved=0,index=0,offset=0,pose=0,buffer=0,model=0;
 BattleSequenceCall call;
 std::unique_ptr<AnimationBufferCursor> child;
};
// Complete retained animPoseBattleInit, animPoseRefresh, testAlloc and
// initTestHeap parents. Heap0 and animPose_AllocBuffer remain real services.
// Recursive recovery is retained, not collapsed to ring-buffer overwrite.
bool runAnimationBufferFunction(uint32_t function,uint32_t bytes,
 const BattleSequenceEntryServices&,AnimationBufferCursor&);
uint32_t pendingAnimationBufferFunction(const AnimationBufferCursor&);
}
