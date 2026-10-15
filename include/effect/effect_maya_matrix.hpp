#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
namespace ttyd {
using EffectMayaMatrix = std::array<float, 12>;
enum class EffectMayaBinary { Add, Subtract, Multiply, Divide };
enum class EffectMayaFused {
  MultiplyAdd,
  MultiplySubtract,
  NegativeMultiplySubtract
};
struct EffectMayaMatrixServices {
  EventMemoryServices memory;
  void *user = nullptr;
  uint8_t (*byte)(void *, uint32_t) = nullptr;
  float (*binary)(void *, EffectMayaBinary, float, float) = nullptr;
  float (*fused)(void *, EffectMayaFused, float, float, float) = nullptr;
  bool (*translate)(void *, const std::array<float, 3> &,
                    EffectMayaMatrix &) = nullptr;
  bool (*rotate)(void *, uint32_t axis, float radians,
                 EffectMayaMatrix &) = nullptr;
  bool (*scale)(void *, const std::array<float, 3> &,
                EffectMayaMatrix &) = nullptr;
  bool (*concat)(void *, const EffectMayaMatrix &, const EffectMayaMatrix &,
                 EffectMayaMatrix &) = nullptr;
};
struct EffectMayaMatrixCursor {
  bool active = false;
  uint32_t phase = 0, animation = 0, channel = 0, matrixOut = 0, scalarOut = 0;
  int32_t count = 0;
  std::array<float, 22> values{};
  std::array<EffectMayaMatrix, 14> matrices{};
  EffectMayaMatrixServices owner;
};
// Whole effCalcMayaAnimMatrix8005e4f4/3ac. False preserves the pending
// SDK operation and its captured operands. Service output on false is ignored.
bool resumeEffectMayaAnimMatrix(uint32_t animation, uint32_t channel,
                                uint32_t matrixOut, uint32_t scalarOut,
                                const EffectMayaMatrixServices &,
                                EffectMayaMatrixCursor &);
// Pure numeric leaves: explicit binary32 rounding and single-rounded fused ops.
float effectMayaBinary32(void *, EffectMayaBinary, float, float);
float effectMayaFused32(void *, EffectMayaFused, float, float, float);
} // namespace ttyd
