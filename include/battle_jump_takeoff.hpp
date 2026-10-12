#pragma once
#include "battle_position.hpp"

namespace ttyd {
// Private semantic sample, not the shared attack-selection buffer or packed ABI.
// Sampling services own initialization and any candidate storage behind data.
struct TakeoffSample {int8_t count;void* data;};
struct TakeoffServices {
    void* user=nullptr;
    void* (*captureWork)(void*)=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    BattlePositionUnit* (*unit)(void*,void* capturedWork,int32_t)=nullptr;
    int32_t (*enemyBelong)(void*,BattlePositionUnit&)=nullptr;
    uint8_t (*sideByte)(void*,void* capturedWork,int8_t belong)=nullptr;
    int32_t (*sample)(void*,TakeoffSample&,const void* weapon,int32_t actor,int8_t belong,
                      uint32_t targetFlags,uint32_t specialFlags,int8_t bias)=nullptr;
    // Must write chosen IDs when positive count makes those outputs readable.
    int32_t (*choose)(void*,TakeoffSample&,int32_t mode,int32_t& unit,int32_t& part)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr;
    void (*store)(void*,int32_t rawDestination,int32_t)=nullptr;
};
int jumpTakeoffPositionEvent(const std::array<int32_t,6>& operands,const TakeoffServices&,
                             const BattleHitPositionServices&);
}
