#pragma once
#include "battle_jump_motion.hpp"

namespace ttyd {
struct MovePositionArguments {
    JumpVector destination{}; // Immediately converted signed integer operands.
    int32_t count=0,soundMode=-1,soundType=0;
};
struct MovementSoundServices {
    void* user=nullptr;
    void (*control)(void*,MarioJumpUnit&)=nullptr;
    void (*end)(void*,MarioJumpUnit&)=nullptr;
};
// Original horizontal helper permits aliased X/Z destinations and reads them
// only after both trig calls. Math-service behavior remains separately owned.
void integrateMoveHorizontal(float* x,float* z,float speed,float heading,const JumpMotionServices&);
int32_t queryMoveFrame(MarioJumpUnit&,float x,float z,float requestedSpeed,const JumpMotionServices&);
int queryMoveFrameEvent(const std::array<int32_t,6>& operands,const JumpEventServices&,const JumpMotionServices&);
int movePosition(MarioJumpUnit&,MarioJumpEvent&,const MovePositionArguments&,bool firstCall,
                 const JumpMotionServices&,const MovementSoundServices&);
int movePositionEvent(MarioJumpEvent&,const std::array<int32_t,7>& operands,bool firstCall,
                      const JumpEventServices&,const JumpMotionServices&,const MovementSoundServices&);
}
