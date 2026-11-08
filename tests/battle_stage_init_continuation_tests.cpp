#include "battle_stage_init_continuation.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B = 0x10000, Info = 0x30000, Setup = 0x31000,
                   Stage = 0x32000, Descriptors = 0x34000, Pouch = 0x40000,
                   MapWork = 0x41000, MapData = 0x42000, Root = 0x43000,
                   SpotWork = 0x44000, SpotBank = 0x45000, Units = 0x50000,
                   Entries = 0x60000;
struct Op {
  uint32_t kind, address, value;
  bool operator==(const Op &o) const {
    return kind == o.kind && address == o.address && value == o.value;
  }
};
struct F {
  EventScriptAssets source;
  std::vector<uint8_t> ram = std::vector<uint8_t>(0x100000);
  std::map<uint32_t, uint8_t> globals;
  std::vector<Op> ops;
  std::vector<BattleStageSetupRequest> calls;
  std::vector<std::array<float, 3>> fmas;
  std::shared_ptr<const uint8_t> life = std::make_shared<const uint8_t>(0);
  BattleStageSetupServices s;
  BattleStageInitCursor all;
  BattleStageObjectInitCursor objects;
  BattleSpotlightInitCursor spot;
  size_t holdAt = SIZE_MAX, mathHoldAt = SIZE_MAX;
  unsigned attempts = 0, mathAttempts = 0, mathCount = 0;
  bool alive = true, reentry = false, namesPresent = true;
  unsigned actorCount = 0, eventCount = 0;
  std::vector<int32_t> rankSequence;
  unsigned rankReads = 0;
  uint8_t byte(uint32_t a) {
    if (a < ram.size())
      return ram[a];
    auto i = globals.find(a);
    if (i != globals.end())
      return i->second;
    uint8_t v;
    assert(source.readByte(a, v));
    return v;
  }
  uint32_t word(uint32_t a) {
    const auto v = (uint32_t(byte(a)) << 24) | (uint32_t(byte(a + 1)) << 16) |
                   (uint32_t(byte(a + 2)) << 8) | byte(a + 3);
    ops.push_back({0, a, v});
    return v;
  }
  uint16_t half(uint32_t a) {
    uint16_t v = (uint16_t(byte(a)) << 8) | byte(a + 1);
    if (a == Pouch + 0x88 && rankReads < rankSequence.size())
      v = uint16_t(rankSequence[rankReads++]);
    ops.push_back({1, a, v});
    return v;
  }
  void put(uint32_t a, uint8_t v) {
    if (a < ram.size())
      ram[a] = v;
    else
      globals[a] = v;
  }
  void w(uint32_t a, uint32_t v) {
    ops.push_back({2, a, v});
    for (unsigned i = 0; i < 4; ++i)
      put(a + i, uint8_t(v >> (24 - 8 * i)));
  }
  void h(uint32_t a, uint16_t v) {
    ops.push_back({3, a, v});
    put(a, uint8_t(v >> 8));
    put(a + 1, uint8_t(v));
  }
  F(int32_t rank = 0, unsigned count = 5, bool interface = true) {
    assert(source.load("romfs/harbor-tutorial-events.pmevt"));
    w(0x8041eb90, B);
    w(B + 0x2738, Info);
    w(Info + 0xc, Setup);
    w(Setup, Stage);
    w(Stage + 8, count);
    w(Stage + 0xc, Descriptors);
    w(0x8041eb00, Pouch);
    h(Pouch + 0x88, uint16_t(rank));
    w(0x804175a0, SpotWork);
    w(0x804295d4, 0x12345678);
    w(SpotWork + 8, 0x87654321);
    w(MapWork + 0xa8, MapData);
    w(MapData + 8, Root);
    w(Root, 0x71000);
    for (unsigned i = 0; i < 40; ++i) {
      const auto d = Descriptors + 24 * i;
      w(d, 0x72000 + 4 * i);
      h(d + 4, i < 2 ? uint16_t(i) : 2);
      h(d + 6, uint16_t(i == 4 ? (interface ? 6 : 4) : i % 4));
      for (unsigned a = 0; a < 3; ++a) {
        float v = float(10 * i + a);
        uint32_t raw;
        std::memcpy(&raw, &v, 4);
        w(d + 8 + 4 * a, raw);
      }
    }
    for (unsigned i = 0; i < 5; ++i)
      w(Stage + (i ? 0x19c + 4 * i : 0x190), 0x73000 + 4 * i);
    s.actors.memory.user = this;
    s.actors.memory.word = [](void *p, uint32_t a) {
      return static_cast<F *>(p)->word(a);
    };
    s.actors.memory.half = [](void *p, uint32_t a) {
      return static_cast<F *>(p)->half(a);
    };
    s.actors.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<F *>(p)->w(a, v);
    };
    s.actors.memory.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      static_cast<F *>(p)->h(a, v);
    };
    s.actors.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      auto &f = *static_cast<F *>(p);
      f.ops.push_back({4, a, v});
      f.put(a, v);
    };
    s.actors.memory.flushBlock = [](void *, uint32_t) {};
    s.actors.memory.systemCall = [](void *) {};
    s.actors.user = this;
    s.actors.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<F *>(p);
      const auto v = f.byte(a);
      f.ops.push_back({5, a, v});
      return v;
    };
    s.user = this;
    s.lifetime = life;
    s.current = [](void *p) {
      auto &f = *static_cast<F *>(p);
      if (f.reentry) {
        assert(!resumePackedBattleStageInit(f.s, f.all));
        assert(!f.all.reset());
      }
      return f.alive;
    };
    s.call = [](void *p,
                const BattleStageSetupRequest &q) -> std::optional<uint32_t> {
      return static_cast<F *>(p)->call(q);
    };
    s.subtractToSingle = [](void *p, double a,
                            double b) -> std::optional<float> {
      auto &f = *static_cast<F *>(p);
      if (!f.math())
        return {};
      return float(a - b);
    };
    s.fma = [](void *p, float a, float b, float c) -> std::optional<float> {
      auto &f = *static_cast<F *>(p);
      if (!f.math())
        return {};
      f.fmas.push_back({a, b, c});
      return std::fma(a, b, c);
    };
    ops.clear();
  }
  bool math() {
    if (mathCount == mathHoldAt && mathAttempts++ < 2)
      return false;
    ++mathCount;
    return true;
  }
  std::optional<uint32_t> call(const BattleStageSetupRequest &q) {
    if (reentry) {
      assert(!resumePackedBattleStageInit(s, all));
      assert(!all.reset());
    }
    if (calls.size() == holdAt && attempts++ < 2)
      return {};
    calls.push_back(q);
    switch (q.function) {
    case 0x8002b438:
      return MapWork;
    case 0x8001f878:
      return namesPresent ? 0x90000u : 0u;
    case 0x80128fe0: {
      const auto u = Units + 0x1000 * actorCount;
      w(u, 10 + actorCount++);
      return u;
    }
    case 0x800374ac: {
      const auto e = Entries + 0x200 * eventCount;
      w(e + 0x15c, 1000 + eventCount++);
      return e;
    }
    case 0x800f8934:
      assert(q.integers[0] == 5 * 0x2c);
      return SpotBank;
    default:
      return 0;
    }
  }
  void run() {
    for (unsigned i = 0; i < 100; ++i)
      if (resumePackedBattleStageInit(s, all))
        return;
    assert(false);
  }
};
void ordinary() {
  F f;
  f.run();
  assert(f.word(B + 0x163fc + 0xb28) == 0x43480000);
  for (unsigned i = 0; i < 7; ++i)
    assert(f.word(B + 0x1715c + i * 0x7c) == i + 1);
  assert(f.word(B + 0x1715c + 5 * 0x7c + 8) == 0x43480000);
  assert(f.word(B + 0x1715c + 5 * 0x7c + 0x68) == 1);
  assert(f.word(B + 0x1715c + 6 * 0x7c + 0x68) == 1);
  assert(f.byte(B + 0x180f8) == 0 && f.eventCount == 5 && f.actorCount == 2);
  for (unsigned i = 0; i < 4; ++i)
    assert(f.word(B + 0x18100 + 4 * i) == 1001 + i);
  assert(f.word(SpotWork) == 5 && f.word(SpotWork + 4) == SpotBank &&
         f.word(SpotWork + 8) == 0x87654321);
  assert(f.fmas.size() == 5);
  for (unsigned i = 0; i < 5; ++i) {
    assert(f.fmas[i][1] == float(i));
    assert(f.word(SpotBank + 0x2c * i + 0x28) == 0xffffffff);
  }
}
void eachHold() {
  F baseline;
  baseline.run();
  const auto expected = baseline.ram;
  const auto op = baseline.ops;
  for (size_t i = 0; i < baseline.calls.size(); ++i) {
    F f;
    f.holdAt = i;
    f.reentry = true;
    assert(!resumePackedBattleStageInit(f.s, f.all));
    const auto prefix = f.ops;
    assert(!resumePackedBattleStageInit(f.s, f.all) && f.ops == prefix);
    f.reentry = false;
    f.run();
    assert(f.ram == expected && f.ops == op && f.attempts == 3 &&
           f.calls.size() == baseline.calls.size());
  }
  for (size_t i = 0; i < 10; ++i) {
    F f;
    f.mathHoldAt = i;
    assert(!resumePackedBattleStageInit(f.s, f.all));
    const auto prefix = f.ops;
    assert(!resumePackedBattleStageInit(f.s, f.all) && f.ops == prefix);
    f.run();
    assert(f.ram == expected && f.ops == op && f.mathAttempts == 3);
  }
}
void branches() {
  for (int rank : {-1, 0, 1, 2, 3, 4})
    for (unsigned count : {0u, 1u, 5u, 32u, 40u, 0xffffffffu}) {
      F f(rank, count, false);
      f.run();
      if (count < 29) {
        const auto rows = (count ? count : 0) + (rank > 0 ? 3 : 0) + 2;
        assert(f.word(B + 0x1715c + (rows - 1) * 0x7c) == rows);
      }
      assert(f.byte(B + 0x180f8) & 0x20);
    }
  // Mandatory wall/built-in scans have no full-bank rejection branch. Both
  // overwrite the last selected row, preserving the source ordinal sequence.
  F full(0, 32, false);
  full.run();
  const auto last = B + 0x1715c + 31 * 0x7c;
  assert(full.word(last) == 34 && full.word(last + 0x64) == 0x80417cb8);
  F f;
  f.rankSequence = {0, 1, 0, 0, 0};
  f.run();
  assert(f.word(B + 0x18110) == 0x80373ff8);
}
void layersAndSourceVisibility() {
  F f(0, 4, false);
  const std::array<uint16_t, 4> layers{4, 5, 7, 8};
  for (unsigned i = 0; i < layers.size(); ++i) {
    f.h(Descriptors + 24 * i + 4, 2);
    f.h(Descriptors + 24 * i + 6, layers[i]);
  }
  f.run();
  for (unsigned i : {0u, 1u, 3u})
    for (unsigned axis = 0; axis < 3; ++axis)
      assert(f.word(B + 0x1715c + 0x7c * i + 4 + 4 * axis) == 0);
  for (unsigned axis = 0; axis < 3; ++axis)
    assert(f.word(B + 0x1715c + 0x7c * 2 + 4 + 4 * axis) ==
           f.word(Descriptors + 24 * 2 + 8 + 4 * axis));
  F flags;
  for (unsigned i = 0; i < 3; ++i)
    flags.put(Stage + 0x1b1 + i, uint8_t(i + 1));
  flags.run();
  assert(flags.byte(B + 0x180f8) == 0x0e);
}
void originalCrumpStage() {
  F f;
  f.w(Setup, 0x95540870);
  f.ops.clear();
  f.run();
  assert(f.actorCount == 0 && f.eventCount == 0);
  for (unsigned i = 0; i < 6; ++i)
    assert(f.word(B + 0x1715c + i * 0x7c) == i + 1);
  for (unsigned i = 0; i < 4; ++i)
    assert(f.word(B + 0x1715c + i * 0x7c + 0x64) == 0x955407e0 + 0x18 * i);
  assert((f.byte(B + 0x180f8) & 0x20) != 0);
}
void namesAndNullArray() {
  F f;
  f.namesPresent = false;
  f.w(Stage + 0xc, 0);
  f.ops.clear();
  f.run();
  assert(f.word(B + 0x1715c) == 1 && f.word(B + 0x1715c + 0x7c) == 2 &&
         f.actorCount == 0 && f.eventCount == 1);
  assert((f.byte(B + 0x180f8) & 0x2e) == 0x2e);
  unsigned lookups = 0;
  for (auto q : f.calls)
    if (q.function == 0x8001f878)
      ++lookups;
  assert(lookups == 4);
}
void lifetime() {
  F f;
  f.holdAt = 0;
  assert(!resumePackedBattleStageInit(f.s, f.all));
  const auto ops = f.ops;
  auto other = f.s;
  other.user = nullptr;
  assert(!resumePackedBattleStageInit(other, f.all) && f.ops == ops);
  other = f.s;
  other.lifetime =
      std::shared_ptr<const void>(f.life.get(), [](const void *) {});
  assert(!resumePackedBattleStageInit(other, f.all) && f.ops == ops);
  other = f.s;
  other.actors.audienceLifecycle.user = &other;
  assert(!resumePackedBattleStageInit(other, f.all) && f.ops == ops);
  f.alive = false;
  assert(!resumePackedBattleStageInit(f.s, f.all));
  f.alive = true;
  assert(!resumePackedBattleStageInit(f.s, f.all) && f.ops == ops);
  assert(f.all.reset());
}
} // namespace
int main() {
  ordinary();
  eachHold();
  branches();
  layersAndSourceVisibility();
  originalCrumpStage();
  namesAndNullArray();
  lifetime();
  std::cout
      << "whole StageInit/ObjectInit/SpotInit source branches/prefix/holds "
         "tests PASS (TEST external map/event/heap leaves)\n";
}
