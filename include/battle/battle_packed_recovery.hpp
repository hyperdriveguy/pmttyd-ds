#pragma once
#include "battle_actor.hpp"
namespace ttyd {
int32_t packedBattleMaxFp(uint32_t unit, const BattleActorServices &);
void recoverPackedBattleHp(uint32_t unit, int32_t amount,
                           const BattleActorServices &);
void recoverPackedBattleFp(uint32_t unit, int32_t amount,
                           const BattleActorServices &);
} // namespace ttyd
