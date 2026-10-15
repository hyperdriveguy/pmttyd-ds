#include "field_npc_matrix.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
} // namespace
bool advanceFieldNpcMatrix(uint32_t actor, int32_t slot,
                           const FieldNpcMatrixServices &s,
                           FieldNpcMatrixCursor &c, BattleActMatrix &result) {
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
    c.slot = uint32_t(slot);
  }
  const auto &p = c.owner;
  auto word = [&](uint32_t a, uint32_t &out) {
    if (!p.word)
      return false;
    auto v = p.word(p.user, a);
    if (!v)
      return false;
    out = *v;
    return true;
  };
  auto load = [&](uint32_t a, float &out) {
    uint32_t w;
    if (!word(a, w))
      return false;
    out = real(w);
    return true;
  };
  auto number = [&](uint32_t fn) {
    if (!c.pending) {
      c.number = {};
      c.number.function = fn;
      c.number.args = c.values;
      c.pending = true;
    }
    if (!p.number || !p.number(p.user, c.number))
      return false;
    c.pending = false;
    return true;
  };
  auto math = [&](uint32_t fn) {
    if (!c.pending) {
      c.math = {};
      c.math.function = fn;
      c.math.left = c.output;
      c.math.right = c.output;
      c.math.xyz = {c.values[0], c.values[1], c.values[2]};
      c.math.axis = c.axis;
      c.math.radians = c.angle;
      if (fn == 0x8029dd64)
        c.math.left = c.rotation;
      c.pending = true;
    }
    if (!p.matrix || !p.matrix(p.user, c.math))
      return false;
    c.pending = false;
    if (fn == 0x8029dff0)
      c.rotation = c.math.output;
    else
      c.output = c.math.output;
    return true;
  };
  // Explicit phases preserve each successful source load on a later hold.
  for (;;)
    switch (c.phase) {
    case 0:
      if (!word(c.actor, c.bits))
        return false;
      ++c.phase;
      break;
    case 1:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.correction = c.zero;
      ++c.phase;
      break;
    case 2:
      if (!load(0x8041fc94, c.one))
        return false;
      c.reflection = c.one;
      c.phase = (c.bits & 0x0a000000) ? 15 : 3;
      break;
    case 3:
      if (!number(0x8000f2e4))
        return false;
      c.camera = c.number.word;
      ++c.phase;
      break;
    case 4:
      if (!load(c.camera + 0xc, c.values[0]))
        return false;
      ++c.phase;
      break;
    case 5:
      if (!load(c.camera + 0x14, c.values[1]))
        return false;
      ++c.phase;
      break;
    case 6:
      if (!load(c.camera + 0x18, c.values[2]))
        return false;
      ++c.phase;
      break;
    case 7:
      if (!load(c.camera + 0x20, c.values[3]))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!number(0x8000703c))
        return false;
      c.first = c.number.result;
      ++c.phase;
      break;
    case 9:
      if (!load(c.camera + 0xc, c.values[0]))
        return false;
      ++c.phase;
      break;
    case 10:
      if (!load(c.camera + 0x14, c.values[1]))
        return false;
      ++c.phase;
      break;
    case 11:
      if (!load(c.actor + 0x8c, c.values[2]))
        return false;
      ++c.phase;
      break;
    case 12:
      if (!load(c.actor + 0x94, c.values[3]))
        return false;
      ++c.phase;
      break;
    case 13:
      if (!number(0x8000703c))
        return false;
      c.values[0] = c.first - c.number.result;
      ++c.phase;
      break;
    case 14:
      if (!number(0x80007328))
        return false;
      c.correction = c.number.result;
      ++c.phase;
      break;
    case 15:
      if (!load(c.actor + 0xf0, c.values[0]))
        return false;
      ++c.phase;
      break;
    case 16:
      if (!number(0x80007328))
        return false;
      c.values[0] = c.number.result;
      ++c.phase;
      break;
    case 17:
      if (!number(0x80007328))
        return false;
      c.angle = c.number.result;
      ++c.phase;
      break;
    case 18:
      if (!load(0x8041fcb0, c.first))
        return false;
      c.phase = c.angle >= c.first ? 19 : 22;
      break;
    case 19:
      if (!load(0x8041fcc4, c.first))
        return false;
      c.phase = c.angle <= c.first ? 20 : 22;
      break;
    case 20:
      if (!load(0x8041fcd8, c.first))
        return false;
      c.reflection *= c.first;
      c.phase = 22;
      break;
    case 22:
      if (!math(0x8029dd04))
        return false;
      ++c.phase;
      break;
    case 23:
      if (!load(0x8041fc94, c.first))
        return false;
      ++c.phase;
      break;
    case 24:
      if (!load(c.actor + 0xe0, c.values[0]))
        return false;
      c.phase = c.first == c.values[0] ? 25 : 28;
      break;
    case 25:
      if (!load(c.actor + 0xe4, c.values[1]))
        return false;
      c.phase = c.first == c.values[1] ? 26 : 28;
      break;
    case 26:
      if (!load(c.actor + 0xe8, c.values[2]))
        return false;
      c.phase = c.first == c.values[2] && c.first == c.reflection ? 31 : 28;
      break;
    case 28:
      if (!load(c.actor + 0xe8, c.values[2]))
        return false;
      ++c.phase;
      break;
    case 29:
      if (!load(c.actor + 0xe4, c.values[1]))
        return false;
      c.values[2] *= c.reflection;
      ++c.phase;
      break;
    case 30:
      if (!math(0x8029e2d8))
        return false;
      ++c.phase;
      break;
    case 31:
      if (!load(c.actor + 0xf8, c.values[0]))
        return false;
      ++c.phase;
      break;
    case 32:
      if (!load(0x8041fc7c, c.first))
        return false;
      c.phase = c.values[0] == c.first ? 33 : 35;
      break;
    case 33:
      if (!load(c.actor + 0xfc, c.values[1]))
        return false;
      c.phase = c.values[1] == c.first ? 34 : 35;
      break;
    case 34:
      if (!load(c.actor + 0x100, c.values[2]))
        return false;
      c.phase = c.values[2] == c.first ? 38 : 35;
      break;
    case 35:
      if (!load(c.actor + 0xfc, c.values[1]))
        return false;
      c.values[0] = -c.values[0];
      c.values[1] = -c.values[1];
      ++c.phase;
      break;
    case 36:
      if (!load(c.actor + 0x100, c.values[2]))
        return false;
      c.values[2] = -c.values[2];
      ++c.phase;
      break;
    case 37:
      if (!math(0x8029e264))
        return false;
      ++c.phase;
      break;
    case 38:
      if (!load(c.actor + 0xf4, c.angle))
        return false;
      c.axis = 122;
      ++c.phase;
      break;
    case 39:
      if (!load(0x8041fc7c, c.first))
        return false;
      c.phase = c.angle == c.first ? 43 : 40;
      break;
    case 40:
      if (!load(0x8041fd7c, c.first))
        return false;
      c.angle = c.first * c.angle;
      ++c.phase;
      break;
    case 41:
      if (!math(0x8029dff0))
        return false;
      ++c.phase;
      break;
    case 42:
      if (!math(0x8029dd64))
        return false;
      ++c.phase;
      break;
    case 43:
      if (!load(c.actor + 0xec, c.angle))
        return false;
      c.axis = 120;
      ++c.phase;
      break;
    case 44:
      if (!load(0x8041fc7c, c.first))
        return false;
      c.phase = c.angle == c.first ? 48 : 45;
      break;
    case 45:
      if (!load(0x8041fd7c, c.first))
        return false;
      c.angle = c.first * c.angle;
      ++c.phase;
      break;
    case 46:
      if (!math(0x8029dff0))
        return false;
      ++c.phase;
      break;
    case 47:
      if (!math(0x8029dd64))
        return false;
      ++c.phase;
      break;
    case 48:
      if (!load(c.actor + 0xf0, c.angle))
        return false;
      c.axis = 121;
      ++c.phase;
      break;
    case 49:
      if (!load(0x8041fc7c, c.first))
        return false;
      c.phase = c.angle == c.first && c.correction == c.first ? 53 : 50;
      if (c.phase == 50)
        c.angle = c.angle + c.correction;
      break;
    case 50:
      if (!load(0x8041fd7c, c.first))
        return false;
      c.angle = c.first * c.angle;
      ++c.phase;
      break;
    case 51:
      if (!math(0x8029dff0))
        return false;
      ++c.phase;
      break;
    case 52:
      if (!math(0x8029dd64))
        return false;
      ++c.phase;
      break;
    case 53:
      if (!load(c.actor + 0xf8, c.values[0]))
        return false;
      ++c.phase;
      break;
    case 54:
      if (!load(0x8041fc7c, c.first))
        return false;
      c.phase = c.values[0] == c.first ? 55 : 57;
      break;
    case 55:
      if (!load(c.actor + 0xfc, c.values[1]))
        return false;
      c.phase = c.values[1] == c.first ? 56 : 57;
      break;
    case 56:
      if (!load(c.actor + 0x100, c.values[2]))
        return false;
      c.phase = c.values[2] == c.first ? 60 : 57;
      break;
    case 57:
      if (!load(c.actor + 0xfc, c.values[1]))
        return false;
      ++c.phase;
      break;
    case 58:
      if (!load(c.actor + 0x100, c.values[2]))
        return false;
      ++c.phase;
      break;
    case 59:
      if (!math(0x8029e264))
        return false;
      ++c.phase;
      break;
    case 60:
      if (!load(c.actor + (c.slot == 0xffffffff ? 0x8c : 0xa4 + 12 * c.slot),
                c.values[0]))
        return false;
      ++c.phase;
      break;
    case 61:
      if (!load(c.actor + (c.slot == 0xffffffff ? 0x90 : 0xa8 + 12 * c.slot),
                c.values[1]))
        return false;
      ++c.phase;
      break;
    case 62:
      if (!load(c.actor + (c.slot == 0xffffffff ? 0x94 : 0xac + 12 * c.slot),
                c.values[2]))
        return false;
      ++c.phase;
      break;
    case 63:
      if (!math(0x8029e264))
        return false;
      result = c.output;
      c = {};
      return true;
    default:
      return false;
    }
}
} // namespace ttyd
