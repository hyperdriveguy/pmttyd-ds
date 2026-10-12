#pragma once
#include "event_manager.hpp"

namespace ttyd {
enum class SavedSwitchFamily {GlobalByte,LocalByte,GlobalFlag,LocalFlag};
struct EventOperandServices {
    void* user=nullptr;
    uint32_t (*contextAddress)(void*)=nullptr; // evtGetWork;called even for immediates.
    uint32_t (*entryAddress)(void*,EventEntry&)=nullptr;
    uint32_t (*globalAddress)(void*)=nullptr; // Saved leaves reload GP separately.
    uint32_t (*readWord)(void*,uint32_t)=nullptr;
    void (*writeWord)(void*,uint32_t,uint32_t)=nullptr;
    uint8_t (*readByte)(void*,uint32_t)=nullptr;
    void (*writeByte)(void*,uint32_t,uint8_t)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr; // Original fctiwz/FPSCR boundary.
};
uint32_t eventShiftMask(int32_t count);
int32_t readSavedSwitch(SavedSwitchFamily,int32_t index,const EventOperandServices&);
void writeSavedSwitch(SavedSwitchFamily,int32_t index,int32_t value,const EventOperandServices&);
int32_t getEventValue(EventEntry&,int32_t operand,const EventOperandServices&);
int32_t setEventValue(EventEntry&,int32_t destination,int32_t value,const EventOperandServices&);
float getEventFloat(EventEntry&,int32_t operand,const EventOperandServices&);
float setEventFloat(EventEntry&,int32_t destination,float value,const EventOperandServices&);
}
