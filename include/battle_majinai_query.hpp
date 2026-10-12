#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct BattleMajinaiQueryServices {
    EventMemoryServices memory;
    EventDispatchServices values;
    void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    int32_t (*irand)(void*,int32_t bound)=nullptr;
};
int32_t battleMajinaiCheck(const BattleMajinaiQueryServices&);
void initializePouchMajinai(int32_t count,const BattleMajinaiQueryServices&);
int32_t pouchMajinaiSetEvent(EventEntry&,const BattleMajinaiQueryServices&);
void battleMajinaiDone(const BattleMajinaiQueryServices&);
void battleMajinaiEndCheck(const BattleMajinaiQueryServices&);
int32_t battleMajinaiPowerQuery(EventEntry&,const BattleMajinaiQueryServices&);
}
