#pragma once
#include "field_npc_matrix.hpp"
namespace ttyd {
// One sine OR cosine evaluation, with independent literal reads. Capture yaw
// in degrees, mode and providers at entry; later retry arguments are ignored.
// Successful completion publishes scalar and resets, so reuse starts a new graph.
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
