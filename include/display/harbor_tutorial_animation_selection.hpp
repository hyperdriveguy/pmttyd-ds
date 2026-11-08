#pragma once
#include "actor_animation.hpp"
#include <cstddef>
#include <optional>
namespace ttyd {
// Visual slot1 is Mario; source registry IDs are deliberately not accepted here.
// Each result identifies the atlas owner whose UVs accompany the selected pose.
enum class HarborTutorialAnimationOwner { Dedicated, OrdinaryMario };
struct HarborTutorialAnimationSelection {
  const ActorAnimationPose* pose = nullptr;
  HarborTutorialAnimationOwner owner = HarborTutorialAnimationOwner::Dedicated;
};
inline std::optional<HarborTutorialAnimationSelection>
selectHarborTutorialAnimation(std::size_t visualSlot, const char* name,
                             const ActorAnimationPack& dedicated,
                             const ActorAnimationPack* ordinaryMario) {
  if (!name) return {};
  if (const auto* pose = dedicated.pose(name))
    return HarborTutorialAnimationSelection{pose, HarborTutorialAnimationOwner::Dedicated};
  constexpr std::size_t MarioVisualSlot = 1;
  if (visualSlot == MarioVisualSlot && ordinaryMario)
    if (const auto* pose = ordinaryMario->pose(name))
      return HarborTutorialAnimationSelection{pose, HarborTutorialAnimationOwner::OrdinaryMario};
  return {};
}
} // namespace ttyd
