#include "effect_maya_matrix.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float v) {
  uint32_t b;
  std::memcpy(&b, &v, 4);
  return b;
}
float number(uint32_t b) {
  float v;
  std::memcpy(&v, &b, 4);
  return v;
}
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> reads, stores;
  std::vector<uint32_t> sdk;
  std::vector<std::array<float, 3>> vectors;
  std::vector<std::array<float, 24>> products;
  int hold = -1, attempts = 0;
  bool ready = false;
  EffectMayaMatrixServices services;
  Fixture() {
    services.memory.user = this;
    services.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.words[a];
    };
    services.memory.half = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.halves[a];
    };
    services.memory.storeWord = [](void *p, uint32_t a, uint32_t b) {
      auto &f = *static_cast<Fixture *>(p);
      f.stores.push_back(a);
      f.words[a] = b;
    };
    services.user = this;
    services.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.bytes[a];
    };
    services.binary = effectMayaBinary32;
    services.fused = effectMayaFused32;
    services.translate = [](void *p, const std::array<float, 3> &v,
                            EffectMayaMatrix &out) {
      return static_cast<Fixture *>(p)->construct(1, v, out);
    };
    services.scale = [](void *p, const std::array<float, 3> &v,
                        EffectMayaMatrix &out) {
      return static_cast<Fixture *>(p)->construct(2, v, out);
    };
    services.rotate = [](void *p, uint32_t axis, float v,
                         EffectMayaMatrix &out) {
      return static_cast<Fixture *>(p)->construct(axis, {v, 0, 0}, out);
    };
    services.concat = [](void *p, const EffectMayaMatrix &a,
                         const EffectMayaMatrix &b, EffectMayaMatrix &out) {
      auto &f = *static_cast<Fixture *>(p);
      std::array<float, 24> operands{};
      for (unsigned i = 0; i < 12; ++i) {
        operands[i] = a[i];
        operands[i + 12] = b[i];
      }
      f.products.push_back(operands);
      const int stage = static_cast<int>(f.sdk.size());
      ++f.attempts;
      if (stage == f.hold && !f.ready) {
        out.fill(-999);
        return false;
      }
      f.sdk.push_back(3);
      out.fill(100.0f + static_cast<float>(stage));
      return true;
    };
    words[0x200] = 2;
    words[0x204] = 0x1000;
    words[0x104] = bits(5);
    halves[0x1000] = 0;
    halves[0x10b2] = 10;
    words[0x804201a0] = bits(0);
    words[0x804201a4] = 0x322bcc77;
    words[0x804201a8] = bits(3);
    words[0x804201ac] = bits(-2);
    words[0x804201b0] = bits(1);
    words[0x804201b4] = bits(2);
    for (unsigned i = 0; i < 22; ++i) {
      halves[0x1002 + i * 8] = static_cast<uint16_t>(64 * i);
      halves[0x10b4 + i * 8] = static_cast<uint16_t>(64 * (i + 2));
    }
  }
  bool construct(uint32_t op, const std::array<float, 3> &v,
                 EffectMayaMatrix &out) {
    vectors.push_back(v);
    ++attempts;
    if (static_cast<int>(sdk.size()) == hold && !ready) {
      out.fill(-999);
      return false;
    }
    out.fill(static_cast<float>(sdk.size() + 1));
    sdk.push_back(op);
    return true;
  }
  bool run(EffectMayaMatrixCursor &c) {
    return resumeEffectMayaAnimMatrix(0x100, 0x200, 0x300, 0x400, services, c);
  }
};
void noWork() {
  for (uint32_t count : {0u, 1u, 0xffffffffu, 0x80000000u}) {
    Fixture f;
    f.words[0x200] = count;
    f.services.memory.half = nullptr;
    f.services.byte = nullptr;
    f.services.binary = nullptr;
    f.services.fused = nullptr;
    f.services.memory.storeWord = nullptr;
    EffectMayaMatrixCursor c;
    assert(f.run(c));
    assert(!c.active);
    assert(f.reads.size() == 1);
    assert(f.stores.empty());
    assert(f.sdk.empty());
  }
}
void channelsAndComposition() {
  Fixture f;
  EffectMayaMatrixCursor c;
  assert(f.run(c));
  assert(f.sdk.size() == 21);
  assert(f.stores.size() == 13);
  assert(
      (f.sdk == std::vector<uint32_t>{1, 0x78, 0x79, 0x7a, 2, 1, 1, 1, 1, 1, 1,
                                      3, 3,    3,    3,    3, 3, 3, 3, 3, 3}));
  assert(f.vectors[0][0] == 1 && f.vectors[0][1] == 2 && f.vectors[0][2] == 3);
  assert(f.vectors[1][0] == 4 && f.vectors[2][0] == 5 && f.vectors[3][0] == 6);
  assert(f.vectors[4][0] == 7 && f.vectors[5][0] == 10 &&
         f.vectors[6][0] == -10);
  assert(f.vectors[8][0] == 16 && f.vectors[9][0] == -16 &&
         f.vectors[10][0] == 19);
  const std::array<std::array<float, 2>, 10> expected{{{11, 9},
                                                       {111, 5},
                                                       {112, 10},
                                                       {8, 6},
                                                       {114, 4},
                                                       {115, 3},
                                                       {116, 2},
                                                       {117, 7},
                                                       {1, 118},
                                                       {119, 113}}};
  for (unsigned i = 0; i < 10; ++i) {
    assert(f.products[i][0] == expected[i][0]);
    assert(f.products[i][12] == expected[i][1]);
  }
  for (unsigned i = 0; i < 12; ++i) {
    assert(f.stores[i] == 0x300 + 4 * i);
    assert(number(f.words[0x300 + 4 * i]) == 120);
  }
  assert(f.stores.back() == 0x400 && number(f.words[0x400]) == 1);
}
void retainedSdk() {
  for (int hold = 0; hold < 21; ++hold) {
    Fixture f;
    f.hold = hold;
    EffectMayaMatrixCursor c;
    assert(!f.run(c));
    assert(c.active);
    assert(static_cast<int>(f.sdk.size()) == hold);
    assert(f.stores.empty());
    const auto reads = f.reads.size();
    const auto operands = hold < 11
                              ? f.vectors.back()
                              : std::array<float, 3>{f.products.back()[0],
                                                     f.products.back()[12], 0};
    f.words[0x104] = bits(-200);
    f.halves[0x1002] = 32000;
    assert(!f.run(c));
    assert(f.reads.size() == reads);
    assert(f.stores.empty());
    if (hold < 11)
      assert(f.vectors.back() == operands);
    else {
      assert(f.products.back()[0] == operands[0]);
      assert(f.products.back()[12] == operands[1]);
    }
    auto bad = f.services;
    bad.user = nullptr;
    assert(!resumeEffectMayaAnimMatrix(0x100, 0x200, 0x300, 0x400, bad, c));
    assert(
        !resumeEffectMayaAnimMatrix(0x101, 0x200, 0x300, 0x400, f.services, c));
    f.ready = true;
    assert(f.run(c));
    assert(!c.active);
    assert(f.sdk.size() == 21);
    assert(f.stores.size() == 13);
    assert(f.vectors[0][0] == 1);
  }
}
void boundariesAndFlags() {
  for (float frame :
       {-10.0f, 0.0f, 10.0f, 20.0f, std::numeric_limits<float>::quiet_NaN()}) {
    Fixture f;
    f.words[0x104] = bits(frame);
    f.bytes[0x1009] = 0x80;
    f.halves[0x1002] = static_cast<uint16_t>(-128);
    EffectMayaMatrixCursor c;
    assert(f.run(c));
    assert(f.vectors[0][0] == -2);
    // Constant channel still executes all six original quantized loads.
    for (uint32_t address :
         {0x1002u, 0x1004u, 0x1006u, 0x10b4u, 0x10b6u, 0x10b8u}) {
      bool found = false;
      for (auto a : f.reads)
        if (a == address)
          found = true;
      assert(found);
    }
  }
  for (float frame :
       {0.0f, 10.0f, 20.0f, std::numeric_limits<float>::quiet_NaN()}) {
    Fixture f;
    f.words[0x104] = bits(frame);
    EffectMayaMatrixCursor c;
    assert(f.run(c));
    assert(f.vectors[0][0] == (frame == 0 ? 0 : 2));
  }
  Fixture f;
  f.words[0x200] = 3;
  f.halves[0x1164] = 20;
  for (unsigned i = 0; i < 22; ++i)
    f.halves[0x1166 + 8 * i] = 64 * (i + 4);
  f.words[0x104] = bits(15);
  EffectMayaMatrixCursor c;
  assert(f.run(c));
  assert(f.vectors[0][0] == 3);
}
void slopesAndZeroDuration() {
  // At t=.5 over span10, h10=.125,h11=-.125: source tangents
  // +2 and-1 contribute3.75, on top of interpolated value1.
  Fixture f;
  f.halves[0x1006] = 128;
  f.halves[0x10b6] = static_cast<uint16_t>(-64);
  EffectMayaMatrixCursor c;
  assert(f.run(c));
  assert(f.vectors[0][0] == 4.75f);
  Fixture z;
  z.halves[0x10b2] = 0;
  z.words[0x104] = bits(0);
  assert(z.run(c));
  assert(z.vectors[0][0] == 0); // epsilon avoids0/0
  // The scalar clamp uses ordered comparisons and retains a NaN.
  Fixture n;
  n.services.fused = [](void *p, EffectMayaFused op, float a, float b,
                        float d) {
    (void)p;
    (void)op;
    (void)a;
    (void)b;
    (void)d;
    return std::numeric_limits<float>::quiet_NaN();
  };
  assert(n.run(c));
  assert(std::isnan(number(n.words[0x400])));
}
void scalarClampAndFused() {
  for (int16_t value : {-64, 0, 32, 64, 128}) {
    Fixture f;
    f.bytes[0x1000 + 21 * 8 + 9] = 1;
    f.halves[0x1002 + 21 * 8] = static_cast<uint16_t>(value);
    EffectMayaMatrixCursor c;
    assert(f.run(c));
    const float expected = value < 0    ? 0
                           : value > 64 ? 1
                                        : static_cast<float>(value) / 64;
    assert(number(f.words[0x400]) == expected);
  }
  const float a = std::nextafter(1.0f, 2.0f), b = std::nextafter(1.0f, 0.0f);
  assert(effectMayaFused32(nullptr, EffectMayaFused::MultiplySubtract, a, b,
                           1) != 0);
  assert(
      effectMayaFused32(nullptr, EffectMayaFused::NegativeMultiplySubtract, a,
                        b, 1) ==
      -effectMayaFused32(nullptr, EffectMayaFused::MultiplySubtract, a, b, 1));
}
} // namespace
int main() {
  noWork();
  channelsAndComposition();
  retainedSdk();
  boundariesAndFlags();
  slopesAndZeroDuration();
  scalarClampAndFused();
}
