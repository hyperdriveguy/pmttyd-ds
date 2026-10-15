#include "field_npc_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
} // namespace
bool advanceFieldNpcDraw(uint32_t callback, uint32_t actor,
                         const FieldNpcDrawServices &s, FieldNpcDrawCursor &c,
                         uint32_t camera) {
  if (!c.active) {
    if (callback != 0x8004c50c && callback != 0x8004c7e0 &&
        callback != 0x8004bf28 && callback != 0x8004c234 &&
        callback != 0x8004bc3c)
      return false;
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
    c.callback = callback;
    c.xlu = callback == 0x8004c7e0 || callback == 0x8004c234;
    c.offscreen = callback == 0x8004bf28 || callback == 0x8004c234;
    c.cameraId = camera;
    c.phase = c.offscreen ? 100 : callback == 0x8004bc3c ? 200 : 0;
  }
  const auto &p = c.owner;
  const auto &m = p.transform;
  auto word = [&](uint32_t a, uint32_t &out) {
    if (!m.word)
      return false;
    auto v = m.word(m.user, a);
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
  auto call = [&](uint32_t fn,
                  std::array<uint32_t, 3> args = std::array<uint32_t, 3>{},
                  std::array<float, 2> scalars = std::array<float, 2>{}) {
    if (!c.pending) {
      c.call = {};
      c.call.function = fn;
      c.call.args = args;
      c.call.scalars = scalars;
      c.call.color = c.color;
      c.call.bounds = c.bounds;
      if (fn == 0x8003b4d8)
        c.call.matrix = c.matrix;
      c.response = {};
      c.pending = true;
    }
    if (!p.request || !p.request(m.user, c.call, c.response))
      return false;
    c.pending = false;
    return true;
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!load(c.actor + 0x144, c.angle))
        return false;
      c.angle = -c.angle;
      ++c.phase;
      break;
    case 1:
      if (!call(0x80007328, {}, {c.angle, 0}))
        return false;
      c.incoming = float(c.response.number);
      ++c.phase;
      break;
    case 2:
      if (!advanceFieldNpcMatrix(c.actor, -1, m, c.transform, c.matrix))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!word(c.actor + 0x1d4, c.value))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!word(c.actor + 0x114, c.color))
        return false;
      if (c.value & 0x00400000) {
        c.color = ((c.color >> 1) & 0x7f7f7f00) | (c.color & 0xff);
        c.phase = 5;
      } else
        c.phase = 9;
      break;
    case 5:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 6:
      if (!call(0x800406b4, {c.handle, 0, 0}))
        return false;
      ++c.phase;
      break;
    case 7:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!call(0x80040730, {c.handle, 0x40, 0}))
        return false;
      ++c.phase;
      break;
    case 9:
      if (c.xlu) {
        c.pass = 1;
        c.phase = 12;
        break;
      }
      if (!call(0x80010864))
        return false;
      c.record = c.response.word;
      ++c.phase;
      break;
    case 10: {
      if (!p.byte)
        return false;
      auto v = p.byte(m.user, c.record + 1);
      if (!v)
        return false;
      c.pass = uint32_t(*v) + 1;
      c.phase = c.pass == 1 ? 12 : c.pass <= 3 ? 60 : 64;
      break;
    }
    case 12:
      if (!word(c.actor, c.value))
        return false;
      c.phase = c.value & 0x00800000 ? 13 : 58;
      break;
    case 13:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 14:
      if (!call(0x80039c44, {c.handle, 0, 0}))
        return false;
      ++c.phase;
      break; // discarded getter is required
    case 15:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 16:
      if (!call(0x80039cf8, {c.handle, 0, 0}))
        return false;
      c.pose = c.response.word;
      ++c.phase;
      break;
    case 17:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 18:
      if (!call(0x80039c44, {c.handle, 0, 0}))
        return false;
      c.name = c.response.word;
      ++c.phase;
      break;
    case 19:
      if (!call(0x8000f2fc, {4, 0, 0}))
        return false;
      c.camera = c.response.word;
      ++c.phase;
      break;
    case 20:
      if (!load(c.camera + 0x18, c.cameraValues[0]))
        return false;
      ++c.phase;
      break;
    case 21:
      if (!load(c.camera + 0xc, c.cameraValues[1]))
        return false;
      ++c.phase;
      break;
    case 22:
      if (!load(c.camera + 0x20, c.cameraValues[2]))
        return false;
      ++c.phase;
      break;
    case 23:
      if (!load(c.camera + 0x14, c.cameraValues[3]))
        return false;
      c.cameraValues[0] = -(c.cameraValues[0] - c.cameraValues[1]);
      c.cameraValues[2] = -(c.cameraValues[2] - c.cameraValues[3]);
      ++c.phase;
      break;
    case 24:
      if (!call(0x802725a0, {}, {c.cameraValues[0], c.cameraValues[2]}))
        return false;
      c.angle = float(c.response.number);
      ++c.phase;
      break;
    case 25:
      if (!load(0x8041fcbc, c.literal))
        return false;
      c.angle = c.literal * c.angle;
      ++c.phase;
      break;
    case 26:
      if (!call(0x80007328, {}, {c.angle, 0}))
        return false;
      c.angle = float(c.response.number);
      ++c.phase;
      break;
    case 27:
      if (!load(0x8041fc7c, c.literal))
        return false;
      c.phase = c.angle < c.literal ? 28 : 29;
      break;
    case 28:
      if (!load(0x8041fcc0, c.literal))
        return false;
      c.angle += c.literal;
      ++c.phase;
      break;
    case 29:
      if (!load(c.pose + 0x74, c.literal))
        return false;
      c.angle -= c.literal;
      ++c.phase;
      break;
    case 30:
      if (!call(0x80007328, {}, {c.angle, 0}))
        return false;
      c.angle = float(c.response.number);
      ++c.phase;
      break;
    case 31:
      if (!load(0x8041fcb0, c.literal))
        return false;
      c.phase = c.angle > c.literal ? 32 : 33;
      break;
    case 32:
      if (!load(0x8041fcc4, c.literal))
        return false;
      c.phase = c.angle <= c.literal ? 36 : 33;
      break;
    case 33:
      if (!load(0x8041fcb8, c.literal))
        return false;
      c.phase = c.angle < c.literal ? 34 : 35;
      break;
    case 34:
      if (!load(0x8041fcc8, c.literal))
        return false;
      c.phase = c.angle >= c.literal ? 36 : 35;
      break;
    case 35:
      c.rear = false;
      c.phase = 37;
      break;
    case 36:
      c.rear = true;
      c.phase = 37;
      break;
    case 37:
      c.index = 0;
      ++c.phase;
      break;
    case 38:
      if (!word(0x803066f8 + 8 * c.index + (c.rear ? 0 : 4), c.pointer))
        return false;
      ++c.phase;
      break;
    case 39:
      if (!call(0x8026d464, {c.pointer, c.name, 0}))
        return false;
      if (!c.response.word)
        c.phase = 40;
      else if (++c.index < 14)
        c.phase = 38;
      else
        c.phase = 58;
      break;
    case 40:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 41:
      if (!word(0x803066f8 + 8 * c.index + (c.rear ? 4 : 0), c.pointer))
        return false;
      ++c.phase;
      break;
    case 42:
      if (!call(0x80041170, {c.handle, c.pointer, 0}))
        return false;
      c.phase = 58;
      break;
    case 58:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 59:
      if (!call(0x8003ed2c, {c.handle, 0, 0}))
        return false;
      ++c.phase;
      break;
    case 60:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 61:
      if (!load(0x8041fd34, c.literal))
        return false;
      ++c.phase;
      break;
    case 62:
      if (!call(0x8003b4d8, {c.handle, 0, c.pass}, {c.incoming, c.literal}))
        return false;
      c.phase = c.xlu && c.pass < 3 ? 63 : 64;
      break;
    case 63:
      ++c.pass;
      c.phase = 60;
      break;
    case 64:
      if (c.offscreen) {
        c.phase = 102;
        break;
      }
      c = {};
      return true;
    case 100:
      if (!call(0x80005ce4))
        return false;
      ++c.phase;
      break;
    case 101:
      if (!call(0x802b69d0))
        return false;
      c.phase = 0;
      break;
    case 102:
      if (!call(0x80005ce4))
        return false;
      ++c.phase;
      break;
    case 103:
      if (!call(0x802b6a08))
        return false;
      c.bounds = c.response.bounds;
      ++c.phase;
      break;
    case 104:
      if (!word(c.actor + 0x110, c.handle))
        return false;
      ++c.phase;
      break;
    case 105:
      if (!call(0x80072f38, {c.handle, 0, 0}))
        return false;
      c = {};
      return true;
    case 200:
      if (!load(c.actor + 0x144, c.angle))
        return false;
      c.angle = -c.angle;
      ++c.phase;
      break;
    case 201:
      if (!call(0x80007328, {}, {c.angle, 0}))
        return false;
      c.incoming = float(c.response.number);
      ++c.phase;
      break;
    case 202:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 203:
      if (!call(0x8004069c, {c.handle, 0, 0}))
        return false;
      c.savedFlags = c.response.word;
      ++c.phase;
      break;
    case 204:
      if (!call(0x8000f2fc, {c.cameraId, 0, 0}))
        return false;
      c.camera = c.response.word;
      ++c.phase;
      break;
    case 205:
      if (!load(c.camera + 0x114, c.angle))
        return false;
      ++c.phase;
      break;
    case 206:
      if (!advanceFieldNpcBlurTrig(c.angle, false, m, c.trig, c.trigValue))
        return false;
      ++c.phase;
      break;
    case 207:
      if (!load(0x8041fd34, c.literal))
        return false;
      c.offsetX = c.literal * c.trigValue;
      ++c.phase;
      break;
    case 208:
      if (!call(0x8000f2fc, {c.cameraId, 0, 0}))
        return false;
      c.camera = c.response.word;
      ++c.phase;
      break;
    case 209:
      if (!load(c.camera + 0x114, c.angle))
        return false;
      ++c.phase;
      break;
    case 210:
      if (!advanceFieldNpcBlurTrig(c.angle, true, m, c.trig, c.trigValue))
        return false;
      c.trigValue = -c.trigValue;
      ++c.phase;
      break;
    case 211:
      if (!load(0x8041fd34, c.literal))
        return false;
      ++c.phase;
      break;
    case 212:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      c.offsetZ = c.literal * c.trigValue;
      ++c.phase;
      break;
    case 213:
      if (!call(0x8003ed2c, {c.handle, 0, 0}))
        return false;
      c.history = 4;
      c.alphaCounter = 100;
      ++c.phase;
      break;
    case 214:
      if (!advanceFieldNpcMatrix(c.actor, int32_t(c.history), m, c.transform,
                                 c.matrix))
        return false;
      ++c.phase;
      break;
    case 215:
      if (!load(0x8041fc7c, c.literal))
        return false;
      ++c.phase;
      break;
    case 216:
      c.transform.math = {};
      c.transform.math.function = 0x8029e264;
      c.transform.math.left = c.matrix;
      c.transform.math.xyz = {c.offsetX, c.literal, c.offsetZ};
      ++c.phase;
      break;
    case 217:
      if (!m.matrix || !m.matrix(m.user, c.transform.math))
        return false;
      c.matrix = c.transform.math.output;
      ++c.phase;
      break;
    case 218:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 219:
      if (!call(0x80040730, {c.handle, 0x40, 0}))
        return false;
      ++c.phase;
      break;
    case 220:
      if (!word(0x8041fc68, c.color))
        return false;
      c.color = (c.color & 0xffffff00) | uint8_t(128 - c.alphaCounter);
      ++c.phase;
      break;
    case 221:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 222:
      if (!call(0x800406b4, {c.handle, 0, 0}))
        return false;
      c.pass = 1;
      ++c.phase;
      break;
    case 223:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 224:
      if (!load(0x8041fd34, c.literal))
        return false;
      ++c.phase;
      break;
    case 225:
      if (!call(0x8003b4d8, {c.handle, 0, c.pass}, {c.incoming, c.literal}))
        return false;
      if (c.pass < 3) {
        ++c.pass;
        c.phase = 223;
      } else {
        --c.history;
        c.alphaCounter -= 25;
        c.phase = c.history ? 214 : 226;
      }
      break;
    case 226:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 227:
      if (!call(0x80040710, {c.handle, 0x40, 0}))
        return false;
      ++c.phase;
      break;
    case 228:
      if (!word(c.actor + 0x104, c.handle))
        return false;
      ++c.phase;
      break;
    case 229:
      if (!call(0x80040730, {c.handle, c.savedFlags, 0}))
        return false;
      c = {};
      return true;
    default:
      return false;
    }
}
} // namespace ttyd
