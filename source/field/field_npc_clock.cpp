#include "field_npc_clock.hpp"
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
int64_t signed64(uint64_t v) {
  return v <= uint64_t(INT64_MAX)
             ? int64_t(v)
             : INT64_MIN + int64_t(v - (uint64_t(1) << 63));
}
uint64_t joined(uint32_t hi, uint32_t lo) { return uint64_t(hi) << 32 | lo; }
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
bool same(const FieldNpcClockServices &a, const FieldNpcClockServices &b) {
  return a.user == b.user && a.word == b.word && a.store == b.store &&
         a.divide == b.divide && a.convert == b.convert;
}
} // namespace
bool nativeFieldNpcClockDivide(void *, uint64_t n, uint32_t d, uint64_t &out) {
  if (!d)
    return false;
  out = uint64_t(signed64(n) / int64_t(d));
  return true;
}
bool nativeFieldNpcClockConvert(void *, uint64_t n, float &out) {
  // Source helper first rounds the signed integer to binary64, then frsp.
  // This intermediate matters near binary32 halfway cases for large integers.
  volatile double intermediate = static_cast<double>(signed64(n));
  out = static_cast<float>(intermediate);
  return true;
}
bool updateFieldNpcClock(uint32_t actor, const FieldNpcClockServices &s,
                         FieldNpcClockCursor &c) {
  if (!s.word || !s.store || uint64_t(actor) + 0x1d8 > 0x100000000ULL)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.actor = actor;
    c.owner = s;
  }
  if (c.actor != actor || !same(c.owner, s))
    return false;
  auto read = [&](uint32_t a, uint32_t &out) {
    const auto v = s.word(s.user, a);
    if (!v)
      return false;
    out = *v;
    return true;
  };
  auto store = [&](uint32_t a, uint32_t v) { return s.store(s.user, a, v); };
  // Each case contains at most one fallible memory/numeric primitive. Retrying
  // never rereads an earlier captured lane or repeats a completed publication.
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(actor + 0x1d4, c.lo))
        return false;
      c.phase = (c.lo & 0x10) ? 1 : 50;
      break;
    case 1:
      if (!read(0x80415fd0, c.gp))
        return false;
      ++c.phase;
      break;
    case 2:
      if (!read(actor + 0x19c, c.otherLo))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!read(c.gp + 0x3c, c.lo))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!read(c.gp + 0x38, c.hi))
        return false;
      ++c.phase;
      break;
    case 5:
      if (!read(actor + 0x198, c.otherHi))
        return false;
      c.number = joined(c.hi, c.lo) - joined(c.otherHi, c.otherLo);
      ++c.phase;
      break;
    case 6:
      if (!store(actor + 0x194, uint32_t(c.number)))
        return false;
      ++c.phase;
      break;
    case 7:
      if (!store(actor + 0x190, uint32_t(c.number >> 32)))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!read(0x800000f8, c.bus))
        return false;
      ++c.phase;
      break;
    case 9:
      if (!read(actor + 0x190, c.hi))
        return false;
      ++c.phase;
      break;
    case 10:
      if (!read(actor + 0x194, c.lo))
        return false;
      c.number = joined(c.hi, c.lo);
      c.divisor = uint32_t((uint64_t(0x10624dd3) * (c.bus >> 2)) >> 32) >> 6;
      ++c.phase;
      break;
    case 11:
      if (!s.divide || !s.divide(s.user, c.number, c.divisor, c.quotient))
        return false;
      c.phase = (!(c.quotient >> 63) && c.quotient > 500) ? 12 : 14;
      break;
    case 12:
      if (!store(actor + 0x194, c.divisor << 4))
        return false;
      ++c.phase;
      break;
    case 13:
      if (!store(actor + 0x190, 0))
        return false;
      ++c.phase;
      break;
    case 14:
      if (!read(actor, c.lo))
        return false;
      c.phase = (c.lo & 0x20000) ? 15 : 21;
      break;
    case 15:
      if (!read(actor + 0x17c, c.lo))
        return false;
      ++c.phase;
      break;
    case 16:
      if (!read(actor + 0x194, c.otherLo))
        return false;
      ++c.phase;
      break;
    case 17:
      if (!read(actor + 0x178, c.hi))
        return false;
      ++c.phase;
      break;
    case 18:
      if (!read(actor + 0x190, c.otherHi))
        return false;
      c.number = joined(c.hi, c.lo) + joined(c.otherHi, c.otherLo);
      ++c.phase;
      break;
    case 19:
      if (!store(actor + 0x17c, uint32_t(c.number)))
        return false;
      ++c.phase;
      break;
    case 20:
      if (!store(actor + 0x178, uint32_t(c.number >> 32)))
        return false;
      c.phase = 23;
      break;
    case 21:
      if (!store(actor + 0x17c, 0))
        return false;
      ++c.phase;
      break;
    case 22:
      if (!store(actor + 0x178, 0))
        return false;
      ++c.phase;
      break;
    case 23:
      if (!read(actor + 0x18c, c.lo))
        return false;
      ++c.phase;
      break;
    case 24:
      if (!read(actor + 0x194, c.otherLo))
        return false;
      ++c.phase;
      break;
    case 25:
      if (!read(actor + 0x188, c.hi))
        return false;
      ++c.phase;
      break;
    case 26:
      if (!read(actor + 0x190, c.otherHi))
        return false;
      c.number = joined(c.hi, c.lo) + joined(c.otherHi, c.otherLo);
      ++c.phase;
      break;
    case 27:
      if (!store(actor + 0x18c, uint32_t(c.number)))
        return false;
      ++c.phase;
      break;
    case 28:
      if (!store(actor + 0x188, uint32_t(c.number >> 32)))
        return false;
      ++c.phase;
      break;
    case 29:
      if (!read(0x80415fd0, c.gp))
        return false;
      ++c.phase;
      break;
    case 30:
      if (!read(c.gp + 0x38, c.hi))
        return false;
      ++c.phase;
      break;
    case 31:
      if (!read(c.gp + 0x3c, c.lo))
        return false;
      ++c.phase;
      break;
    case 32:
      if (!store(actor + 0x19c, c.lo))
        return false;
      ++c.phase;
      break;
    case 33:
      if (!store(actor + 0x198, c.hi))
        return false;
      c.phase = 40;
      break;
    // Repeat the source conversion four times, with fresh bus/source reads
    // after the previous output store. No millisecond rounding or render-frame
    // delta.
    case 40:
      if (!read(0x800000f8, c.bus))
        return false;
      ++c.phase;
      break;
    case 41: {
      constexpr uint32_t src[] = {0x188, 0x178, 0x190, 0x198};
      if (!read(actor + src[c.index], c.hi))
        return false;
      ++c.phase;
      break;
    }
    case 42: {
      constexpr uint32_t src[] = {0x188, 0x178, 0x190, 0x198};
      if (!read(actor + src[c.index] + 4, c.lo))
        return false;
      c.number = joined(c.hi, c.lo) << 3;
      c.divisor = uint32_t((uint64_t(0x431cde83) * (c.bus >> 2)) >> 32) >> 15;
      ++c.phase;
      break;
    }
    case 43:
      if (!s.divide || !s.divide(s.user, c.number, c.divisor, c.quotient))
        return false;
      ++c.phase;
      break;
    case 44:
      if (!s.convert || !s.convert(s.user, c.quotient, c.converted))
        return false;
      ++c.phase;
      break;
    case 45:
      if (!read(0x8041fd50, c.lo))
        return false;
      c.seconds = c.converted / real(c.lo);
      ++c.phase;
      break;
    case 46: {
      constexpr uint32_t dst[] = {0x1a0, 0x180, 0x1a4, 0x1a8};
      if (!store(actor + dst[c.index], bits(c.seconds)))
        return false;
      if (++c.index < 4) {
        c.phase = 40;
        break;
      }
      c = {};
      return true;
    }
    // Stopped state10 clock only refreshes anchor; stale dt remains observable.
    case 50:
      if (!read(0x80415fd0, c.gp))
        return false;
      ++c.phase;
      break;
    case 51:
      if (!read(c.gp + 0x38, c.hi))
        return false;
      ++c.phase;
      break;
    case 52:
      if (!read(c.gp + 0x3c, c.lo))
        return false;
      ++c.phase;
      break;
    case 53:
      if (!store(actor + 0x19c, c.lo))
        return false;
      ++c.phase;
      break;
    case 54:
      if (!store(actor + 0x198, c.hi))
        return false;
      c = {};
      return true;
    default:
      return false;
    }
}
} // namespace ttyd
