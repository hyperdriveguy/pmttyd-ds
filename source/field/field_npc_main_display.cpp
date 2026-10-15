#include "field_npc_main_display.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Raw float words and explicit SP rounding; ordered culling/clamping retains
// source NaN and signed-zero behavior rather than normalizing projection data.
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
// Actor main800 skips to History. Otherwise camera4 queried TWICE: view11c
// via PSMTXMultVec8029e6a4 then fresh projection15c via8029eb08, using semantic
// local point. Actual camera/SDK grouping and queue pass are provider-owned.
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
    // Ordered cull:Z>-1(fcd8),Z<0(fc7c),X/Y within inclusive-2(fd5c)..2(fd34).
    // Any failed/NaN test freshly reads force mask40; success skips that read.
    // Literal loads short-circuit in source order; no generic frustum fallback.
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
    // Sequential RGBA bytes114..117 stop on first non-ff. White clears pose
    // material40(FlagOff40710); nonwhite sets40(FlagOn40730), THEN copies word114,
    // reloads handle104 and publishes copied color via406b4. No color pointer.
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
    // Depth=fma(5000(fd60),projectedZ,5000); negative clamps to0 without
    // loading10000(fd64), otherwise clamp only >10000. NaN survives. Fresh main
    // flags, fresh-1, SP multiply precede main80->SeparatePose. Half320 mask2
    // also selects SeparatePose unless fresh state1d4 has10000.
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
    // Alpha117==ff chooses ordinary8004c50c classes0/1/2; otherwise xlu
    //8004c7e0 class2 only. dispEntry80010bc4 reloads camera1f4 for EACH record,
    // retaining callback/actor/depth. Completion goes to Shadow, not draw dispatch.
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
