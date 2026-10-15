#include "animation_facing_binding.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_address_space.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Stride = 0x280, Size = 13 * Stride;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
} // namespace
AnimationFacingBinding::AnimationFacingBinding(
    std::shared_ptr<BattlePosePool> pool, EventAddressSpace &m,
    const AnimationPoolMemory &p, std::shared_ptr<const CameraManager> c,
    AnimationFacingAddressPolicy a, std::function<bool()> current)
    : pool_(std::move(pool)), memory_(m), poses_(p), cameras_(std::move(c)),
      addresses_(std::move(a)), current_(std::move(current)) {}
std::unique_ptr<AnimationFacingBinding> AnimationFacingBinding::bind(
    std::shared_ptr<BattlePosePool> pool, EventAddressSpace &m,
    const AnimationPoolMemory &p, std::shared_ptr<const CameraManager> c,
    AnimationFacingAddressPolicy a, std::function<bool()> current) {
  if (!pool || !c || !a.base || uint64_t(a.base) + Size > 0x100000000ull ||
      !a.unmapped || !current)
    return {};
  auto b = std::unique_ptr<AnimationFacingBinding>(new AnimationFacingBinding(
      std::move(pool), m, p, std::move(c), std::move(a), std::move(current)));
  if (!b->available())
    return {};
  return b;
}
bool AnimationFacingBinding::available() const {
  if (!current_() || !addresses_.unmapped(addresses_.base, Size) ||
      !poses_.aliases(*pool_, memory_))
    return false;
  const auto *c = cameras_->camera(4);
  return c && c->mode != 2 && c->projectionKind == 0 && c->viewPublished;
}
bool AnimationFacingBinding::owns(uint32_t a) const {
  return a >= addresses_.base && uint64_t(a) < uint64_t(addresses_.base) + Size;
}
std::optional<uint32_t> AnimationFacingBinding::poseAddress(int32_t h) const {
  if (!available() || h < 0 || h >= 256)
    return {};
  return poses_.poseAddress(unsigned(h));
}
std::optional<uint32_t> AnimationFacingBinding::word(uint32_t a) const {
  if (!available())
    return {};
  if (owns(a)) {
    const uint32_t offset = a - addresses_.base;
    if (offset / Stride != 4)
      return {};
    const auto *c = cameras_->camera(4);
    switch (offset % Stride) {
    case 0xc:
      return bits(c->eye[0]);
    case 0x14:
      return bits(c->eye[2]);
    case 0x18:
      return bits(c->at[0]);
    case 0x20:
      return bits(c->at[2]);
    default:
      return {};
    }
  }
  uint32_t w;
  if (!memory_.readWord(a, w))
    return {};
  return w;
}
AnimationFacingServices AnimationFacingBinding::services() {
  AnimationFacingServices s;
  s.user = this;
  s.word = [](void *u, uint32_t a) {
    return static_cast<AnimationFacingBinding *>(u)->word(a);
  };
  s.store = [](void *u, uint32_t a, uint32_t w) {
    auto &b = *static_cast<AnimationFacingBinding *>(u);
    return b.available() && !b.owns(a) && b.memory_.writeWord(a, w);
  };
  s.call = [](void *u, AnimationFacingCall &q) {
    auto &b = *static_cast<AnimationFacingBinding *>(u);
    if (!b.available())
      return false;
    if (q.function == 0x8000f2fc) {
      if (q.argument != 4)
        return false;
      q.word = b.addresses_.base + 4 * Stride;
      return true;
    }
    if (q.function == 0x802725a0) {
      q.number = std::atan2(double(q.scalars[0]), double(q.scalars[1]));
      return true;
    }
    return false;
  };
  s.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
    if (!static_cast<AnimationFacingBinding *>(u)->available())
      return false;
    BattleAudienceAuxServices math;
    bindAudienceAuxMathAdapter(math);
    return math.matrix(nullptr, q);
  };
  return s;
}
} // namespace ttyd
