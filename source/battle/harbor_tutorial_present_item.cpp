#include "harbor_tutorial_packed.hpp"
namespace ttyd {
// MenuRam+20 is stable native XYZ scratch across retained visits. The cursor
// owns captured recovery/math progress; rebuilding services must not reset it.
// Missing math returns nullopt; effect admission is separately unavailable.
std::optional<int32_t> HarborTutorialPacked::presentItem() {
  BattlePresentItemServices s;
  s.actors = nativeBattleActorServices(vm);
  s.user = this;
  s.privatePosition = MenuRam + 0x20;
  // Preserve double subtraction THEN single rounding (two platform calls).
  // Computing float(a)-float(b) would move the rounding boundary.
  s.subtractDoubleSingle = [](void *p, double a,
                              double b) -> std::optional<float> {
    const auto &ops = static_cast<HarborTutorialPacked *>(p)->vm.platform;
    if (!ops.subtractDouble || !ops.roundDoubleFloat)
      return {};
    return ops.roundDoubleFloat(ops.user, ops.subtractDouble(ops.user, a, b));
  };
  s.fma = [](void *p, float a, float b, float c) -> std::optional<float> {
    const auto &ops = static_cast<HarborTutorialPacked *>(p)->vm.platform;
    return ops.fmaFloat ? std::optional<float>(ops.fmaFloat(ops.user, a, b, c))
                        : std::nullopt;
  };
  s.add = [](void *p, float a, float b) -> std::optional<float> {
    const auto &ops = static_cast<HarborTutorialPacked *>(p)->vm.platform;
    return ops.addFloat ? std::optional<float>(ops.addFloat(ops.user, a, b))
                        : std::nullopt;
  };
  // Actual effRecoveryEntry ownership/resource/renderer is not yet admitted.
  // Retain the original recovery and private effect position without fake
  // clear.
  return resumePackedBattlePresentItem(s, presentItemCursor);
}
} // namespace ttyd
