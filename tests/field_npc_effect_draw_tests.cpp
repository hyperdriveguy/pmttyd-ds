#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include "field_npc_draw.hpp"
#include "npc_collision.hpp"
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
constexpr uint32_t A = 0x10000, C = 0x20000, C2 = 0x21000, R = 0x30000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
struct Fixture {
  using Effect = std::tuple<char, uint32_t, uint32_t>;
  EventScriptAssets source;
  std::map<uint32_t, uint32_t> words;
  std::vector<Effect> trace;
  std::vector<FieldNpcDrawRequest> calls;
  std::vector<BattleAudienceMatrixRequest> math;
  std::array<uint16_t, 4> bounds{0xffff, 0x8000, 23, 42};
  unsigned attempts = 0, fail = ~0u, cameras = 0, matrixCameras = 0;
  uint32_t flags = 0x140, color = 0x10203040;
  uint8_t drawClass = 0;
  bool mutate = false, blur = false;
  Fixture() {
    assert(source.load("romfs/door-data.pmevt"));
    words = {
        {A, 0x02000000},      {A + 0x1d4, 0},        {A + 0x114, 0xffffffff},
        {A + 0x104, 7},       {A + 0x110, 99},       {A + 0x144, bits(90)},
        {C + 0xc, 0},         {C + 0x14, 0},         {C + 0x18, 0},
        {C + 0x20, bits(-1)}, {C + 0x114, bits(45)}, {C2 + 0x114, bits(90)}};
    for (unsigned o : {0xe0, 0xe4, 0xe8})
      words[A + o] = bits(1);
    for (unsigned o : {0xec, 0xf0, 0xf4, 0xf8, 0xfc, 0x100})
      words[A + o] = 0;
    for (unsigned n = 0; n <= 4; ++n) {
      words[A + 0xa4 + 12 * n] = bits(float(10 * n));
      words[A + 0xa8 + 12 * n] = bits(float(20 * n));
      words[A + 0xac + 12 * n] = bits(float(30 * n));
    }
    words[A + 0x8c] = bits(1);
    words[A + 0x90] = bits(2);
    words[A + 0x94] = bits(3);
  }
  uint32_t word(uint32_t a) {
    if (words.count(a))
      return words.at(a);
    uint32_t v;
    assert(source.readWord(a, v));
    return v;
  }
  bool ready() { return attempts++ != fail; }
  FieldNpcDrawServices services() {
    FieldNpcDrawServices s;
    s.transform.user = this;
    s.transform.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return {};
      const auto w = f.word(a);
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
      if (q.function == 0x8000f2e4) {
        q.word = C;
        ++f.matrixCameras;
      } else if (q.function == 0x8000703c)
        q.result = 0; // Explicit camera-angle fixture.
      else {
        assert(q.function == 0x80007328);
        q.result = npcReviseAngle(q.args[0]);
      }
      f.trace.emplace_back('n', q.function, bits(q.result));
      return true;
    };
    s.transform.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.ready())
        return false;
      BattleAudienceAuxServices adapter;
      bindAudienceAuxMathAdapter(adapter);
      assert(adapter.matrix(nullptr, q));
      f.math.push_back(q);
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
        break;
      case 0x80010864:
        r.word = R;
        break;
      case 0x8004069c:
        r.word = f.flags;
        break;
      case 0x8000f2fc:
        assert(q.args[0] == 9);
        r.word = f.cameras++ ? C2 : C;
        break;
      case 0x80040730:
        f.flags |= q.args[1];
        break;
      case 0x80040710:
        f.flags &= ~q.args[1];
        break;
      case 0x800406b4:
        f.color = q.color;
        break;
      case 0x802b6a08:
        r.bounds = f.bounds;
        if (f.mutate)
          f.words[A + 0x110] = 123;
        break;
      case 0x80072f38:
        assert(q.args[0] == (f.mutate ? 123u : 99u) && q.bounds == f.bounds);
        break;
      case 0x80005ce4:
      case 0x802b69d0:
      case 0x8003ed2c:
        break;
      case 0x8003b4d8:
        assert(q.args[0] == f.words[A + 0x104] && q.args[2] >= 1 &&
               q.args[2] <= 3 && q.scalars[0] == 270);
        break;
      default:
        assert(false);
      }
      f.calls.push_back(q);
      f.trace.emplace_back('c', q.function, q.args[0]);
      if (f.mutate) {
        if (q.function == 0x8000f2fc && f.cameras == 1) {
          f.words[C + 0x114] = bits(135);
          f.words[C2 + 0x114] = bits(180);
        }
        if (q.function == 0x8003b4d8) {
          ++f.words[A + 0x104];
          f.flags |= 0x200;
          f.words[0x8041fd34] = bits(3);
        }
        if (q.function == 0x80040710) {
          f.flags |= 0x800;
          f.words[A + 0x104] = 77;
        }
      }
      return true;
    };
    return s;
  }
  bool run(FieldNpcDrawCursor &c, uint32_t callback) {
    blur = callback == 0x8004bc3c;
    return advanceFieldNpcDraw(callback, A, services(), c, 9);
  }
};
void offscreenAndHolds() {
  for (unsigned variant = 0; variant < 6; ++variant) {
    auto setup = [&](Fixture &f) {
      f.drawClass = uint8_t(variant < 4 ? variant : 0);
      f.mutate = variant == 4;
    };
    const uint32_t callback = variant == 5 ? 0x8004c234 : 0x8004bf28;
    Fixture f;
    setup(f);
    FieldNpcDrawCursor c;
    assert(f.run(c, callback));
    assert(f.calls[0].function == 0x80005ce4 &&
           f.calls[1].function == 0x802b69d0);
    const auto n = f.calls.size();
    assert(f.calls[n - 3].function == 0x80005ce4 &&
           f.calls[n - 2].function == 0x802b6a08 &&
           f.calls[n - 1].function == 0x80072f38);
    unsigned draws = 0;
    for (auto &q : f.calls)
      if (q.function == 0x8003b4d8) {
        ++draws;
        assert(q.args[0] != (f.mutate ? 123u : 99u));
      }
    assert(draws == (variant == 3 ? 0u : variant == 5 ? 3u : 1u));
    for (unsigned failure = 0; failure < f.attempts; ++failure) {
      Fixture h, other;
      setup(h);
      h.fail = failure;
      FieldNpcDrawCursor held;
      assert(!h.run(held, callback));
      const auto before = h.trace;
      h.fail = h.attempts;
      assert(!h.run(held, callback) && h.trace == before);
      h.fail = ~0u;
      assert(advanceFieldNpcDraw(0, 0, other.services(), held, 0));
      assert(h.trace == f.trace && other.trace.empty() && !held.active);
      assert(h.calls.back().bounds == f.bounds);
    }
  }
}
void blurAndHolds() {
  for (unsigned variant = 0; variant < 4; ++variant) {
    auto setup = [&](Fixture &f) {
      f.mutate = variant == 1;
      if (variant == 2)
        f.flags = 0x100;
      if (variant == 3)
        f.words[A] = 0;
    };
    Fixture f;
    setup(f);
    FieldNpcDrawCursor c;
    assert(f.run(c, 0x8004bc3c));
    assert(f.matrixCameras == (variant == 3 ? 4u : 0u));
    assert(f.cameras == 2 && f.calls[0].function == 0x80007328 &&
           f.calls[1].function == 0x8004069c);
    unsigned draws = 0, colors = 0, mains = 0;
    std::vector<uint32_t> handles;
    for (auto &q : f.calls) {
      if (q.function == 0x8003ed2c)
        ++mains;
      if (q.function == 0x800406b4) {
        assert(q.color == 0xffffff00u + (28 + 25 * colors));
        ++colors;
      }
      if (q.function == 0x8003b4d8) {
        const unsigned slot = 4 - draws / 3;
        const auto sine = npcSideTrig(f.mutate ? 135.f : 45.f).sine;
        const auto cosine = npcSideTrig(f.mutate ? 180.f : 90.f).cosine;
        // Scale literal changes AFTER each draw; both camera offsets were
        // captured at TWO=2.
        assert(q.matrix[3] == float(10 * slot) + 2 * sine &&
               q.matrix[7] == float(20 * slot) &&
               q.matrix[11] == float(30 * slot) + 2 * (-cosine));
        assert(q.args[2] == draws % 3 + 1 &&
               q.scalars[1] == (f.mutate && draws ? 3 : 2));
        handles.push_back(q.args[0]);
        ++draws;
      }
    }
    assert(mains == 1 && colors == 4 && draws == 12 && f.color == 0xffffff67);
    assert(f.calls[f.calls.size() - 2].function == 0x80040710 &&
           f.calls.back().function == 0x80040730);
    assert(f.flags == (f.mutate ? 0xb40u : variant == 2 ? 0x100u : 0x140u));
    if (f.mutate)
      assert(f.calls.back().args[0] == 77 && handles.front() == 7 &&
             handles.back() == 18);
    for (unsigned failure = 0; failure < f.attempts; ++failure) {
      Fixture h, other;
      setup(h);
      h.fail = failure;
      FieldNpcDrawCursor held;
      assert(!h.run(held, 0x8004bc3c));
      const auto before = h.trace;
      h.fail = h.attempts;
      assert(!h.run(held, 0x8004bc3c) && before == h.trace);
      h.fail = ~0u;
      assert(advanceFieldNpcDraw(0, 0, other.services(), held, 4));
      assert(h.trace == f.trace && h.flags == f.flags && h.color == f.color &&
             h.cameras == 2 && other.trace.empty() && !held.active);
    }
  }
}
void trigReadsAndSpecialValues() {
  for (float yaw : {0.f, -0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f,
                    360.f, -90.f, 450.f, std::numeric_limits<float>::infinity(),
                    -std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::quiet_NaN()})
    for (bool cosine : {false, true}) {
      Fixture f;
      FieldNpcBlurTrigCursor c;
      float value = 123;
      assert(advanceFieldNpcBlurTrig(yaw, cosine, f.services().transform, c,
                                     value));
      const auto expected = npcSideTrig(yaw);
      const float golden = cosine ? expected.cosine : expected.sine;
      assert((std::isnan(value) && std::isnan(golden)) ||
             bits(value) == bits(golden));
      const std::vector<uint32_t> prefix = {0x8041fd10,
                                            0x8041fcc0,
                                            0x8041fd1c,
                                            cosine ? 0x8041fd28u : 0x8041fd14u,
                                            cosine ? 0x8041fd2cu : 0x8041fd18u,
                                            0x8041fc94};
      for (unsigned i = 0; i < prefix.size(); ++i)
        assert(std::get<1>(f.trace[i]) == prefix[i]);
      for (unsigned failure = 0; failure < f.attempts; ++failure) {
        Fixture h;
        h.fail = failure;
        FieldNpcBlurTrigCursor held;
        float actual = 123;
        assert(!advanceFieldNpcBlurTrig(yaw, cosine, h.services().transform,
                                        held, actual) &&
               actual == 123);
        h.fail = ~0u;
        assert(advanceFieldNpcBlurTrig(999, !cosine, {}, held, actual));
        assert(h.trace == f.trace &&
               (std::isnan(actual) ? std::isnan(value)
                                   : bits(actual) == bits(value)));
      }
    }
}
} // namespace
int main() {
  offscreenAndHolds();
  blurAndHolds();
  trigReadsAndSpecialValues();
}
