#include "field_npc_main_auto_pose.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000, D = 0x20000, OtherD = 0x30000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
uint64_t bits(double f) {
  uint64_t w;
  std::memcpy(&w, &f, 8);
  return w;
}
struct World {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint64_t> doubles;
  using Effect = std::tuple<char, uint32_t, uint64_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcAutoPoseRequest> calls;
  unsigned attempts = 0, fail = ~0u, seeds = 0;
  bool mutateDt = false, mutateDescriptor = false, mutateCall = false,
       overrideSeed = false;
  double seedInput = 0;
  World() {
    words = {{A, 0x400},
             {A + 0x1a4, bits(1.f)},
             {A + 0x108, 0},
             {A + 0x28, D},
             {A + 0x104, 7},
             {D + 0x3c, bits(2.f)},
             {D + 0x10, 0x80010000},
             {D + 0x18, 0x80020000},
             {D + 0x1c, 0x80030000},
             {0x8041fc7c, 0},
             {0x8041fcdc, 0x3dcccccd},
             {0x8041e3c8, 0x7fffffff}};
    doubles = {
        {0x802c13c8, bits(.5)}, {0x802c13d0, bits(3.)}, {0x802c13d8, bits(0.)}};
  }
  bool admit() { return attempts++ != fail; }
  FieldNpcAutoPoseServices services() {
    FieldNpcAutoPoseServices s;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.words.find(a);
      if (i == f.words.end())
        return {};
      const auto v = i->second;
      f.trace.emplace_back('r', a, v);
      if (f.mutateDescriptor && a == A + 0x108) {
        f.words[A + 0x28] = OtherD;
        f.words[D + 0x18] = 0x80040000;
      }
      return v;
    };
    s.memory.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.words[a] = v;
      f.trace.emplace_back('w', a, v);
      return true;
    };
    s.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.doubles.find(a);
      if (i == f.doubles.end())
        return {};
      f.trace.emplace_back('d', a, i->second);
      return i->second;
    };
    s.estimate = [](void *p, double q, double &out) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.trace.emplace_back('e', 0, bits(q));
      ++f.seeds;
      f.seedInput = q;
      if (f.mutateDt)
        f.words[A + 0x1a4] = bits(2.f);
      if (f.overrideSeed) {
        out = .4;
        return true;
      }
      return nativeFieldNpcReciprocalSqrtEstimate(nullptr, q, out);
    };
    s.request = [](void *p, const FieldNpcAutoPoseRequest &q) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      assert(q.function == 0x80041170 && q.args[0] == 7 && q.args[2] == 1);
      f.trace.emplace_back('c', q.function, q.args[1]);
      f.calls.push_back(q);
      if (f.mutateCall)
        f.words[A + 0x108] = 0xfeed;
      return true;
    };
    return s;
  }
};
bool run(World &f, FieldNpcAutoPoseCursor &c, float x, float z = 0,
         float factor = 1) {
  for (unsigned n = 0; n < 4; ++n)
    if (advanceFieldNpcMainAutoPose(A, x, z, factor, f.services(), c))
      return true;
  return false;
}
void holds(World original, float x, float z = 0, float factor = 1) {
  World baseline = original;
  FieldNpcAutoPoseCursor b;
  assert(run(baseline, b, x, z, factor));
  for (unsigned n = 0; n < baseline.attempts; ++n) {
    World f = original;
    f.fail = n;
    FieldNpcAutoPoseCursor c;
    assert(!advanceFieldNpcMainAutoPose(A, x, z, factor, f.services(), c));
    assert(run(f, c, x, z, factor));
    assert(f.words == baseline.words);
    assert(f.trace == baseline.trace &&
           f.calls.size() == baseline.calls.size());
    assert(bits(c.norm) == bits(b.norm));
    auto before = f.trace;
    assert(run(f, c, x, z, factor) && f.trace == before);
  }
}
void ordinary() {
  World f;
  holds(f, 0);
  FieldNpcAutoPoseCursor c;
  assert(run(f, c, 0));
  const std::vector<World::Effect> expected = {{'r', A, 0x400},
                                               {'r', A + 0x1a4, bits(1.f)},
                                               {'r', 0x8041fc7c, 0},
                                               {'d', 0x802c13d8, bits(0.)},
                                               {'r', A + 0x1a4, bits(1.f)},
                                               {'r', 0x8041fcdc, 0x3dcccccd},
                                               {'r', A + 0x108, 0},
                                               {'r', A + 0x28, D},
                                               {'r', A + 0x104, 7},
                                               {'r', D + 0x10, 0x80010000},
                                               {'c', 0x80041170, 0x80010000},
                                               {'w', A + 0x108, 1}};
  assert(f.trace == expected && f.seeds == 0);
  for (auto input :
       std::vector<std::tuple<float, float, uint32_t>>{{1, 1, 2},
                                                       {2, 1, 3},
                                                       {1, .5f, 3},
                                                       {0, 1, 1},
                                                       {.05f, 1, 1},
                                                       {.125f, 1, 2}}) {
    World w;
    FieldNpcAutoPoseCursor p;
    const auto [x, factor, phase] = input;
    holds(w, x, 0, factor);
    assert(run(w, p, x, 0, factor));
    assert(w.words[A + 0x108] == phase);
    w.calls.clear();
    w.trace.clear();
    FieldNpcAutoPoseCursor same;
    assert(run(w, same, x, 0, factor) && w.calls.empty());
    for (auto e : w.trace)
      assert(std::get<0>(e) != 'w');
  }
  World mutate;
  mutate.mutateDt = true;
  mutate.mutateDescriptor = true;
  mutate.mutateCall = true;
  holds(mutate, 2);
  FieldNpcAutoPoseCursor m;
  assert(run(mutate, m, 2));
  assert(m.speed == 1 && mutate.calls.at(0).args[1] == 0x80040000 &&
         mutate.words[A + 0x108] ==
             2); // retained descriptor, fresh name, late phase
}
void gates() {
  for (float dt : {0.f, -1.f, std::numeric_limits<float>::quiet_NaN()}) {
    World f;
    f.words[A + 0x1a4] = bits(dt);
    holds(f, 1);
    FieldNpcAutoPoseCursor c;
    assert(run(f, c, 1) && f.trace.size() == 3 && f.calls.empty());
  }
  World off;
  off.words[A] = 0;
  holds(off, 1);
  FieldNpcAutoPoseCursor c;
  assert(run(off, c, 1) && off.trace.size() == 1);
  // A changed child argument or owner cannot resume a captured operation.
  World held;
  auto svc = held.services();
  svc.request = nullptr;
  FieldNpcAutoPoseCursor h;
  assert(!advanceFieldNpcMainAutoPose(A, 0, 0, 1, svc, h) && h.phase == 42);
  auto trace = held.trace;
  assert(!advanceFieldNpcMainAutoPose(A, 0, 0, .5f, svc, h) &&
         held.trace == trace);
  svc = held.services();
  assert(!advanceFieldNpcMainAutoPose(A, 0, 0, 1, svc, h));
}
void exceptional() {
  World coarse;
  coarse.overrideSeed = true;
  holds(coarse, 2);
  FieldNpcAutoPoseCursor coarseCursor;
  assert(run(coarse, coarseCursor, 2));
  assert(bits(coarseCursor.norm) ==
         0x3ffffde9); // independent rational-rounding oracle

  const float nan = real(0x7fc12345),
              inf = std::numeric_limits<float>::infinity();
  for (float x : {nan, inf}) {
    World f;
    holds(f, x);
    FieldNpcAutoPoseCursor c;
    assert(run(f, c, x));
    assert(std::isnan(c.speed) && f.words[A + 0x108] == 3);
    assert(f.seeds == (std::isinf(x) ? 1u : 0u));
    if (std::isnan(x))
      assert(bits(c.norm) == 0x7fffffff);
  }
  World threshold;
  threshold.words[D + 0x3c] = bits(nan);
  holds(threshold, 1);
  FieldNpcAutoPoseCursor t;
  assert(run(threshold, t, 1) && threshold.words[A + 0x108] == 3);
  World idle;
  idle.words[A + 0x108] = 1;
  idle.words.erase(A + 0x28);
  FieldNpcAutoPoseCursor i;
  assert(run(idle, i, 0) && idle.calls.empty());
  // These operands distinguish separate SP squares/add from a fused norm.
  World squares;
  FieldNpcAutoPoseCursor q;
  assert(run(squares, q, real(0x41837bdb), real(0x423937c2)));
  assert(squares.seedInput == double(real(0x4516e3a2)));
  assert(squares.seedInput != double(real(0x4516e3a3)));
}
} // namespace
int main() {
  ordinary();
  gates();
  exceptional();
}
