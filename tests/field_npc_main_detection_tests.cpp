#include "field_npc_main_detection.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000, E = 0x20000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
uint64_t bits(double f) {
  uint64_t w;
  std::memcpy(&w, &f, 8);
  return w;
}
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
struct World {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::map<uint32_t, uint64_t> doubles;
  using Effect = std::tuple<char, uint32_t, uint64_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcDetectionRequest> clouds;
  std::array<float, 3> powers{};
  unsigned attempts = 0, fail = ~0u, interrupts = 0, seeds = 0;
  uint32_t found = E;
  bool mutateCloud = false, mutateInterrupt = false, interruptReady = true,
       mutateRadius = false;
  double seedInput = 0;
  World() {
    for (unsigned o = 0; o < 0x340; o += 4)
      words[A + o] = 0;
    words[A + 0x11c] = 42;
    words[A + 0x13c] = 0x90000;
    words[A + 0x140] = 0x80;
    words[A + 0x8c] = bits(10.f);
    words[A + 0x90] = bits(20.f);
    words[A + 0x94] = bits(30.f);
    words[A + 0x14c] = bits(4.f);
    words[A + 0x150] = bits(8.f);
    words[0x8041fc7c] = 0;
    words[0x8041fc80] = bits(.5f);
    words[0x8041e3c8] = 0x7fffffff;
    bytes = {{E + 8, 1}, {E + 0x10, 1}};
    doubles = {
        {0x802c13c8, bits(.5)}, {0x802c13d0, bits(3.)}, {0x802c13d8, bits(0.)}};
  }
  bool admit() { return attempts++ != fail; }
  FieldNpcDetectionServices services() {
    FieldNpcDetectionServices s;
    s.norm.memory.user = this;
    s.norm.memory.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.words.find(a);
      if (i == f.words.end())
        return {};
      f.trace.emplace_back('r', a, i->second);
      return i->second;
    };
    s.norm.memory.store = [](void *p, uint32_t a, uint32_t w) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.words[a] = w;
      f.trace.emplace_back('w', a, w);
      return true;
    };
    s.norm.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.doubles.find(a);
      if (i == f.doubles.end())
        return {};
      f.trace.emplace_back('d', a, i->second);
      return i->second;
    };
    s.norm.estimate = [](void *p, double q, double &out) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.trace.emplace_back('e', 0, bits(q));
      ++f.seeds;
      f.seedInput = q;
      if (f.mutateRadius)
        f.words[A + 0x214] = bits(4.f);
      return nativeFieldNpcReciprocalSqrtEstimate(nullptr, q, out);
    };
    s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.bytes.find(a);
      if (i == f.bytes.end())
        return {};
      f.trace.emplace_back('b', a, i->second);
      return i->second;
    };
    s.request = [](void *p, const FieldNpcDetectionRequest &q,
                   FieldNpcDetectionResponse &out) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      if (q.function == 0x800440f8 && !f.interruptReady)
        return false;
      f.trace.emplace_back('c', q.function, q.argument);
      if (q.function == 0x80035a28) {
        assert(q.argument == 42);
        out.word = f.found;
        return true;
      }
      if (q.function == 0x80187f10) {
        const auto n = f.clouds.size();
        assert(n < 3);
        out.power = f.powers[n];
        f.clouds.push_back(q);
        if (f.mutateCloud) {
          const float k = n == 0 ? 100.f : 1000.f;
          f.words[A + 0x8c] = bits(k);
          f.words[A + 0x90] = bits(2 * k);
          f.words[A + 0x94] = bits(3 * k);
          f.words[A + 0x150] = bits(n == 0 ? 20.f : 40.f);
          f.words[A + 0x14c] = bits(n == 0 ? 7.f : 9.f);
          f.words[A + 0x13c] = 0;
          f.words[A] |= 0x20000020;
          f.bytes[E + 0x10] = 8; // captured event class/script survive
        }
        return true;
      }
      assert(q.function == 0x800440f8 && q.argument == A);
      ++f.interrupts;
      if (f.mutateInterrupt)
        f.words[A + 0x140] = 0xfeed;
      return true;
    };
    return s;
  }
};
FieldNpcDetectionFrontier run(World &f, FieldNpcDetectionCursor &c) {
  for (unsigned n = 0; n < 4; ++n) {
    auto r = advanceFieldNpcMainDetection(A, f.services(), c);
    if (r != FieldNpcDetectionFrontier::Pending)
      return r;
  }
  assert(false);
  return FieldNpcDetectionFrontier::Pending;
}
void holds(World original) {
  original.attempts = 0;
  original.fail = ~0u;
  original.trace.clear();
  original.clouds.clear();
  original.interrupts = 0;
  original.seeds = 0;
  World baseline = original;
  FieldNpcDetectionCursor b;
  auto expected = run(baseline, b);
  for (unsigned n = 0; n < baseline.attempts; ++n) {
    World f = original;
    f.fail = n;
    FieldNpcDetectionCursor c;
    assert(advanceFieldNpcMainDetection(A, f.services(), c) ==
           FieldNpcDetectionFrontier::Pending);
    assert(run(f, c) == expected && f.words == baseline.words &&
           f.bytes == baseline.bytes);
    assert(f.trace == baseline.trace && f.interrupts == baseline.interrupts &&
           f.clouds.size() == baseline.clouds.size());
    for (unsigned i = 0; i < f.clouds.size(); ++i) {
      assert(f.clouds[i].point == baseline.clouds[i].point);
      assert(bits(f.clouds[i].width) == bits(baseline.clouds[i].width));
    }
    auto before = f.trace;
    assert(run(f, c) == expected && f.trace == before);
  }
}
void cloud() {
  World f;
  f.powers[0] = 1;
  holds(f);
  FieldNpcDetectionCursor c;
  assert(run(f, c) == FieldNpcDetectionFrontier::Display && f.interrupts == 1);
  const std::vector<World::Effect> expected = {{'r', A + 0x11c, 42},
                                               {'c', 0x80035a28, 42},
                                               {'b', E + 8, 1},
                                               {'r', A + 0x13c, 0x90000},
                                               {'b', E + 0x10, 1},
                                               {'r', A, 0},
                                               {'r', A + 0x8c, bits(10.f)},
                                               {'r', A + 0x90, bits(20.f)},
                                               {'r', A + 0x94, bits(30.f)},
                                               {'r', A + 0x14c, bits(4.f)},
                                               {'c', 0x80187f10, 0},
                                               {'r', 0x8041fc7c, 0},
                                               {'r', A + 0x140, 0x80},
                                               {'w', A + 0x140, 0x88},
                                               {'c', 0x800440f8, A}};
  assert(f.trace == expected);
  for (unsigned positive = 0; positive < 3; ++positive) {
    World w;
    w.powers[positive] = 1;
    holds(w);
    FieldNpcDetectionCursor p;
    assert(run(w, p) == FieldNpcDetectionFrontier::Display &&
           w.clouds.size() == positive + 1);
    const std::array<float, 3> y{20, 16, 12};
    for (unsigned n = 0; n < w.clouds.size(); ++n)
      assert(real(w.clouds[n].point[1]) == y[n]);
    assert(w.words[A + 0x140] == 0x88);
  }
  World fresh;
  fresh.mutateCloud = true;
  fresh.powers[0] = std::numeric_limits<float>::quiet_NaN();
  holds(fresh);
  FieldNpcDetectionCursor r;
  assert(run(fresh, r) == FieldNpcDetectionFrontier::Mario &&
         r.eventClass == 1);
  assert((fresh.clouds[1].point ==
          std::array<uint32_t, 3>{bits(100.f), bits(190.f), bits(300.f)}));
  assert((fresh.clouds[2].point ==
          std::array<uint32_t, 3>{bits(1000.f), bits(1960.f), bits(3000.f)}));
  assert(fresh.clouds[1].width == 7 && fresh.clouds[2].width == 9);
  World held;
  held.powers[0] = 1;
  held.interruptReady = false;
  FieldNpcDetectionCursor h;
  assert(advanceFieldNpcMainDetection(A, held.services(), h) ==
             FieldNpcDetectionFrontier::Pending &&
         h.phase == 181);
  assert(held.words[A + 0x140] == 0x88 && held.clouds.size() == 1);
  const auto beforeHold = held.trace;
  assert(advanceFieldNpcMainDetection(A + 0x340, held.services(), h) ==
             FieldNpcDetectionFrontier::Pending &&
         held.trace == beforeHold);
  auto foreign = held.services();
  foreign.byte = nullptr;
  assert(advanceFieldNpcMainDetection(A, foreign, h) ==
             FieldNpcDetectionFrontier::Pending &&
         held.trace == beforeHold);
  held.interruptReady = true;
  held.mutateInterrupt = true;
  assert(run(held, h) == FieldNpcDetectionFrontier::Display &&
         held.words[A + 0x140] == 0xfeed && held.clouds.size() == 1);
}
void gates() {
  World absent;
  absent.words[A + 0x11c] = 0;
  holds(absent);
  FieldNpcDetectionCursor a;
  assert(run(absent, a) == FieldNpcDetectionFrontier::Display &&
         absent.trace.size() == 1 && absent.interrupts == 0);
  for (bool stopped : {false, true}) {
    World f;
    if (stopped)
      f.bytes[E + 8] = 3;
    else
      f.found = 0;
    holds(f);
    FieldNpcDetectionCursor c;
    assert(run(f, c) == FieldNpcDetectionFrontier::Display &&
           f.interrupts == 1 && f.clouds.empty());
  }
  for (uint32_t bypass : {0x20000000u, 0x20u}) {
    World f;
    f.words[A] = bypass;
    holds(f);
    FieldNpcDetectionCursor c;
    assert(run(f, c) == FieldNpcDetectionFrontier::Mario && f.clouds.empty());
  }
  for (uint8_t eventClass : {5, 8}) {
    World f;
    f.bytes[E + 0x10] = eventClass;
    holds(f);
    FieldNpcDetectionCursor c;
    assert(run(f, c) == FieldNpcDetectionFrontier::Mario && f.clouds.empty());
  }
  World noScript;
  noScript.words[A + 0x13c] = 0;
  holds(noScript);
  FieldNpcDetectionCursor n;
  assert(run(noScript, n) == FieldNpcDetectionFrontier::Mario &&
         noScript.trace[4] == World::Effect('b', E + 0x10, 1));
}
void territory() {
  World circle;
  circle.words[A + 0x13c] = 0;
  circle.words[A + 0x1f8] = 1;
  circle.words[A + 0x8c] = bits(3.f);
  circle.words[A + 0x94] = bits(4.f);
  circle.words[A + 0x214] = bits(5.f);
  holds(circle);
  FieldNpcDetectionCursor equal;
  assert(run(circle, equal) == FieldNpcDetectionFrontier::Mario);
  for (float radius :
       {std::nextafter(5.f, 0.f), std::numeric_limits<float>::quiet_NaN()}) {
    World f = circle;
    f.words[A + 0x214] = bits(radius);
    holds(f);
    FieldNpcDetectionCursor c;
    assert(run(f, c) == (std::isnan(radius)
                             ? FieldNpcDetectionFrontier::Mario
                             : FieldNpcDetectionFrontier::Display));
    assert(f.words[A + 0x140] == (std::isnan(radius) ? 0x80u : 0x84u));
  }
  for (uint8_t cls : {1, 2}) {
    World freshRadius = circle;
    freshRadius.bytes[E + 0x10] = cls;
    freshRadius.mutateRadius = true;
    holds(freshRadius);
    FieldNpcDetectionCursor r;
    assert(run(freshRadius, r) == FieldNpcDetectionFrontier::Display &&
           freshRadius.words[A + 0x140] == 0x84);
  }
  for (float x : {std::numeric_limits<float>::quiet_NaN(),
                  std::numeric_limits<float>::infinity()}) {
    World exceptional = circle;
    exceptional.words[A + 0x8c] = bits(x);
    holds(exceptional);
    FieldNpcDetectionCursor e;
    assert(run(exceptional, e) == FieldNpcDetectionFrontier::Mario &&
           std::isnan(e.distance));
  }
  World fused = circle;
  fused.words[A + 0x8c] = 0;
  fused.words[A + 0x94] = 0;
  fused.words[A + 0x1fc] = 0x41837bdb;
  fused.words[A + 0x204] = 0x423937c2;
  holds(fused);
  FieldNpcDetectionCursor q;
  run(fused, q);
  assert(fused.seedInput ==
         double(real(0x4516e3a3))); // auto-pose separate sum is4516e3a2
  for (uint8_t cls : {1, 2}) {
    World rect;
    rect.words[A + 0x13c] = 0;
    rect.bytes[E + 0x10] = cls;
    rect.words[A + 0x1f8] = 2;
    const auto xo = cls == 2 ? 0x214 : 0x208, zo = cls == 2 ? 0x21c : 0x210;
    rect.words[A + xo] = bits(2.f);
    rect.words[A + zo] = bits(3.f);
    for (float x : {-2.f, 2.f, 3.f, std::numeric_limits<float>::quiet_NaN()}) {
      World f = rect;
      f.words[A + 0x8c] = bits(x);
      f.words[A + 0x94] = bits(3.f);
      holds(f);
      FieldNpcDetectionCursor c;
      assert(run(f, c) == (x > 2 ? FieldNpcDetectionFrontier::Display
                                 : FieldNpcDetectionFrontier::Mario));
    }
    World shortCircuit = rect;
    shortCircuit.words[A + 0x8c] = bits(3.f);
    shortCircuit.words.erase(A + 0x204);
    holds(shortCircuit);
    FieldNpcDetectionCursor s;
    assert(run(shortCircuit, s) == FieldNpcDetectionFrontier::Display);
    World nanX = rect;
    nanX.words[A + 0x8c] = bits(std::numeric_limits<float>::quiet_NaN());
    nanX.words[A + 0x94] = bits(4.f);
    holds(nanX);
    FieldNpcDetectionCursor n;
    assert(run(nanX, n) == FieldNpcDetectionFrontier::Display);
  }
  World unknown = circle;
  unknown.words[A + 0x1f8] = 3;
  unknown.words.erase(A + 0x204);
  holds(unknown);
  FieldNpcDetectionCursor u;
  assert(run(unknown, u) == FieldNpcDetectionFrontier::Mario);
}
} // namespace
int main() {
  cloud();
  gates();
  territory();
}
