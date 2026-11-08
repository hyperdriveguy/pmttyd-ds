#include "native_harbor_reward_counter.hpp"
#include <cassert>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
constexpr uint32_t Battle = 0x84000000, Pouch = 0x85000000;
struct Fixture {
  bool alive = true, hold = false;
  unsigned calls = 0, sounds = 0, writes = 0, holdAt = 0;
  std::map<uint32_t, uint32_t> memory{{Battle + 0xf00, 9},
                                      {Battle + 0x18ff9, 0},
                                      {Pouch + 0x8a, 1},
                                      {Pouch + 0x96, 0}};
  std::shared_ptr<int> lifetime = std::make_shared<int>(1);
  bool ready() {
    ++calls;
    if (calls == holdAt)
      return false;
    if (hold) {
      hold = false;
      return false;
    }
    return true;
  }
  NativeHarborRewardCounterServices services() {
    NativeHarborRewardCounterServices s;
    s.user = this;
    s.lifetime = lifetime;
    s.current = [](void *p) { return static_cast<Fixture *>(p)->alive; };
    s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.ready())
        return {};
      return f.memory.at(a);
    };
    s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.ready())
        return {};
      return uint8_t(f.memory.at(a));
    };
    s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.ready())
        return {};
      return uint16_t(f.memory.at(a));
    };
    s.writeHalf = [](void *p, uint32_t a, uint16_t n) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.ready())
        return false;
      ++f.writes;
      f.memory[a] = n;
      return true;
    };
    s.sound = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.ready())
        return false;
      ++f.sounds;
      return true;
    };
    return s;
  }
};
void normal(bool accelerated) {
  Fixture f;
  NativeHarborRewardCounter c(Battle, Pouch, f.services());
  assert(c.admit());
  assert(c.total() == 9);
  assert(c.start());
  const uint32_t held = accelerated ? 0x100 : 0;
  uint64_t t = 1;
  while (c.phase() != NativeHarborRewardCounter::Phase::Done) {
    assert(t < 200);
    assert(c.advance(t, held));
    auto calls = f.calls;
    assert(c.advance(t, held));
    assert(f.calls == calls);
    ++t;
  }
  assert(f.memory.at(Pouch + 0x96) == 9);
  assert(f.writes == 9);
  assert(f.sounds == (accelerated ? 1u : 4u));
  assert(t == (accelerated ? 21u : 142u));
}
void holds() {
  // Hold each external access in turn and replay the exact tick; compare all
  // stores/sounds with the no-hold execution. Includes after successful writes.
  for (unsigned target = 1; target < 125; ++target) {
    Fixture f;
    f.holdAt = target;
    auto s = f.services();
    NativeHarborRewardCounter c(Battle, Pouch, s);
    unsigned stalled = 0;
    while (!c.admit()) {
      assert(++stalled < 10);
    }
    assert(c.start());
    for (uint64_t t = 1; c.phase() != NativeHarborRewardCounter::Phase::Done;
         ++t) {
      assert(t < 200);
      // A false service resumes without incrementing the counter twice.
      if (!c.advance(t, 0)) {
        auto before = f.calls;
        assert(!c.advance(t + 1, 0));
        assert(f.calls == before);
        assert(!c.advance(t, 0x100));
        assert(f.calls == before);
        assert(c.advance(t, 0));
      }
    }
    assert(f.memory.at(Pouch + 0x96) == 9 && f.writes == 9 && f.sounds == 4);
  }
}
void domains() {
  for (auto stock : {-5, 0, 9, 80, 150})
    for (auto fortune : {0, 3})
      for (auto level : {1, 98, 99}) {
        Fixture f;
        f.memory[Battle + 0xf00] = uint32_t(stock);
        f.memory[Battle + 0x18ff9] = fortune;
        f.memory[Pouch + 0x8a] = level;
        NativeHarborRewardCounter c(Battle, Pouch, f.services());
        assert(c.admit());
        int n = stock <= 0 ? 1 : stock > 100 ? 100 : stock;
        if (fortune == 3)
          n *= 2;
        if (level >= 99)
          n = 0;
        if (n > 100)
          n = 100;
        assert(c.total() == n);
        if (!n) {
          assert(!c.start());
          assert(c.phase() == NativeHarborRewardCounter::Phase::ZeroReward);
        }
      }
  for (auto level : {1, 98}) {
    Fixture f;
    f.memory[Pouch + 0x96] = 99;
    f.memory[Pouch + 0x8a] = level;
    NativeHarborRewardCounter c(Battle, Pouch, f.services());
    assert(c.admit() && c.start());
    for (uint64_t t = 1; c.phase() != NativeHarborRewardCounter::Phase::LevelUp;
         ++t) {
      assert(t < 150);
      assert(c.advance(t, 0x200));
    }
    assert(f.memory.at(Pouch + 0x96) == (level == 98 ? 0u : 8u));
  }
  Fixture f;
  NativeHarborRewardCounter c(Battle, Pouch, f.services());
  f.alive = false;
  assert(!c.admit());
  assert(f.calls == 0);
  f.alive = true;
  assert(c.admit() && c.start());
  f.alive = false;
  assert(!c.advance(1, 0));
  assert(f.writes == 0);
}
} // namespace
int main() {
  normal(false);
  normal(true);
  holds();
  domains();
  std::cout << "native reward counter: source cadence, physical acceleration, "
               "retained holds and domains passed\n";
}
