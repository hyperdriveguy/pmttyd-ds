#include "field_npc_main_movement.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
float rounded(float f) {
  volatile float v = f;
  return v;
}
} // namespace
FieldNpcMoveFrontier
advanceFieldNpcMainMovement(uint32_t actor, const FieldNpcMainMoveServices &s,
                            FieldNpcMainMoveCursor &c) {
  using R = FieldNpcMoveFrontier;
  if (!s.memory.word || !s.memory.store)
    return R::Pending;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.actor = actor;
  }
  if (actor != c.actor || s.memory.user != c.owner.memory.user ||
      s.memory.word != c.owner.memory.word ||
      s.memory.store != c.owner.memory.store || s.request != c.owner.request ||
      s.pair != c.owner.pair)
    return R::Pending;
  auto read = [&](uint32_t a, uint32_t &v) {
    const auto w = s.memory.word(s.memory.user, a);
    if (!w)
      return false;
    v = *w;
    return true;
  };
  auto load = [&](uint32_t a, float &f) {
    uint32_t w;
    if (!read(a, w))
      return false;
    f = real(w);
    return true;
  };
  auto store = [&](uint32_t a, uint32_t w) {
    return s.memory.store(s.memory.user, a, w);
  };
  auto call = [&] {
    return s.request && s.request(s.memory.user, c.call, c.response);
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(actor + 0x1d4, c.state))
        return R::Pending;
      c.phase = c.state & 0x10 ? 1 : 90;
      break;
    case 1:
      if (!load(actor + 0x1a4, c.x))
        return R::Pending;
      ++c.phase;
      break;
    case 2:
      if (!load(0x8041fc7c, c.zero))
        return R::Pending;
      c.phase = c.x > c.zero ? 3 : 30;
      break;
    case 3:
      if (!load(actor + 0x1b4, c.dx))
        return R::Pending;
      ++c.phase;
      break;
    case 4:
      if (!load(actor + 0x1bc, c.dz))
        return R::Pending;
      c.phase = (c.state & 0x100000) && !(c.state & 0x10000000) ? 5 : 6;
      break;
    case 5:
      if (!load(0x8041fc80, c.x))
        return R::Pending;
      c.dx = rounded(c.dx * c.x);
      c.dz = rounded(c.dz * c.x);
      ++c.phase;
      break;
    case 6:
      if (!read(actor, c.flags))
        return R::Pending;
      c.phase = c.flags & 0x08000000 ? 7 : 8;
      break;
    case 7:
      if (!load(0x8041fc7c, c.dz))
        return R::Pending;
      ++c.phase;
      break;
    case 8:
      c.phase = c.flags & 0x40000000 ? 9 : 13;
      break;
    case 9:
      if (!load(actor + 0x8c, c.x))
        return R::Pending;
      c.x = rounded(c.x + c.dx);
      ++c.phase;
      break;
    case 10:
      if (!store(actor + 0x8c, bits(c.x)))
        return R::Pending;
      ++c.phase;
      break;
    case 11:
      if (!load(actor + 0x94, c.z))
        return R::Pending;
      c.z = rounded(c.z + c.dz);
      ++c.phase;
      break;
    case 12:
      if (!store(actor + 0x94, bits(c.z)))
        return R::Pending;
      c.phase = 26;
      break;
    case 13:
      if (!load(actor + 0x8c, c.x))
        return R::Pending;
      ++c.phase;
      break;
    case 14:
      if (!load(actor + 0x94, c.z))
        return R::Pending;
      c.call = {0x80048418,
                {actor, 0, 0, actor + 0x1ac},
                FieldNpcMoveOutput::TemporaryXZ,
                c.dx,
                c.dz,
                c.x,
                c.z};
      ++c.phase;
      break;
    case 15:
      if (!call())
        return R::Pending;
      c.x = c.response.x;
      c.z = c.response.z;
      ++c.phase;
      break;
    case 16:
      if (!store(actor + 0x2fc, c.response.contact))
        return R::Pending;
      ++c.phase;
      break;
    case 17:
      if (!read(actor + 0x2fc, c.word))
        return R::Pending;
      c.phase = c.word ? 18 : 24;
      break;
    case 18:
      if (!load(actor + 0x8c, c.dx))
        return R::Pending;
      ++c.phase;
      break;
    case 19:
      if (!load(actor + 0x94, c.dz))
        return R::Pending;
      c.call = {0x80048418,
                {actor, actor + 0x8c, actor + 0x94, actor + 0x1ac},
                FieldNpcMoveOutput::ActorXZ,
                rounded(c.x - c.dx),
                rounded(c.z - c.dz),
                0,
                0};
      ++c.phase;
      break;
    case 20:
      if (!call())
        return R::Pending;
      c.phase = 26;
      break; // return discarded; actor outputs already published
    case 24:
      if (!store(actor + 0x8c, bits(c.x)))
        return R::Pending;
      ++c.phase;
      break;
    case 25:
      if (!store(actor + 0x94, bits(c.z)))
        return R::Pending;
      ++c.phase;
      break;
    case 26:
      if (!read(0x8041fc7c, c.word))
        return R::Pending;
      ++c.phase;
      break;
    case 27:
      if (!store(actor + 0x1b4, c.word))
        return R::Pending;
      ++c.phase;
      break;
    case 28:
      if (!store(actor + 0x1bc, c.word))
        return R::Pending;
      c.call = {0x80048d9c, {actor, 0, 0, 0}};
      ++c.phase;
      break;
    case 29:
      if (!call())
        return R::Pending;
      ++c.phase;
      break;
    case 30:
      if (!read(actor, c.flags))
        return R::Pending;
      c.call = {0x800490cc, {actor, 0, 0, 0}};
      c.phase = c.flags & (8 | 0x20 | 0x4000) ? 32 : 31;
      break;
    case 31:
      if (!call())
        return R::Pending;
      ++c.phase;
      break;
    // Original paired XY loads, local XY subtraction/store, then fresh Z loads.
    case 32: {
      const auto v =
          s.pair ? s.pair(s.memory.user, actor + 0x8c) : std::nullopt;
      if (!v)
        return R::Pending;
      for (unsigned i = 0; i < 2; ++i)
        c.current[i] = real((*v)[i]);
      c.phase = 34;
      break;
    }
    case 34: {
      const auto v =
          s.pair ? s.pair(s.memory.user, actor + 0x98) : std::nullopt;
      if (!v)
        return R::Pending;
      for (unsigned i = 0; i < 2; ++i) {
        c.previous[i] = real((*v)[i]);
        c.delta[i] = rounded(c.current[i] - c.previous[i]);
      }
      c.phase = 36;
      break;
    }
    case 36:
      if (!load(actor + 0x94, c.current[2]))
        return R::Pending;
      ++c.phase;
      break;
    case 37:
      if (!load(actor + 0xa0, c.previous[2]))
        return R::Pending;
      c.delta[2] = rounded(c.current[2] - c.previous[2]);
      ++c.phase;
      break;
    case 38:
      return R::AutomaticPose; // next source load80049fbc
    case 90:
      return R::Detection; // S10 clear skips directly to8004a160
    default:
      return R::Pending;
    }
}
} // namespace ttyd
