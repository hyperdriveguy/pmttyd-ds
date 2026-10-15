#include "field_npc_main_movement.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000;
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
struct World {
  std::map<uint32_t, uint32_t> w;
  std::vector<std::tuple<char, uint32_t, uint32_t>> trace;
  unsigned attempts = 0, fail = ~0u, hits = 0;
  bool contact = false, mutate = false;
  World() {
    for (unsigned o = 0; o < 0x340; o += 4)
      w[A + o] = 0;
    w[A + 0x1d4] = 0x10;
    w[A] = 0x40000000 | 8;
    w[A + 0x1a4] = bits(1);
    w[A + 0x1b4] = bits(4);
    w[A + 0x1bc] = bits(6);
    w[A + 0x8c] = bits(10);
    w[A + 0x90] = bits(20);
    w[A + 0x94] = bits(30);
    w[A + 0x98] = bits(8);
    w[A + 0x9c] = bits(19);
    w[A + 0xa0] = bits(27);
    w[0x8041fc7c] = bits(0);
    w[0x8041fc80] = bits(.5f);
  }
  bool admit() { return attempts++ != fail; }
  FieldNpcMainMoveServices services() {
    FieldNpcMainMoveServices s;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto i = f.w.find(a);
      if (i == f.w.end())
        return {};
      f.trace.emplace_back('r', a, i->second);
      return i->second;
    };
    s.memory.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.w[a] = v;
      f.trace.emplace_back('w', a, v);
      if (f.mutate && a == A + 0x8c)
        f.w[A + 0x94] = bits(100);
      return true;
    };
    s.pair = [](void *p, uint32_t a) -> std::optional<std::array<uint32_t, 2>> {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return {};
      auto x = f.w.find(a), y = f.w.find(a + 4);
      if (x == f.w.end() || y == f.w.end())
        return {};
      f.trace.emplace_back('r', a, x->second);
      f.trace.emplace_back('r', a + 4, y->second);
      return std::array<uint32_t, 2>{x->second, y->second};
    };
    s.request = [](void *p, const FieldNpcMainMoveRequest &q,
                   FieldNpcMainMoveResponse &r) {
      auto &f = *static_cast<World *>(p);
      if (!f.admit())
        return false;
      f.trace.emplace_back('c', q.function, 0);
      assert(q.args[0] == A);
      if (q.function == 0x80048418) {
        ++f.hits;
        assert(q.args[3] == A + 0x1ac);
        if (q.output == FieldNpcMoveOutput::TemporaryXZ) {
          assert(q.args[1] == 0 && q.args[2] == 0);
          r = {f.contact ? 0x90000u : 0u, q.x + q.dx, q.z + q.dz};
          if (f.mutate) {
            f.w[A + 0x8c] = bits(12);
            f.w[A + 0x94] = bits(35);
          }
        } else {
          assert(q.output == FieldNpcMoveOutput::ActorXZ);
          assert(q.args[1] == A + 0x8c && q.args[2] == A + 0x94);
          assert(q.dx == (f.mutate ? 2 : 4) && q.dz == (f.mutate ? 1 : 6));
          f.w[q.args[1]] = bits(40);
          f.w[q.args[2]] = bits(50);
          r = {0xdeadbeef, 999,
               999}; // must not overwrite actual aliases/contact
        }
      } else if (q.function == 0x80048d9c) {
        assert(f.w[A + 0x1b4] == 0 && f.w[A + 0x1bc] == 0);
        if (f.mutate)
          f.w[A] = 0; // side gates use post-gravity fresh flags
      } else {
        assert(q.function == 0x800490cc);
        f.w[A + 0x90] = bits(24);
      }
      return true;
    };
    return s;
  }
};
FieldNpcMoveFrontier run(World &w, FieldNpcMainMoveCursor &c) {
  for (unsigned i = 0; i < 4; ++i) {
    auto r = advanceFieldNpcMainMovement(A, w.services(), c);
    if (r != FieldNpcMoveFrontier::Pending)
      return r;
  }
  assert(false);
  return FieldNpcMoveFrontier::Pending;
}
void holds(World original) {
  World baseline = original;
  FieldNpcMainMoveCursor b;
  const auto result = run(baseline, b);
  const auto count = baseline.attempts;
  for (unsigned n = 0; n < count; ++n) {
    World f = original;
    f.fail = n;
    FieldNpcMainMoveCursor c;
    assert(advanceFieldNpcMainMovement(A, f.services(), c) ==
           FieldNpcMoveFrontier::Pending);
    assert(run(f, c) == result);
    assert(f.w == baseline.w);
    assert(f.trace == baseline.trace);
    assert(c.delta == b.delta);
    auto trace = f.trace;
    assert(run(f, c) == result && f.trace == trace); // frontier stable
  }
}
} // namespace
int main() {
  World direct;
  holds(direct);
  FieldNpcMainMoveCursor c;
  assert(run(direct, c) == FieldNpcMoveFrontier::AutomaticPose);
  assert((c.delta == std::array<float, 3>{6, 1, 9}));
  const std::vector<std::tuple<char, uint32_t, uint32_t>> expected = {
      {'r', A + 0x1d4, 0x10},    {'r', A + 0x1a4, bits(1)},
      {'r', 0x8041fc7c, 0},      {'r', A + 0x1b4, bits(4)},
      {'r', A + 0x1bc, bits(6)}, {'r', A, 0x40000008},
      {'r', A + 0x8c, bits(10)}, {'w', A + 0x8c, bits(14)},
      {'r', A + 0x94, bits(30)}, {'w', A + 0x94, bits(36)},
      {'r', 0x8041fc7c, 0},      {'w', A + 0x1b4, 0},
      {'w', A + 0x1bc, 0},       {'c', 0x80048d9c, 0},
      {'r', A, 0x40000008},      {'r', A + 0x8c, bits(14)},
      {'r', A + 0x90, bits(20)}, {'r', A + 0x98, bits(8)},
      {'r', A + 0x9c, bits(19)}, {'r', A + 0x94, bits(36)},
      {'r', A + 0xa0, bits(27)}};
  assert(direct.trace == expected);
  World two;
  two.w[A] = 0;
  two.contact = true;
  two.mutate = true;
  holds(two);
  FieldNpcMainMoveCursor t;
  run(two, t);
  assert(two.hits == 2 && two.w[A + 0x2fc] == 0x90000);
  assert(real(two.w[A + 0x8c]) == 40 && real(two.w[A + 0x94]) == 50);
  assert(t.delta[1] == 5); // side child runs after gravity changes the flags
  World one;
  one.w[A] = 8;
  holds(one);
  FieldNpcMainMoveCursor o;
  run(one, o);
  assert(one.hits == 1 && real(one.w[A + 0x8c]) == 14);
  World special;
  special.w[A + 0x1d4] |= 0x100000;
  special.w[A] |= 0x08000000;
  holds(special);
  FieldNpcMainMoveCursor s;
  run(special, s);
  assert(real(special.w[A + 0x8c]) == 12 && real(special.w[A + 0x94]) == 30);
  World bypass;
  bypass.w[A + 0x1d4] |= 0x10100000;
  holds(bypass);
  FieldNpcMainMoveCursor bp;
  run(bypass, bp);
  assert(real(bypass.w[A + 0x8c]) == 14 && real(bypass.w[A + 0x94]) == 36);
  for (uint32_t flag : {8u, 0x20u, 0x4000u}) {
    World gated;
    gated.w[A] = 0x40000000 | flag;
    holds(gated);
    FieldNpcMainMoveCursor g;
    run(gated, g);
    assert(g.delta[1] == 1);
    for (auto effect : gated.trace)
      assert(std::get<0>(effect) != 'c' || std::get<1>(effect) != 0x800490cc);
  }
  World order;
  order.mutate = true;
  holds(order);
  FieldNpcMainMoveCursor d;
  run(order, d);
  assert(real(order.w[A + 0x94]) == 106); // X publication precedes fresh Z load
  for (float dt : {0.f, -1.f, std::numeric_limits<float>::quiet_NaN()}) {
    World paused;
    paused.w[A] = 0;
    paused.w[A + 0x1a4] = bits(dt);
    holds(paused);
    FieldNpcMainMoveCursor p;
    run(paused, p);
    assert(paused.hits == 0 && paused.w[A + 0x1b4] == bits(4));
    assert(p.delta[1] == 5); // side response + subtraction still run
  }
  World skipped;
  skipped.w[A + 0x1d4] = 0;
  holds(skipped);
  FieldNpcMainMoveCursor k;
  assert(run(skipped, k) == FieldNpcMoveFrontier::Detection);
  assert(skipped.trace.size() == 1);
  World missing;
  missing.w[A] = 0;
  auto svc = missing.services();
  svc.request = nullptr;
  FieldNpcMainMoveCursor m;
  assert(advanceFieldNpcMainMovement(A, svc, m) ==
         FieldNpcMoveFrontier::Pending);
  assert(m.phase == 15 && missing.w[A + 0x2fc] == 0);
  auto before = missing.trace;
  assert(advanceFieldNpcMainMovement(A + 0x340, svc, m) ==
         FieldNpcMoveFrontier::Pending);
  assert(missing.trace == before);
}
