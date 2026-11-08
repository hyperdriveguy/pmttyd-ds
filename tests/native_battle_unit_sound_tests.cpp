#include "battle_timing_a.hpp"
#include "event_native_runtime.hpp"
#include "native_battle_sound.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Unit = 0x84101000, Relief1 = 0x802feafc,
                   Relief2 = 0x802feb14, DefaultVolume = 0xf1194d80;
struct Fixture {
  EventNativePlatform platform;
  std::unique_ptr<EventNativeRuntime> vm;
  std::shared_ptr<PositionalSfxRegistry> registry =
      std::make_shared<PositionalSfxRegistry>();
  std::shared_ptr<const void> life = std::make_shared<int>(1);
  BattleRandom random{0};
  std::unique_ptr<NativeBattleSound> owner;
  bool alive = true, reenter = false;
  int budget = -1;
  SourceBattleVoiceCursor cursor;
  Fixture() {
    platform.truncate = [](void *, float v) {
      assert(std::isfinite(v));
      return int32_t(v);
    };
    vm = std::make_unique<EventNativeRuntime>(0x81000000, platform);
    EventScriptAssets data;
    assert(data.load("romfs/harbor-tutorial-events.pmevt") &&
           vm->memory.importScripts(data));
    assert(vm->memory.registerRam(Unit, std::vector<uint8_t>(0x1000)));
    auto word = [&](uint32_t at, uint32_t value) {
      assert(vm->memory.writeWord(at, value));
    };
    for (unsigned i = 0; i < 3; ++i) {
      float value = float(i + 1);
      uint32_t bits;
      std::memcpy(&bits, &value, 4);
      word(Unit + 0x3c + 4 * i, bits);
    }
    word(vm->gpAddresses.currentAddress() + 4, 60);
    word(vm->gpAddresses.currentAddress() + 0x174, 4);
    owner = std::make_unique<NativeBattleSound>(NativeBattleSoundBinding{
        vm.get(), &random, registry, life, this, [](void *p) {
          auto &f = *static_cast<Fixture *>(p);
          if (f.reenter) {
            SourceBattleVoiceCursor nested;
            assert(
                !f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, nested));
            assert(!f.owner->menu(2616));
            assert(!f.owner->retireOwned({}));
          }
          if (f.budget == 0)
            return false;
          if (f.budget > 0)
            --f.budget;
          return f.alive;
        }});
  }
};
} // namespace
int main() {
  for (int boundary = 0; boundary < 80; ++boundary) {
    Fixture f;
    f.budget = boundary;
    BattleRandom expected = f.random;
    const auto pitch = expected.bounded(41) - 20;
    auto result = f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, f.cursor);
    f.budget = -1;
    f.reenter = true;
    if (!result)
      result = f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, f.cursor);
    assert(result && *result != -1 && f.registry->allocationGeneration == 1 &&
           f.random.state == expected.state);
    f.reenter = false;
    auto birth = f.registry->slotLifetime(uint32_t(*result) & 255);
    assert(
        f.owner->ownsPending(f.registry.get(), uint32_t(*result) & 255, birth));
    const auto &slot = f.registry->slots[uint32_t(*result) & 255];
    assert(slot.key == 225 && slot.pitch == uint16_t(pitch) &&
           slot.volume == 127 &&
           (slot.position == std::array<float, 3>{1, 2, 3}));
    f.reenter = true;
    assert(f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, f.cursor) ==
               result &&
           f.registry->allocationGeneration == 1 &&
           f.random.state == expected.state);
    assert(!f.owner->unitSound(Unit, Relief2, DefaultVolume, 0, f.cursor));
    f.reenter = false;
    assert(f.owner->retireOwned({}));
    SourceBattleVoiceCursor next;
    assert(!f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, next));
  }
  Fixture f;
  SourceBattleVoiceCursor second;
  const auto result = f.owner->unitSound(Unit, Relief2, 17, 0, second);
  assert(result && f.registry->slots[uint32_t(*result) & 255].key == 226 &&
         f.registry->slots[uint32_t(*result) & 255].volume == 17);
  const auto point = f.owner->menu(0x802feaa8);
  assert(point && *point != -1);
  const auto pointSlot = uint32_t(*point) & 255;
  assert(f.registry->slots[pointSlot].key == 2616 &&
         f.owner->ownsPending(f.registry.get(), pointSlot,
                              f.registry->slotLifetime(pointSlot)));
  f.alive = false;
  const auto seed = f.random.state;
  SourceBattleVoiceCursor stale;
  assert(!f.owner->unitSound(Unit, Relief1, DefaultVolume, 0, stale) &&
         f.random.state == seed);
  std::cout << "Actual original unitSound End keys, held-current boundaries, "
               "one RNG/On birth, rawvolume/reentry/stale/retirement pass\n";
}
