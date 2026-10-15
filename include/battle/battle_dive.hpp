#pragma once
#include <cstdint>
// These source intpl_sub curve IDs are not generic easing enum values.
// Counts/conversion domains are caller-owned; no renderer timing or automatic
// endpoint clamp is introduced by the native adapter.
namespace ttyd {
// Original intpl_sub curves used by the ordinary Paragoomba DivePosition.
// Count/conversion/division domains are caller invariants; no endpoint clamp.
float battleDiveInterpolate(int32_t curve,int32_t elapsed,int32_t count,float start,float target);
}
