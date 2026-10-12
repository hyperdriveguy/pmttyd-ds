#pragma once
#include "battle_unit_event.hpp"
#include "battle_actor.hpp"

namespace ttyd {
uint32_t packedBattlePartnerUnit(uint32_t battle,uint32_t unit,const BattleActorServices&);
// Outputs are native caller-owned slots;their write order and aliasing are
// preserved. Return is the last getter's original pointer token,not target/miss.
uint32_t packedFirstAttackPair(uint32_t battle,uint32_t* attacker,uint32_t* target,const BattleActorServices&);
struct BattleFirstAttackServices {
    BattleActorServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    uint32_t (*pair)(void*,uint32_t battle,uint32_t* attacker,uint32_t* target)=nullptr;
};
int32_t packedFirstAttackTargetEvent(EventEntry&,const BattleFirstAttackServices&);
}
