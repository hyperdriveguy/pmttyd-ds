#pragma once
#include "battle_weapon_native_frame.hpp"
namespace ttyd {
// Actual runtime-owned UI/source resource binding. Native renderer contexts
// separately own all linked drawing services and complete ordered submission.
// The Trio facade outlives use; replacing runtime invalidates old closures.
struct TrioWeaponMenuFrameBinding {
 BattleWeaponNativeFrameBinding native;
 BattleWeaponDrawServices source;
 std::function<std::optional<uint32_t>(uint32_t)> resourceWord;
};
}
