#include "recovery_effect_draw.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
BattleActMatrix identity() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0}; }
uint32_t bits(float f) {
  uint32_t b;
  std::memcpy(&b, &f, 4);
  return b;
}
struct Fixture {
  static constexpr uint32_t E = 0x1000, P = 0x2000, A = 0x3000, C = 0x4000,
                            GP = 0x6000;
  std::map<uint32_t, uint8_t> memory;
  RecoveryDrawServices s;
  std::vector<SourceIconGXCall> calls;
  std::vector<SourceIconGXQuad> quads;
  std::vector<BattleAudienceMatrixRequest> math;
  std::vector<RecoveryNumberRequest> numbers;
  std::vector<uint32_t> reads;
  std::function<void(uint32_t)> readHook;
  std::function<void(const SourceIconGXCall &)> acknowledged;
  bool distinctCameras = false;
  bool callReady = true, mathReady = true, quadReady = true, numberReady = true;
  uint32_t heldFunction = 0;
  unsigned cameraCalls = 0, formatCalls = 0, lengthCalls = 0;
  std::string text;
  void put(uint32_t a, uint32_t v) {
    for (unsigned i = 0; i < 4; ++i)
      memory[a + i] = uint8_t(v >> (24 - 8 * i));
  }
  uint32_t word(uint32_t a) {
    reads.push_back(a);
    if (readHook)
      readHook(a);
    uint32_t n = 0;
    for (unsigned i = 0; i < 4; ++i)
      n = (n << 8) | memory[a + i];
    return n;
  }
  void xyz(uint32_t a, float x, float y, float z) {
    put(a, bits(x));
    put(a + 4, bits(y));
    put(a + 8, bits(z));
  }
  void matrix(uint32_t a, const BattleActMatrix &m) {
    for (unsigned i = 0; i < 12; ++i)
      put(a + 4 * i, bits(m[i]));
  }
  Fixture() {
    put(E + 0xc, P);
    put(E + 8, 2);
    memory[P] = 0;
    memory[P + 1] = 1;
    xyz(P + 8, 2, 3, 4);
    put(P + 0x38, A);
    matrix(A + 0x14, identity());
    matrix(C + 0x11c, identity());
    put(C + 0x114, 0);
    put(0x80415fd0, GP);
    put(GP + 0x1c, 0xffffffff);
    const std::pair<uint32_t, float> constants[] = {{0x80422dc0, 0.017453292f},
                                                    {0x80422dcc, 0},
                                                    {0x80422dd0, 1},
                                                    {0x80422dd8, 2},
                                                    {0x80422ddc, .5f},
                                                    {0x80422de0, 16},
                                                    {0x80422de4, 10},
                                                    {0x80422dfc, .035f},
                                                    {0x80422e00, 24},
                                                    {0x80422e04, .1f},
                                                    {0x80422e08, -16},
                                                    {0x80422e0c, -6},
                                                    {0x80422e10, 6}};
    for (const auto &item : constants)
      put(item.first, bits(item.second));
    put(0x802f3b68, 0x43300000);
    put(0x802f3b6c, 0x80000000);
    xyz(P + 0x44 + 8, 1, 2, 3);
    xyz(P + 0x44 + 0x20, 2, 3, 4);
    put(P + 0x44 + 4, 0x123456ab);
    for (unsigned i = 0; i < 36; ++i)
      memory[0x804179d8 + i] = uint8_t(10 + i);
    s.user = this;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->word(a);
    };
    s.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.memory[a];
    };
    s.multiply = [](void *, float a, float b) { return a * b; };
    s.matrix = [](void *p, BattleAudienceMatrixRequest &r) {
      auto &f = *static_cast<Fixture *>(p);
      f.math.push_back(r);
      if (!f.mathReady) {
        r.output.fill(999);
        return false;
      }
      auto out = identity();
      switch (r.function) {
      case 0x8029e230:
        out[3] = r.xyz[0];
        out[7] = r.xyz[1];
        out[11] = r.xyz[2];
        break;
      case 0x8029e2b0:
        out[0] = r.xyz[0];
        out[5] = r.xyz[1];
        out[10] = r.xyz[2];
        break;
      case 0x8029dff0:
        assert(r.axis == 121 && r.radians == 0);
        break;
      case 0x8029dd64:
        for (unsigned row = 0; row < 3; ++row)
          for (unsigned col = 0; col < 4; ++col) {
            out[row * 4 + col] = r.left[row * 4] * r.right[col] +
                                 r.left[row * 4 + 1] * r.right[4 + col] +
                                 r.left[row * 4 + 2] * r.right[8 + col] +
                                 (col == 3 ? r.left[row * 4 + 3] : 0);
          }
        break;
      default:
        assert(false);
      }
      r.output = out;
      return true;
    };
    s.call = [](void *p, const SourceIconGXCall &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.callReady && r.function == f.heldFunction)
        return {};
      f.calls.push_back(r);
      if (f.acknowledged)
        f.acknowledged(r);
      switch (r.function) {
      case 0x8000f2fc:
        assert(r.integers[0] == 4);
        ++f.cameraCalls;
        return C + (f.distinctCameras ? (f.cameraCalls - 1) * 0x10000 : 0);
      case 0x8005ed58:
        return 0x9000 + r.integers[0];
      case 0x8026a0b8: {
        assert(r.integers[0] == 0x80422df8);
        ++f.formatCalls;
        int32_t n;
        auto b = r.integers[1];
        std::memcpy(&n, &b, 4);
        f.text = std::to_string(n);
        return 0x8000;
      }
      case 0x8026d6b4:
        assert(r.integers[0] == 0x8000);
        ++f.lengthCalls;
        return uint32_t(f.text.size());
      default:
        return 0;
      }
    };
    s.quad = [](void *p, const SourceIconGXQuad &q) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.quadReady)
        return false;
      f.quads.push_back(q);
      return true;
    };
    s.number = [](void *p, const RecoveryNumberRequest &n) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.numberReady)
        return false;
      f.numbers.push_back(n);
      return true;
    };
  }
  std::vector<uint32_t> textures() {
    std::vector<uint32_t> v;
    for (const auto &c : calls)
      if (c.function == 0x8005ed58)
        v.push_back(c.integers[0]);
    return v;
  }
};
void branches() {
  for (int kind = 0; kind <= 8; ++kind) {
    Fixture f;
    f.memory[Fixture::P] = uint8_t(kind);
    RecoveryDrawCursor c;
    assert(resumeRecoveryEffectDraw(4, Fixture::E, f.s, c) && !c.operation);
    assert(f.quads.size() == (kind == 2 || kind == 3 ? 1u : 2u));
    const std::vector<uint32_t> expected =
        kind == 6 ? std::vector<uint32_t>{}
        : (kind % 2 == 0 && kind < 6) || kind == 7
            ? std::vector<uint32_t>{18, 20}
            : std::vector<uint32_t>{19, 21};
    assert(f.textures() == expected);
    const auto &q = f.quads[0];
    assert(q.positions[0] == (std::array<float, 3>{-16, 16, 0}) &&
           q.positions[2] == (std::array<float, 3>{16, -16, 0}) &&
           q.uv[1][0] == 1);
    if (f.quads.size() == 2) {
      assert(f.quads[1].positions[0] == (std::array<float, 3>{-6, 6, 0}) &&
             f.quads[1].uv[1][0] == 2);
      bool found = false;
      for (const auto &call : f.calls)
        if (call.function == 0x802b8ab8) {
          assert(call.integers[0] == 1 && call.integers[1] == 0x0a0b0cab);
          found = true;
        }
      assert(found);
    }
    assert(f.cameraCalls == (kind == 2 || kind == 3 ? 3u : 4u));
  }
}
void holds() {
  Fixture f;
  RecoveryDrawCursor c;
  f.callReady = false;
  f.heldFunction = 0x802b7880;
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, f.s, c));
  const auto before = f.calls.size(), reads = f.reads.size();
  const auto request = c.request;
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, f.s, c) &&
         f.calls.size() == before && f.reads.size() == reads &&
         c.request.integers == request.integers);
  auto foreign = f.s;
  foreign.memory.user = &c;
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, foreign, c));
  f.callReady = true;
  f.quadReady = false;
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, f.s, c) && f.quads.empty());
  const auto setup = f.calls.size(), loads = f.reads.size();
  const auto first = c.geometry;
  f.put(0x80422e08, bits(888));
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, f.s, c) &&
         f.calls.size() == setup && f.reads.size() == loads &&
         c.geometry.positions == first.positions);
  f.quadReady = true;
  assert(resumeRecoveryEffectDraw(4, Fixture::E, f.s, c) &&
         f.quads.size() == 2 && f.quads[0].positions == first.positions);
  Fixture m;
  RecoveryDrawCursor mc;
  m.mathReady = false;
  assert(!resumeRecoveryEffectDraw(4, Fixture::E, m.s, mc));
  auto operand = mc.math.xyz;
  m.xyz(Fixture::P + 8, 99, 99, 99);
  m.mathReady = true;
  assert(resumeRecoveryEffectDraw(4, Fixture::E, m.s, mc));
  assert(m.math[1].xyz == operand);
}
void liveOwners() {
  Fixture f;
  f.distinctCameras = true;
  for (unsigned i = 0; i < 4; ++i) {
    auto m = identity();
    m[3] = float(100 * (i + 1));
    f.matrix(Fixture::C + i * 0x10000 + 0x11c, m);
    f.put(Fixture::C + i * 0x10000 + 0x114, 0);
  }
  const uint32_t later = 0x9000;
  f.memory[later] = 1;
  f.put(later + 0x38, Fixture::A);
  f.xyz(later + 8, 20, 30, 40);
  f.xyz(later + 0x44 + 8, 1, 2, 3);
  f.xyz(later + 0x44 + 0x20, 2, 3, 4);
  f.put(later + 0x44 + 4, 0xab);
  f.acknowledged = [&](const SourceIconGXCall &call) {
    if (call.function == 0x802b7880 && call.integers[1] == 1)
      f.put(Fixture::E + 0xc, later);
  };
  RecoveryDrawCursor c;
  assert(resumeRecoveryEffectDraw(4, Fixture::E, f.s, c));
  assert(f.textures() ==
         std::vector<uint32_t>({18, 20})); // Original kind stays captured0.
  std::vector<BattleActMatrix> loaded;
  for (const auto &call : f.calls)
    if (call.function == 0x802ba2fc)
      loaded.push_back(call.matrix);
  assert(loaded.size() == 2 && loaded[0][3] == 120 && loaded[0][7] == 50 &&
         loaded[0][11] == 40);
  assert(loaded[1][3] == 403 && loaded[1][7] == 5 && loaded[1][11] == 7);
}
void numbers() {
  for (int kind = 0; kind <= 8; ++kind) {
    Fixture f;
    f.memory[Fixture::P] = uint8_t(kind);
    f.memory[Fixture::P + 1] = uint8_t(-12);
    RecoveryDrawCursor c;
    f.numberReady = false;
    const bool supported = kind < 4 || kind > 6;
    const bool done = resumeRecoveryEffectNumberDraw(4, Fixture::E, f.s, c);
    assert(done == !supported);
    assert(f.formatCalls == 1 && f.lengthCalls == 1 && f.text == "-12");
    if (supported) {
      const auto math = f.math.size(), calls = f.calls.size(),
                 reads = f.reads.size();
      f.memory[Fixture::P + 1] = 55;
      assert(!resumeRecoveryEffectNumberDraw(4, Fixture::E, f.s, c) &&
             f.math.size() == math && f.calls.size() == calls &&
             f.reads.size() == reads);
      f.numberReady = true;
      assert(resumeRecoveryEffectNumberDraw(4, Fixture::E, f.s, c));
      assert(f.numbers.size() == 1 && f.numbers[0].amount == -12 &&
             f.numbers[0].style ==
                 (kind == 1 || kind == 3 || kind == 8 ? 1 : 0) &&
             f.numbers[0].alpha == 255 && f.numbers[0].mode == 1);
      const auto &m = f.numbers[0].matrix;
      assert(std::fabs(m[3] - (2.f - 24.f * .35f)) < 1e-5 &&
             std::fabs(m[7] - 23.f) < 1e-5 && std::fabs(m[11] - 5.f) < 1e-5);
    }
  }
}
} // namespace
int main() {
  branches();
  holds();
  numbers();
  liveOwners();
}
