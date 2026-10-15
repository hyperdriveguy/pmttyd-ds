#pragma once
#include "battle_move_frame_event.hpp"
// Operand4 is both resolved requested count and RAW output destination.
// Skip depends on RESULT sentinel, unlike GetMoveFrame's raw-output sentinel.
namespace ttyd {
int32_t packedMarioJumpParameterEvent(EventEntry&,const BattleMoveFrameServices&);
}
