#pragma once
#include "battle_actor.hpp"
// Pure judgment over retained unsigned action-record bytes. Does not clear
// counters, award results, play messages or prove a challenge has concluded.
namespace ttyd {
int32_t checkBattleRuleNoUse(int32_t,const BattleActorServices&);
int32_t checkBattleRuleUse(int32_t,const BattleActorServices&);
// Complete general/end rule selector, with unconditional explicit-B unit
// queries and independent helper recapture/write ownership.
void judgeBattleGeneralRule(const BattleActorServices&);
}
