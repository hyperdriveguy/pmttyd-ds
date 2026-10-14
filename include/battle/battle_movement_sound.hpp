#pragma once
#include "battle_move_position.hpp"

namespace ttyd {
struct MovementPlaybackServices {
    void* user=nullptr;
    uint32_t (*trigger)(void*,MarioJumpUnit&,uint32_t key,int32_t rawSentinel,int32_t zero,JumpVector)=nullptr;
    JumpVector (*unitPosition)(void*,MarioJumpUnit&)=nullptr;
    JumpVector (*partPosition)(void*,void* part)=nullptr;
    void (*position)(void*,uint32_t handle,JumpVector)=nullptr;
    void (*off)(void*,uint32_t handle)=nullptr;
};
void controlMovementSound(MarioJumpUnit&,uint8_t* record,void* part,JumpVector,
                          const MovementPlaybackServices&);
void endMovementSound(uint8_t* record,const MovementPlaybackServices&);
void controlUnitMovementSound(MarioJumpUnit&,const MovementPlaybackServices&);
void endUnitMovementSound(MarioJumpUnit&,const MovementPlaybackServices&);
// The referenced playback-services object must outlive this adapter.
MovementSoundServices movementSoundAdapter(MovementPlaybackServices&);
}
