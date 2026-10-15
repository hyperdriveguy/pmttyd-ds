#pragma once
#include "physical_input.hpp"
namespace ttyd {
// Byte view of the SAME producer/countdown/motor storage,offsets relative GP.
// BE word lanes preserve signed countdown bits;analog axes are signed bytes.
// Unsupported offsets return false;this does not poll hardware or advance input.
bool readPhysicalInputGpByte(const PhysicalInputProducer&,uint32_t offset,uint8_t&);
bool writePhysicalInputGpByte(PhysicalInputProducer&,uint32_t offset,uint8_t);
}
