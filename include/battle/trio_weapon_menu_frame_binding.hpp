#pragma once
#include "battle_weapon_native_frame.hpp"
namespace ttyd {
// Actual runtime-owned UI/source resource binding. Native renderer contexts
// separately own all linked drawing services and complete ordered submission.
// Weak public-owner registration rejects destruction/replacement without
// dereferencing a retired facade; actual resource/allocation proofs remain.
struct TrioWeaponMenuFrameBinding {
 BattleWeaponNativeFrameBinding native;
 BattleWeaponDrawServices source;
 std::function<std::optional<uint32_t>(uint32_t)> resourceWord;
 // Actual original irand owner. Caller must hold the real source admission
 // through atomic reserve→two RNG→commit; this closure is not readiness.
 std::function<uint32_t(uint32_t)>random;
};
}
