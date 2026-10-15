#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
#include <optional>
namespace ttyd {
using BattleScreenPoint=std::array<float,3>;
struct BattleScreenPointServices {
    EventMemoryServices memory;
    void* user=nullptr;
    std::optional<uint32_t> (*camera)(void*,uint32_t id)=nullptr;
    bool (*view)(void*,uint32_t matrix,const BattleScreenPoint&,BattleScreenPoint&)=nullptr;
    bool (*project)(void*,uint32_t matrix,const BattleScreenPoint&,BattleScreenPoint&)=nullptr;
};
// Complete local btlGetScreenPoint801009b4/0x98. Camera4 is captured before
// copying the supplied guest XYZ. Linked SDK calls receive that camera's actual
// view+11c/projection+15c; no fallback camera, reflow or screen clamp is supplied.
// False marks an unavailable provider, not rollback or a retry-safe boundary.
// Output remains untouched until both SDK providers acknowledge their work.
bool packedBattleScreenPoint(uint32_t position,BattleScreenPoint& output,
                             const BattleScreenPointServices&);
}
