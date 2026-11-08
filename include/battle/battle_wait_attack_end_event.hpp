#pragma once
#include "battle_actor.hpp"
#include "event_creation.hpp"
namespace ttyd {
// Whole8010a154/74. Incoming EVT pointer is source-unused. Return0 is an
// executed live-event poll, not unavailable; every later poll rereads Battle.
int32_t battleWaitAttackEndEvent(const BattleActorServices &,
                                 const EventCreationServices &);
} // namespace ttyd
