#pragma once
#include "event_memory_opcodes.hpp"
// Returns an original guest string pointer. Caller owns admitted terminating
// table storage and actual animation-bank resolution, not a native fallback.
namespace ttyd {
// Original BtlUnit_GetPoseNameFromType8012772c: first exact OR range fallback
// wins in table order. No guessed pose or native clip substitution is provided.
uint32_t packedBattlePoseName(uint32_t part,int32_t type,const EventMemoryServices&);
}
