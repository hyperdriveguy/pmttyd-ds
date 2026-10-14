#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
enum class ClassIconChannel : uint8_t { Red, Green, Blue };
struct ClassIconStage {
  ClassIconChannel channel;
  uint8_t coefficient;
  bool addPrevious;
  bool recordAlpha;
};
// Original iconGX flag32: swap tables1/2/3 replicate R/G/B, color register0
// is555555ff, and final alpha multiplies texture by record alpha. Record RGB
// is not an operand. Native PICA fixed-point raster equality remains separate.
constexpr std::array<ClassIconStage, 3> disabledClassIconProgram() {
  return {{{ClassIconChannel::Red, 85, false, false},
           {ClassIconChannel::Green, 85, true, false},
           {ClassIconChannel::Blue, 85, true, true}}};
}
// Semantic rational-color reference for validation, not a software replacement
// for original GX/PICA per-stage rounding and texture filtering.
uint32_t disabledClassIconColor(uint32_t textureRGBA, uint32_t recordRGBA);
} // namespace ttyd
