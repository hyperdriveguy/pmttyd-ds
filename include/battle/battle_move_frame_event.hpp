#pragma once
#include "battle_packed_choice.hpp"
// Packed EVT query only: result is source simulation-visit count, not a render
// frame measurement. Inputs/output keep their own integer/float/sentinel domains.
namespace ttyd {
struct BattleMoveFrameServices {
    BattlePackedChoiceServices common;
    void* user=nullptr;
    float (*distance)(void*,float currentX,float currentZ,float targetX,float targetZ)=nullptr;
};
int32_t packedMoveFrameEvent(EventEntry&,const BattleMoveFrameServices&);
}
