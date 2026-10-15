#include "animation_facing.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
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
constexpr uint32_t P = 0x10000, C = 0x20000, C2 = 0x21000;
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
  std::vector<BattleAudienceMatrixRequest> matrices;
  std::vector<AnimationFacingCall> calls;
  unsigned attempts = 0, fail = ~0u, cameras = 0, atans = 0;
  bool mutate = false;
  std::array<double, 2> radians{0, 0};
  Fixture() {
    assert(source.load("romfs/door-data.pmevt"));
    words = {{P, 9},
             {P + 0x70, bits(777)},
             {P + 0x74, 0},
             {P + 0x78, bits(-1)},
             {P + 0x7c, bits(888)}};
    for (uint32_t base : {C, C2})
      for (unsigned o : {0x18, 0xc, 0x20, 0x14})
        words[base + o] = bits(float(o));
  }
  bool ready() { return attempts++ != fail; }
  AnimationFacingServices services() {
    AnimationFacingServices s;
    s.user = this;
    s.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
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
    s.store = [](void *u, uint32_t a, uint32_t w) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      f.words[a] = w;
      f.trace.emplace_back('w', a, w);
      return true;
    };
    s.call = [](void *u, AnimationFacingCall &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      if (q.function == 0x8000f2fc) {
        assert(q.argument == 4);
        q.word = f.cameras++ ? C2 : C;
      } else {
        assert(q.function == 0x802725a0 &&
               q.scalars == (std::array<float, 2>{-12, -12}));
        assert(f.atans < 2);
        q.number = f.radians[f.atans++];
        if (f.mutate && f.atans == 1)
          f.words[P + 0x70] = bits(-180);
      }
      f.calls.push_back(q);
      f.trace.emplace_back('c', q.function, q.word);
      return true;
    };
    s.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      BattleAudienceAuxServices adapter;
      bindAudienceAuxMathAdapter(adapter);
      assert(adapter.matrix(nullptr, q));
      f.matrices.push_back(q);
      f.trace.emplace_back('m', q.function, 0);
      return true;
    };
    return s;
  }
  bool run(AnimationFacingCursor &c, bool inner, BattleActMatrix &out,
           float angle = 90, float scale = 2) {
    const BattleActMatrix model{1, 0, 0, 3, 0, 1, 0, 4, 0, 0, 1, 5};
    return inner ? advanceAnimationInnerFacing(P, angle, scale, model,
                                               services(), c, out)
                 : advanceAnimationMainFacing(P, services(), c);
  }
  void finish(AnimationFacingCursor &c, bool inner, BattleActMatrix &out,
              float angle = 90, float scale = 2) {
    unsigned visits = 0;
    while (!run(c, inner, out, angle, scale))
      assert(++visits < 500);
  }
};
void completeGraphsAndHolds() {
  for (bool inner : {false, true})
    for (unsigned variant = 0; variant < 8; ++variant) {
      auto setup = [&](Fixture &f) {
        if (variant == 0)
          f.words[P] = 1;
        if (variant == 1)
          f.words[P + 0x74] = bits(-180);
        if (variant == 2) {
          f.words[P + 0x74] = bits(-1080);
          f.radians[1] = 1;
        }
        if (variant == 3) {
          f.words[P + 0x74] = bits(1080);
          f.radians[1] = -1;
        }
        if (variant == 4) {
          f.words[P] = 0x109;
          f.radians[1] = 1;
        }
        if (variant == 5)
          f.mutate = true;
        if (variant == 6)
          f.radians = {-1, 1};
        if (variant == 7) {
          f.words[P + 0x78] = bits(1);
          f.radians[1] = 1. + std::ldexp(1., -24);
        }
      };
      Fixture f;
      setup(f);
      AnimationFacingCursor c;
      BattleActMatrix out;
      out.fill(123);
      f.finish(c, inner, out);
      assert(!c.active && f.cameras == (variant == 0 ? 0u : inner ? 2u : 1u));
      if (!inner)
        assert(out[0] == 123);
      for (unsigned fail = 0; fail < f.attempts; ++fail) {
        Fixture h, other;
        setup(h);
        h.fail = fail;
        AnimationFacingCursor held;
        BattleActMatrix actual;
        actual.fill(123);
        unsigned visits = 0;
        while (!h.run(held, inner, actual)) {
          assert(++visits < 500);
          if (h.attempts > fail)
            break;
        }
        assert(held.active);
        assert(!inner || actual[0] == 123);
        h.fail = ~0u;
        const auto before = other.trace;
        unsigned resumes = 0;
        while (!(inner ? advanceAnimationInnerFacing(
                             0, 0, 1, {}, other.services(), held, actual)
                       : advanceAnimationMainFacing(0, other.services(), held)))
          assert(++resumes < 500);
        assert(other.trace == before && h.trace == f.trace &&
               h.words == f.words && actual == out && !held.active);
      }
    }
}
void exactMainTraceAndBounds() {
  Fixture f;
  AnimationFacingCursor c;
  BattleActMatrix unused;
  f.finish(c, false, unused);
  using E = Fixture::Effect;
  const std::vector<E> expected = {{'r', P, 9},
                                   {'c', 0x8000f2fc, C},
                                   {'r', C + 0x18, bits(24)},
                                   {'r', C + 0xc, bits(12)},
                                   {'r', C + 0x20, bits(32)},
                                   {'r', C + 0x14, bits(20)},
                                   {'c', 0x802725a0, 0},
                                   {'r', 0x8041fb74, 0x42652ee4},
                                   {'r', 0x8041fb78, bits(360)},
                                   {'r', 0x8041fb7c, bits(-360)},
                                   {'r', 0x8041fb78, bits(360)},
                                   {'r', 0x8041fb28, 0},
                                   {'r', P + 0x74, 0},
                                   {'r', 0x8041fb78, bits(360)},
                                   {'r', 0x8041fb7c, bits(-360)},
                                   {'r', 0x8041fb78, bits(360)},
                                   {'r', 0x8041fb80, bits(90)},
                                   {'r', 0x8041fb70, bits(-90)},
                                   {'r', 0x8041fb24, bits(1)},
                                   {'w', P + 0x78, bits(1)}};
  assert(f.trace == expected && f.matrices.empty());
  for (float d : {90.f, 90.001f, 270.f, 270.001f, -90.f, -90.001f, -270.f,
                  -270.001f, std::numeric_limits<float>::quiet_NaN()}) {
    Fixture h;
    h.words[P + 0x74] = bits(-d);
    AnimationFacingCursor held;
    h.finish(held, false, unused);
    const bool rear = (d > 90 && d <= 270) || (d < -90 && d >= -270);
    assert(h.words[P + 0x78] == bits(rear ? -1.f : 1.f));
  }
}
void innerOrderAndCaptures() {
  Fixture f;
  f.mutate = true;
  f.words[P] |= 0x100;
  f.radians[1] = 0;
  AnimationFacingCursor c;
  BattleActMatrix out;
  f.finish(c, true, out);
  assert(f.words[P + 0x70] == bits(-180) && f.words[P + 0x7c] == bits(180) &&
         f.words[P + 0x74] == bits(180));
  const auto stored = std::find_if(
      f.trace.begin(), f.trace.end(), [](const Fixture::Effect &e) {
        return std::get<0>(e) == 'w' && std::get<1>(e) == P + 0x70;
      });
  const auto queried = std::find_if(
      f.trace.begin(), f.trace.end(),
      [](const Fixture::Effect &e) { return std::get<0>(e) == 'c'; });
  assert(stored < queried);
  std::vector<uint32_t> functions;
  for (auto &q : f.matrices)
    functions.push_back(q.function);
  assert(functions ==
         (std::vector<uint32_t>{0x8029dff0, 0x8029dd64, 0x8029e2b0, 0x8029dd64,
                                0x8029e2b0, 0x8029dd64}));
  assert(f.matrices[2].xyz == (std::array<float, 3>{1, 1, -1}) &&
         f.matrices[4].xyz == (std::array<float, 3>{2, 2, 2}));
  // Snap must use the target captured BEFORE the fresh flags load.
  Fixture snap;
  snap.words[P] = 0x109;
  snap.radians[1] = 1;
  auto s = snap.services();
  auto original = s.word;
  s.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
    auto &h = *static_cast<Fixture *>(u);
    if (!h.ready())
      return {};
    uint32_t w;
    if (h.words.count(a))
      w = h.words.at(a);
    else if (!h.source.readWord(a, w))
      return {};
    h.trace.emplace_back('r', a, w);
    if (a == P && h.cameras == 2)
      h.words[P + 0x7c] = bits(999);
    return w;
  };
  (void)original;
  AnimationFacingCursor held;
  unsigned visits = 0;
  while (!advanceAnimationInnerFacing(P, 90, 1, {}, s, held, out))
    assert(++visits < 500);
  assert(snap.words[P + 0x7c] == bits(999) &&
         snap.words[P + 0x74] == bits(real(0x42652ee4) * float(1.)));
}
void sourceLoopsYield() {
  Fixture f;
  f.radians[0] = std::numeric_limits<double>::infinity();
  AnimationFacingCursor c;
  BattleActMatrix out;
  for (unsigned i = 0; i < 5; ++i)
    assert(!f.run(c, false, out) && c.active && c.heading.active &&
           c.heading.phase == 11);
  assert(f.words[P + 0x78] == bits(-1) && f.cameras == 1 && f.atans == 1);
}
} // namespace
int main() {
  exactMainTraceAndBounds();
  completeGraphsAndHolds();
  innerOrderAndCaptures();
  sourceLoopsYield();
}
