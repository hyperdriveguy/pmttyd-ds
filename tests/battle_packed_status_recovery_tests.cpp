#include "battle_packed_status_recovery.hpp"
#include "battle_status.hpp"
#include <array>
#include <cassert>
#include <functional>
#include <vector>
using namespace ttyd;
struct F {
  static constexpr uint32_t U = 0x1000;
  std::array<uint8_t, 0x400> bytes{};
  std::vector<std::array<uint32_t, 3>> trace;
  std::function<void(uint32_t)> onRead;
  void half(uint32_t o, uint16_t n) {
    bytes[o] = uint8_t(n >> 8);
    bytes[o + 1] = uint8_t(n);
  }
  void word(uint32_t o, uint32_t n) {
    for (unsigned i = 0; i < 4; ++i)
      bytes[o + i] = uint8_t(n >> (24 - 8 * i));
  }
  F() {
    half(0x10c, 7);
    bytes[0x21] = 1;
    bytes[0x22] = 1;
    bytes[0x23] = 1;
  }
  BattleActorServices services() {
    BattleActorServices s;
    s.user = this;
    s.memory.user = this;
    s.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<F *>(p);
      const auto o = a - U;
      assert(o < f.bytes.size());
      const auto v = f.bytes[o];
      f.trace.push_back({0, o, v});
      if (f.onRead)
        f.onRead(o);
      return v;
    };
    s.memory.half = [](void *p, uint32_t a) {
      auto &f = *static_cast<F *>(p);
      const auto o = a - U;
      assert(o + 1 < f.bytes.size());
      const auto v = uint16_t(uint16_t(f.bytes[o]) << 8 | f.bytes[o + 1]);
      f.trace.push_back({1, o, v});
      return v;
    };
    s.memory.word = [](void *p, uint32_t a) -> uint32_t {
      auto &f = *static_cast<F *>(p);
      const auto o = a - U;
      assert(o + 3 < f.bytes.size());
      uint32_t n = 0;
      for (unsigned i = 0; i < 4; ++i)
        n = n << 8 | f.bytes[o + i];
      f.trace.push_back({2, o, n});
      return n;
    };
    s.memory.storeByte = [](void *p, uint32_t a, uint8_t n) {
      auto &f = *static_cast<F *>(p);
      const auto o = a - U;
      assert(o < f.bytes.size());
      f.bytes[o] = n;
      f.trace.push_back({3, o, n});
    };
    return s;
  }
  int32_t run(int32_t id) {
    return checkPackedBattleRecoveryStatus(U, id, services());
  }
};
int main() {
  assert(checkPackedBattleRecoveryStatus(0, 1, {}) == 0);
  constexpr uint32_t offsets[] = {
      0x12a, 0x118, 0x11a, 0x11b, 0x11c, 0x11e, 0x11f, 0x120, 0x121, 0x122,
      0x123, 0x123, 0x125, 0x125, 0x127, 0x127, 0x129, 0x12b, 0x12c, 0x12f,
      0x130, 0x12d, 0x12e, 0x131, 0x133, 0,     0,     0x135};
  for (int32_t id = 0; id < 28; ++id)
    for (uint8_t duration : {0, 1, 2, 127, 128, 255})
      for (uint8_t strength : {0, 1, 127, 128, 255})
        for (bool opposite : {false, true}) {
          F f;
          if (offsets[id])
            f.bytes[offsets[id]] = duration;
          if (id == 4 || id == 23 || id == 24 || (id >= 10 && id <= 15))
            f.bytes[offsets[id] + 1] = strength;
          if (opposite && (id == 8 || id == 9))
            f.bytes[id == 8 ? 0x122 : 0x121] = 3;
          if (opposite && (id == 19 || id == 20))
            f.bytes[id == 19 ? 0x130 : 0x12f] = 3;
          f.bytes[0x119] = 77;
          f.bytes[0x24] = 2;
          BattleStatusUnit unit;
          unit.hp = 7;
          for (unsigned i = 0; i < unit.statusBytes.size(); ++i)
            unit.statusBytes[i] = f.bytes[0x118 + i];
          for (unsigned i = 0; i < 5; ++i)
            unit.moveBytes[i] = f.bytes[0x20 + i];
          const auto before = f.bytes;
          const auto expected = checkRecoveryStatus(&unit, id);
          assert(f.run(id) == expected);
          for (unsigned i = 0; i < unit.statusBytes.size(); ++i)
            assert(f.bytes[0x118 + i] == unit.statusBytes[i]);
          for (unsigned i = 0; i < 5; ++i)
            assert(f.bytes[0x20 + i] == unit.moveBytes[i]);
          for (unsigned i = 0; i < f.bytes.size(); ++i)
            if (!(i >= 0x118 && i < 0x136) && !(i >= 0x20 && i < 0x25))
              assert(f.bytes[i] == before[i]);
        }
  F sleep;
  sleep.bytes[0x118] = 255;
  sleep.bytes[0x119] = 99;
  assert(sleep.run(257) == 1);
  assert((sleep.trace ==
          std::vector<std::array<uint32_t, 3>>{
              {0, 0x135, 0}, {1, 0x10c, 7}, {0, 0x118, 255}, {3, 0x118, 0}}));
  assert(sleep.bytes[0x119] == 99);
  F down;
  down.bytes[0x135] = 1;
  assert(down.run(-999) == 0 && down.trace.size() == 1);
  for (uint16_t hp : {uint16_t(0), uint16_t(0xffff), uint16_t(0x8000)}) {
    F dead;
    dead.half(0x10c, hp);
    assert(dead.run(1) == 0 && dead.trace.size() == 2);
  }
  F invalid;
  assert(invalid.run(128) == 0 && invalid.trace.size() == 2);
  F charge;
  charge.bytes[0x129] = 4;
  assert(charge.run(16) == 0 && charge.bytes[0x129] == 4);
  F permanent;
  permanent.bytes[0x12b] = 4;
  assert(permanent.run(17) == 0 && permanent.bytes[0x12b] == 4);
  F badge;
  badge.bytes[0x2f7] = 1;
  assert(badge.run(6) == 0);
  for (const auto &e : badge.trace)
    assert(e[1] != 0x2f7);
  // Shared sign mismatch skips duration entirely; recovery must add no read.
  F mismatch;
  mismatch.bytes[0x128] = 255;
  mismatch.bytes[0x127] = 2;
  assert(mismatch.run(14) == 0);
  assert(
      (mismatch.trace == std::vector<std::array<uint32_t, 3>>{
                             {0, 0x135, 0}, {1, 0x10c, 7}, {0, 0x128, 255}}));
  // Getter's positive sign is retained locally, but SetStatus rechecks fresh
  // defense strength and cancels the opposite sign installed between calls.
  F fresh;
  fresh.bytes[0x127] = 3;
  fresh.bytes[0x128] = 2;
  fresh.onRead = [&](uint32_t o) {
    if (o == 0x127)
      fresh.bytes[0x128] = 255;
  };
  assert(fresh.run(14) == 0 && fresh.bytes[0x127] == 0 &&
         fresh.bytes[0x128] == 0);
  // Fast cancellation first clears Slow, then reads move-count flags; the reset
  // child sees current source bytes and writes only its original move fields.
  F fast;
  fast.bytes[0x12f] = 3;
  fast.bytes[0x130] = 2;
  assert(fast.run(19) == 0);
  assert(fast.bytes[0x130] == 0 && fast.bytes[0x12f] == 3 &&
         fast.bytes[0x22] == 2 && fast.bytes[0x23] == 2);
  size_t clear = 0, flags = 0;
  for (size_t i = 0; i < fast.trace.size(); ++i) {
    if (fast.trace[i][0] == 3 && fast.trace[i][1] == 0x130)
      clear = i;
    if (fast.trace[i][0] == 2 && fast.trace[i][1] == 0x27c)
      flags = i;
  }
  assert(clear < flags);
  F inhibit;
  inhibit.bytes[0x12f] = 2;
  inhibit.word(0x27c, 0x1000);
  assert(inhibit.run(19) == 0);
  assert(inhibit.trace.back()[0] == 2 && inhibit.trace.back()[1] == 0x27c);
  F noBaseline;
  noBaseline.bytes[0x130] = 2;
  noBaseline.bytes[0x23] = 0;
  assert(noBaseline.run(20) == 0);
  assert(noBaseline.trace.back()[1] == 0x23);
  return 0;
}
