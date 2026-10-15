#pragma once
#include "field_npc_clock.hpp"
namespace ttyd {
struct FieldNpcNormServices {
  FieldNpcClockServices memory; // word/user only
  std::optional<uint64_t> (*doubleWord)(void *, uint32_t) = nullptr;
  bool (*estimate)(void *, double, double &) = nullptr;
};
// Retained inlined Gekko norm AFTER caller-specific SP squared-norm graph.
// Services are no-effects on false/empty; owners and input bits stay stable.
struct FieldNpcNormCursor {
  bool active = false;
  FieldNpcNormServices owner;
  uint32_t phase = 0;
  float squared = 0, zero = 0, result = 0;
  double seed = 0, half = 0, three = 0;
};
bool advanceFieldNpcInlineNorm(float squared, float capturedZero,
                               const FieldNpcNormServices &,
                               FieldNpcNormCursor &, float &result);
// Explicit opt-in adaptation, NOT original frsqrte/FPSCR equivalence.
bool nativeFieldNpcReciprocalSqrtEstimate(void *, double, double &);
} // namespace ttyd
