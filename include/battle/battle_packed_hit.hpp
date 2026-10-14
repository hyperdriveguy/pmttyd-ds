#pragma once
#include "battle_packed_part.hpp"

namespace ttyd {
void packedHitPosition(uint32_t unit,uint32_t part,const std::array<float*,3>& outputs,const BattlePartPositionServices&);
std::array<float,3> packedHitPosition(uint32_t unit,uint32_t part,const BattlePartPositionServices&);
void writePackedHitPosition(uint32_t unit,uint32_t part,const std::array<uint32_t,3>& outputs,const BattlePartPositionServices&);
int32_t packedHitPositionEvent(EventEntry&,const BattlePartPositionServices&);
}
