#include "field_npc_main_auto_pose.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
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
bool advanceFieldNpcMainAutoPose(uint32_t actor, float dx, float dz,
                                 float factor,
                                 const FieldNpcAutoPoseServices &s,
                                 FieldNpcAutoPoseCursor &c) {
  if (!s.memory.word || !s.memory.store)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
    c.dx = dx;
    c.dz = dz;
    c.factor = factor;
  }
  if (c.actor != actor || bits(c.dx) != bits(dx) || bits(c.dz) != bits(dz) ||
      bits(c.factor) != bits(factor) || c.owner.memory.user != s.memory.user ||
      c.owner.memory.word != s.memory.word ||
      c.owner.memory.store != s.memory.store ||
      c.owner.doubleWord != s.doubleWord || c.owner.estimate != s.estimate ||
      c.owner.request != s.request)
    return false;
  auto read = [&](uint32_t a, uint32_t &v) {
    const auto w = s.memory.word(s.memory.user, a);
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
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(actor, c.value))
        return false;
      c.phase = c.value & 0x400 ? 1 : 90;
      break;
    case 1:
      if (!load(actor + 0x1a4, c.dt))
        return false;
      ++c.phase;
      break;
    case 2:
      if (!load(0x8041fc7c, c.zero))
        return false;
      c.phase = c.dt > c.zero ? 3 : 90;
      break;
    case 3: {
      const float x2 = single(c.dx * c.dx);
      const float z2 = single(c.dz * c.dz);
      c.norm = single(x2 + z2);
      c.phase = 4;
      break;
    }
    case 4:
      if (!advanceFieldNpcInlineNorm(c.norm, c.zero,
                                     {s.memory, s.doubleWord, s.estimate},
                                     c.normCursor, c.speed))
        return false;
      c.norm = c.speed;
      c.phase = 20;
      break;
    case 20:
      if (!load(actor + 0x1a4, c.dt))
        return false;
      ++c.phase;
      break;
    case 21:
      if (!load(0x8041fcdc, c.idle))
        return false;
      c.speed = single(c.norm / c.dt);
      c.phase = c.speed < c.idle ? 22 : 30;
      break;
    case 22:
      c.selectedPhase = 1;
      c.nameOffset = 0x10;
      if (!read(actor + 0x108, c.value))
        return false;
      c.phase = c.value == 1 ? 90 : 23;
      break;
    case 23:
      if (!read(actor + 0x28, c.descriptor))
        return false;
      c.phase = 40;
      break;
    case 30:
      if (!read(actor + 0x28, c.descriptor))
        return false;
      ++c.phase;
      break;
    case 31:
      if (!load(c.descriptor + 0x3c, c.threshold))
        return false;
      c.threshold = single(c.threshold * c.factor);
      c.selectedPhase = c.speed < c.threshold ? 2 : 3;
      c.nameOffset = c.selectedPhase == 2 ? 0x18 : 0x1c;
      ++c.phase;
      break;
    case 32:
      if (!read(actor + 0x108, c.value))
        return false;
      c.phase = c.value == c.selectedPhase ? 90 : 40;
      break;
    case 40:
      if (!read(actor + 0x104, c.call.args[0]))
        return false;
      c.call.function = 0x80041170;
      c.call.args[2] = 1;
      ++c.phase;
      break;
    case 41:
      if (!read(c.descriptor + c.nameOffset, c.call.args[1]))
        return false;
      ++c.phase;
      break;
    case 42:
      if (!s.request || !s.request(s.memory.user, c.call))
        return false;
      ++c.phase;
      break;
    case 43:
      if (!s.memory.store(s.memory.user, actor + 0x108, c.selectedPhase))
        return false;
      c.phase = 90;
      break;
    case 90:
      return true;
    default:
      return false;
    }
}
} // namespace ttyd
