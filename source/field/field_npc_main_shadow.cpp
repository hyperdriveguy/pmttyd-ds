#include "field_npc_main_shadow.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f) { uint32_t w; std::memcpy(&w, &f, 4); return w; }
float real(uint32_t w) { float f; std::memcpy(&f, &w, 4); return f; }
float single(float f) { volatile float v = f; return v; }
} // namespace
bool advanceFieldNpcMainShadow(uint32_t actor, FieldNpcDisplayFrontier start,
    float depth, const FieldNpcShadowServices &s, FieldNpcShadowCursor &c) {
  const auto &m = s.memory;
  if (!m.word || start == FieldNpcDisplayFrontier::Pending)
    return false;
  if (!c.active) {
    c = {}; c.active = true; c.owner = s; c.actor = actor; c.start = start;
    c.depth = depth; c.depthBits = bits(depth);
    c.phase = start == FieldNpcDisplayFrontier::Shadow ? 0 :
              start == FieldNpcDisplayFrontier::SeparatePose ? 100 : 200;
  }
  if (c.actor != actor || c.start != start || c.depthBits != bits(depth) ||
      c.owner.memory.user != m.user || c.owner.memory.word != m.word ||
      c.owner.byte != s.byte || c.owner.request != s.request ||
      c.owner.queue != s.queue)
    return false;
  auto read = [&](uint32_t a, uint32_t &v) {
    auto w = m.word(m.user, a); if (!w) return false; v = *w; return true;
  };
  auto load = [&](uint32_t a, float &v) {
    uint32_t w; if (!read(a, w)) return false; v = real(w); return true;
  };
  auto request = [&] { return s.request && s.request(m.user, c.call, c.response); };
  auto queue = [&] { return s.queue && s.queue(m.user, c.draw, c.drawResponse); };
  for (;;) switch (c.phase) {
  case 0:
    if (!read(actor, c.flags)) return false;
    c.phase = c.flags & 0x04000000 ? 50 : (c.flags & 0x40000 ? 90 : 1);
    break;
  case 1:
    if (!read(actor + 0x28, c.pointer)) return false;
    ++c.phase; break;
  case 2: case 3: case 4:
    if (!load(actor + 0x8c + (c.phase-2)*4, c.root[c.phase-2])) return false;
    if (++c.phase == 5 && !c.pointer) c.phase = 30;
    break;
  case 5:
    if (!load(actor + 0x144, c.angle)) return false;
    c.call = {0x80007328}; c.call.scalars[0] = c.angle;
    ++c.phase; break;
  case 6:
    if (!request()) return false;
    c.angle = c.response.scalar; ++c.phase; break;
  case 7:
    if (!load(0x8041fccc, c.factor)) return false;
    c.pointer = c.angle < c.factor ? 0x8041fcb0 : 0x8041fcc4;
    ++c.phase; break;
  case 8:
    if (!load(c.pointer, c.angle)) return false;
    c.call = {0x80125cd4}; c.call.scalars[0] = c.angle;
    ++c.phase; break;
  case 9:
    if (!request()) return false;
    c.sign = c.response.scalar; ++c.phase; break;
  case 10:
    if (!read(actor + 0x28, c.pointer)) return false;
    ++c.phase; break;
  case 11:
    if (!load(actor + 0xe0, c.scale[0])) return false;
    ++c.phase; break;
  case 12:
    if (!load(c.pointer + 0x30, c.offset[0])) return false;
    c.offset[0] = single(c.offset[0] * c.scale[0]);
    ++c.phase; break;
  case 13:
    if (!load(c.pointer + 0x34, c.offset[1])) return false;
    ++c.phase; break;
  case 14:
    if (!load(actor + 0xe4, c.scale[1])) return false;
    c.root[0] = std::fma(c.offset[0], c.sign, c.root[0]);
    ++c.phase; break;
  case 15:
    if (!load(c.pointer + 0x38, c.offset[2])) return false;
    c.root[1] = std::fma(c.offset[1], c.scale[1], c.root[1]);
    ++c.phase; break;
  case 16:
    if (!load(actor + 0xe8, c.scale[2])) return false;
    c.root[2] = std::fma(c.offset[2], c.scale[2], c.root[2]);
    c.phase = 30; break;
  case 30: case 70:
    if (!load(actor + 0x14c, c.width)) return false;
    ++c.phase; break;
  case 31: case 71:
    if (!load(0x8041fce0, c.divisor)) return false;
    ++c.phase; break;
  case 32: case 72:
    if (!load(actor + 0xe0, c.scale[0])) return false;
    c.radius = single(c.width / c.divisor);
    ++c.phase; break;
  case 33: case 73:
    if (!load(actor + 0x154, c.factor)) return false;
    ++c.phase; break;
  case 34:
    if (!load(0x8041fd68, c.divisor)) return false;
    c.radius = single(c.radius * c.scale[0]);
    c.radius = single(c.factor * c.radius);
    c.radius = single(c.divisor * c.radius);
    c.phase = 80; break;
  case 50: case 51:
    if (!load(actor + 0x8c + (c.phase-50)*4, c.root[c.phase-50])) return false;
    ++c.phase; break;
  case 52:
    if (!load(0x8041fc7c, c.zero)) return false;
    ++c.phase; break;
  case 53:
    if (!load(0x8041fc94, c.one)) return false;
    ++c.phase; break;
  case 54:
    if (!load(0x8041fcf0, c.width)) return false;
    ++c.phase; break;
  case 55:
    if (!load(actor + 0x94, c.root[2])) return false;
    ++c.phase; break;
  case 56:
    if (!load(0x8041fcd8, c.minusOne)) return false;
    c.call = {0x8001577c, {0}};
    c.call.scalars = {c.root[0], single(c.one + c.root[1]), c.root[2],
                      c.zero, c.minusOne, c.zero, c.width};
    ++c.phase; break;
  case 57:
    if (!request()) return false;
    c.phase = 60; break;
  case 60:
    // A completed hit must publish the actually-written Y output. Missing
    // output is an unsupported provider domain, not permission to invent Y.
    if (c.response.word && !c.response.hit[1]) return false;
    if (c.response.word) c.root[1] = *c.response.hit[1];
    c.phase = 58; break;
  case 58:
    if (!read(actor + 0x104, c.handle)) return false;
    c.call = {0x80039824, {c.handle}}; c.call.point = c.root;
    ++c.phase; break;
  case 59:
    if (!request()) return false;
    c.phase = 70; break;
  case 74: case 75: case 76:
    if (!load(actor + 0x8c + (c.phase-74)*4, c.root[c.phase-74])) return false;
    if (++c.phase == 77) {
      c.radius = single(c.radius * c.scale[0]);
      c.radius = single(c.factor * c.radius);
      c.phase = 80;
    }
    break;
  case 80:
    c.call = {0x8002be98};
    c.call.scalars = {c.root[0], c.root[1], c.root[2], c.radius};
    ++c.phase; break;
  case 81:
    if (!request()) return false;
    c.token = c.response.word; ++c.phase; break;
  case 82:
    if (!read(actor, c.flags)) return false;
    c.phase = c.flags & 0x80000 ? 83 : 90; break;
  case 83:
    c.call = {0x8002bcfc, {c.token, 0}}; ++c.phase; break;
  case 84:
    if (!request()) return false;
    c.phase = 90; break;
  case 90:
    if (!read(actor, c.flags)) return false;
    c.phase = c.flags & 0x100 ? 91 : 100; break;
  case 91:
    if (!read(actor + 0x1f4, c.pointer)) return false;
    c.draw = {0x80010bc4, {c.pointer, 2, 0x8004bc3c, actor}};
    c.draw.depth = c.depth; ++c.phase; break;
  case 92:
    if (!queue()) return false;
    c.phase = 100; break;
  case 100:
    if (!read(actor + 0x110, c.handle)) return false;
    c.phase = c.handle == 0xffffffff ? 200 : 101; break;
  case 101: {
    auto alpha = s.byte ? s.byte(m.user, actor + 0x117) : std::nullopt;
    if (!alpha) return false;
    c.callback = *alpha == 255 ? 0x8004bf28 : 0x8004c234;
    c.drawClass = *alpha == 255 ? 0 : 2;
    ++c.phase; break;
  }
  case 102:
    c.draw = {0x80010bc4, {1, c.drawClass, c.callback, actor}};
    c.draw.depth = c.depth; ++c.phase; break;
  case 103:
    if (!queue()) return false;
    c.phase = ++c.drawClass == 3 ? 200 : 102; break;
  case 200: return true;
  default: return false;
  }
}
} // namespace ttyd
