#pragma once
#include "event_dispatch.hpp"
#include "event_operands.hpp"
#include <optional>

namespace ttyd {
// Arguments for the original sprintf call; the formatter owns its documented
// varargs/string sink. This request does not itself display or emit debug text.
struct EventDiagnosticCall {
    uint32_t destination=0x803d8430,format=0;
    std::array<int32_t,2> integers{};
    float floating=0;
    bool saveFloatArguments=false; // Original CR6 varargs save selection.
};
struct EventTailOpcodeServices {
    EventDispatchServices values;
    EventOperandServices operands;
    EventManagerServices manager;
    void* formatUser=nullptr;
    void (*format)(void*,const EventDiagnosticCall&)=nullptr;
};
// Semantic equivalent within the original addressable entry-array domain;
// deliberately does not bound the lookup by the live context capacity.
EventEntry* eventEntryPointer(int32_t index,const EventManagerServices&);
int32_t eventDebugRegister(EventEntry&,const EventTailOpcodeServices&);
// Runtime no-ops1/3/113/114/117,WAIT_WHILE11,debug115,metadata116,slot118.
std::optional<int32_t> eventTailOpcode(EventEntry&,uint8_t,const EventTailOpcodeServices&);
}
