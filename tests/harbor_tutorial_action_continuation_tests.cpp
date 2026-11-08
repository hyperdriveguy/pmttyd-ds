#include "battle_packed_pad.hpp"
#include "fixtures/host_event_cache.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <memory>
using namespace ttyd;
struct NativeFixture {
  std::unique_ptr<EventNativeRuntime> vm =
      std::make_unique<EventNativeRuntime>(0x81000000, EventNativePlatform{});
  std::unique_ptr<test::HostEventCache> cache;
  std::unique_ptr<HarborTutorialPacked> packed;
  NativeFixture() {
    cache=std::make_unique<test::HostEventCache>(*vm);
    // Actual original stage-floor constants used by the native provisioner.
    assert(vm->memory.registerRam(0x80422b44,
                                  {0x42, 0xb4, 0, 0, 0xc2, 0x48, 0, 0}));
    assert(vm->memory.registerRam(0x80422d68, {0x41, 0x70, 0, 0}));
    packed = std::make_unique<HarborTutorialPacked>(*vm);
    assert(packed->provision());
    cache->requireBattlePoolPublication();
  }
  void put(uint32_t offset, uint32_t n) {
    assert(vm->memory.writeWord(HarborTutorialPacked::Battle + offset, n));
  }
  uint32_t get(uint32_t offset) {
    uint32_t n;
    assert(vm->memory.readWord(HarborTutorialPacked::Battle + offset, n));
    return n;
  }
  BattlePadWork pad() {
    return readPackedBattlePad(HarborTutorialPacked::Battle + 0x1d1c,
                               nativeBattleActorServices(*vm));
  }
};
int main() {
  NativeFixture supported;
  auto &p = *supported.packed;
  supported.put(0x1ca0, 0x8012aae0);
  supported.put(0x1ca8, 0x8012a7cc);
  supported.put(0x1c9c, 1000);
  BattlePadSample first;
  first.snapshot.buttons = 0x100;
  first.pressed = 0x100;
  p.samplePhysical(10, &first);
  assert(p.actionManager(10));
  assert(!p.actionManagerPending());
  assert(supported.get(0x1c9c) == 1001 && supported.get(0x1f54) == 59);
  assert(p.actionDisplays.count() == 1);
  const auto epoch = p.actionDisplays.resetEpoch();
  const auto privatePad = supported.pad();
  assert(p.actionManager(10));
  assert(supported.get(0x1f54) == 59 && p.actionDisplays.count() == 1 &&
         p.actionDisplays.resetEpoch() == epoch);
  assert(supported.pad().triggers.entries == privatePad.triggers.entries);
  BattlePadSample second;
  second.snapshot.buttons = 0x200;
  second.pressed = 0x200;
  p.samplePhysical(20, &second);
  assert(p.actionManager(20));
  assert(supported.get(0x1f54) == 58 && p.actionDisplays.count() == 1 &&
         p.actionDisplays.resetEpoch() != epoch);
  p.samplePhysical(30, nullptr);
  assert(p.actionManager(30));
  assert(supported.get(0x1f54) == 58 && p.actionDisplays.count() == 0);
  NativeFixture unknown;
  auto &u = *unknown.packed;
  // Explicit unbound callback tests the real native admission frontier; it is
  // never executed or acknowledged as original source behavior.
  unknown.put(0x1ca0, 0x12345678);
  unknown.put(0x1ca8, 0x8012a7cc);
  u.samplePhysical(40, &first);
  assert(!u.actionManager(40) && u.actionManagerPending());
  const auto held = unknown.pad();
  const auto heldEpoch = u.actionDisplays.resetEpoch();
  assert(!u.actionManager(40) &&
         unknown.pad().triggers.entries == held.triggers.entries);
  // A newer physical observation cannot replace this manager's private input
  // or the callback operand it already captured.
  u.samplePhysical(50, &second);
  unknown.put(0x1ca0, 0x8012aae0);
  unknown.put(0x1c9c, 1000);
  assert(!u.actionManager(50) && u.actionManagerPending());
  assert(unknown.pad().triggers.entries == held.triggers.entries &&
         unknown.get(0x1c9c) == 1000);
  assert(u.actionDisplays.resetEpoch() == heldEpoch &&
         u.actionDisplays.count() == 0 && u.actionManagerSampleTime() == 40);
  HarborTutorial scene;
  scene.actionCommandIcons.push_back({});
  assert(!u.actionDisplay(50, scene));
  assert(scene.actionCommandIcons.empty());
  return 0;
}
