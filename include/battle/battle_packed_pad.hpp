#pragma once
#include "battle_defense.hpp"
#include "battle_actor.hpp"
namespace ttyd {
BattlePadWork readPackedBattlePad(uint32_t address,const BattleActorServices&);
void writePackedBattlePad(uint32_t address,const BattlePadWork&,const BattleActorServices&);
}
