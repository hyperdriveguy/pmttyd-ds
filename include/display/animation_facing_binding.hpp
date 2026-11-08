#pragma once
#include "animation_facing.hpp"
#include "animation_pool_memory.hpp"
#include "camera_manager.hpp"
#include <functional>
namespace ttyd {
class EventAddressSpace;
struct AnimationFacingAddressPolicy {
  uint32_t base = 0;
  // Proves the complete native token reservation has no VM/file-cache storage.
  // Pure/non-reentrant; rechecked at every boundary. Never fall through inside
  // it.
  std::function<bool(uint32_t, uint32_t)> unmapped;
};
// Borrows the real source VM/pool aliases and owns a shared semantic camera
// bank. Tokens adapt source camera reads; they are never host/guest pointers.
// The pure current predicate must retain this exact field publication/lifetime
// pass.
class AnimationFacingBinding {
public:
  static std::unique_ptr<AnimationFacingBinding>
  bind(std::shared_ptr<BattlePosePool>, EventAddressSpace &,
       const AnimationPoolMemory &, std::shared_ptr<const CameraManager>,
       AnimationFacingAddressPolicy, std::function<bool()> current);
  AnimationFacingBinding(const AnimationFacingBinding &) = delete;
  AnimationFacingBinding &operator=(const AnimationFacingBinding &) = delete;
  bool available() const;
  // Tests semantic reservation only; availability/lifetime is a separate guard.
  bool owns(uint32_t) const;
  std::optional<uint32_t> poseAddress(int32_t) const;
  AnimationFacingServices services();

private:
  AnimationFacingBinding(std::shared_ptr<BattlePosePool>, EventAddressSpace &,
                         const AnimationPoolMemory &,
                         std::shared_ptr<const CameraManager>,
                         AnimationFacingAddressPolicy, std::function<bool()>);
  std::optional<uint32_t> word(uint32_t) const;
  std::shared_ptr<BattlePosePool> pool_;
  EventAddressSpace &memory_;
  const AnimationPoolMemory &poses_;
  std::shared_ptr<const CameraManager> cameras_;
  AnimationFacingAddressPolicy addresses_;
  std::function<bool()> current_;
};
} // namespace ttyd
