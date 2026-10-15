#pragma once
#include "battle_actor.hpp"
#include "battle_status.hpp"

// Packed status bytes, not a serialized native StatusValue. ID narrows through
// signed low8 before dispatch; get requires an admitted unit, check handles null.
namespace ttyd {
StatusValue getPackedBattleStatus(uint32_t unit,int32_t id,const BattleActorServices&);
// Original output pointers are optional;both source bytes are read BEFORE any
// output store. Duration output precedes strength even when pointers alias.
void writePackedBattleStatus(uint32_t unit,int32_t id,uint32_t durationOutput,uint32_t strengthOutput,const BattleActorServices&);
bool checkPackedBattleStatus(uint32_t unit,int32_t id,const BattleActorServices&);
}
