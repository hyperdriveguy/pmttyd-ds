#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
// Whole pouchCheckItem800d4ed4/1f8. Captures the actual global pouch once on
// supported signed item ranges. Every source halfword read remains ordered.
// Atomic native leaf: caller admits the real complete memory/lifetime domain.
// There is no unavailable-as-zero policy or capacity/equipped-item substitution.
int32_t packedPouchItemCount(int32_t item,const EventMemoryServices&);
}
