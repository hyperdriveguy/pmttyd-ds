#pragma once
#include "battle_actor.hpp"

namespace ttyd {
uint8_t checkBattleZeroTurnCount(uint32_t sum,const BattleActorServices&);
void judgeBattleTurnRule(const BattleActorServices&);
}
