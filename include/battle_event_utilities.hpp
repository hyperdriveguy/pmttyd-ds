#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleEventUtilityServices {
    EventMemoryServices memory;
    EventDispatchServices values;
};
int32_t battleFlagQueryEvent(EventEntry&,const BattleEventUtilityServices&);
int32_t battleFramesToMillisecondsEvent(EventEntry&,const BattleEventUtilityServices&);
}
