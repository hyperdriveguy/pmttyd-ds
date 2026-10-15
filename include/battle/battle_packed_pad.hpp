#pragma once
#include "battle_defense.hpp"
#include "battle_actor.hpp"
// Packed private battle-pad history, not physical input polling. Read/write
// services preserve exact source order and aliasing; this helper does not advance
// histories or normalize axes. See battle_defense for simulation-tick ownership.
namespace ttyd {
BattlePadWork readPackedBattlePad(uint32_t address,const BattleActorServices&);
void writePackedBattlePad(uint32_t address,const BattlePadWork&,const BattleActorServices&);
}
