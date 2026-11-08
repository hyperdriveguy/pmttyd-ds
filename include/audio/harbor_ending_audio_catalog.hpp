#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
// Original btlseqEnd literal keys resolve to RELIEF1/2, not invented WIN
// aliases.
const char *harborEndingPcmFile(uint32_t sourceRow);
const char *harborEndingPcmName(uint32_t sourceRow);
std::optional<uint32_t> harborEndingPcmDescriptor(uint32_t sourceRow);
std::optional<unsigned> harborEndingPcmIndex(uint32_t sourceRow);
constexpr unsigned HarborEndingPcmCount = 3, MarioActionPcmCount = 15;
constexpr uint32_t HarborStarPointPcmRow = 2616, HarborStarPointPcmRate = 16000;
} // namespace ttyd
