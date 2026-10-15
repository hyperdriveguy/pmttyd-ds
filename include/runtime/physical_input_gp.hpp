#pragma once
#include "physical_input.hpp"
namespace ttyd {
bool readPhysicalInputGpByte(const PhysicalInputProducer&,uint32_t offset,uint8_t&);
bool writePhysicalInputGpByte(PhysicalInputProducer&,uint32_t offset,uint8_t);
}
