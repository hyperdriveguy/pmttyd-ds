#include "effect_maya_matrix.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float fromBits(uint32_t b) {
  float v;
  std::memcpy(&v, &b, 4);
  return v;
}
uint32_t bits(float v) {
  uint32_t b;
  std::memcpy(&b, &v, 4);
  return b;
}
bool same(const EffectMayaMatrixServices &a,
          const EffectMayaMatrixServices &b) {
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.word == b.memory.word && a.memory.half == b.memory.half &&
         a.memory.storeWord == b.memory.storeWord && a.byte == b.byte &&
         a.binary == b.binary && a.fused == b.fused &&
         a.translate == b.translate && a.rotate == b.rotate &&
         a.scale == b.scale && a.concat == b.concat;
}
} // namespace
float effectMayaBinary32(void *, EffectMayaBinary op, float a, float b) {
  volatile float result = 0;
  switch (op) {
  case EffectMayaBinary::Add:
    result = a + b;
    break;
  case EffectMayaBinary::Subtract:
    result = a - b;
    break;
  case EffectMayaBinary::Multiply:
    result = a * b;
    break;
  case EffectMayaBinary::Divide:
    result = a / b;
    break;
  }
  return result;
}
float effectMayaFused32(void *, EffectMayaFused op, float a, float b, float c) {
  if (op == EffectMayaFused::MultiplyAdd)
    return std::fma(a, b, c);
  const float v = std::fma(a, b, -c);
  return op == EffectMayaFused::MultiplySubtract ? v : -v;
}
bool resumeEffectMayaAnimMatrix(uint32_t animation, uint32_t channel,
                                uint32_t matrixOut, uint32_t scalarOut,
                                const EffectMayaMatrixServices &s,
                                EffectMayaMatrixCursor &c) {
  if (c.active) {
    if (c.animation != animation || c.channel != channel ||
        c.matrixOut != matrixOut || c.scalarOut != scalarOut ||
        !same(s, c.owner))
      return false;
  } else {
    if (!s.memory.word)
      return false;
    c = {};
    c.active = true;
    c.animation = animation;
    c.channel = channel;
    c.matrixOut = matrixOut;
    c.scalarOut = scalarOut;
    c.owner = s;
    c.count = static_cast<int32_t>(s.memory.word(s.memory.user, channel));
    if (c.count <= 1) {
      c = {};
      return true;
    }
    c.phase = 1;
  }
  auto w = [&](uint32_t a) { return s.memory.word(s.memory.user, a); };
  auto h = [&](uint32_t a) {
    return static_cast<int16_t>(s.memory.half(s.memory.user, a));
  };
  auto f = [&](uint32_t a) { return fromBits(w(a)); };
  auto binary = [&](EffectMayaBinary op, float a, float b) {
    return s.binary(s.user, op, a, b);
  };
  auto add = [&](float a, float b) {
    return binary(EffectMayaBinary::Add, a, b);
  };
  auto sub = [&](float a, float b) {
    return binary(EffectMayaBinary::Subtract, a, b);
  };
  auto mul = [&](float a, float b) {
    return binary(EffectMayaBinary::Multiply, a, b);
  };
  auto fused = [&](EffectMayaFused op, float a, float b, float d) {
    return s.fused(s.user, op, a, b, d);
  };
  if (c.phase == 1) {
    if (!s.memory.half || !s.byte || !s.binary || !s.fused)
      return false;
    uint32_t previous = 0, next = 0;
    float position = 0;
    for (int32_t i = 1; i < c.count; ++i) {
      const auto table = w(channel + 4);
      previous = table + static_cast<uint32_t>(i - 1) * 0xb2;
      next = table + static_cast<uint32_t>(i) * 0xb2;
      const float upper = static_cast<float>(h(next));
      position = f(animation + 4);
      if (position <= upper)
        break;
      if (i == c.count - 1) {
        position = upper;
        break;
      }
    }
    for (uint32_t i = 0; i < 22; ++i) {
      const auto a = previous + i * 8, b = next + i * 8;
      // GQR5=06070607: signed16 load type7, scale exponent6.
      const auto decode = [&](uint32_t p) {
        return static_cast<float>(h(p)) * 0.015625f;
      };
      const float v0 = decode(a + 2);
      (void)decode(a + 4);
      const float tangent0 = decode(a + 6);
      const float v1 = decode(b + 2);
      const float tangent1 = decode(b + 4);
      (void)decode(b + 6);
      if (s.byte(s.memory.user, a + 9) != 0) {
        c.values[i] = v0;
        continue;
      }
      const int32_t t0 = h(previous), t1 = h(next);
      const float duration = static_cast<float>(t1 - t0);
      const float zero = f(0x804201a0);
      const float offset = sub(position, static_cast<float>(t0));
      const float divisor = duration == zero ? f(0x804201a4) : duration;
      const float t = binary(EffectMayaBinary::Divide, offset, divisor);
      const float two = f(0x804201b4), three = f(0x804201a8);
      const float one = f(0x804201b0), negativeTwo = f(0x804201ac);
      const float tSquared = mul(t, t), tCubed = mul(tSquared, t);
      const float tripleSquared = mul(three, tSquared);
      float first = fused(EffectMayaFused::NegativeMultiplySubtract, two,
                          tSquared, tCubed);
      float second =
          fused(EffectMayaFused::MultiplySubtract, two, tCubed, tripleSquared);
      const float fourth = sub(tCubed, tSquared);
      first = add(t, first);
      second = add(one, second);
      const float third = fused(EffectMayaFused::MultiplyAdd, negativeTwo,
                                tCubed, tripleSquared);
      first = mul(duration, first);
      const float fourthScaled = mul(duration, fourth);
      float result = mul(tangent0, first);
      result = fused(EffectMayaFused::MultiplyAdd, v0, second, result);
      result = fused(EffectMayaFused::MultiplyAdd, v1, third, result);
      c.values[i] =
          fused(EffectMayaFused::MultiplyAdd, tangent1, fourthScaled, result);
    }
    c.phase = 2;
  }
  // Stack matrices retain original SDK operation order; no host trig fallback.
  while (c.phase >= 2 && c.phase < 23) {
    const uint32_t stage = c.phase - 2;
    EffectMayaMatrix output{};
    bool done = false;
    uint32_t destination = stage;
    if (stage <= 10) {
      if (stage >= 1 && stage <= 3) {
        if (!s.rotate)
          return false;
        done = s.rotate(s.user, 0x78 + stage - 1, c.values[stage + 2], output);
      } else {
        uint32_t first = 0;
        bool negative = false;
        switch (stage) {
        case 0:
          first = 0;
          break;
        case 4:
          first = 6;
          break;
        case 5:
          first = 9;
          break;
        case 6:
          first = 9;
          negative = true;
          break;
        case 7:
          first = 12;
          break;
        case 8:
          first = 15;
          break;
        case 9:
          first = 15;
          negative = true;
          break;
        case 10:
          first = 18;
          break;
        }
        std::array<float, 3> xyz{c.values[first], c.values[first + 1],
                                 c.values[first + 2]};
        if (negative)
          for (auto &v : xyz)
            v = -v;
        const auto service = stage == 4 ? s.scale : s.translate;
        if (!service)
          return false;
        done = service(s.user, xyz, output);
      }
    } else {
      if (!s.concat)
        return false;
      static constexpr uint8_t operations[10][3] = {
          {10, 8, 11}, {11, 4, 11}, {11, 9, 4},  {7, 5, 11},  {11, 3, 11},
          {11, 2, 11}, {11, 1, 11}, {11, 6, 12}, {0, 12, 11}, {11, 4, 13}};
      const auto &op = operations[stage - 11];
      destination = op[2];
      done = s.concat(s.user, c.matrices[op[0]], c.matrices[op[1]], output);
    }
    if (!done)
      return false;
    c.matrices[destination] = output;
    ++c.phase;
  }
  if (!s.memory.storeWord)
    return false;
  if (c.phase == 23) {
    for (uint32_t i = 0; i < 12; ++i)
      s.memory.storeWord(s.memory.user, matrixOut + i * 4,
                         bits(c.matrices[13][i]));
    c.phase = 24;
  }
  float scalar = c.values[21];
  const float one = f(0x804201b0);
  if (scalar > one)
    scalar = one;
  else {
    const float zero = f(0x804201a0);
    if (scalar < zero)
      scalar = zero;
  }
  s.memory.storeWord(s.memory.user, scalarOut, bits(scalar));
  c = {};
  return true;
}
} // namespace ttyd
