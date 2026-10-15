#pragma once
#include "battle_actor.hpp"
// Desired count A+0x1377c and offset+0x13780 are distinct floats; baseline
// +0x13778 and signed capacity+0x13790 govern conversion/clamp. Immediate
// arithmetic failure can follow earlier chunk/offset stores; no rollback.
namespace ttyd {
// Original source chunk/ratio helper in its finite signed-conversion domain.
// False preserves performed stores and reports an unresolved numeric provider.
bool packedBattleAudienceAddTargetNumSub(float delta,const BattleActorServices&);
bool packedBattleAudienceAddTargetNum(float delta,float bias,const BattleActorServices&);
}
