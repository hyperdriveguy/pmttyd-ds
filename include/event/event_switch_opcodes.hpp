#pragma once
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
// SWITCH/CASE34..49. Empty means unsupported. Semantic stack domain0..7;
// packed aliasing and invalid script depths remain native storage obligations.
std::optional<int32_t> eventSwitchOpcode(EventEntry&,uint8_t,const EventDispatchServices&);
}
