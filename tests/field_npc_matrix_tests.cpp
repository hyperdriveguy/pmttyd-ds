#include "battle_audience_aux_callback.hpp"
#include "field_npc_matrix.hpp"
#include "npc_presentation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000, C = 0x20000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
struct Fixture {
  using Effect = std::tuple<char, uint32_t, uint32_t>;
  std::map<uint32_t, uint32_t> words;
  std::vector<Effect> trace;
  std::vector<BattleAudienceMatrixRequest> matrices;
  std::vector<FieldNpcMatrixNumber> numbers;
  unsigned attempts = 0, fail = ~0u, angleCalls = 0;
  bool mutate = false;
  Fixture() {
    words = {{A, 0x02000000},          {0x8041fc7c, bits(0)},
             {0x8041fc94, bits(1)},    {0x8041fcb0, bits(90)},
             {0x8041fcc4, bits(270)},  {0x8041fcd8, bits(-1)},
             {0x8041fd7c, 0x3c8efa35}, {C + 0xc, bits(1)},
             {C + 0x14, bits(2)},      {C + 0x18, bits(3)},
             {C + 0x20, bits(4)}};
    for (unsigned o : {0xe0, 0xe4, 0xe8})
      words[A + o] = bits(1);
    for (unsigned o : {0x8c, 0x90, 0x94, 0xa4, 0xa8, 0xac, 0xec, 0xf0, 0xf4,
                       0xf8, 0xfc, 0x100})
      words[A + o] = 0;
    words[A + 0x8c] = bits(10);
    words[A + 0x90] = bits(20);
    words[A + 0x94] = bits(30);
  }
  bool ready() { return attempts++ != fail; }
  FieldNpcMatrixServices services() {
    FieldNpcMatrixServices s;
    s.user = this;
    s.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready() || !f.words.count(a))
        return {};
      const auto w = f.words[a];
      f.trace.emplace_back('r', a, w);
      return w;
    };
    s.number = [](void *u, FieldNpcMatrixNumber &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      if (q.function == 0x8000f2e4)
        q.word = C;
      else if (q.function == 0x8000703c) {
        // Independent numeric provider: discriminate both original argument
        // sets.
        if (!f.angleCalls++) {
          assert(q.args == (std::array<float, 4>{1, 2, 3, 4}));
          q.result = 60;
          if (f.mutate) {
            f.words[C + 0xc] = bits(11);
            f.words[C + 0x14] = bits(12);
            f.words[A + 0x8c] = bits(13);
            f.words[A + 0x94] = bits(14);
          }
        } else {
          assert(q.args == (f.mutate ? std::array<float, 4>{11, 12, 13, 14}
                                     : std::array<float, 4>{1, 2, 10, 30}));
          q.result = 30;
        }
      } else {
        assert(q.function == 0x80007328);
        q.result = npcReviseAngle(q.args[0]);
      }
      f.numbers.push_back(q);
      f.trace.emplace_back('n', q.function, bits(q.result));
      return true;
    };
    s.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      BattleAudienceAuxServices math;
      bindAudienceAuxMathAdapter(math);
      assert(math.matrix(nullptr, q));
      f.matrices.push_back(q);
      f.trace.emplace_back('m', q.function, q.axis);
      if (f.mutate && q.function == 0x8029e2d8) {
        f.words[A + 0xf8] = bits(5);
        f.words[A + 0xfc] = bits(6);
        f.words[A + 0x100] = bits(7);
      }
      if (f.mutate && q.function == 0x8029dff0) {
        f.words[A + 0xf8] = bits(8);
        f.words[A + 0xfc] = bits(9);
        f.words[A + 0x100] = bits(10);
      }
      return true;
    };
    return s;
  }
  bool run(FieldNpcMatrixCursor &c, BattleActMatrix &out, int32_t slot = -1) {
    return advanceFieldNpcMatrix(A, slot, services(), c, out);
  }
};
void simpleTrace() {
  Fixture f;
  FieldNpcMatrixCursor c;
  BattleActMatrix out;
  assert(f.run(c, out));
  const std::vector<Fixture::Effect> expected = {
      {'r', A, 0x02000000},        {'r', 0x8041fc7c, 0},
      {'r', 0x8041fc94, bits(1)},  {'r', A + 0xf0, 0},
      {'n', 0x80007328, 0},        {'n', 0x80007328, 0},
      {'r', 0x8041fcb0, bits(90)}, {'m', 0x8029dd04, 0},
      {'r', 0x8041fc94, bits(1)},  {'r', A + 0xe0, bits(1)},
      {'r', A + 0xe4, bits(1)},    {'r', A + 0xe8, bits(1)},
      {'r', A + 0xf8, 0},          {'r', 0x8041fc7c, 0},
      {'r', A + 0xfc, 0},          {'r', A + 0x100, 0},
      {'r', A + 0xf4, 0},          {'r', 0x8041fc7c, 0},
      {'r', A + 0xec, 0},          {'r', 0x8041fc7c, 0},
      {'r', A + 0xf0, 0},          {'r', 0x8041fc7c, 0},
      {'r', A + 0xf8, 0},          {'r', 0x8041fc7c, 0},
      {'r', A + 0xfc, 0},          {'r', A + 0x100, 0},
      {'r', A + 0x8c, bits(10)},   {'r', A + 0x90, bits(20)},
      {'r', A + 0x94, bits(30)},   {'m', 0x8029e264, 121}};
  assert(f.trace == expected && !c.active);
  assert(out == (BattleActMatrix{1, 0, 0, 10, 0, 1, 0, 20, 0, 0, 1, 30}));
}
void completeVariantsAndHolds() {
  for (unsigned variant = 0; variant < 9; ++variant) {
    auto setup = [&](Fixture &f) {
      if (variant >= 1) {
        f.words[A + 0xe0] = bits(2);
        f.words[A + 0xe4] = bits(3);
        f.words[A + 0xe8] = bits(4);
        f.words[A + 0xf8] = bits(2);
        f.words[A + 0xfc] = bits(3);
        f.words[A + 0x100] = bits(4);
        f.words[A + 0xec] = bits(15);
        f.words[A + 0xf4] = bits(25);
        f.words[A + 0xf0] = bits(180);
      }
      if (variant == 2 || variant == 3) {
        f.words[A] = 0;
        f.mutate = variant == 3;
      }
      if (variant == 4)
        f.words[A] = 0x08000000;
      if (variant >= 5) {
        f.words[A + 0xe0] = bits(1);
        f.words[A + 0xf8] = 0;
        f.words[A + 0xfc] = variant == 5 ? 0 : bits(3);
      }
      if (variant == 7) {
        f.words[A + 0xe4] = bits(1);
        f.words[A + 0xe8] = bits(1);
      }
      if (variant == 8)
        f.words[A + 0xf0] = 0;
    };
    Fixture f;
    setup(f);
    FieldNpcMatrixCursor c;
    BattleActMatrix out;
    assert(f.run(c, out, variant == 4 ? 0 : -1));
    for (unsigned failure = 0; failure < f.attempts; ++failure) {
      Fixture h, foreign;
      setup(h);
      h.fail = failure;
      FieldNpcMatrixCursor held;
      BattleActMatrix actual;
      actual.fill(123);
      assert(!h.run(held, actual, variant == 4 ? 0 : -1));
      assert(held.active);
      for (float x : actual)
        assert(x == 123);
      // Resuming arguments/providers cannot replace the retained invocation.
      h.fail = ~0u;
      assert(advanceFieldNpcMatrix(0xdeadbeef, 9, foreign.services(), held,
                                   actual));
      assert(foreign.trace.empty() && actual == out && h.trace == f.trace &&
             !held.active);
    }
    std::vector<uint32_t> functions;
    for (auto &m : f.matrices)
      functions.push_back(m.function);
    if (variant == 1) {
      assert(functions == (std::vector<uint32_t>{
                              0x8029dd04, 0x8029e2d8, 0x8029e264, 0x8029dff0,
                              0x8029dd64, 0x8029dff0, 0x8029dd64, 0x8029dff0,
                              0x8029dd64, 0x8029e264, 0x8029e264}));
      assert(f.matrices[1].xyz == (std::array<float, 3>{2, 3, -4}));
      assert(f.matrices[2].xyz == (std::array<float, 3>{-2, -3, -4}));
      assert(f.matrices[3].axis == 122 && f.matrices[5].axis == 120 &&
             f.matrices[7].axis == 121);
      for (unsigned i : {4, 6, 8}) {
        assert(f.matrices[i].left == f.matrices[i - 1].output);
        assert(f.matrices[i].right == f.matrices[i - 2].output);
      }
    }
    if (variant == 4) {
      assert(f.numbers.size() == 2);
      assert(out[3] != 10);
    }
    if (variant == 2 || variant == 3) {
      assert(f.numbers.size() == 6 && f.numbers[3].args[0] == 30);
    }
  }
}
void reflectionAndFreshReads() {
  for (float a : {89.999f, 90.f, 270.f, 270.001f, 450.f, -90.f}) {
    Fixture f;
    f.words[A + 0xf0] = bits(a);
    FieldNpcMatrixCursor c;
    BattleActMatrix out;
    assert(f.run(c, out));
    const float revised = npcReviseAngle(a);
    const bool back = revised >= 90 && revised <= 270;
    assert(f.matrices[0].function == 0x8029dd04);
    assert((f.matrices[1].function == 0x8029e2d8) == back);
    if (back)
      assert(f.matrices[1].xyz[2] == -1);
  }
  // Hold between scale component reads: E0 is captured; E8/E4 are fresh.
  Fixture f;
  f.words[A + 0xe0] = bits(2);
  f.fail = 10;
  FieldNpcMatrixCursor c;
  BattleActMatrix out;
  assert(!f.run(c, out));
  assert(c.phase == 28);
  f.words[A + 0xe0] = bits(9);
  f.words[A + 0xe8] = bits(4);
  f.words[A + 0xe4] = bits(3);
  f.fail = ~0u;
  assert(f.run(c, out));
  assert(f.matrices[1].xyz == (std::array<float, 3>{2, 3, 4}));
  // Any unavailable primitive holds repeatedly; numeric/SDK work is required.
  Fixture missing;
  auto s = missing.services();
  s.matrix = nullptr;
  FieldNpcMatrixCursor held;
  out.fill(99);
  assert(!advanceFieldNpcMatrix(A, -1, s, held, out));
  const auto trace = missing.trace;
  assert(!advanceFieldNpcMatrix(A, -1, missing.services(), held, out));
  assert(missing.trace == trace);
  for (float x : out)
    assert(x == 99);
}
void rawSlotAndNonfiniteGates() {
  for (int32_t slot : std::array<int32_t, 5>{-2, 0, 4, int32_t(0x80000000u),
                                             int32_t(0x40000000u)}) {
    Fixture f;
    const uint32_t base = A + 0xa4 + 12 * uint32_t(slot);
    f.words[base] = bits(41);
    f.words[base + 4] = bits(42);
    f.words[base + 8] = bits(43);
    FieldNpcMatrixCursor c;
    BattleActMatrix out;
    assert(f.run(c, out, slot));
    assert(out[3] == 41 && out[7] == 42 && out[11] == 43);
    const auto n = f.trace.size();
    assert(std::get<1>(f.trace[n - 4]) == base);
  }
  // A graph-only SDK provider acknowledges NaN requests to inspect ORIGINAL
  // branch admission; this is not a renderer or SDK numeric parity fixture.
  for (unsigned offset :
       {0xe0u, 0xe4u, 0xe8u, 0xecu, 0xf0u, 0xf4u, 0xf8u, 0xfcu, 0x100u}) {
    Fixture f;
    f.words[A + offset] = 0x7fc01234;
    auto s = f.services();
    s.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &h = *static_cast<Fixture *>(u);
      h.matrices.push_back(q);
      q.output = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
      return true;
    };
    FieldNpcMatrixCursor c;
    BattleActMatrix out;
    assert(advanceFieldNpcMatrix(A, -1, s, c, out));
    unsigned scales = 0, pivots = 0, rotations = 0;
    for (const auto &q : f.matrices) {
      scales += q.function == 0x8029e2d8;
      pivots += q.function == 0x8029e264;
      rotations += q.function == 0x8029dff0;
    }
    assert(scales == (offset <= 0xe8 ? 1u : 0u));
    assert(pivots == (offset >= 0xf8 ? 3u : 1u));
    assert(rotations == (offset >= 0xec && offset <= 0xf4 ? 1u : 0u));
    if (offset == 0xf0)
      assert(f.numbers[0].result == 0 && f.numbers[1].result == 0 && !scales);
  }
}

} // namespace
int main() {
  simpleTrace();
  completeVariantsAndHolds();
  reflectionAndFreshReads();
  rawSlotAndNonfiniteGates();
}
