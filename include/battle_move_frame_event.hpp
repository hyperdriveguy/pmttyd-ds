#pragma once
#include "battle_packed_choice.hpp"
namespace ttyd {
struct BattleMoveFrameServices {
    BattlePackedChoiceServices common;
    void* user=nullptr;
    float (*distance)(void*,float currentX,float currentZ,float targetX,float targetZ)=nullptr;
};
int32_t packedMoveFrameEvent(EventEntry&,const BattleMoveFrameServices&);
}
