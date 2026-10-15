#pragma once
#include "battle_move_frame_event.hpp"
#include "battle_packed_movement_sound.hpp"
// firstCall owns setup only; all operands still resolve on every invocation.
// Providers retain exact numeric/RNG/audio boundaries; this synchronous leaf
// has no continuation for unavailable children. Source returns are0/1, not bool.
namespace ttyd {
struct BattlePackedMovePositionServices {
    BattleMoveFrameServices query;
    BattlePackedMovementSoundServices sound;
    float (*heading)(void*,float,float,float,float)=nullptr; // query.user
    double (*sine)(void*,double)=nullptr; // query.common.position.numericUser
    double (*cosine)(void*,double)=nullptr;
    float (*roundDouble)(void*,double)=nullptr;
};
void setPackedMovePosition(uint32_t unit,bool target,const std::array<float,3>&,const EventMemoryServices&);
void integratePackedMoveHorizontal(uint32_t x,uint32_t z,float speed,float heading,const BattlePackedMovePositionServices&);
int32_t packedMovePositionEvent(EventEntry&,int32_t firstCall,const BattlePackedMovePositionServices&);
}
