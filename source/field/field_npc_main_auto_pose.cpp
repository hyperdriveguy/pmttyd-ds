#include "field_npc_main_auto_pose.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Exact float bits for words/retry identity; single() forces binary32
// rounding, preserving separate squares plus add instead of a fused norm.
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
// Original automatic pose only, not animation Main/evaluation. Actor main400
// enables; fresh dt1a4>ZERO admits. Descriptor28 supplies idle/walk/run names
// at10/18/1c and run threshold3c. Actor phase108 is1/2/3; handle104 is separate.
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
    // Separate rounded dx*dx,dz*dz,then sum. Inline norm owns its three double
    // refinements and canonical NaN branch; caller must not replace with hypotf.
    // Reload dt AFTER the norm child, so provider mutations remain observable.
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
    // Original idle threshold8041fcdc bits3dcccccd(~.1); strict speed<idle
    // selects stay1. Otherwise captured descriptor3c*retainedFactor chooses
    // walk2/run3 with strict comparison; unordered speed/threshold selects run.
    case 21:
      if (!load(0x8041fcdc, c.idle))
        return false;
      c.speed = single(c.norm / c.dt);
      c.phase = c.speed < c.idle ? 22 : 30;
      break;
    // Stay checks phase BEFORE descriptor lookup. Walk/run capture descriptor
    // for threshold BEFORE phase check and reuse it for later name load.
    // Equal phase skips child AND phase store; no forced restart in that case.
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
    // Force1 source selection request; read name at its original later point.
    // Acknowledged child effects precede phase108 store; a held store must not
    // replay animation selection. Completion retains phase90 for parent reuse.
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
