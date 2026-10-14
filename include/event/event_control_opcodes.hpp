#pragma once
#include "event_controls.hpp"
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
struct EventControlOpcodeServices {
    EventSchedule* schedule=nullptr;
    EventCreationServices creation;
    EventDispatchServices values;
};
// Empty optional means this routing module does not handle that opcode. It is
// not an original handler result or authorization to substitute a no-op.
std::optional<int32_t> eventControlOpcode(EventEntry&,uint8_t opcode,const EventControlOpcodeServices&);
}
