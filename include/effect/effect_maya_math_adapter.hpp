#pragma once
#include "effect_maya_matrix.hpp"
namespace ttyd {
// Explicit native-port numeric policy: reuses the existing rendering affine
// adapter (including its host/libm rotation and finite-domain admission).
// This is not a claim of Gekko FPSCR, NaN/subnormal or raster parity.
// Installs numeric and SDK callbacks only;caller retains memory/user wiring.
// Explicit adapter opt-in is separate from source resource/provider admission.
void bindEffectMayaMatrixMathAdapter(EffectMayaMatrixServices &);
} // namespace ttyd
