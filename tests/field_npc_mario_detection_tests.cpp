#include "field_npc_mario_detection.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000, M = 0x20000, M2 = 0x30000, G = 0x40000;
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
  std::map<uint32_t, uint16_t> halves;
  std::map<uint32_t, uint64_t> doubles;
  using Effect = std::tuple<char, uint32_t, uint64_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcMarioRequest> calls;
  std::vector<uint32_t> statuses{0};
  unsigned attempts = 0, fail = ~0u, gets = 0, vivians = 0, interrupts = 0;
  uint32_t sequence = 2, level = 0;
  float compared = 0, revised = 0;
  bool ready = true, secondMario = false, mutateLevel = false,
       mutateSeq = false, mutateCompare = false, mutateRange = false;
  World() {
    for (uint32_t o = 0; o < 0x340; o += 4)
      words[A + o] = 0;
    for (uint32_t p : {M, M2}) {
      words[p] = 0;
      words[p + 0x8c] = bits(10.f);
      words[p + 0x90] = 0;
      words[p + 0x94] = 0;
      halves[p + 0x2e] = 0;
    }
    words[0x80415fd0] = G;
    words[G + 0x164] = 0;
    words[A + 0x140] = 0x80;
    words[A + 0x144] = bits(33.f);
    words[A + 0x1ac] = bits(77.f);
    for (uint32_t o : {0x220u, 0x228u})
      words[A + o] = bits(100.f);
    for (uint32_t o : {0x224u, 0x22cu})
      words[A + o] = bits(90.f);
    words[A + 0x20c] = bits(5.f);
    words[A + 0x218] = bits(6.f);
    words[A + 0x214] = bits(20.f);
    words[A + 0x21c] = bits(20.f);
    words[0x8041fc7c] = 0;
    words[0x8041fc80] = bits(.5f);
    words[0x8041fd54] = bits(.05f);
    words[0x8041fd58] = bits(.25f);
    words[0x8041fcb0] = bits(90.f);
    words[0x8041fcc4] = bits(270.f);
    words[0x8041fccc] = bits(180.f);
    words[0x8041fcd4] = bits(.75f);
    words[0x8041e3c8] = 0x7fffffff;
    words[0x802c1210] = 0;
    words[0x802c1214] = 0;
    words[0x802c1218] = bits(30.f);
    words[0x802c121c] = bits(100.f);
    doubles = {{0x802c13c8, bits(.5)},
               {0x802c13d0, bits(3.)},
               {0x802c13d8, bits(0.)},
               {0x802c1460, bits(.75)}};
  }
  bool admit() { return attempts++ != fail; }
  FieldNpcMarioServices services() {
    FieldNpcMarioServices s;
    s.norm.memory.user = this;
    s.norm.memory.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.words.find(a);
      if (i == f.words.end())
        return {};
      const auto value = i->second;
      f.trace.emplace_back('r', a, value);
      if (f.mutateRange && a == A + 0x20c)
        f.words[A + 0x228] = 0;
      return value;
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
      return nativeFieldNpcReciprocalSqrtEstimate(nullptr, q, out);
    };
    s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.halves.find(a);
      if (i == f.halves.end())
        return {};
      f.trace.emplace_back('h', a, i->second);
      return i->second;
    };
    s.pair = [](void *p, uint32_t a) -> std::optional<std::array<uint32_t, 2>> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto x = f.words.find(a), y = f.words.find(a + 4);
      if (x == f.words.end() || y == f.words.end())
        return {};
      f.trace.emplace_back('r', a, x->second);
      f.trace.emplace_back('r', a + 4, y->second);
      return std::array<uint32_t, 2>{x->second, y->second};
    };
    s.request = [](void *p, const FieldNpcMarioRequest &q,
                   FieldNpcMarioResponse &out) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit() || !f.ready)
        return false;
      f.trace.emplace_back('c', q.function, q.args[0]);
      f.calls.push_back(q);
      if (q.function == 0x8005e0c0) {
        out.word = f.secondMario && f.gets ? M2 : M;
        ++f.gets;
        return true;
      }
      if (q.function == 0x8002e210) {
        out.word = f.sequence;
        if (f.mutateSeq) {
          f.words[M2] = 0xffffffff;
        }
        return true;
      }
      if (q.function == 0x801865b8) {
        out.word = f.statuses.at(f.vivians % f.statuses.size());
        ++f.vivians;
        return true;
      }
      if (q.function == 0x801ff028) {
        out.word = f.level;
        if (f.mutateLevel)
          f.words[0x802c1218] = bits(900.f);
        return true;
      }
      if (q.function == 0x80007328) {
        out.scalar = f.revised;
        return true;
      }
      if (q.function == 0x8000703c) {
        assert(q.scalars[0] == 0 && q.scalars[1] == 0);
        out.scalar = 90;
        return true;
      }
      if (q.function == 0x800071f0) {
        out.scalar = f.compared;
        if (f.mutateCompare)
          f.words[A + 0x224] = bits(2.f);
        return true;
      }
      assert(q.function == 0x800440f8 && q.args[0] == A);
      ++f.interrupts;
      out.word = 0xdeadbeef;
      return true;
    };
    return s;
  }
};
bool run(World &f, uint8_t cls, FieldNpcMarioCursor &c) {
  for (unsigned n = 0; n < 4; ++n)
    if (advanceFieldNpcMarioDetection(A, cls, f.services(), c))
      return true;
  return false;
}
void holds(World original, uint8_t cls) {
  original.attempts = 0;
  original.fail = ~0u;
  original.trace.clear();
  original.calls.clear();
  original.gets = original.vivians = original.interrupts = 0;
  World baseline = original;
  FieldNpcMarioCursor b;
  assert(run(baseline, cls, b));
  for (unsigned n = 0; n < baseline.attempts; ++n) {
    World f = original;
    f.fail = n;
    FieldNpcMarioCursor c;
    assert(!advanceFieldNpcMarioDetection(A, cls, f.services(), c));
    assert(run(f, cls, c));
    assert(f.words == baseline.words && f.halves == baseline.halves &&
           f.trace == baseline.trace &&
           f.calls.size() == baseline.calls.size());
    for (unsigned i = 0; i < f.calls.size(); ++i) {
      assert(f.calls[i].function == baseline.calls[i].function);
      for (unsigned k = 0; k < 4; ++k)
        assert(bits(f.calls[i].scalars[k]) ==
               bits(baseline.calls[i].scalars[k]));
    }
    auto before = f.trace;
    assert(run(f, cls, c) && f.trace == before && f.interrupts == 1);
  }
}
void ordinary() {
  World blocked;
  blocked.words[M] = 2;
  holds(blocked, 2);
  FieldNpcMarioCursor c;
  assert(run(blocked, 2, c));
  const std::vector<World::Effect> expected = {{'c', 0x8005e0c0, 0},
                                               {'r', M + 0x8c, bits(10.f)},
                                               {'r', A + 0x8c, 0},
                                               {'r', M + 0x90, 0},
                                               {'r', A + 0x90, 0},
                                               {'r', M + 0x94, 0},
                                               {'r', A + 0x94, 0},
                                               {'r', A + 0x98, 0},
                                               {'r', A + 0x9c, 0},
                                               {'r', A + 0x8c, 0},
                                               {'r', A + 0x90, 0},
                                               {'r', A + 0xa0, 0},
                                               {'r', A + 0x94, 0},
                                               {'r', A + 0x1d4, 0},
                                               {'r', A + 0x144, bits(33.f)},
                                               {'c', 0x8005e0c0, 0},
                                               {'r', M, 2},
                                               {'r', A + 0x140, 0x80},
                                               {'w', A + 0x140, 0x82},
                                               {'c', 0x800440f8, A}};
  assert(blocked.trace == expected && blocked.interrupts == 1);
  for (uint8_t cls : {0, 1, 2, 4, 5, 8}) {
    World f;
    holds(f, cls);
    FieldNpcMarioCursor p;
    assert(run(f, cls, p));
    assert(f.words[A + 0x140] == (cls == 1 || cls == 4 ? 0x81u : 0x80u));
  }
  World unknown;
  FieldNpcMarioCursor u;
  const std::vector<World::Effect> onlyInterrupt{{'c', 0x800440f8, A}};
  assert(run(unknown, 7, u) && unknown.trace == onlyInterrupt);
}
void eligibility() {
  for (uint8_t cls : {1, 2}) {
    for (uint32_t flags : {0x80000000u, 0x02000000u, 2u, 8u}) {
      World f;
      f.words[M] = flags;
      holds(f, cls);
      FieldNpcMarioCursor c;
      assert(run(f, cls, c) &&
             f.words[A + 0x140] == (cls == 2 ? 0x82u : 0x80u) &&
             f.vivians == 0);
    }
    for (unsigned gate = 0; gate < 3; ++gate) {
      World f;
      if (gate == 0)
        f.words[G + 0x164] = 1;
      else if (gate == 1)
        f.sequence = 1;
      else
        f.halves[M + 0x2e] = 25;
      holds(f, cls);
      FieldNpcMarioCursor c;
      assert(run(f, cls, c) &&
             f.words[A + 0x140] == (cls == 2 ? 0x82u : 0x80u));
    }
    for (auto statuses :
         std::vector<std::vector<uint32_t>>{{0}, {2, 1}, {1, 2}}) {
      World f;
      f.statuses = statuses;
      holds(f, cls);
      FieldNpcMarioCursor c;
      assert(run(f, cls, c));
      const bool admitted = statuses[0] == 0 || statuses[1] == 1;
      assert(f.words[A + 0x140] == (cls == 2 ? (admitted ? 0x80u : 0x82u)
                                             : (admitted ? 0x81u : 0x80u)));
      assert(f.vivians == (statuses[0] ? 2u : 1u));
    }
  }
  World retained;
  retained.secondMario = true;
  retained.mutateSeq = true;
  retained.mutateRange = true;
  retained.words[M + 0x8c] = bits(30.f);
  holds(retained, 2);
  FieldNpcMarioCursor r;
  assert(run(retained, 2, r) && retained.words[A + 0x140] == 0x80 &&
         r.admissionMario == M2 &&
         r.range == 100); // captured flags/range; later actor228 is0
}
void geometry() {
  for (uint8_t cls : {1, 2}) {
    World close;
    close.words[M + 0x90] = bits(9.f);
    holds(close, cls);
    FieldNpcMarioCursor c;
    run(close, cls, c);
    assert(close.words[A + 0x140] ==
           0x80); // T2 skips extra heights; T1 rejects height
    for (float compare :
         {45.f, 46.f, std::numeric_limits<float>::quiet_NaN()}) {
      World f;
      f.words[M + 0x8c] = bits(30.f);
      f.compared = compare;
      holds(f, cls);
      FieldNpcMarioCursor p;
      run(f, cls, p);
      bool outside = compare > 45;
      assert(f.words[A + 0x140] == (cls == 2 ? (outside ? 0x82u : 0x80u)
                                             : (outside ? 0x80u : 0x81u)));
    }
    World nan;
    nan.words[M + 0x8c] = bits(std::numeric_limits<float>::quiet_NaN());
    holds(nan, cls);
    FieldNpcMarioCursor n;
    run(nan, cls, n);
    assert(nan.words[A + 0x140] == (cls == 2 ? 0x80u : 0x81u));
  }
  World move;
  move.words[A + 0x1d4] = 2;
  move.words[A + 0x98] = bits(.1f);
  move.words[M + 0x8c] = bits(30.f);
  holds(move, 2);
  FieldNpcMarioCursor m;
  run(move, 2, m);
  assert(m.facing == 77);
  World late;
  late.words[M + 0x8c] = bits(30.f);
  late.compared = 2;
  late.mutateCompare = true;
  holds(late, 1);
  FieldNpcMarioCursor l;
  run(late, 1, l);
  assert(late.words[A + 0x140] == 0x80);
  for (float yaw : {179.f, 180.f, std::numeric_limits<float>::quiet_NaN()}) {
    World f;
    f.revised = yaw;
    holds(f, 1);
    FieldNpcMarioCursor c;
    run(f, 1, c);
    assert(c.facing == (yaw < 180 ? 90 : 270));
  }
}
void kpaAndInner() {
  World kpa;
  kpa.words[A] = 0x08000000;
  kpa.words[M + 0x8c] = bits(30.f);
  kpa.level = 2;
  kpa.mutateLevel = true;
  holds(kpa, 1);
  FieldNpcMarioCursor k;
  run(kpa, 1, k);
  assert(k.distance == 0 &&
         kpa.words[A + 0x140] == 0x81); // copied radius30, not mutated900
  World unsupported;
  unsupported.words[A] = 0x08000000;
  unsupported.level = 4;
  FieldNpcMarioCursor invalid;
  assert(!run(unsupported, 1, invalid) && invalid.phase == 217 &&
         unsupported.interrupts == 0);
  for (uint32_t kind : {0u, 1u, 2u, 99u}) {
    for (float x : {15.f, 16.f, std::numeric_limits<float>::quiet_NaN()}) {
      World f;
      f.words[A + 0x1f8] = kind;
      f.words[A + 0x8c] = bits(x);
      holds(f, 4);
      FieldNpcMarioCursor c;
      run(f, 4, c);
      assert(f.words[A + 0x140] == (x > 15 ? 0x80u : 0x81u));
    }
  }
  for (uint32_t kind : {1u, 2u}) {
    World boundary;
    boundary.words[A + 0x1f8] = kind;
    boundary.words[A + 0x214] = 0x41a00001;
    boundary.words[A + 0x8c] = 0x41700002;
    holds(boundary, 4);
    FieldNpcMarioCursor b;
    run(boundary, 4, b);
    assert(boundary.words[A + 0x140] == (kind == 1 ? 0x80u : 0x81u));
    assert(real(0x41700002) > .75 * double(real(0x41a00001)));
  }
  World z;
  z.words[A + 0x1f8] = 2;
  z.words[A + 0x8c] = bits(std::numeric_limits<float>::quiet_NaN());
  z.words[A + 0x94] = bits(16.f);
  holds(z, 4);
  FieldNpcMarioCursor n;
  run(z, 4, n);
  assert(z.words[A + 0x140] == 0x80);
  World held;
  held.ready = false;
  FieldNpcMarioCursor h;
  assert(!advanceFieldNpcMarioDetection(A, 1, held.services(), h));
  auto before = held.trace;
  assert(!advanceFieldNpcMarioDetection(A, 2, held.services(), h) &&
         held.trace == before);
  held.ready = true;
  assert(run(held, 1, h));
}
} // namespace
int main() {
  ordinary();
  eligibility();
  geometry();
  kpaAndInner();
}
