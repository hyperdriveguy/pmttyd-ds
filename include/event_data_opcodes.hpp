#pragma once
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
struct EventDataOpcodeServices {
    EventDispatchServices values;
    void* numericUser=nullptr;
    int32_t (*divideWord)(void*,int32_t,int32_t)=nullptr; // Original divw domain.
    int32_t (*truncate)(void*,float)=nullptr; // Original fctiwz/FPSCR domain.
};
// Assignment/arithmetic50..61 and cursor/data62..73. Empty means unsupported.
std::optional<int32_t> eventDataOpcode(EventEntry&,uint8_t,const EventDataOpcodeServices&);
}
