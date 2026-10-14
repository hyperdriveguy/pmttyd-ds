#include "battle_timing_display.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float v) {
  uint32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
struct Fixture {
  static constexpr uint32_t B = 0x10000, C = 0x40000;
  std::map<uint32_t, uint32_t> words{
      {0x8041eb90, B},           {0x80422710, bits(-200)},
      {0x80422714, bits(50)},    {0x80422718, bits(1)},
      {0x8042271c, bits(-300)},  {0x80422720, bits(30)},
      {0x802f01c8 + 8, bits(7)}, {0x802f01d4 + 8, bits(8)}};
  std::vector<BattleTimingIconRequest> icons;
  std::vector<int32_t> steps;
  std::function<void()> onCamera, onIcon;
  bool cameraReady = true, iconReady = true, curveReady = true;
  BattleTimingDisplayServices services() {
    BattleTimingDisplayServices s;
    s.memory.memory.user = this;
    s.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
      static_cast<Fixture *>(p)->words[a] = n;
    };
    s.interpolate = [](void *p, int32_t mode, int32_t step, int32_t duration,
                       float from, float to) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      assert(mode == 4 && duration == 20);
      if (f.words[B + 0x1c9c] == 99)
        assert(from == -300 && to == 30);
      else
        assert(from == 30 && to == -300);
      f.steps.push_back(step);
      return f.curveReady ? std::optional<float>(123) : std::nullopt;
    };
    s.camera = [](void *p, uint32_t camera) {
      auto &f = *static_cast<Fixture *>(p);
      assert(camera == 8);
      if (f.onCamera)
        f.onCamera();
      return f.cameraReady;
    };
    s.icon = [](void *p, const BattleTimingIconRequest &r) {
      auto &f = *static_cast<Fixture *>(p);
      f.icons.push_back(r);
      if (f.onIcon)
        f.onIcon();
      return f.iconReady;
    };
    return s;
  }
};
} // namespace
int main() {
  for (auto state : {0u, 98u, 101u, 999u, 1002u, 0xffffffffu}) {
    Fixture f;
    f.words[f.B + 0x1c9c] = state;
    assert(packedBattleTimingADisplay(f.B, f.services()) && f.icons.empty() &&
           f.steps.empty());
  }
  Fixture f;
  f.words[f.B + 0x1c9c] = 99;
  f.words[f.B + 0x1f40] = 20;
  f.words[f.B + 0x1f38] = bits(4);
  assert(packedBattleTimingADisplay(f.B, f.services()));
  assert(f.steps == std::vector<int32_t>{0});
  assert(f.words[f.B + 0x1f34] == bits(30) && f.words[f.B + 0x1f40] == 19 &&
         f.words[f.B + 0x1f20] == 1);
  auto r = f.icons.back();
  assert(r.icon == 108 && r.camera == 8 && r.flags == 16 && r.scale == 1 &&
         r.position[0] == -170 && r.position[1] == 54 && r.position[2] == 8);
  // Camera provider may change global B. Draw retains its initial B and freshly
  // reads its flag after the provider; wrapper counter still belongs to caller
  // B.
  Fixture retained;
  retained.words[retained.B + 0x1c9c] = 100;
  retained.words[retained.B + 0x1f40] = 1;
  retained.onCamera = [&] {
    retained.words[0x8041eb90] = retained.C;
    retained.words[retained.B + 0x1f50] = 1;
  };
  retained.onIcon = [&] {
    retained.words[retained.B + 0x1f20] = 0xffffffff;
    retained.words[retained.B + 0x1f40] = 5;
  };
  assert(packedBattleTimingADisplay(retained.B, retained.services()));
  assert(retained.icons[0].icon == 109 && retained.icons[0].position[2] == 7);
  assert(retained.words[retained.B + 0x1f20] == 0 &&
         retained.words[retained.B + 0x1f40] == 4 &&
         retained.words[retained.C + 0x1f20] == 0);
  // Draw captures a different current global owner from the display callback.
  Fixture split;
  split.words[0x8041eb90] = split.C;
  split.words[split.B + 0x1c9c] = 100;
  split.words[split.B + 0x1f40] = 0;
  assert(packedBattleTimingADisplay(split.B, split.services()) &&
         split.words[split.C + 0x1f20] == 1 &&
         split.words[split.B + 0x1f20] == 0 &&
         split.words[split.B + 0x1f40] == 0);
  for (auto count : {0u, 39u, 40u, 59u, 60u, 0xffffffffu}) {
    Fixture tail;
    tail.words[tail.B + 0x1c9c] = 1001;
    tail.words[tail.B + 0x1f40] = count;
    assert(packedBattleTimingADisplay(tail.B, tail.services()));
    if (count >= 40 && count < 0x80000000) {
      assert(tail.steps == std::vector<int32_t>{int32_t(count - 40)} &&
             tail.words[tail.B + 0x1f34] == bits(123));
    } else
      assert(tail.steps.empty() && tail.words[tail.B + 0x1f34] == bits(30));
    assert(tail.words[tail.B + 0x1f40] == (count == 60 ? 60 : count + 1));
  }
  Fixture fresh;
  fresh.words[fresh.B + 0x1c9c] = 1000;
  fresh.words[fresh.B + 0x1f40] = 39;
  fresh.onIcon = [&] { fresh.words[fresh.B + 0x1f40] = 60; };
  assert(packedBattleTimingADisplay(fresh.B, fresh.services()) &&
         fresh.words[fresh.B + 0x1f40] == 60);
  Fixture missing;
  missing.words[missing.B + 0x1c9c] = 99;
  missing.curveReady = false;
  assert(!packedBattleTimingADisplay(missing.B, missing.services()) &&
         missing.icons.empty() && missing.words[missing.B + 0x1f20] == 0);
  missing.words[missing.B + 0x1c9c] = 100;
  missing.cameraReady = false;
  assert(!packedBattleTimingADisplay(missing.B, missing.services()) &&
         missing.icons.empty() &&
         missing.words[missing.B + 0x1f34] == bits(30));
  missing.cameraReady = true;
  missing.iconReady = false;
  assert(!packedBattleTimingADisplay(missing.B, missing.services()) &&
         missing.words[missing.B + 0x1f20] == 0);
}
