#pragma once
#include "battle_packed_mario_jump.hpp"
namespace ttyd {
// Atomic source leaves: caller admits complete actual EVT/memory/math domains
// and stable owner for the whole call. No allocation, rendering, async provider
// acknowledgement or unavailable-as-zero behavior is supplied here.
struct BattleUnitRotationEventServices {
    BattleUnitEventServices units;
    BattlePackedMarioJumpServices angle;
};
// Public native reuse of the existing source integer conversion owner.
float convertPackedBattleRotationWord(int32_t,const BattleUnitRotationEventServices&);
void addPackedBattleUnitRotate(uint32_t unit,const std::array<float,3>&,
    const BattleUnitRotationEventServices&);
void setPackedBattleUnitRotate(uint32_t unit,const std::array<float,3>&,
    const BattleUnitRotationEventServices&);
int32_t battleAddUnitRotateEvent(EventEntry&,const BattleUnitRotationEventServices&);
int32_t battleSetUnitRotateEvent(EventEntry&,const BattleUnitRotationEventServices&);
int32_t battleCameraPriorityLimitEvent(EventEntry&,const BattleUnitEventServices&);
}
