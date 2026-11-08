#include "battle_star_point_offset.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args = 0x81000000, Battle = 0x82000000, Unit = 0x83000000,
                   Kind = 0x84000000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
struct Fixture {
  EventEntry event;
  BattleStarPointOffsetCursor cursor;
  BattleStarPointOffsetServices services;
  std::map<uint32_t, uint32_t> words;
  std::map<int32_t, int32_t> values;
  std::vector<std::tuple<unsigned, uint64_t, uint64_t>> trace;
  bool alive = true, reenter = false, mutate = false;
  int hold = -1, remaining = 0;
  unsigned writes = 0;
  Fixture() {
    event.flags = 1;
    event.id = 4;
    event.arguments = Args;
    event.currentCommand = 0x95544f78;
    event.argumentCount = 4;
    words = {{0x8041eb90, Battle},
             {Args, 123},
             {Args + 4, 100},
             {Args + 8, 101},
             {Args + 12, 102},
             {Battle + 0x24, Unit},
             {Unit + 0x10, Kind},
             {Kind + 0x90, bits(1.75f)},
             {Kind + 0x94, bits(-2.5f)},
             {Kind + 0x98, bits(.5f)}};
    values = {{123, 1}, {100, 10}, {101, -10}, {102, 0}};
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter)
        assert(!resumeBattleStarPointOffset(f.event, f.services, f.cursor));
      return f.alive;
    };
    services.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(0, a))
        return {};
      auto i = f.words.find(a);
      return i == f.words.end() ? std::optional<uint32_t>{} : i->second;
    };
    services.doubleBits = [](void *p, uint32_t a) -> std::optional<uint64_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(a == 0x802eea38);
      if (!f.enter(3, a))
        return {};
      return UINT64_C(0x4330000080000000);
    };
    services.value = [](void *p, EventEntry &,
                        int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(1, uint32_t(raw)))
        return {};
      return f.values[raw];
    };
    services.translate = [](void *p, EventEntry &,
                            int32_t id) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(2, uint32_t(id)))
        return {};
      return id;
    };
    services.subtractDoubleFloat = [](void *p, double a,
                                      double b) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      uint64_t aa, bb;
      std::memcpy(&aa, &a, 8);
      std::memcpy(&bb, &b, 8);
      if (!f.enter(4, aa, bb))
        return {};
      return float(a - b);
    };
    services.add = [](void *p, float a, float b) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(5, bits(a), bits(b)))
        return {};
      volatile float sum = a + b;
      return sum;
    };
    services.truncate = [](void *p, float a) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(6, bits(a)))
        return {};
      return int32_t(std::trunc(a));
    };
    services.store = [](void *p, EventEntry &, int32_t dest, int32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(7, uint32_t(dest), uint32_t(value)))
        return false;
      f.values[dest] = value;
      ++f.writes;
      if (f.mutate && f.writes == 1)
        f.values[101] = 999;
      return true;
    };
  }
  bool enter(unsigned kind, uint64_t a, uint64_t b = 0) {
    if (int(trace.size()) == hold && remaining) {
      --remaining;
      return false;
    }
    trace.emplace_back(kind, a, b);
    return true;
  }
  std::optional<int32_t> resume() {
    return resumeBattleStarPointOffset(event, services, cursor);
  }
};
} // namespace
int main() {
  Fixture baseline;
  assert(baseline.resume() == 2 && baseline.values[100] == 11 &&
         baseline.values[101] == -12 && baseline.values[102] == 0 &&
         baseline.writes == 3);
  assert(baseline.resume() == 2 && baseline.writes == 3);
  assert(std::get<0>(baseline.trace[0]) == 0 &&
         std::get<1>(baseline.trace[0]) == 0x8041eb90);
  // Source X truncation precedes lateZ offset read; sourceY truncation follows
  // actualX setter. These effect/math boundaries may not be grouped by axis.
  size_t xTrunc = 0, zRead = 0, xStore = 0, yTrunc = 0;
  unsigned truncs = 0;
  for (size_t n = 0; n < baseline.trace.size(); ++n) {
    auto [k, a, b] = baseline.trace[n];
    (void)b;
    if (k == 6) {
      if (!truncs)
        xTrunc = n;
      if (truncs == 1)
        yTrunc = n;
      ++truncs;
    }
    if (k == 0 && a == Kind + 0x98)
      zRead = n;
    if (k == 7 && a == 100)
      xStore = n;
  }
  assert(truncs == 3 && xTrunc < zRead && zRead < xStore && xStore < yTrunc);
  for (unsigned at = 0; at < baseline.trace.size(); ++at) {
    Fixture f;
    f.hold = int(at);
    f.remaining = 2;
    assert(!f.resume() && !f.resume());
    assert(f.resume() == 2 && f.trace == baseline.trace && f.writes == 3 &&
           f.values == baseline.values);
  }
  for (int32_t x :
       std::array<int32_t, 7>{-32768, -1, 0, 17, 32767, 16777217, -16777217}) {
    Fixture f;
    f.values[100] = x;
    assert(f.resume() == 2);
    volatile float exact = float(x) + 1.75f;
    assert(f.values[100] == int32_t(std::trunc(exact)));
  }
  {
    Fixture f;
    f.words[Args + 8] = 100;
    f.words[Args + 12] = 100;
    assert(f.resume() == 2 && f.values[100] == 10 && f.writes == 3);
  }
  {
    Fixture f;
    f.mutate = true;
    assert(f.resume() == 2 && f.values[101] == -12);
  }
  {
    Fixture f;
    f.reenter = true;
    assert(f.resume() == 2);
  }
  {
    Fixture f;
    f.words[Battle + 0x24] = 0;
    assert(!f.resume() && !f.writes);
  }
  {
    Fixture f;
    f.hold = 12;
    f.remaining = 1;
    assert(!f.resume());
    auto wrong = f.services;
    wrong.user = nullptr;
    assert(!resumeBattleStarPointOffset(f.event, wrong, f.cursor));
    f.alive = false;
    assert(!f.resume());
    f.alive = true;
    assert(f.resume() == 2);
  }
  std::cout << "Whole star-point offset source read/math/setter order, every "
               "hold, aliasing and lifetime pass\n";
}
