#pragma once
#include "battle_packed_move_position.hpp"
namespace ttyd {
struct BattlePackedGenericJumpServices {
 BattlePackedMovePositionServices movement;
 void* user=nullptr;
 void (*play)(void*,uint32_t unit,uint32_t key,const std::array<float,3>&)=nullptr;
 float (*floor)(void*,uint32_t battle,float x,float y,float z)=nullptr;
};
// Original non-Mario JumpPosition/JumpContinue over packed source unit fields.
int32_t packedGenericJumpPositionEvent(EventEntry&,int32_t first,const BattlePackedGenericJumpServices&);
int32_t packedGenericJumpContinueEvent(EventEntry&,int32_t first,const BattlePackedGenericJumpServices&);
}
