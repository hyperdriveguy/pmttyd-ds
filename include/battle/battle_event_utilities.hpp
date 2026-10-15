#pragma once
#include "event_memory_opcodes.hpp"

// EVT completed2 versus continuing0 differs from query output values0/1.
// Services retain source raw destinations and masks rather than resolving all
// arguments uniformly; native caller admits current battle memory.
namespace ttyd {
struct BattleEventUtilityServices {
    EventMemoryServices memory;
    EventDispatchServices values;
};
int32_t battleWaitGuardMove(const BattleEventUtilityServices&);
int32_t battleFlagQueryEvent(EventEntry&,const BattleEventUtilityServices&);
int32_t battleFramesToMillisecondsEvent(EventEntry&,const BattleEventUtilityServices&);
}
