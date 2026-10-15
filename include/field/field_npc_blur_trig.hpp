#pragma once
#include "field_npc_matrix.hpp"
namespace ttyd {
struct FieldNpcBlurTrigCursor {
  bool active = false, cosine = false, negative = false;
  uint32_t phase = 0;
  FieldNpcMatrixServices owner;
  float yaw = 0, value = 0, tau = 0, divisor = 0, pi = 0, a = 0, b = 0, one = 0,
        bound = 0;
};
// Complete scalar/load graph shared by the TWO independent inline blur
// polynomials. Captures each original read; no general periodic reduction or
// finite-only gate. Native std::fma adapter, not measured Gekko numeric parity.
bool advanceFieldNpcBlurTrig(float yaw, bool cosine,
                             const FieldNpcMatrixServices &,
                             FieldNpcBlurTrigCursor &, float &result);
} // namespace ttyd
