#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include "field_npc_draw.hpp"
#include "npc_presentation.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x10000, P = 0x20000, C = 0x30000, R = 0x40000;
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
struct Fixture {
  using Effect = std::tuple<char, uint32_t, uint32_t>;
  EventScriptAssets source;
  std::map<uint32_t, uint32_t> words;
  std::vector<Effect> trace;
  std::vector<FieldNpcDrawRequest> calls;
  unsigned attempts = 0, fail = ~0u, revise = 0;
  uint8_t drawClass = 0;
  uint32_t name = 0;
  float facing = 0;
  bool overrideFacing = false, mutate = false, overrideAtan = false;
  double atanResult = 0;
  Fixture() {
    assert(source.load("romfs/door-data.pmevt"));
    words = {{A, 0x02000000}, {A + 0x1d4, 0},        {A + 0x114, 0xff7f03ab},
             {A + 0x104, 7},  {A + 0x144, bits(90)}, {P + 0x74, 0},
             {C + 0x18, 0},   {C + 0xc, 0},          {C + 0x20, bits(-1)},
             {C + 0x14, 0}};
    for (unsigned o : {0xe0, 0xe4, 0xe8})
      words[A + o] = bits(1);
    for (unsigned o : {0xec, 0xf0, 0xf4, 0xf8, 0xfc, 0x100})
      words[A + o] = 0;
    words[A + 0x8c] = bits(10);
    words[A + 0x90] = bits(20);
    words[A + 0x94] = bits(30);
    assert(source.readWord(0x803066f8, name));
  }
  uint32_t word(uint32_t a) {
    if (words.count(a))
      return words.at(a);
    uint32_t w;
    assert(source.readWord(a, w));
    return w;
  }
  bool ready() { return attempts++ != fail; }
  int compare(uint32_t a, uint32_t b) {
    for (unsigned i = 0; i < 256; ++i) {
      uint8_t x, y;
      assert(source.readByte(a + i, x) && source.readByte(b + i, y));
      if (x != y || !x)
        return int(x) - int(y);
    }
    assert(false);
    return 0;
  }
  FieldNpcDrawServices services() {
    FieldNpcDrawServices s;
    s.transform.user = this;
    s.transform.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return {};
      uint32_t w;
      if (f.words.count(a))
        w = f.words.at(a);
      else if (!f.source.readWord(a, w))
        return {};
      f.trace.emplace_back('r', a, w);
      return w;
    };
    s.byte = [](void *u, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<Fixture *>(u);
      assert(a == R + 1);
      if (!f.ready())
        return {};
      f.trace.emplace_back('b', a, f.drawClass);
      return f.drawClass;
    };
    s.transform.number = [](void *u, FieldNpcMatrixNumber &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      assert(q.function == 0x80007328);
      q.result = npcReviseAngle(q.args[0]);
      f.trace.emplace_back('n', q.function, bits(q.result));
      return true;
    };
    s.transform.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      BattleAudienceAuxServices math;
      bindAudienceAuxMathAdapter(math);
      assert(math.matrix(nullptr, q));
      f.trace.emplace_back('m', q.function, 0);
      return true;
    };
    s.request = [](void *u, const FieldNpcDrawRequest &q,
                   FieldNpcDrawResponse &r) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      switch (q.function) {
      case 0x80007328:
        r.number = npcReviseAngle(q.scalars[0]);
        if (f.overrideFacing && ++f.revise == 3)
          r.number = f.facing;
        break;
      case 0x80010864:
        r.word = R;
        break;
      case 0x80039c44:
        r.word = f.name;
        break;
      case 0x80039cf8:
        r.word = P;
        break;
      case 0x8000f2fc:
        assert(q.args[0] == 4);
        r.word = C;
        break;
      case 0x802725a0:
        r.number = f.overrideAtan
                       ? f.atanResult
                       : std::atan2(double(q.scalars[0]), double(q.scalars[1]));
        break;
      case 0x8026d464:
        r.word = uint32_t(f.compare(q.args[0], q.args[1]));
        break;
      case 0x80041170:
        assert(q.args[2] == 0);
        f.name = q.args[1];
        break;
      case 0x800406b4:
        assert(q.color == 0x7f3f01ab);
        break;
      case 0x80040730:
        assert(q.args[1] == 0x40);
        break;
      case 0x8003ed2c:
        break;
      case 0x8003b4d8:
        assert(q.args[1] == 0 && q.args[2] >= 1 && q.args[2] <= 3 &&
               q.scalars[0] == 270);
        assert(q.matrix ==
               (BattleActMatrix{1, 0, 0, 10, 0, 1, 0, 20, 0, 0, 1, 30}));
        break;
      default:
        assert(false);
      }
      f.calls.push_back(q);
      f.trace.emplace_back('c', q.function, q.args[0]);
      if (f.mutate) {
        if (q.function == 0x800406b4) {
          f.words[A + 0x104] = 8;
          f.words[A + 0x114] = 0x12345678;
        }
        if (q.function == 0x80040730)
          f.words[A + 0x104] = 9;
        if (q.function == 0x8003ed2c) {
          f.words[A + 0x104] = 11;
          f.words[A + 0x144] = 0;
        }
        if (q.function == 0x8003b4d8) {
          ++f.words[A + 0x104];
          f.words[0x8041fd34] = bits(3);
        }
      }
      return true;
    };
    return s;
  }
  bool run(FieldNpcDrawCursor &c, uint32_t fn = 0x8004c50c) {
    return advanceFieldNpcDraw(fn, A, services(), c);
  }
};
void passOrder() {
  for (unsigned mode = 0; mode < 5; ++mode) {
    Fixture f;
    f.drawClass = uint8_t(mode);
    FieldNpcDrawCursor c;
    assert(f.run(c));
    std::vector<uint32_t> fn;
    for (auto &q : f.calls)
      fn.push_back(q.function);
    const std::vector<uint32_t> prefix{0x80007328, 0x80010864};
    auto expected = prefix;
    if (mode == 0)
      expected.push_back(0x8003ed2c);
    if (mode <= 2)
      expected.push_back(0x8003b4d8);
    assert(fn == expected && !c.active);
    if (mode <= 2)
      assert(f.calls.back().args[2] == mode + 1);
  }
  Fixture f;
  f.words[A + 0x1d4] = 0x00400000;
  f.mutate = true;
  FieldNpcDrawCursor c;
  assert(f.run(c, 0x8004c7e0));
  assert(f.calls.size() == 7 && f.calls[1].color == 0x7f3f01ab &&
         f.calls[1].args[0] == 7 && f.calls[2].args[0] == 8);
  for (unsigned i = 0; i < 3; ++i) {
    const auto &q = f.calls[4 + i];
    assert(q.args[0] == 11 + i && q.args[2] == i + 1 &&
           q.scalars[1] == (i ? 3 : 2));
  }
  assert(f.calls[3].function == 0x8003ed2c && f.calls[3].args[0] == 9);
}
void facingTablesAndHolds() {
  for (unsigned variant = 0; variant < 8; ++variant) {
    auto setup = [&](Fixture &f) {
      f.words[A] |= 0x00800000;
      f.words[A + 0x1d4] = 0x00400000;
      const unsigned index = variant == 0 ? 0 : variant == 1 ? 5 : 13;
      const bool front = variant == 2 || variant == 3;
      f.name = f.word(0x803066f8 + 8 * index + (front ? 4 : 0));
      f.words[P + 0x74] = bits(front ? 0 : -180);
      if (variant == 3)
        f.name = f.word(0x803066f8); // no match in right-facing column
      if (variant == 4) {
        f.overrideFacing = true;
        f.facing = -100;
      }
      if (variant == 5) {
        f.overrideFacing = true;
        f.facing = std::numeric_limits<float>::quiet_NaN();
      }
      if (variant == 6)
        f.drawClass = 2;
    };
    Fixture f;
    setup(f);
    FieldNpcDrawCursor c;
    const auto callback = variant == 7 ? 0x8004c7e0 : 0x8004c50c;
    assert(f.run(c, callback));
    for (unsigned fail = 0; fail < f.attempts; ++fail) {
      Fixture h, foreign;
      setup(h);
      h.fail = fail;
      FieldNpcDrawCursor held;
      assert(!h.run(held, callback) && held.active);
      const auto before = h.trace;
      h.fail = h.attempts;
      assert(!h.run(held, callback));
      assert(h.trace == before); // same primitive held twice
      h.fail = ~0u;
      assert(advanceFieldNpcDraw(0, 0, foreign.services(), held));
      assert(h.trace == f.trace && h.name == f.name && foreign.trace.empty() &&
             !held.active);
    }
    unsigned comparisons = 0, setters = 0, getters = 0, mains = 0;
    for (auto &q : f.calls) {
      comparisons += q.function == 0x8026d464;
      setters += q.function == 0x80041170;
      getters += q.function == 0x80039c44;
      mains += q.function == 0x8003ed2c;
    }
    if (variant == 6)
      assert(!comparisons && !getters && !mains);
    else {
      assert(getters == 2 && mains == 1);
      assert(setters == (variant == 3 || variant == 5 ? 0u : 1u));
      if (variant == 0)
        assert(comparisons == 1);
      if (variant == 1)
        assert(comparisons == 6);
      if (variant == 2)
        assert(comparisons == 14);
    }
  }
  // Source's unusual pair is tested through ORIGINAL data, never rewritten.
  Fixture f;
  const auto p = f.word(0x803066f8 + 5 * 8 + 4);
  uint8_t ch;
  assert(f.source.readByte(p + 3, ch) && ch == 'R' &&
         f.source.readByte(p + 4, ch) && ch == 'R');
}
void facingBoundsAndReturns() {
  for (float angle :
       {90.f, 90.001f, 270.f, 270.001f, -90.f, -90.001f, -270.f, -270.001f}) {
    Fixture f;
    f.words[A] |= 0x00800000;
    f.overrideFacing = true;
    f.facing = angle;
    FieldNpcDrawCursor c;
    assert(f.run(c));
    const bool rear =
        (angle > 90 && angle <= 270) || (angle < -90 && angle >= -270);
    assert((f.name == f.word(0x803066fc)) == rear);
  }
  Fixture unsupported;
  FieldNpcDrawCursor c;
  assert(!unsupported.run(c, 0x8004bf28) && !c.active &&
         unsupported.trace.empty());
  Fixture absent;
  auto s = absent.services();
  s.request = nullptr;
  assert(!advanceFieldNpcDraw(0x8004c50c, A, s, c));
  const auto old = absent.trace;
  assert(!absent.run(c));
  assert(absent.trace == old && c.active);
}
void explicitSuffixAndRounding() {
  Fixture f;
  FieldNpcDrawCursor c;
  assert(f.run(c));
  auto begin = std::find_if(f.trace.begin(), f.trace.end(),
                            [](const Fixture::Effect &e) {
                              return e == Fixture::Effect{'r', A + 0x1d4, 0};
                            });
  assert(begin != f.trace.end());
  const std::vector<Fixture::Effect> expected = {
      {'r', A + 0x1d4, 0},        {'r', A + 0x114, 0xff7f03ab},
      {'c', 0x80010864, 0},       {'b', R + 1, 0},
      {'r', A, 0x02000000},       {'r', A + 0x104, 7},
      {'c', 0x8003ed2c, 7},       {'r', A + 0x104, 7},
      {'r', 0x8041fd34, bits(2)}, {'c', 0x8003b4d8, 7}};
  assert(std::vector<Fixture::Effect>(begin, f.trace.end()) == expected);
  Fixture rounded;
  rounded.words[A] |= 0x00800000;
  rounded.overrideAtan = true;
  rounded.atanResult = 1. + std::ldexp(1., -24);
  assert(rounded.run(c));
  const float factor = real(rounded.word(0x8041fcbc));
  const float expectedAngle = factor * float(rounded.atanResult);
  assert(bits(expectedAngle) !=
         bits(float(double(factor) * rounded.atanResult)));
  const auto at = std::find_if(
      rounded.calls.begin(), rounded.calls.end(),
      [](const FieldNpcDrawRequest &q) { return q.function == 0x802725a0; });
  assert(at != rounded.calls.end() && (at + 1)->function == 0x80007328 &&
         bits((at + 1)->scalars[0]) == bits(expectedAngle));
}

} // namespace
int main() {
  explicitSuffixAndRounding();
  passOrder();
  facingTablesAndHolds();
  facingBoundsAndReturns();
}
