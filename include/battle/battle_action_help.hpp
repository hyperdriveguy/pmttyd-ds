#pragma once
#include "battle_actor.hpp"
#include "battle_unit_event.hpp"
namespace ttyd {
struct BattleActionHelpServices {
    BattleActorServices actors;
    void* user=nullptr;
    bool (*display)(void*,int32_t camera,int32_t displayClass,uint32_t callback,
        uint32_t argument,float priority)=nullptr;
};
// Source HelpMain: only actual key0, mode0 or Bef4&2 skip registration.
void packedBattleActionHelpSetKey(uint32_t,const BattleActorServices&);
void packedBattleActionHelpSetDisplay(int32_t,const BattleActorServices&);
int32_t packedBattleStartActionCommandEvent(EventEntry&,const BattleUnitEventServices&,const BattleActorServices&);
bool packedBattleActionHelpMain(const BattleActionHelpServices&);
}
