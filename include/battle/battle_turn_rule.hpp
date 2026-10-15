#pragma once
#include "battle_actor.hpp"

// Action-record rule judgment is storage-only. Source outcome bytes1/2/6
// remain numeric domains, not bool success/failure or a user-visible message.
namespace ttyd {
uint8_t checkBattleZeroTurnCount(uint32_t sum,const BattleActorServices&);
void judgeBattleTurnRule(const BattleActorServices&);
}
