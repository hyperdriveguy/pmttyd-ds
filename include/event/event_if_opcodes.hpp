#pragma once
#include "event_dispatch.hpp"
#include <optional>

namespace ttyd {
struct EventIfOpcodeServices {
    EventDispatchServices values;
    uint8_t (*byte)(void*,uint32_t originalAddress)=nullptr; // values.user owner.
};
// Original-address string bytes and EVT operands remain provider-owned.
// Optional opcode result is routing membership; engaged handler result0 is not
// an unhandled instruction. Preserve source ordered/NaN comparison branches.
int32_t eventCompareStrings(uint32_t left,uint32_t right,const EventIfOpcodeServices&);
// IF12..31,ELSE32,END_IF33. Empty means this module does not handle the opcode.
// Float control flow follows the recovered CR tests; native FPSCR is separate.
std::optional<int32_t> eventIfOpcode(EventEntry&,uint8_t,const EventIfOpcodeServices&);
}
