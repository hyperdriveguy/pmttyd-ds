#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
// Complete local type5 Set/Clear handlers, after dispatch has resolved payload.
// Original synchronous reads/stores and word wrap are observable. Caller must
// provide the complete writable source address graph; no format/extent guard or
// source file/holder lifecycle is invented here. Addresses are guest tokens.
// Root first-pointer offset0x14c chooses relative/absolute path; root pointer
// words through0x1ac and child words0x24..0x40 wrap with their respective bases.
// Set reloads child after parent publication; Clear retains its absolute child.
void setPackedFileModelPointers(uint32_t payload,const EventMemoryServices&);
void clearPackedFileModelPointers(uint32_t payload,const EventMemoryServices&);
}
