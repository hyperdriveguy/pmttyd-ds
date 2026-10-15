#include "recovery_effect.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t bits) {
  float n;
  std::memcpy(&n, &bits, 4);
  return n;
}
uint32_t bits(float n) {
  uint32_t b;
  std::memcpy(&b, &n, 4);
  return b;
}
int32_t signedByte(uint8_t b) { return b < 128 ? b : int32_t(b) - 256; }
int32_t signedHalf(uint16_t b) { return b < 32768 ? b : int32_t(b) - 65536; }
int32_t signedWord(uint32_t b) {
  int32_t n;
  std::memcpy(&n, &b, 4);
  return n;
}
bool same(const EventMemoryServices &a, const EventMemoryServices &b) {
  return a.user == b.user && a.word == b.word && a.half == b.half &&
         a.storeWord == b.storeWord && a.storeHalf == b.storeHalf &&
         a.storeByte == b.storeByte;
}
bool available(const RecoveryEffectServices &s) {
  return s.memory.word && s.memory.half && s.memory.storeWord &&
         s.memory.storeHalf && s.memory.storeByte && s.byte;
}
uint32_t soundFor(int32_t kind, int32_t amount, bool entry) {
  const uint32_t base = 0x802f3b20;
  const auto tier = amount <= 5 ? 0u : amount <= 10 ? 1u : 2u;
  if (entry) {
    if (kind == 0 || kind == 7)
      return base + (tier == 0 ? 0xec : tier == 1 ? 0x104 : 0x118);
    if (kind == 1 || kind == 8)
      return base + (tier == 0 ? 0x12c : tier == 1 ? 0x144 : 0x158);
    if (kind == 4)
      return base + 0x118;
    if (kind == 5)
      return base + 0x158;
  } else {
    if (kind == 0)
      return base + (tier == 0 ? 0x50 : tier == 1 ? 0x68 : 0x80);
    if (kind == 1)
      return base + (tier == 0 ? 0x98 : tier == 1 ? 0xb0 : 0xc8);
    if (kind == 4)
      return base + 0x80;
    if (kind == 5)
      return base + 0xc8;
  }
  return 0;
}
} // namespace
bool recoveryEffectKindSupported(int32_t kind) {
  return kind >= 0 && kind <= 8 && kind != 6;
}
std::optional<uint32_t> resumeRecoveryEffectEntry(
    int32_t kind, int32_t amount, const std::array<float, 3> &position,
    const RecoveryEffectServices &s, RecoveryEntryCursor &c) {
  if (!available(s))
    return {};
  const auto &m = s.memory;
  auto put = [&](uint32_t a, uint32_t v) { m.storeWord(m.user, a, v); };
  auto pf = [&](uint32_t a, float v) { put(a, bits(v)); };
  if (c.active && (!same(c.owner, m) || c.serviceOwner != s.user ||
                   c.byteReader != s.byte || c.kind != kind ||
                   c.requestedAmount != amount ||
                   std::memcmp(c.requestPosition.data(), position.data(),
                               sizeof(float) * 3) != 0))
    return {};
  if (!c.active) {
    if (!recoveryEffectKindSupported(kind))
      return {};
    c.active = true;
    c.owner = m;
    c.serviceOwner = s.user;
    c.byteReader = s.byte;
    c.kind = kind;
    c.amount = amount;
    c.requestedAmount = amount;
    c.position = position;
    c.requestPosition = position;
  }
  for (;;)
    switch (c.phase) {
    case 0: {
      if (!s.effectEntry)
        return {};
      auto r = s.effectEntry(s.user);
      if (!r)
        return {};
      c.effect = *r;
      if (kind == 2 || kind == 3)
        c.amount = 0;
      if (kind == 4 || kind == 5)
        c.amount = 20;
      put(c.effect + 0x14, 0x802f3c00);
      put(c.effect + 8, uint32_t(c.amount) + 1u);
      c.heapBytes = m.word(m.user, c.effect + 8) * 0x44u;
      c.phase = 1;
      break;
    }
    case 1: {
      if (!s.heapAllocate)
        return {};
      auto r = s.heapAllocate(s.user, 3, c.heapBytes);
      if (!r)
        return {};
      c.payload = *r;
      put(c.effect + 0xc, c.payload);
      put(c.effect + 0x10, 0x80142bc4);
      c.phase = 2;
      break;
    }
    case 2: {
      if (!s.mayaAllocate)
        return {};
      auto r = s.mayaAllocate(s.user, 0x80417a44);
      if (!r)
        return {};
      c.maya = *r;
      put(c.payload + 0x38, *r);
      c.maya = m.word(m.user, c.payload + 0x38);
      c.phase = 3;
      break;
    }
    case 3:
      if (!s.mayaPlay || !s.mayaPlay(s.user, c.maya))
        return {};
      c.phase = 4;
      break;
    case 4: {
      if (!s.tick)
        return {};
      m.storeByte(m.user, c.payload, uint8_t(c.kind));
      const auto rootOffset = m.word(m.user, 0x80422e38);
      m.storeByte(m.user, c.payload + 1, uint8_t(c.amount));
      pf(c.payload + 8, c.position[0]);
      pf(c.payload + 0xc, c.position[1]);
      pf(c.payload + 0x10, c.position[2]);
      m.storeHalf(m.user, c.payload + 0x2c, 0);
      put(c.payload + 0x34, s.tick(s.user));
      put(c.payload + 4, 255);
      put(c.payload + 0x40, rootOffset);
      // Source copies the now-stored XYZ before the immediate SFX call.
      for (unsigned i = 0; i < 3; ++i)
        c.position[i] = scalar(m.word(m.user, c.payload + 8 + i * 4));
      const auto soundKind = signedByte(s.byte(s.user, c.payload));
      const auto soundAmount =
          (soundKind == 0 || soundKind == 1 || soundKind == 7 || soundKind == 8)
              ? signedByte(s.byte(s.user, c.payload + 1))
              : 0;
      c.sound = soundFor(soundKind, soundAmount, true);
      c.phase = 5;
      break;
    }
    case 5:
      if (c.sound && (!s.sfx || !s.sfx(s.user, c.sound, c.position)))
        return {};
      c.phase = 6;
      break;
    case 6: {
      const auto zero = m.word(m.user, 0x80422dcc);
      for (int32_t i = 1; i < signedWord(m.word(m.user, c.effect + 8)); ++i) {
        const auto p = c.payload + uint32_t(i) * 0x44u;
        put(p + 8, zero);
        const auto particleY =
            (kind == 4 || kind == 5)
                ? (i >= 10 ? m.word(m.user, 0x80422e3c) : zero)
                : m.word(m.user, 0x80422dcc);
        put(p + 0xc, particleY);
        put(p + 0x10, zero);
        put(p + 0x20, zero);
        put(p + 0x24, zero);
        put(p + 0x28, zero);
        put(p + 0x14, zero);
        put(p + 0x18, zero);
        put(p + 0x1c, zero);
        m.storeHalf(m.user, p + 0x2c, 0);
        m.storeByte(m.user, p + 2, 0);
        put(p + 4, 255);
        put(p + 0x30, zero);
      }
      const auto effect = c.effect;
      c = {};
      return effect;
    }
    default:
      return {};
    }
}
bool resumeRecoveryEffectMain(uint32_t effect, const RecoveryEffectServices &s,
                              RecoveryMainCursor &c) {
  if (!available(s) || !s.binary)
    return false;
  const auto &m = s.memory;
  auto w = [&](uint32_t a) { return m.word(m.user, a); };
  auto f = [&](uint32_t a) { return scalar(w(a)); };
  auto put = [&](uint32_t a, uint32_t n) { m.storeWord(m.user, a, n); };
  auto pf = [&](uint32_t a, float n) { put(a, bits(n)); };
  auto half = [&](uint32_t a) { return signedHalf(m.half(m.user, a)); };
  auto ph = [&](uint32_t a, int32_t n) { m.storeHalf(m.user, a, uint16_t(n)); };
  auto add = [&](float a, float b) {
    return s.binary(s.user, RecoveryBinary::Add, a, b);
  };
  auto sub = [&](float a, float b) {
    return s.binary(s.user, RecoveryBinary::Subtract, a, b);
  };
  auto mul = [&](float a, float b) {
    return s.binary(s.user, RecoveryBinary::Multiply, a, b);
  };
  auto div = [&](float a, float b) {
    return s.binary(s.user, RecoveryBinary::Divide, a, b);
  };
  auto done = [&]() {
    c = {};
    return true;
  };
  auto next = [&]() {
    ++c.index;
    c.particle += 0x44;
    c.phase = 20;
  };
  if (c.active && (!same(c.owner, m) || c.serviceOwner != s.user ||
                   c.byteReader != s.byte || c.effect != effect))
    return false;
  if (!c.active) {
    c.payload = w(effect + 0xc);
    c.kind = signedByte(s.byte(s.user, c.payload));
    if (!recoveryEffectKindSupported(c.kind)) {
      c = {};
      return false;
    }
    c.active = true;
    c.owner = m;
    c.serviceOwner = s.user;
    c.byteReader = s.byte;
    c.effect = effect;
    c.targetY = f(c.payload + 0x40);
    c.maya = w(c.payload + 0x38);
  }
  for (;;)
    switch (c.phase) {
    case 0:
      if (!s.mayaCalc || !s.mayaCalc(s.user, c.maya))
        return false;
      c.phase = 1;
      break;
    case 1: {
      if (!s.tick)
        return false;
      const auto previous = w(c.payload + 0x34), now = s.tick(s.user);
      c.remaining = now > previous ? now - previous : 1;
      c.phase = 10;
      break;
    }
    case 10:
      c.payload = w(effect + 0xc);
      c.amount = signedByte(s.byte(s.user, c.payload + 1));
      for (unsigned i = 0; i < 3; ++i)
        c.position[i] = f(c.payload + 8 + i * 4);
      if (c.kind == 4 || c.kind == 5)
        c.amount = 10;
      ph(c.payload + 0x2c, half(c.payload + 0x2c) + 1);
      if (half(c.payload + 0x2c) > 10)
        c.launched = true;
      c.index = 1;
      c.particle = c.payload + 0x44;
      c.phase = 20;
      break;
    case 20:
      if (c.index >= signedWord(w(effect + 8))) {
        c.phase = 100;
        break;
      }
      switch (signedByte(s.byte(s.user, c.particle + 2))) {
      case 0: {
        if (!c.launched) {
          next();
          break;
        }
        const auto degrees = f(0x80422e24);
        const auto quarter = f(0x80422dc8);
        const auto tau = f(0x80422e20);
        auto theta = mul(degrees, float(c.index - 1));
        theta = div(theta, float(c.amount));
        theta = add(quarter, theta);
        theta = mul(tau, theta);
        c.angle = div(theta, degrees);
        c.phase = 21;
        break;
      }
      case 1:
        if (half(c.particle + 0x2c) < 10) {
          const auto vx = f(c.particle + 0x14);
          const auto damping = f(0x80422e2c);
          const auto one = w(0x80422dd0);
          pf(c.particle + 0x14, mul(vx, damping));
          for (unsigned i = 1; i < 3; ++i)
            pf(c.particle + 0x14 + i * 4,
               mul(f(c.particle + 0x14 + i * 4), damping));
          for (unsigned i = 0; i < 3; ++i) {
            const auto position = f(c.particle + 8 + i * 4);
            const auto velocity = f(c.particle + 0x14 + i * 4);
            pf(c.particle + 8 + i * 4, add(position, velocity));
          }
          put(c.particle + 0x28, one);
          put(c.particle + 0x24, one);
          put(c.particle + 0x20, one);
          ph(c.particle + 0x2c, half(c.particle + 0x2c) + 1);
        } else {
          ph(c.particle + 0x2c, 10);
          m.storeByte(m.user, c.particle + 2,
                      s.byte(s.user, c.particle + 2) + 1);
        }
        next();
        break;
      case 2:
      case 4: {
        const auto n = signedHalf(uint16_t(half(c.particle + 0x2c) - 1));
        ph(c.particle + 0x2c, n);
        if (n <= 0) {
          ph(c.particle + 0x2c, 0);
          m.storeByte(m.user, c.particle + 2,
                      s.byte(s.user, c.particle + 2) + 1);
        }
        next();
        break;
      }
      case 3: {
        const auto counter = half(c.particle + 0x2c);
        if (counter < 40) {
          c.counter = signedHalf(uint16_t(counter + 1));
          c.operands[0] = f(0x80422dcc);
          ph(c.particle + 0x2c, counter + 1);
          c.operands[1] = f(0x80422e24);
          c.phase = 30;
        } else
          c.phase = 35;
        break;
      }
      case 5: {
        const auto counter = half(c.particle + 0x2c);
        if (counter < 16) {
          ph(c.particle + 0x2c, counter + 1);
          c.operands[0] = f(0x80422dcc);
          c.value = f(c.particle + 8);
          c.counter = half(c.particle + 0x2c);
          c.phase = 40;
        } else {
          m.storeByte(m.user, c.particle + 2,
                      s.byte(s.user, c.particle + 2) + 1);
          next();
        }
        break;
      }
      case 6:
        ++c.completed;
        next();
        break;
      default:
        next();
        break;
      }
      break;
    case 21: {
      if (!s.cosine)
        return false;
      auto n = s.cosine(s.user, c.angle);
      if (!n)
        return false;
      const auto speed = f(0x80422e28);
      const auto zero = w(0x80422dcc);
      pf(c.particle + 0x14, mul(speed, *n));
      put(c.particle + 0x18, zero);
      c.phase = 22;
      break;
    }
    case 22: {
      if (!s.sine)
        return false;
      auto n = s.sine(s.user, c.angle);
      if (!n)
        return false;
      pf(c.particle + 0x1c, mul(f(0x80422e28), -*n));
      ph(c.particle + 0x2c, 0);
      m.storeByte(m.user, c.particle + 2, s.byte(s.user, c.particle + 2) + 1);
      next();
      break;
    }
    case 30: {
      if (!s.interpolate)
        return false;
      auto n = s.interpolate(s.user, 11, c.counter, 40, c.operands[0],
                             c.operands[1]);
      if (!n)
        return false;
      c.angle = *n;
      const auto origin = f(0x80422dcc);
      const auto x = f(c.particle + 8);
      const auto z = f(c.particle + 0x10);
      c.operands = {origin, origin, x, z, 0};
      c.phase = 31;
      break;
    }
    case 31: {
      if (!s.distance)
        return false;
      auto n = s.distance(s.user, c.operands[0], c.operands[1], c.operands[2],
                          c.operands[3]);
      if (!n)
        return false;
      c.radius = mul(*n, f(0x80422e30));
      const auto degrees = f(0x80422e24);
      const auto quarter = f(0x80422dc8);
      const auto tau = f(0x80422e20);
      auto theta = mul(degrees, float(c.index - 2));
      theta = div(theta, float(c.amount));
      theta = add(c.angle, theta);
      theta = add(quarter, theta);
      theta = mul(tau, theta);
      c.angle = div(theta, degrees);
      c.phase = 32;
      break;
    }
    case 32: {
      if (!s.cosine)
        return false;
      auto n = s.cosine(s.user, c.angle);
      if (!n)
        return false;
      pf(c.particle + 8, mul(c.radius, *n));
      c.phase = 33;
      break;
    }
    case 33: {
      if (!s.sine)
        return false;
      auto n = s.sine(s.user, c.angle);
      if (!n)
        return false;
      pf(c.particle + 0x10, mul(c.radius, -*n));
      next();
      break;
    }
    case 35: {
      if (!s.random)
        return false;
      auto n = s.random(s.user);
      if (!n)
        return false;
      ph(c.particle + 0x2c, *n % 20);
      m.storeByte(m.user, c.particle + 2, s.byte(s.user, c.particle + 2) + 1);
      next();
      break;
    }
    case 40:
    case 41:
    case 42:
    case 43: {
      if (!s.interpolate)
        return false;
      const auto phase = c.phase;
      const auto end = c.operands[0];
      auto n = s.interpolate(s.user, 1, c.counter, phase == 42 ? 20 : 16,
                             c.value, end);
      if (!n)
        return false;
      if (phase == 43) {
        pf(c.particle + 0x28, *n);
        pf(c.particle + 0x24, *n);
        pf(c.particle + 0x20, *n);
        next();
      } else {
        pf(c.particle + 8 + (phase - 40) * 4, *n);
        ++c.phase;
        c.value = phase == 40   ? f(c.particle + 0xc)
                  : phase == 41 ? f(c.particle + 0x10)
                                : f(0x80422dd0);
        c.operands[0] = phase == 40 ? c.targetY : f(0x80422dcc);
        c.counter = half(c.particle + 0x2c);
      }
      break;
    }
    case 100:
      if ((uint32_t(c.kind) <= 1 || uint32_t(c.kind - 7) <= 1 || c.kind == 4 ||
           c.kind == 5) &&
          c.completed >= c.amount) {
        c.payload = w(effect + 0xc);
        if (c.kind == 7 || c.kind == 8) {
          c.maya = w(c.payload + 0x38);
          c.phase = 103;
          break;
        }
        c.position[1] = sub(c.position[1], f(0x80422df0));
        c.operands[0] = f(0x80422e34);
        c.operands[1] = c.operands[0];
        c.phase = 101;
        break;
      }
      if (--c.remaining) {
        c.phase = 10;
        break;
      }
      c.phase = 110;
      break;
    case 101:
      if (!s.stardust || !s.stardust(s.user, 2, c.position, 2, 30,
                                     c.operands[0], c.operands[1]))
        return false;
      for (unsigned i = 0; i < 3; ++i)
        c.soundPosition[i] = f(c.payload + 8 + i * 4);
      {
        const auto soundKind = signedByte(s.byte(s.user, c.payload));
        const auto soundAmount = (soundKind == 0 || soundKind == 1)
                                     ? signedByte(s.byte(s.user, c.payload + 1))
                                     : 0;
        c.sound = soundFor(soundKind, soundAmount, false);
      }
      c.phase = 102;
      break;
    case 102:
      if (c.sound && (!s.sfx || !s.sfx(s.user, c.sound, c.soundPosition)))
        return false;
      c.maya = w(c.payload + 0x38);
      c.phase = 103;
      break;
    case 103:
      if (!s.mayaDelete || !s.mayaDelete(s.user, c.maya))
        return false;
      c.phase = 104;
      break;
    case 104:
      if (!s.effectDelete || !s.effectDelete(s.user, effect))
        return false;
      return done();
    case 110:
      if (!s.tick)
        return false;
      c.payload = w(effect + 0xc);
      put(c.payload + 0x34, s.tick(s.user));
      for (unsigned i = 0; i < 3; ++i)
        c.position[i] = f(c.payload + 8 + i * 4);
      c.phase = 111;
      break;
    case 111:
    case 113:
    case 115: {
      if (!s.depth)
        return false;
      auto n = s.depth(s.user, c.position);
      if (!n)
        return false;
      c.depth = *n;
      ++c.phase;
      break;
    }
    case 112:
      if (!s.display || !s.display(s.user, 4, 1, 0x801422f4, effect, c.depth))
        return false;
      if (uint32_t(c.kind) <= 1 || c.kind == 7 || c.kind == 8) {
        for (unsigned i = 0; i < 3; ++i)
          c.position[i] = f(c.payload + 8 + i * 4);
        c.phase = 113;
        break;
      }
      if (c.kind == 4 || c.kind == 5) {
        for (unsigned i = 0; i < 3; ++i)
          c.position[i] = f(c.payload + 8 + i * 4);
        c.phase = 115;
        break;
      }
      return done();
    case 114:
      if (!s.display || !s.display(s.user, 4, 2, 0x801420fc, effect, c.depth))
        return false;
      return done();
    case 116:
      if (!s.display || !s.display(s.user, 4, 2, 0x80141c40, effect, c.depth))
        return false;
      return done();
    default:
      return false;
    }
}
} // namespace ttyd
