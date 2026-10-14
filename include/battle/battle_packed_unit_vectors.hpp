#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
namespace ttyd {
struct PackedUnitVectorServices {
 EventMemoryServices memory;void* user=nullptr;
 float (*reviseAngle)(void*,float)=nullptr; // Original system reviseAngle80007328.
};
// Original thirteen position/scale/rotation leaves used by UnitEntry.
// Unsupported source helper returns false without mutation.
bool setPackedUnitVector(uint32_t sourceFunction,uint32_t unitOrPart,const std::array<float,3>&,const PackedUnitVectorServices&);
}
