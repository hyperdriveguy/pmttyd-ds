#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleMajinaiQueryServices {
    EventMemoryServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
};
void battleMajinaiDone(const BattleMajinaiQueryServices&);
void battleMajinaiEndCheck(const BattleMajinaiQueryServices&);
int32_t battleMajinaiPowerQuery(EventEntry&,const BattleMajinaiQueryServices&);
}
