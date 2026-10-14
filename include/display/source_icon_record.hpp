#pragma once
#include <cstdint>
namespace ttyd {
// Defined fields of the original stack icon record. No fabricated values for
// fields that iconDispGx2/Col leave uninitialized.
struct SourceMatrixIcon {
  uint16_t flags = 0, id = 0, width = 0, height = 0, texture = 0, delay = 0;
  uint32_t rgba = 0;
};
} // namespace ttyd
