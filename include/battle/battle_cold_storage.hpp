#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
// Closed original storage initializers only. Guest memory must be admitted
// synchronously before entry; these bodies have no asynchronous child service.
// argument is the original r3 for private-pad/action/party initialization.
// Engaged0 completes void functions; Unit_Init returns1. Unknown function holds
// without side effects. Display/stage/audience/break-slot owners are separate.
std::optional<uint32_t> initializePackedBattleColdStorage(uint32_t function,
                                                         uint32_t argument,
                                                         const BattleActorServices&);
}
