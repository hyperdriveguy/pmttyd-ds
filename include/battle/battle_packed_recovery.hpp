#pragma once
#include "battle_actor.hpp"
// HP/FP fields are signed16. Addition wraps word then narrows half BEFORE
// upper clamp; there is no lower clamp. These leaves do not update HP gauge data.
namespace ttyd {
int32_t packedBattleMaxFp(uint32_t unit, const BattleActorServices &);
void recoverPackedBattleHp(uint32_t unit, int32_t amount,
                           const BattleActorServices &);
void recoverPackedBattleFp(uint32_t unit, int32_t amount,
                           const BattleActorServices &);
} // namespace ttyd
