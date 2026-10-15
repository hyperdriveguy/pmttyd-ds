#pragma once
#include "battle_actor.hpp"
namespace ttyd {
// Whole BtlUnit_CheckRecoveryStatus80127890/2c8, one explicit source step.
// Result1 means the local recovered duration reached zero; result0 may mutate.
// Raw id narrows to signed low8. Guest reads/writes and reached move-reset
// children remain source ordered; no unit snapshot or writeback is performed.
int32_t checkPackedBattleRecoveryStatus(uint32_t unit, int32_t rawId,
                                        const BattleActorServices &);
} // namespace ttyd
