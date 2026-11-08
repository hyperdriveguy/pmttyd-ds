#include "battle_ap_report.hpp"
#include "native_harbor_stylish.hpp"
#include "event_heap_allocation.hpp"
#include "harbor_tutorial_packed.hpp"
#include <climits>
#include <cmath>
namespace ttyd {
// Battle+18ffc is source AP report work; byte+a is the bingo-chance gate.
// Nonzero chance is also admitted with the genuine PointInc zero-SP earlyreturn.
// Scalar arithmetic is routed through VM platform operations to retain source
// conversion/rounding order; a native algebraic rewrite can change results.
bool HarborTutorialPacked::inviteApReport() {
  // Enabled bingo reels remain unbound. The actual prologue pouch gate0
  // makes PointInc a source earlyreturn; preserve the original RNG draw.
  if (byte(battleAddress() + 0x18ffc + 0xa) && !nativeHarborPointIncrementZeroDomain(vm))
    return false;
  BattleApReportServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = this;
  s.allocate = [](void *self, uint32_t size) -> std::optional<uint32_t> {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    const auto at = eventSdkAllocate(0, size, p.vm.memoryBinding.services());
    if (!at)
      return std::nullopt;
    return at;
  }; // Native heap policy; source request remains12 bytes.
  s.mario = [](void *self, uint32_t b) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return battleMarioUnit(b, nativeBattleActorServices(p.vm));
  };
  s.party = [](void *self, uint32_t b) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return battlePartyUnit(b, nativeBattleActorServices(p.vm));
  };
  s.kindCount = [](void *self, int32_t kind) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return packedBattleAudiencePpKindCount(kind,
                                           nativeBattleActorServices(p.vm));
  };
  s.wordFloat = [](void *self, int32_t n) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.vm.platform.signedWordFloat(p.vm.platform.user, n);
  };
  s.fma = [](void *self, float a, float b, float c) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.vm.platform.fmaFloat(p.vm.platform.user, a, b, c);
  };
  s.add = [](void *self, float a, float b) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.vm.platform.addFloat(p.vm.platform.user, a, b);
  };
  s.multiply = [](void *self, float a, float b) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.vm.platform.multiplyFloat(p.vm.platform.user, a, b);
  };
  s.squareRoot = [](void *, double v) { return std::sqrt(v); };
  s.roundFloat = [](void *, double v) { return float(v); };
  s.truncate = [](void *self, float v) -> int32_t {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.vm.platform.truncate(p.vm.platform.user, v);
  };
  // Reload global BattleWork* (8041eb90) for each callback. Break-slot work
  // is B+1616c, its turn word at+10; it need not equal this adapter's Battle.
  s.breakTurn = [](void *self) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return int32_t(p.word(p.word(0x8041eb90) + 0x1616c + 0x10));
  };
  s.breakSlot = [](void *self) {
    return static_cast<HarborTutorialPacked *>(self)->word(0x8041eb90) +
           0x1616c;
  };
  s.random = [](void *self, int32_t bound) {
    return static_cast<HarborTutorialPacked *>(self)->random.bounded(bound);
  };
  s.pointIncrement=[](void*self){return nativeHarborPointIncrementZeroDomain(static_cast<HarborTutorialPacked*>(self)->vm);};
  return packedBattleInviteApReport(s);
}
} // namespace ttyd
