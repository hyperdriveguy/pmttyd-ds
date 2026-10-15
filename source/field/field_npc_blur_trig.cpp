#include "field_npc_blur_trig.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
bool advanceFieldNpcBlurTrig(float yaw, bool cosine,
                             const FieldNpcMatrixServices &s,
                             FieldNpcBlurTrigCursor &c, float &result) {
  if (!c.active) {
    c = {};
    c.active = true;
    c.yaw = yaw;
    c.cosine = cosine;
    c.owner = s;
  }
  auto load = [&](uint32_t address, float &out) {
    if (!c.owner.word)
      return false;
    auto w = c.owner.word(c.owner.user, address);
    if (!w)
      return false;
    std::memcpy(&out, &*w, 4);
    return true;
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!load(0x8041fd10, c.tau))
        return false;
      ++c.phase;
      break;
    case 1:
      if (!load(0x8041fcc0, c.divisor))
        return false;
      c.value = c.tau * c.yaw;
      ++c.phase;
      break;
    case 2:
      if (!load(0x8041fd1c, c.pi))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!load(c.cosine ? 0x8041fd28 : 0x8041fd14, c.a))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!load(c.cosine ? 0x8041fd2c : 0x8041fd18, c.b))
        return false;
      c.value /= c.divisor;
      ++c.phase;
      break;
    case 5:
      if (!load(0x8041fc94, c.one))
        return false;
      c.phase = c.value <= c.pi ? 6 : 7;
      break;
    case 6:
      if (!load(0x8041fd20, c.bound))
        return false;
      if (!(c.value <= c.bound)) {
        const float delta = c.value - c.bound;
        c.value = c.bound - delta;
        c.negative = c.cosine;
      }
      c.phase = 9;
      break;
    case 7:
      if (!load(0x8041fd24, c.bound))
        return false;
      if (c.value < c.bound) {
        c.value -= c.pi;
        c.negative = true;
        c.phase = 9;
      } else {
        c.value -= c.bound;
        c.phase = 8;
      }
      break;
    case 8:
      if (!load(0x8041fd20, c.bound))
        return false;
      c.value = c.bound - c.value;
      c.negative = !c.cosine;
      ++c.phase;
      break;
    case 9: {
      const float square = c.value * c.value;
      float out = std::fma(std::fma(c.a, square, -c.b), square, c.one);
      if (!c.cosine)
        out *= c.value;
      if (c.negative)
        out = -out;
      result = out;
      c = {};
      return true;
    }
    default:
      return false;
    }
}
} // namespace ttyd
