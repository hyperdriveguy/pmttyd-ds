#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
// References to the active command's shared semantic fields. No copied result
// or per-hit count reset: callback mutations are visible to later script reads.
struct ActionCommandView {
    int32_t* state=nullptr;
    uint32_t* result=nullptr;
    int32_t* count=nullptr;
    uint32_t* timingOptions=nullptr;
    uint32_t* setupFlags=nullptr;
    std::array<int32_t,8>* parameters=nullptr;
    const uint32_t* actorFlags=nullptr;
    void* user=nullptr;
    int32_t (*evaluate)(void*)=nullptr;
    void (*remove)(void*)=nullptr; // Original delete callback, not removal itself.
    int32_t (*successWindow)(void*)=nullptr;
    void (*help)(void*,int32_t)=nullptr;
};
struct ActionValueServices {
    void* user=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    void (*store)(void*,int32_t,int32_t)=nullptr;
};
int setActionParameters(ActionCommandView&,const std::array<int32_t,8>&,const ActionValueServices&);
int setActionFlags(ActionCommandView&,uint32_t rawMask);
int startActionCommand(ActionCommandView&,int32_t rawOn,const ActionValueServices&);
int32_t evaluateActionCommand(ActionCommandView&);
int resultActionCommandEvent(ActionCommandView&);
int readActionResult(ActionCommandView&,int32_t rawDestination,const ActionValueServices&);
int readActionCount(ActionCommandView&,int32_t rawDestination,const ActionValueServices&);
int setTimingOptions(ActionCommandView&,int32_t rawOn,uint32_t rawMask,const ActionValueServices&);
int readTimingSuccessWindow(ActionCommandView&,int32_t rawDestination,const ActionValueServices&);
int stopActionCommandEvent(ActionCommandView&);
struct ActionPrizeActor {uint32_t lastHitCode=0;};
struct ActionPrizeServices {
    void* user=nullptr;
    // Captured work is distinct from the work used by actor lookup.
    void* (*captureWork)(void*)=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    ActionPrizeActor* (*unit)(void*,int32_t)=nullptr;
    uint8_t (*prizeByte)(void*,void* capturedWork)=nullptr;
    void (*store)(void*,int32_t,int32_t)=nullptr;
};
int32_t actionPrizeLevel(void* capturedWork,const ActionPrizeActor*,int32_t,const ActionPrizeServices&);
int readActionPrize(const std::array<int32_t,3>& operands,const ActionPrizeServices&);
}
