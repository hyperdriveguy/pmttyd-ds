#include "field_npc_main_display.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
float single(float f) {
  volatile float v = f;
  return v;
}
} // namespace
FieldNpcDisplayFrontier advanceFieldNpcMainDisplay(
    uint32_t actor, const FieldNpcDisplayServices &s, FieldNpcDisplayCursor &c) {
  const auto &m = s.memory;
  if (!m.word)
    return FieldNpcDisplayFrontier::Pending;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
  }
  if (c.actor != actor || c.owner.memory.user != m.user ||
      c.owner.memory.word != m.word || c.owner.byte != s.byte ||
      c.owner.half != s.half || c.owner.request != s.request)
    return FieldNpcDisplayFrontier::Pending;
  auto read = [&](uint32_t a, uint32_t &v) {
    auto w = m.word(m.user, a);
    if (!w)
      return false;
    v = *w;
    return true;
  };
  auto load = [&](uint32_t a, float &v) {
    uint32_t w;
    if (!read(a, w))
      return false;
    v = real(w);
    return true;
  };
  auto request = [&] {
    return s.request && s.request(m.user, c.call, c.response);
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(actor, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.value & 0x800 ? 102 : 1;
      break;
    case 1:
      c.call = {0x8000f2fc, {4}};
      ++c.phase;
      break;
    case 2:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.call = {0x8029e6a4, {c.response.word + 0x11c, actor + 0x8c}};
      ++c.phase;
      break;
    case 3:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.projected = c.response.point;
      c.call = {0x8000f2fc, {4}};
      ++c.phase;
      break;
    case 4:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.call = {0x8029eb08, {c.response.word + 0x15c}, c.projected};
      ++c.phase;
      break;
    case 5:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.projected = c.response.point;
      ++c.phase;
      break;
    case 6:
      if (!load(0x8041fcd8, c.low))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.projected[2] > c.low ? 7 : 12;
      break;
    case 7:
      if (!load(0x8041fc7c, c.zero))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.projected[2] < c.zero ? 8 : 12;
      break;
    case 8:
      if (!load(0x8041fd5c, c.low))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.projected[0] >= c.low ? 9 : 12;
      break;
    case 9:
      if (!load(0x8041fd34, c.high))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.projected[0] <= c.high &&
                        c.projected[1] >= c.low && c.projected[1] <= c.high
                    ? 20 : 12;
      break;
    case 12:
      if (!read(actor, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.value & 0x40 ? 20 : 102;
      break;
    case 20: {
      auto v = s.byte ? s.byte(m.user, actor + 0x114 + c.colorIndex)
                      : std::nullopt;
      if (!v)
        return FieldNpcDisplayFrontier::Pending;
      if (*v != 0xff)
        c.phase = 22;
      else if (++c.colorIndex == 4)
        c.phase = 21;
      break;
    }
    case 21:
    case 22:
      if (!read(actor + 0x104, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.call = {c.phase == 21 ? 0x80040710u : 0x80040730u, {c.value, 0x40}};
      c.phase = c.phase == 21 ? 23 : 24;
      break;
    case 23:
    case 24:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.phase == 23 ? 30 : 25;
      break;
    case 25:
      if (!read(actor + 0x114, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.call = {0x800406b4};
      c.call.color = c.value;
      ++c.phase;
      break;
    case 26:
      if (!read(actor + 0x104, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.call.args[0] = c.value;
      ++c.phase;
      break;
    case 27:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.phase = 30;
      break;
    case 30:
      if (!load(0x8041fd60, c.scale))
        return FieldNpcDisplayFrontier::Pending;
      ++c.phase;
      break;
    case 31:
      if (!load(0x8041fc7c, c.zero))
        return FieldNpcDisplayFrontier::Pending;
      c.depth = std::fma(c.scale, c.projected[2], c.scale);
      if (c.depth < c.zero) {
        c.depth = c.zero;
        c.phase = 33;
      } else
        c.phase = 32;
      break;
    case 32:
      if (!load(0x8041fd64, c.high))
        return FieldNpcDisplayFrontier::Pending;
      if (c.depth > c.high)
        c.depth = c.high;
      ++c.phase;
      break;
    case 33:
      if (!read(actor, c.value))
        return FieldNpcDisplayFrontier::Pending;
      ++c.phase;
      break;
    case 34:
      if (!load(0x8041fcd8, c.scale))
        return FieldNpcDisplayFrontier::Pending;
      c.depth = single(c.depth * c.scale);
      c.phase = c.value & 0x80 ? 101 : 35;
      break;
    case 35: {
      auto v = s.half ? s.half(m.user, actor + 0x320) : std::nullopt;
      if (!v)
        return FieldNpcDisplayFrontier::Pending;
      c.phase = *v & 2 ? 36 : 37;
      break;
    }
    case 36:
      if (!read(actor + 0x1d4, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.phase = c.value & 0x10000 ? 37 : 101;
      break;
    case 37: {
      auto v = s.byte ? s.byte(m.user, actor + 0x117) : std::nullopt;
      if (!v)
        return FieldNpcDisplayFrontier::Pending;
      c.callback = *v == 0xff ? 0x8004c50c : 0x8004c7e0;
      c.drawClass = *v == 0xff ? 0 : 2;
      ++c.phase;
      break;
    }
    case 38:
      if (!read(actor + 0x1f4, c.value))
        return FieldNpcDisplayFrontier::Pending;
      c.call = {0x80010bc4, {c.value, c.drawClass, c.callback, actor}};
      c.call.depth = c.depth;
      ++c.phase;
      break;
    case 39:
      if (!request())
        return FieldNpcDisplayFrontier::Pending;
      c.phase = ++c.drawClass == 3 ? 100 : 38;
      break;
    case 100:
      return FieldNpcDisplayFrontier::Shadow;
    case 101:
      return FieldNpcDisplayFrontier::SeparatePose;
    case 102:
      return FieldNpcDisplayFrontier::History;
    default:
      return FieldNpcDisplayFrontier::Pending;
    }
}
} // namespace ttyd
