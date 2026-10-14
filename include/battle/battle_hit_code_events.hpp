#pragma once
#include "battle_unit_event.hpp"
#include "battle_actor.hpp"
namespace ttyd {
struct BattleCheckDamageEventServices {
    BattleUnitEventServices units;
    void* user=nullptr;
    uint32_t (*part)(void*,uint32_t unit,int32_t id)=nullptr;
    // False retains an unbound port service; no output/ID query is performed.
    bool (*checkDamage)(void*,uint32_t attacker,uint32_t defender,uint32_t part,
                        uint32_t weapon,uint32_t flags,int32_t& result)=nullptr;
    bool (*eventExists)(void*,uint32_t id)=nullptr;
};
int32_t battleCheckDamageEvent(EventEntry&,const BattleCheckDamageEventServices&);

int32_t battleMarkUnit1feEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleGetHitDamageEvent(EventEntry&,const BattleUnitEventServices&,const BattleActorServices&);
int32_t battleUnitFlagEvent(EventEntry&,bool on,const BattleUnitEventServices&);
int32_t battleResetUnitFacingEvent(EventEntry&,const BattleUnitEventServices&,const BattleActorServices&);
int32_t battleCheckUnitAttributeEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckHitPatternEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleGetHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
}
