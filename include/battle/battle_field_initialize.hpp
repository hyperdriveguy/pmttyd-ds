#pragma once
#include "battle_sequence_entry.hpp"
namespace ttyd {
struct BattleFieldInitializeCursor {
 bool active=false,queued=false;
 uint32_t phase=0,pending=0,gp=0,field=0,npc=0,pouch=0,party=0,kind=0,value=0,information=0;
 BattleSequenceCall call;
};
// Complete battle_init800738ac parent. Child completion is explicit, including
// the lower BattleInit; successful completion returns original integer zero.
bool initializeFieldBattle(const BattleSequenceEntryServices&,BattleFieldInitializeCursor&);
}
