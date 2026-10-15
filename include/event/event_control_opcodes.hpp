#pragma once
#include "event_controls.hpp"
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
// Scheduler/creation plus real EVT value services compose lifecycle/control
// instructions. Result optional is routing membership, not provider readiness;
// engaged0 retains the original handler retry meaning.
struct EventControlOpcodeServices {
    EventSchedule* schedule=nullptr;
    EventCreationServices creation;
    EventDispatchServices values;
};
// Empty optional means this routing module does not handle that opcode. It is
// not an original handler result or authorization to substitute a no-op.
std::optional<int32_t> eventControlOpcode(EventEntry&,uint8_t opcode,const EventControlOpcodeServices&);
}
