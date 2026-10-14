#pragma once
#include "battle_unit_event.hpp"
#include "battle_actor.hpp"
namespace ttyd {
int32_t battleMarkUnit1feEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleGetHitDamageEvent(EventEntry&,const BattleUnitEventServices&,const BattleActorServices&);
int32_t battleUnitFlagEvent(EventEntry&,bool on,const BattleUnitEventServices&);
int32_t battleResetUnitFacingEvent(EventEntry&,const BattleUnitEventServices&,const BattleActorServices&);
int32_t battleCheckUnitAttributeEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckHitPatternEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleCheckHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
int32_t battleGetHitCodeEvent(EventEntry&,const BattleUnitEventServices&);
}
