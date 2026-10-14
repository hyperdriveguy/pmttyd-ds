#pragma once
#include "battle_actor.hpp"
#include <array>
namespace ttyd {
// Whole original GetItemOn and linked base/member/item getters. Arguments are
// original output addresses(index,X,Y,Z,item); zero XYZ/item outputs are
// optional. The no-match path unconditionally writes index=-1, matching the
// source.
void packedBattleAudienceItemOn(const std::array<uint32_t, 5> &,
                                const BattleActorServices &);
} // namespace ttyd
