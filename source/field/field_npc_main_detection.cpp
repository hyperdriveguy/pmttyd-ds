#include "field_npc_main_detection.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Float words retain payloads; single() preserves source SP rounding.
// This norm uses rounded dz*dz plus fused dx*dx, unlike auto-pose's two squares.
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float single(float f) {
  volatile float v = f;
  return v;
}
} // namespace
FieldNpcDetectionFrontier
// Packed identity:regular event ID11c,script13c,trigger140,root8c/90/94,
// width14c,height150,territory kind1f8,center1fc/204. Phases retain individual
// primitives; changing actor/provider during a hold cannot replace the visit.
advanceFieldNpcMainDetection(uint32_t actor, const FieldNpcDetectionServices &s,
                             FieldNpcDetectionCursor &c) {
  using R = FieldNpcDetectionFrontier;
  const auto &m = s.norm.memory;
  if (!m.word || !m.store)
    return R::Pending;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
  }
  if (c.actor != actor || c.owner.norm.memory.user != m.user ||
      c.owner.norm.memory.word != m.word ||
      c.owner.norm.memory.store != m.store ||
      c.owner.norm.doubleWord != s.norm.doubleWord ||
      c.owner.norm.estimate != s.norm.estimate || c.owner.byte != s.byte ||
      c.owner.request != s.request)
    return R::Pending;
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
    // ID0 skips lookup AND interrupt. Otherwise evtGetPtrID80035a28 supplies
    // actual entry address; null/stopped(byte8 mask2) interrupts. Capture script
    // THEN raw class byte10, even when script0. Cloud excludes classes5/8.
    case 0:
      if (!read(actor + 0x11c, c.value))
        return R::Pending;
      c.call = {0x80035a28, c.value};
      c.phase = c.value ? 1 : 200;
      break;
    case 1:
      if (!request())
        return R::Pending;
      c.event = c.response.word;
      c.phase = c.event ? 3 : 180;
      break;
    case 3: {
      auto b = s.byte ? s.byte(m.user, c.event + 8) : std::nullopt;
      if (!b)
        return R::Pending;
      c.flags = *b;
      c.phase = c.flags & 2 ? 180 : 4;
      break;
    }
    case 4:
      if (!read(actor + 0x13c, c.script))
        return R::Pending;
      ++c.phase;
      break;
    case 5: {
      auto b = s.byte ? s.byte(m.user, c.event + 0x10) : std::nullopt;
      if (!b)
        return R::Pending;
      c.eventClass = *b;
      c.phase = c.script && c.eventClass != 5 && c.eventClass != 8 ? 6 : 40;
      break;
    }
    // Captured main20000000/20 bypasses cloud. Samples top(X,Y,Z), then
    // middle/bottom(FRESH Y,X,Z,height); each reloads width before the child.
    // cloudGetBreathPower80187f10 consumes semantic copied point, not pointer.
    // Fresh ZERO after each result:strict power>0 posts8 and skips later tests.
    case 6:
      if (!read(actor, c.value))
        return R::Pending;
      c.phase = c.value & (0x20000000 | 0x20) ? 40 : 10;
      break;
    case 10:
    case 11:
    case 12:
      if (!read(actor + 0x8c + (c.phase - 10) * 4, c.call.point[c.phase - 10]))
        return R::Pending;
      ++c.phase;
      break;
    case 13:
      if (!load(actor + 0x14c, c.call.width))
        return R::Pending;
      c.call.function = 0x80187f10;
      c.call.argument = 0;
      ++c.phase;
      break;
    case 14:
      if (!request())
        return R::Pending;
      ++c.phase;
      break;
    case 15:
      if (!load(0x8041fc7c, c.zero))
        return R::Pending;
      if (c.response.power > c.zero) {
        c.trigger = 8;
        c.phase = 120;
      } else {
        ++c.sample;
        c.phase = c.sample == 3 ? 40 : 20;
      }
      break;
    case 20:
      if (!read(actor + 0x90, c.call.point[1]))
        return R::Pending;
      ++c.phase;
      break;
    case 21:
      if (!read(actor + 0x8c, c.call.point[0]))
        return R::Pending;
      ++c.phase;
      break;
    case 22:
      if (!read(actor + 0x94, c.call.point[2]))
        return R::Pending;
      ++c.phase;
      break;
    case 23:
      if (!load(actor + 0x150, c.height))
        return R::Pending;
      if (c.sample == 1)
        c.phase = 24;
      else {
        c.call.point[1] = bits(single(real(c.call.point[1]) - c.height));
        c.phase = 13;
      }
      break;
    // Source fnmsubs sign boundary:NEGATE(fused(height*.5-Y)), not separately
    // rounded subtraction. Bottom uses single(Y-height) without half literal.
    case 24:
      if (!load(0x8041fc80, c.half))
        return R::Pending;
      // Original fnmsubs negates a fused product-minus-Y, including zero sign.
      c.call.point[1] =
          bits(-std::fma(c.height, c.half, -real(c.call.point[1])));
      c.phase = 13;
      break;
    // Territory only kind1/2 and captured class1/2. Kind1 loads Z-center/root
    // before X-center/root, computes fused norm, then freshly reads radius214.
    // Strict outside posts4; equality/unordered do not trigger.
    case 40:
      if (!read(actor + 0x1f8, c.kind))
        return R::Pending;
      if (!c.kind || (c.eventClass != 1 && c.eventClass != 2))
        c.phase = 100;
      else
        c.phase = c.kind == 1 ? 50 : c.kind == 2 ? 80 : 100;
      break;
    case 50:
      if (!load(actor + 0x204, c.z))
        return R::Pending;
      ++c.phase;
      break;
    case 51:
      if (!load(actor + 0x94, c.current))
        return R::Pending;
      ++c.phase;
      break;
    case 52:
      if (!load(actor + 0x1fc, c.x))
        return R::Pending;
      c.z = single(c.z - c.current);
      ++c.phase;
      break;
    case 53:
      if (!load(actor + 0x8c, c.current))
        return R::Pending;
      ++c.phase;
      break;
    case 54:
      if (!load(0x8041fc7c, c.zero))
        return R::Pending;
      c.x = single(c.x - c.current);
      c.squared = std::fma(c.x, c.x, single(c.z * c.z));
      ++c.phase;
      break;
    case 55:
      if (!advanceFieldNpcInlineNorm(c.squared, c.zero, s.norm, c.normCursor,
                                     c.distance))
        return R::Pending;
      ++c.phase;
      break;
    case 56:
      if (!load(actor + 0x214, c.extent))
        return R::Pending;
      if (c.distance > c.extent) {
        c.trigger = 4;
        c.phase = 120;
      } else
        c.phase = 100;
      break;
    // Kind2 rectangle:class2 extents214/21c, class1 extents208/210.
    // Per axis center->extent->current, strict rounded outside comparisons.
    // X rejection skips Z; NaN on one axis continues; negative extents unchanged.
    case 80:
      c.xExtent = c.eventClass == 2 ? 0x214 : 0x208;
      c.zExtent = c.eventClass == 2 ? 0x21c : 0x210;
      ++c.phase;
      break;
    case 81:
      if (!load(actor + 0x1fc, c.x))
        return R::Pending;
      ++c.phase;
      break;
    case 82:
      if (!load(actor + c.xExtent, c.extent))
        return R::Pending;
      ++c.phase;
      break;
    case 83:
      if (!load(actor + 0x8c, c.current))
        return R::Pending;
      if (c.current < single(c.x - c.extent) ||
          c.current > single(c.x + c.extent)) {
        c.trigger = 4;
        c.phase = 120;
      } else
        ++c.phase;
      break;
    case 84:
      if (!load(actor + 0x204, c.z))
        return R::Pending;
      ++c.phase;
      break;
    case 85:
      if (!load(actor + c.zExtent, c.extent))
        return R::Pending;
      ++c.phase;
      break;
    case 86:
      if (!load(actor + 0x94, c.current))
        return R::Pending;
      if (c.current < single(c.z - c.extent) ||
          c.current > single(c.z + c.extent)) {
        c.trigger = 4;
        c.phase = 120;
      } else
        c.phase = 100;
      break;
    case 100:
      return R::Mario; // next source comparison8004a4e4; retained eventClass
    // Fresh trigger140 read/OR/store, then npcCheckInterrupt800440f8 once.
    // Its result is discarded. Reaching Display never implies callback dispatch
    // or history; reaching Mario preserves captured raw event class.
    case 120:
      if (!read(actor + 0x140, c.value))
        return R::Pending;
      ++c.phase;
      break;
    case 121:
      if (!m.store(m.user, actor + 0x140, c.value | c.trigger))
        return R::Pending;
      c.phase = 180;
      break;
    case 180:
      c.call = {0x800440f8, actor};
      ++c.phase;
      break;
    case 181:
      if (!request())
        return R::Pending;
      c.phase = 200;
      break;
    case 200:
      return R::Display; // next fresh source mainflags load8004accc
    default:
      return R::Pending;
    }
}
} // namespace ttyd
