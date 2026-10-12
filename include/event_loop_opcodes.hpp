#pragma once
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
// GOTO4,DO5,NEXT6,BREAK7,CONTINUE8. Empty means unsupported.
// Stack storage is semantic; native packed alias/invalid-depth domains are open.
std::optional<int32_t> eventLoopOpcode(EventEntry&,uint8_t,const EventDispatchServices&);
}
