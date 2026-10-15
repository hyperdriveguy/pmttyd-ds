#include "field_npc_mario_detection.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
double realDouble(uint64_t w) {
  double f;
  std::memcpy(&f, &w, 8);
  return f;
}
float single(float f) {
  volatile float v = f;
  return v;
}
double wide(double f) {
  volatile double v = f;
  return v;
}
} // namespace
bool advanceFieldNpcMarioDetection(uint32_t actor, uint8_t cls,
                                   const FieldNpcMarioServices &s,
                                   FieldNpcMarioCursor &c) {
  const auto &m = s.norm.memory;
  if (!m.word || !m.store)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
    c.eventClass = cls;
  }
  if (c.actor != actor || c.eventClass != cls ||
      c.owner.norm.memory.user != m.user ||
      c.owner.norm.memory.word != m.word ||
      c.owner.norm.memory.store != m.store ||
      c.owner.norm.doubleWord != s.norm.doubleWord ||
      c.owner.norm.estimate != s.norm.estimate || c.owner.half != s.half ||
      c.owner.pair != s.pair || c.owner.request != s.request)
    return false;
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
  auto fail = [&] {
    if (cls == 2) {
      c.trigger = 2;
      c.phase = 900;
    } else
      c.phase = 910;
  };
  auto vectorXY = [&](uint32_t a, std::array<float, 3> &v) {
    auto p = s.pair ? s.pair(m.user, a) : std::nullopt;
    if (!p)
      return false;
    v[0] = real((*p)[0]);
    v[1] = real((*p)[1]);
    return true;
  };
  auto norm = [&] {
    return advanceFieldNpcInlineNorm(c.squared, c.zero, s.norm, c.normCursor,
                                     c.distance);
  };
  for (;;)
    switch (c.phase) {
    case 0:
      c.phase = cls == 2 ? 10 : (cls == 1 || cls == 4 ? 200 : 910);
      break;
    case 10:
      c.call = {0x8005e0c0};
      ++c.phase;
      break;
    case 11:
      if (!request())
        return false;
      c.mario = c.response.word;
      ++c.phase;
      break;
    case 12:
      if (!load(c.mario + 0x8c, c.player[0]))
        return false;
      ++c.phase;
      break;
    case 13:
      if (!load(actor + 0x8c, c.root[0]))
        return false;
      ++c.phase;
      break;
    case 14:
      if (!load(c.mario + 0x90, c.player[1]))
        return false;
      ++c.phase;
      break;
    case 15:
      if (!load(actor + 0x90, c.root[1]))
        return false;
      c.dx = single(c.player[0] - c.root[0]);
      ++c.phase;
      break;
    case 16:
      if (!load(c.mario + 0x94, c.player[2]))
        return false;
      ++c.phase;
      break;
    case 17:
      if (!load(actor + 0x94, c.root[2]))
        return false;
      c.dy = single(c.player[1] - c.root[1]);
      c.dz = single(c.player[2] - c.root[2]);
      ++c.phase;
      break;
    case 18:
      if (!vectorXY(actor + 0x98, c.previous))
        return false;
      ++c.phase;
      break;
    case 19:
      if (!vectorXY(actor + 0x8c, c.current))
        return false;
      for (unsigned i = 0; i < 2; ++i)
        c.change[i] = single(c.previous[i] - c.current[i]);
      ++c.phase;
      break;
    case 20:
      if (!load(actor + 0xa0, c.previous[2]))
        return false;
      ++c.phase;
      break;
    case 21:
      if (!load(actor + 0x94, c.current[2]))
        return false;
      c.change[2] = single(c.previous[2] - c.current[2]);
      ++c.phase;
      break;
    case 22:
      if (!read(actor + 0x1d4, c.value))
        return false;
      ++c.phase;
      break;
    case 23:
      if (!load(actor + 0x144, c.facing))
        return false;
      c.phase = c.value & 2 ? 24 : 100;
      break;
    case 24:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.squared = single(single(c.change[0] * c.change[0]) +
                         single(c.change[2] * c.change[2]));
      c.normCursor = {};
      ++c.phase;
      break;
    case 25:
      if (!norm())
        return false;
      ++c.phase;
      break;
    case 26:
      if (!load(0x8041fd54, c.threshold))
        return false;
      c.phase = c.distance > c.threshold ? 27 : 100;
      break;
    case 27:
      if (!load(actor + 0x1ac, c.facing))
        return false;
      c.phase = 100;
      break;
    // Two distinct source copies of Mario admission, at their original points.
    case 100:
      c.call = {0x8005e0c0};
      ++c.phase;
      break;
    case 101:
      if (!request())
        return false;
      c.admissionMario = c.response.word;
      ++c.phase;
      break;
    case 102:
      if (!read(c.admissionMario, c.value))
        return false;
      if (c.value & (0x82000000u | 0xa))
        fail();
      else
        ++c.phase;
      break;
    case 103:
      if (!read(0x80415fd0, c.gp))
        return false;
      ++c.phase;
      break;
    case 104:
      if (!read(c.gp + 0x164, c.value))
        return false;
      if (c.value)
        fail();
      else
        ++c.phase;
      break;
    case 105:
      c.call = {0x8002e210};
      ++c.phase;
      break;
    case 106:
      if (!request())
        return false;
      if (c.response.word != 2)
        fail();
      else
        ++c.phase;
      break;
    case 107: {
      auto h = s.half ? s.half(m.user, c.admissionMario + 0x2e) : std::nullopt;
      if (!h)
        return false;
      if (*h == 25)
        fail();
      else
        ++c.phase;
      break;
    }
    case 108:
      c.call = {0x801865b8};
      ++c.phase;
      break;
    case 109:
      if (!request())
        return false;
      c.phase = c.response.word ? 110 : (cls == 2 ? 120 : 300);
      break;
    case 110:
      c.call = {0x801865b8};
      ++c.phase;
      break;
    case 111:
      if (!request())
        return false;
      if (c.response.word == 1)
        c.phase = cls == 2 ? 120 : 300;
      else
        fail();
      break;
    case 120:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.squared = std::fma(c.dx, c.dx, single(c.dz * c.dz));
      c.normCursor = {};
      ++c.phase;
      break;
    case 121:
      if (!norm())
        return false;
      ++c.phase;
      break;
    case 122:
      if (!load(0x8041fd58, c.threshold))
        return false;
      ++c.phase;
      break;
    case 123:
      if (!load(actor + 0x228, c.range))
        return false;
      c.phase = c.distance > single(c.threshold * c.range) ? 124 : 910;
      break;
    case 124:
      if (!load(actor + 0x20c, c.threshold))
        return false;
      if (c.dy > c.threshold)
        fail();
      else
        ++c.phase;
      break;
    case 125:
      if (!load(actor + 0x218, c.threshold))
        return false;
      if (-c.dy > c.threshold)
        fail();
      else
        ++c.phase;
      break;
    case 126:
      if (c.distance > c.range)
        fail();
      else
        ++c.phase;
      break;
    case 127:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.call = {0x8000703c, {}, {c.zero, c.zero, c.dx, c.dz}};
      ++c.phase;
      break;
    case 128:
      if (!request())
        return false;
      c.call = {0x800071f0, {}, {c.facing, c.response.scalar, 0, 0}};
      ++c.phase;
      break;
    case 129:
      if (!request())
        return false;
      c.angle = c.response.scalar;
      ++c.phase;
      break;
    case 130:
      if (!load(0x8041fc80, c.halfWidth))
        return false;
      c.angle = std::fabs(c.angle);
      ++c.phase;
      break;
    case 131:
      if (!load(actor + 0x22c, c.threshold))
        return false;
      if (c.angle > single(c.halfWidth * c.threshold))
        fail();
      else
        c.phase = 910;
      break;
    // Proximity captures Z before X and computes its norm BEFORE eligibility.
    case 200:
      c.call = {0x8005e0c0};
      ++c.phase;
      break;
    case 201:
      if (!request())
        return false;
      c.mario = c.response.word;
      ++c.phase;
      break;
    case 202:
      if (!load(c.mario + 0x94, c.player[2]))
        return false;
      ++c.phase;
      break;
    case 203:
      if (!load(actor + 0x94, c.root[2]))
        return false;
      ++c.phase;
      break;
    case 204:
      if (!load(c.mario + 0x8c, c.player[0]))
        return false;
      c.dz = single(c.player[2] - c.root[2]);
      ++c.phase;
      break;
    case 205:
      if (!load(actor + 0x8c, c.root[0]))
        return false;
      ++c.phase;
      break;
    case 206:
      if (!load(c.mario + 0x90, c.player[1]))
        return false;
      c.dx = single(c.player[0] - c.root[0]);
      ++c.phase;
      break;
    case 207:
      if (!load(actor + 0x90, c.root[1]))
        return false;
      ++c.phase;
      break;
    case 208:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.dy = single(c.player[1] - c.root[1]);
      c.squared = std::fma(c.dx, c.dx, single(c.dz * c.dz));
      c.normCursor = {};
      ++c.phase;
      break;
    case 209:
      if (!norm())
        return false;
      ++c.phase;
      break;
    case 210:
      if (!read(actor, c.value))
        return false;
      c.phase = c.value & 0x08000000 ? 211 : 220;
      break;
    case 211:
    case 212:
    case 213:
    case 214:
      if (!read(0x802c1210 + (c.phase - 211) * 4, c.radii[c.phase - 211]))
        return false;
      ++c.phase;
      break;
    case 215:
      c.call = {0x801ff028};
      ++c.phase;
      break;
    case 216:
      if (!request())
        return false;
      c.value = c.response.word;
      ++c.phase;
      break;
    case 217:
      if (c.value >= 4)
        return false; // source local table index domain; never clamp
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.distance = single(c.distance - real(c.radii[c.value]));
      if (c.distance < c.zero)
        c.distance = c.zero;
      c.phase = 220;
      break;
    case 220:
      if (!vectorXY(actor + 0x98, c.previous))
        return false;
      ++c.phase;
      break;
    case 221:
      if (!vectorXY(actor + 0x8c, c.current))
        return false;
      for (unsigned i = 0; i < 2; ++i)
        c.change[i] = single(c.previous[i] - c.current[i]);
      ++c.phase;
      break;
    case 222:
      if (!load(actor + 0xa0, c.previous[2]))
        return false;
      ++c.phase;
      break;
    case 223:
      if (!load(actor + 0x94, c.current[2]))
        return false;
      c.change[2] = single(c.previous[2] - c.current[2]);
      ++c.phase;
      break;
    case 224:
      if (!load(actor + 0x144, c.facing))
        return false;
      c.call = {0x80007328, {}, {c.facing, 0, 0, 0}};
      ++c.phase;
      break;
    case 225:
      if (!request())
        return false;
      c.facing = c.response.scalar;
      ++c.phase;
      break;
    case 226:
      if (!load(0x8041fccc, c.threshold))
        return false;
      c.phase = c.facing < c.threshold ? 227 : 228;
      break;
    case 227:
      if (!load(0x8041fcb0, c.facing))
        return false;
      c.phase = 100;
      break;
    case 228:
      if (!load(0x8041fcc4, c.facing))
        return false;
      c.phase = 100;
      break;
    case 300:
      if (!load(0x8041fd58, c.threshold))
        return false;
      ++c.phase;
      break;
    case 301:
      if (!load(actor + 0x220, c.range))
        return false;
      c.phase = c.distance > single(c.threshold * c.range) ? 302 : 310;
      break;
    case 302:
      if (c.distance > c.range)
        c.phase = 910;
      else
        ++c.phase;
      break;
    case 303:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.call = {0x8000703c, {}, {c.zero, c.zero, c.dx, c.dz}};
      ++c.phase;
      break;
    case 304:
      if (!request())
        return false;
      c.call = {0x800071f0, {}, {c.facing, c.response.scalar, 0, 0}};
      ++c.phase;
      break;
    case 305:
      if (!request())
        return false;
      c.angle = c.response.scalar;
      ++c.phase;
      break;
    case 306:
      if (!load(0x8041fc80, c.halfWidth))
        return false;
      c.angle = std::fabs(c.angle);
      ++c.phase;
      break;
    case 307:
      if (!load(actor + 0x224, c.threshold))
        return false;
      c.phase = c.angle > single(c.halfWidth * c.threshold) ? 910 : 310;
      break;
    case 310:
      if (!load(actor + 0x20c, c.threshold))
        return false;
      c.phase = c.dy > c.threshold ? 910 : 311;
      break;
    case 311:
      if (!load(actor + 0x218, c.threshold))
        return false;
      c.phase = -c.dy > c.threshold ? 910 : (cls == 4 ? 320 : 890);
      break;
    case 320:
      if (!read(actor + 0x1f8, c.value))
        return false;
      c.phase = c.value == 1 ? 330 : 360;
      break;
    case 330:
      if (!load(actor + 0x204, c.z))
        return false;
      ++c.phase;
      break;
    case 331:
      if (!load(actor + 0x94, c.extent))
        return false;
      ++c.phase;
      break;
    case 332:
      if (!load(actor + 0x1fc, c.x))
        return false;
      c.z = single(c.z - c.extent);
      ++c.phase;
      break;
    case 333:
      if (!load(actor + 0x8c, c.extent))
        return false;
      ++c.phase;
      break;
    case 334:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.x = single(c.x - c.extent);
      c.squared = std::fma(c.x, c.x, single(c.z * c.z));
      c.normCursor = {};
      ++c.phase;
      break;
    case 335:
      if (!norm())
        return false;
      ++c.phase;
      break;
    case 336: {
      auto w = s.norm.doubleWord ? s.norm.doubleWord(m.user, 0x802c1460)
                                 : std::nullopt;
      if (!w)
        return false;
      c.doubleScale = realDouble(*w);
      ++c.phase;
      break;
    }
    case 337:
      if (!load(actor + 0x214, c.extent))
        return false;
      c.phase = double(c.distance) > wide(c.doubleScale * double(c.extent))
                    ? 910
                    : 890;
      break;
    case 360:
      if (!load(0x8041fcd4, c.scale))
        return false;
      ++c.phase;
      break;
    case 361:
      if (!load(actor + 0x214, c.extent))
        return false;
      ++c.phase;
      break;
    case 362:
      if (!load(actor + 0x1fc, c.x))
        return false;
      c.extent = single(c.scale * c.extent);
      ++c.phase;
      break;
    case 363:
      if (!load(actor + 0x8c, c.threshold))
        return false;
      c.phase = c.threshold < single(c.x - c.extent) ||
                        c.threshold > single(c.x + c.extent)
                    ? 910
                    : 364;
      break;
    case 364:
      if (!load(actor + 0x21c, c.extent))
        return false;
      ++c.phase;
      break;
    case 365:
      if (!load(actor + 0x204, c.z))
        return false;
      c.extent = single(c.scale * c.extent);
      ++c.phase;
      break;
    case 366:
      if (!load(actor + 0x94, c.threshold))
        return false;
      c.phase = c.threshold < single(c.z - c.extent) ||
                        c.threshold > single(c.z + c.extent)
                    ? 910
                    : 890;
      break;
    case 890:
      c.trigger = 1;
      c.phase = 900;
      break;
    case 900:
      if (!read(actor + 0x140, c.value))
        return false;
      ++c.phase;
      break;
    case 901:
      if (!m.store(m.user, actor + 0x140, c.value | c.trigger))
        return false;
      c.phase = 910;
      break;
    case 910:
      c.call = {0x800440f8, {actor, 0}};
      ++c.phase;
      break;
    case 911:
      if (!request())
        return false;
      ++c.phase;
      break;
    case 912:
      return true; // next fresh source mainflags8004accc, Display unfinished
    default:
      return false;
    }
}
} // namespace ttyd
