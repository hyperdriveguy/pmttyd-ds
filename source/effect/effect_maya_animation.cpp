#include "effect_maya_animation.hpp"
#include <cstring>

namespace ttyd {
namespace {
uint32_t w(const EffectMayaAnimationServices &s, uint32_t a) {
  return s.memory.word(s.memory.user, a);
}
uint16_t h(const EffectMayaAnimationServices &s, uint32_t a) {
  return s.memory.half(s.memory.user, a);
}
float f(const EffectMayaAnimationServices &s, uint32_t a) {
  uint32_t b = w(s, a);
  float v;
  std::memcpy(&v, &b, 4);
  return v;
}
void sw(const EffectMayaAnimationServices &s, uint32_t a, uint32_t v) {
  s.memory.storeWord(s.memory.user, a, v);
}
void sf(const EffectMayaAnimationServices &s, uint32_t a, float v) {
  uint32_t b;
  std::memcpy(&b, &v, 4);
  sw(s, a, b);
}
bool same(const EffectMayaAnimationServices &a,
          const EffectMayaAnimationServices &b) {
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.word == b.memory.word && a.memory.half == b.memory.half &&
         a.memory.storeWord == b.memory.storeWord &&
         a.memory.storeHalf == b.memory.storeHalf && a.allocate == b.allocate &&
         a.clear == b.clear && a.release == b.release &&
         a.divideUnsigned == b.divideUnsigned &&
         a.convertUnsigned == b.convertUnsigned && a.matrix == b.matrix;
}
bool begin(uint8_t op, uint32_t arg, const EffectMayaAnimationServices &s,
           EffectMayaAnimationCursor &c) {
  if (c.operation)
    return c.operation == op && c.argument == arg && same(s, c.owner);
  if (!s.memory.word || !s.memory.half || !s.memory.storeWord ||
      !s.memory.storeHalf)
    return false;
  c = {};
  c.operation = op;
  c.argument = arg;
  c.owner = s;
  return true;
}
bool finish(EffectMayaAnimationCursor &c) {
  c = {};
  return true;
}
void captureTime(const EffectMayaAnimationServices &s,
                 EffectMayaAnimationCursor &c) {
  c.gp = w(s, 0x80415fd0);
  uint32_t selected = w(s, c.gp + 0x14);
  uint32_t hi = w(s, c.gp + (selected ? 0x38 : 0x40));
  uint32_t lo = w(s, c.gp + (selected ? 0x3c : 0x44));
  c.numerator = (uint64_t(hi) << 32) | lo;
  uint32_t clock = w(s, 0x800000f8) >> 2;
  c.denominator = ((uint64_t(0x10624dd3) * clock) >> 32) >> 6;
}
} // namespace
bool effectMayaDivideUnsigned(void *, uint64_t n, uint64_t d, uint64_t &out) {
  if (!d)
    return false;
  out = n / d;
  return true;
}
bool effectMayaConvertUnsigned(void *, uint64_t n, float &out) {
  // Volatile preserves the original helper's intermediate binary64 rounding.
  volatile double intermediate = static_cast<double>(n);
  out = static_cast<float>(intermediate);
  return true;
}
bool resumeEffectMayaAnimAllocate(uint32_t d,
                                  const EffectMayaAnimationServices &s,
                                  EffectMayaAnimationCursor &c, uint32_t &out) {
  if (!begin(1, d, s, c))
    return false;
  if (c.phase == 0) {
    uint32_t t = w(s, d + 8);
    c.bytes = w(s, t) * 0x34u + 0x18u;
    c.phase = 1;
  }
  if (c.phase == 1) {
    if (!s.allocate || !s.allocate(s.user, 3, c.bytes, c.result))
      return false;
    c.phase = 2;
  }
  if (c.phase == 2) {
    if (!s.clear || !s.clear(s.user, c.result, c.bytes))
      return false;
    c.phase = 3;
  }
  sw(s, c.result + 0x10, d);
  out = c.result;
  return finish(c);
}
bool resumeEffectMayaAnimPlay(uint32_t a, const EffectMayaAnimationServices &s,
                              EffectMayaAnimationCursor &c) {
  if (!begin(2, a, s, c))
    return false;
  if (c.phase == 0) {
    captureTime(s, c);
    c.phase = 1;
  }
  if (!s.divideUnsigned ||
      !s.divideUnsigned(s.user, c.numerator, c.denominator, c.quotient))
    return false;
  sw(s, a + 0xc, uint32_t(c.quotient));
  float zero = f(s, 0x804201a0);
  sw(s, a + 8, uint32_t(c.quotient >> 32));
  sf(s, a + 4, zero);
  uint16_t flags = h(s, a);
  s.memory.storeHalf(s.memory.user, a, flags | 1);
  return finish(c);
}
bool resumeEffectMayaAnimCalculate(uint32_t a,
                                   const EffectMayaAnimationServices &s,
                                   EffectMayaAnimationCursor &c) {
  if (!begin(3, a, s, c))
    return false;
  if (c.phase == 0) {
    uint16_t flags = h(s, a);
    if (!(flags & 1) || (flags & 2))
      return finish(c);
    captureTime(s, c);
    c.phase = 1;
  }
  if (c.phase == 1) {
    if (!s.divideUnsigned ||
        !s.divideUnsigned(s.user, c.numerator, c.denominator, c.quotient))
      return false;
    uint32_t lo = w(s, a + 0xc), hi = w(s, a + 8);
    c.elapsed = c.quotient - ((uint64_t(hi) << 32) | lo);
    c.phase = 2;
  }
  if (c.phase == 2) {
    if (!s.convertUnsigned ||
        !s.convertUnsigned(s.user, c.elapsed, c.converted))
      return false;
    c.phase = 3;
  }
  if (c.phase == 3) {
    uint32_t rate = w(s, c.gp + 4);
    uint32_t biasHi = w(s, 0x802c1f18);
    uint32_t biasLo = w(s, 0x802c1f1c);
    uint64_t biasBits = (uint64_t(biasHi) << 32) | biasLo;
    double bias;
    std::memcpy(&bias, &biasBits, 8);
    float divisor = f(s, 0x8042019c);
    uint64_t rateBits = 0x4330000000000000ull | uint64_t(rate ^ 0x80000000u);
    double rateDouble;
    std::memcpy(&rateDouble, &rateBits, 8);
    volatile float rateFloat = static_cast<float>(rateDouble - bias);
    volatile float product = rateFloat * c.converted;
    float position = product / divisor;
    sf(s, a + 4, position);
    uint32_t descriptor = w(s, a + 0x10);
    float current = f(s, a + 4), end = f(s, descriptor + 4);
    if (current >= end) {
      sf(s, a + 4, end);
      uint16_t flags = h(s, a);
      s.memory.storeHalf(s.memory.user, a, flags | 2);
    }
    c.index = 0;
    c.phase = 4;
  }
  for (;;) {
    if (c.phase == 4) {
      uint32_t descriptor = w(s, a + 0x10);
      c.table = w(s, descriptor + 8);
      int32_t count = static_cast<int32_t>(w(s, c.table));
      if (static_cast<int32_t>(c.index) >= count)
        return finish(c);
      c.channel = w(s, c.table + 4) + c.index * 0x10u;
      c.phase = 5;
    }
    if (!s.matrix || !s.matrix(s.user, a, c.channel, a + 0x14 + c.index * 0x34u,
                               a + 0x44 + c.index * 0x34u))
      return false;
    ++c.index;
    c.phase = 4;
  }
}
bool resumeEffectMayaAnimDelete(uint32_t a,
                                const EffectMayaAnimationServices &s,
                                EffectMayaAnimationCursor &c) {
  if (!begin(4, a, s, c))
    return false;
  if (!s.release || !s.release(s.user, 3, a))
    return false;
  return finish(c);
}
} // namespace ttyd
