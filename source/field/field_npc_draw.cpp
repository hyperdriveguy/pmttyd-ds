#include "field_npc_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
// Guest float bit copy. Source comparisons/loads below remain observable;
// native angle/SDK/GPU providers must explicitly admit their numeric domains.
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
} // namespace
// Five original callback identities:ordinary c50c,xlu c7e0,offscreen bf28,
// offscreen-xlu c234,blur bc3c (all8004xxxx). Unsupported fresh callback leaves
// cursor untouched. Capture source arguments/providers once, rather than
// retargeting an active draw when later invocation parameters change.
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
  // Child request function/raw registers/scalars/copied matrix/color/bounds
  // stay pinned on false. args[1]=0 for draw replaces ONLY source stack matrix
  // address; actual matrix is native payload. No fake guest pointer is supplied.
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
    // Negate event yaw144,revise80007328,then full matrix at current slot-1.
    // Source state1d4 then color114 both load even when darkening is disabled.
    //00400000 halves copied unsigned RGB(mask7f7f7f00),preserves alpha; fresh
    // handle104 for color406b4 then another handle for material FlagOn40730/40.
    // Ordinary return does not restore that color/material state.
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
    // Ordinary reads actual queue work80010864,class byte+1:class0 runs
    // facing-name/Main/pass1;classes1/2 draw passes2/3 only;others finish prelude.
    // Xlu ignores queue class and runs facing/Main then passes1,2,3.
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
    // Main00800000 enables name-facing. Preserve discarded GetCurrentAnim
    //39c44 call,then fresh handle/GetAnimPosePtr39cf8,then fresh handle/name.
    // Camera4 loads atX18,eyeXc,atZ20,eyeZ14 before negated differences/atan2.
    // Double atan2 result narrows to FLOAT before source factorfcbc multiplication.
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
    // Subtract retained pose+74 then revise. Rear iff(d>90&&d<=270) OR
    //(d<-90&&d>=-270);NaN is front. Literals atfcb0/fcc4/fcb8/fcc8 retain
    // asymmetric strict/inclusive boundaries; no generic angle normalization.
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
    // Original803066f8 table:14 pairs,stride8. Rear compares left/selects
    // right;front compares right/selects left. First strcmp match only; fresh
    // opposite pointer and handle go to SetAnim41170(force0). No match skips it.
    // Table/name identities are guest pointers, not native text metadata.
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
    // Animation Main3ed2c and EVERY draw3b4d8 reload handle104 independently.
    // Each draw reloads source TWO(fd34),retaining incoming A and matrix.
    // Ordinary current-class pass differs from xlu's consecutive1..3 sequence.
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
    // Offscreen wraps ordinary/xlu body with sync05ce4/clearBBox2b69d0,
    // then sync/readBBox2b6a08 on EVERY body exit(even unsupported queue class).
    // Copy four u16 bounds,load fresh alternate110,forward to72f38. Draws still
    // use PRIMARY104; no active/-1 gate or synthetic model-space bounds added.
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
    // Blur saves actual material flags4069c,queries dispatched cameraId TWICE
    // for independent yaw114 sine/cosine graphs. X=TWO*sin;Z=TWO*(-cos),with
    // fresh TWO and primary handle load before final Z multiply. Main runs once.
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
    // History4,3,2,1:full matrix per sample then SDK TransApply29e264 with
    // retained X/Z offsets and fresh ZERO Y. Stable SDK payload survives holds.
    // No history0 draw and no repeated Main per historical sample.
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
    // Original color WORDfc68 with alpha128-counter:counter100/75/50/25
    // gives28/53/78/103. Four samples each run passes1..3(twelve draw calls),
    // each with fresh handle and TWO. Only alpha replaces the copied color word.
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
    // Footer clears mask40 on fresh handle,then reloads and ORs saved flags.
    // It does NOT assign old flags or erase unrelated flags added by children.
    // Final event color(alpha103) remains; no color restore. Completion resets.
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
