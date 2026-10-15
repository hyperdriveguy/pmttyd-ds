#pragma once
#include "effect_maya_matrix.hpp"
namespace ttyd {
// Explicit native-port numeric policy: reuses the existing rendering affine
// adapter (including its host/libm rotation and finite-domain admission).
// This is not a claim of Gekko FPSCR, NaN/subnormal or raster parity.
void bindEffectMayaMatrixMathAdapter(EffectMayaMatrixServices &);
} // namespace ttyd
