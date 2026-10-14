#pragma once
#include <cstdint>
namespace ttyd {
// Original intpl_sub curves used by the ordinary Paragoomba DivePosition.
// Count/conversion/division domains are caller invariants; no endpoint clamp.
float battleDiveInterpolate(int32_t curve,int32_t elapsed,int32_t count,float start,float target);
}
