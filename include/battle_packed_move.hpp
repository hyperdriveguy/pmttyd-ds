#pragma once
#include "battle_actor.hpp"

namespace ttyd {
// Complete ResetMoveStatus on original unit bytes. Its used CheckStatus19/20
// cases each load one duration byte12f/130;other status cases remain separate.
void resetPackedBattleMoveStatus(uint32_t unit,const BattleActorServices&);
}
