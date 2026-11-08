#pragma once
#include "battle_first_act_continuation.hpp"
namespace ttyd {
// Shared startup child ABI, with no firstAttackUnits on Init requests.
using BattleInitSequenceServices=BattleFirstActContinuationServices;
class BattleInitSequenceCursor {
public:
 BattleInitSequenceCursor();~BattleInitSequenceCursor();
 BattleInitSequenceCursor(const BattleInitSequenceCursor&)=delete;
 BattleInitSequenceCursor&operator=(const BattleInitSequenceCursor&)=delete;
 bool active()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumePackedBattleSequenceInit(uint32_t,const BattleInitSequenceServices&,BattleInitSequenceCursor&);
};
// Whole8011ee40/198; actual UnitSetup and optional resource children retain
// their work. Original resource0 returns normally; unavailable is nullopt.
bool resumePackedBattleSequenceInit(uint32_t,const BattleInitSequenceServices&,BattleInitSequenceCursor&);
}
