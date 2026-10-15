#include "battle_class_footer_backend.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
bool BattleClassFooterAssets::load(const char *path) {
  FILE *f = path ? std::fopen(path, "rb") : nullptr;
  if (!f)
    return false;
  uint8_t data[12 + 13 * 28];
  const bool ok = std::fread(data, 1, sizeof(data), f) == sizeof(data) &&
                  std::fgetc(f) == EOF;
  std::fclose(f);
  if (!ok || std::memcmp(data, "PMBCLF01", 8))
    return false;
  const auto word = [&](unsigned o) {
    return uint32_t(data[o]) | uint32_t(data[o + 1]) << 8 |
           uint32_t(data[o + 2]) << 16 | uint32_t(data[o + 3]) << 24;
  };
  if (word(8) != 13)
    return false;
  std::array<BattleClassRegion, 13> regions;
  for (unsigned i = 0; i < 13; ++i) {
    const auto at = 12 + 28 * i, index = i + 4;
    if (word(at) != index)
      return false;
    auto &r = regions[i];
    r.width = word(at + 4);
    r.height = word(at + 8);
    if (r.width != (index < 6 ? 80u : 112u) || r.height != (index < 6    ? 32u
                                                            : index < 11 ? 48u
                                                                         : 56u))
      return false;
    float *values[] = {&r.u0, &r.v0, &r.u1, &r.v1};
    for (unsigned j = 0; j < 4; ++j) {
      const auto bits = word(at + 12 + 4 * j);
      std::memcpy(values[j], &bits, 4);
      if (!std::isfinite(*values[j]) || *values[j] < 0 || *values[j] > 1)
        return false;
    }
    const float x = float(2 + 116 * (i % 4)), y = float(2 + 60 * (i / 4));
    if (r.u0 != x / 512 || r.v0 != 1 - y / 256 || r.u1 != (x + r.width) / 512 ||
        r.v1 != 1 - (y + r.height) / 256)
      return false;
  }
  regions_ = regions;
  ready_ = true;
  return true;
}

namespace {
using F = BattleClassRasterService;
bool same(const BattleClassFooterServices &a,
          const BattleClassFooterServices &b) {
  const auto &x = a.raster;
  const auto &y = b.raster;
  return a.user == b.user && a.project == b.project &&
         a.audienceItem == b.audienceItem &&
         a.changePosition == b.changePosition && a.sound == b.sound &&
         a.button == b.button && x.user == y.user && x.request == y.request &&
         x.memory.user == y.memory.user && x.memory.byte == y.memory.byte &&
         x.memory.memory.user == y.memory.memory.user &&
         x.memory.memory.word == y.memory.memory.word &&
         x.memory.memory.half == y.memory.memory.half &&
         x.memory.memory.storeWord == y.memory.memory.storeWord &&
         x.math.user == y.math.user && x.math.rotation == y.math.rotation &&
         x.math.translation == y.math.translation &&
         x.math.concat == y.math.concat && x.math.scale == y.math.scale;
}
bool enter(const BattleClassFooterServices &s, BattleClassFooterCursor &c,
           uint32_t op) {
  if (c.operation)
    return c.operation == op && same(s, c.owner);
  c = {};
  c.operation = op;
  c.owner = s;
  return true;
}
uint32_t word(const BattleClassFooterServices &s, uint32_t a) {
  const auto &m = s.raster.memory.memory;
  return m.word(m.user, a);
}
float real(const BattleClassFooterServices &s, uint32_t a) {
  const auto n = word(s, a);
  float v;
  std::memcpy(&v, &n, 4);
  return v;
}
float k(const BattleClassFooterServices &s, int32_t o) {
  return real(s, 0x80427440u + uint32_t(o));
}
void putFloat(const BattleClassFooterServices &s, uint32_t a, float v) {
  uint32_t n;
  std::memcpy(&n, &v, 4);
  const auto &m = s.raster.memory.memory;
  m.storeWord(m.user, a, n);
}
int32_t half(const BattleClassFooterServices &s, uint32_t a) {
  const auto &m = s.raster.memory.memory;
  const auto n = m.half(m.user, a);
  return n < 32768 ? int32_t(n) : int32_t(n) - 65536;
}
void decrement(const BattleClassFooterServices &s, uint32_t w) {
  const float v = real(s, w + 0x120), zero = k(s, -20500);
  putFloat(s, w + 0x120, v > zero ? v - k(s, -20368) : zero);
}
template <class Build>
std::optional<uint32_t> send(const BattleClassFooterServices &s,
                             BattleClassFooterCursor &c, Build build) {
  if (!c.queued) {
    c.request = build();
    c.queued = true;
  }
  const auto v = s.raster.request ? s.raster.request(s.raster.user, c.request)
                                  : std::nullopt;
  if (v)
    c.queued = false;
  return v;
}
BattleClassRasterRequest request(F f, std::vector<uint32_t> n = {},
                                 std::vector<float> v = {},
                                 std::optional<BattleActMatrix> m = {}) {
  return {f, std::move(n), std::move(v), std::move(m)};
}
bool finish(BattleClassFooterCursor &c) {
  c = {};
  return true;
}
// Retains each child independently; parent order survives both child waits.
bool pinchChild(const BattleClassFooterServices &s,
                BattleClassFooterCursor &c) {
  const auto &math = s.raster.math;
  for (;;)
    switch (c.child) {
    case 0:
      c.battle = currentBattleAddress(s.raster.memory);
      c.child = 1;
      break;
    case 1: {
      const auto v = send(s, c, [] { return request(F::Camera, {8}); });
      if (!v)
        return false;
      c.camera = *v;
      c.unit = c.partyChild ? battlePartyUnit(c.battle, s.raster.memory)
                            : battleMarioUnit(c.battle, s.raster.memory);
      if (c.partyChild && !c.unit) {
        c.child = 0;
        return true;
      }
      const auto hp = half(s, c.unit + 0x10c);
      if (hp <= 0 || hp >= half(s, c.unit + 0x108) || hp > 5) {
        c.child = 0;
        return true;
      }
      c.color = 0xffe940ff;
      const auto gp = word(s, 0x80415fd0);
      if (hp == 1) {
        const auto clock = word(s, gp + 0x1c), language = word(s, gp + 0x16c);
        c.index = s.raster.memory.memory.half(s.raster.memory.memory.user,
                                              0x80416d30 + language * 2);
        if (clock % 50 > 24)
          c.color = 0xe9522fff;
      } else {
        const auto language = word(s, gp + 0x16c);
        c.index = s.raster.memory.memory.half(s.raster.memory.memory.user,
                                              0x80416d24 + language * 2);
      }
      c.texture = word(s, word(s, word(s, c.battle + 0x163f8) + 0xa0));
      c.child = 2;
      break;
    }
    case 2: {
      const auto v = send(s, c, [&] {
        return request(F::TextureObject, {c.texture, c.index});
      });
      if (!v)
        return false;
      c.texture = *v;
      c.child = 3;
      break;
    }
    case 3:
      if (!send(s, c, [&] { return request(F::LoadTexture, {c.texture, 0}); }))
        return false;
      c.child = 4;
      break;
    case 4: {
      if (!s.project)
        return false;
      const auto v = s.project(s.user, c.unit + 0x3c);
      if (!v)
        return false;
      const auto ux = real(s, c.unit + 0x3c);
      const bool right = ux > k(s, -20388);
      const auto zero = k(s, -20500), twenty = k(s, -20468);
      c.point = {c.partyChild ? (*v)[0] - twenty
                              : (right ? (*v)[0] - twenty : twenty + (*v)[0]),
                 k(s, -20384) + (*v)[1],
                 c.partyChild && right ? k(s, -20436)
                                       : (c.partyChild ? zero : twenty)};
      c.child = 5;
      break;
    }
    case 5: {
      if (!math.translation)
        return false;
      const auto v = math.translation(math.user, c.point);
      if (!v)
        return false;
      c.b = *v;
      for (unsigned i = 0; i < 12; ++i)
        c.a[i] = real(s, c.camera + 0x11c + 4 * i);
      c.child = 6;
      break;
    }
    case 6: {
      if (!math.concat)
        return false;
      const auto v = math.concat(math.user, c.a, c.b);
      if (!v)
        return false;
      c.a = *v;
      const auto language = word(s, word(s, 0x80415fd0) + 0x16c);
      if (language) {
        c.scale = k(s, -20380);
        c.child = 7;
      } else
        c.child = 9;
      break;
    }
    case 7: {
      if (!math.scale)
        return false;
      const auto v = math.scale(math.user, c.scale, c.scale, c.scale);
      if (!v)
        return false;
      c.b = *v;
      c.child = 8;
      break;
    }
    case 8: {
      if (!math.concat)
        return false;
      const auto v = math.concat(math.user, c.a, c.b);
      if (!v)
        return false;
      c.a = *v;
      c.child = 9;
      break;
    }
    case 9:
      if (!send(s, c, [&] { return request(F::LoadMatrix, {0}, {}, c.a); }))
        return false;
      c.child = 10;
      break;
    case 10:
      if (!send(s, c, [&] {
            return request(
                F::Quad,
                {c.color >> 24, (c.color >> 16) & 255, (c.color >> 8) & 255,
                 c.color & 255},
                {k(s, -20376), k(s, -20496), k(s, -20396), k(s, -20372)});
          }))
        return false;
      c.child = 0;
      return true;
    default:
      return false;
    }
}
} // namespace
bool resumeBattleClassPartyFooter(const BattleClassFooterServices &s,
                                  BattleClassFooterCursor &c) {
  if (!enter(s, c, 1))
    return false;
  for (;;)
    switch (c.phase) {
    case 0:
      c.battle = currentBattleAddress(s.raster.memory);
      c.work = word(s, c.battle + 0x1c78);
      c.phase = 1;
      break;
    case 1:
      if (!send(s, c, [] { return request(F::Camera, {8}); }))
        return false;
      c.mario = battleMarioUnit(c.battle, s.raster.memory);
      c.party = battlePartyUnit(c.battle, s.raster.memory);
      c.phase = c.party ? 2 : 4;
      break;
    case 2: {
      if (!s.project)
        return false;
      const auto p = s.project(s.user, c.mario + 0x3c);
      if (!p)
        return false;
      c.point = *p;
      c.phase = 3;
      break;
    }
    case 3: {
      if (!s.project)
        return false;
      const auto p = s.project(s.user, c.party + 0x3c);
      if (!p)
        return false;
      c.x = (c.point[0] + (*p)[0]) * k(s, -20364);
      c.y = c.point[1];
      c.phase = 4;
      break;
    }
    case 4: {
      if (!s.audienceItem)
        return false;
      const auto item = s.audienceItem(s.user, 0);
      if (!item)
        return false;
      if (!c.party) {
        if (*item == -1) {
          putFloat(s, c.work + 0x120, k(s, -20480));
          return finish(c);
        }
        decrement(s, c.work);
        c.phase = 5;
      } else if (*item != -1)
        c.phase = real(s, c.work + 0x120) == k(s, -20480) ? 6 : 7;
      else {
        putFloat(s, c.work + 0x120, k(s, -20480));
        c.phase = 8;
      }
      break;
    }
    case 5: {
      if (!s.project)
        return false;
      const auto p = s.project(s.user, c.mario + 0x3c);
      if (!p)
        return false;
      c.x = (*p)[0];
      c.y = (*p)[1] - k(s, -20468);
      c.angle = real(s, c.work + 0x120);
      c.type = 5;
      c.phase = 10;
      break;
    }
    case 6:
      if (!s.sound || !s.sound(s.user, 0x802ef7a4))
        return false;
      c.phase = 7;
      break;
    case 7:
      decrement(s, c.work);
      c.type = 5;
      c.y -= k(s, -20468);
      c.angle = real(s, c.work + 0x120);
      c.phase = 10;
      break;
    case 8: {
      if (!s.changePosition)
        return false;
      const auto allowed = s.changePosition(s.user, c.battle);
      if (!allowed)
        return false;
      if (!*allowed)
        return finish(c);
      c.type = 4;
      c.y -= k(s, -20468);
      c.angle = k(s, -20500);
      c.phase = 10;
      break;
    }
    case 10:
      if (!s.button || !s.button(s.user, c.type, c.x, c.y, c.angle))
        return false;
      return finish(c);
    default:
      return false;
    }
}
bool resumeBattleClassPinchFooter(const BattleClassFooterServices &s,
                                  BattleClassFooterCursor &c) {
  if (!enter(s, c, 2))
    return false;
  for (;;)
    switch (c.phase) {
    case 0:
      c.mario = battleMarioUnit(currentBattleAddress(s.raster.memory),
                                s.raster.memory);
      c.phase = 1;
      break;
    case 1:
      if (!send(s, c, [] { return request(F::Init2D); }))
        return false;
      c.partyFirst = real(s, c.mario + 0x3c) > k(s, -20388);
      c.partyChild = c.partyFirst;
      c.phase = 2;
      break;
    case 2:
      if (!pinchChild(s, c))
        return false;
      c.partyChild = !c.partyFirst;
      c.phase = 3;
      break;
    case 3:
      if (!pinchChild(s, c))
        return false;
      return finish(c);
    default:
      return false;
    }
}
bool resumeBattleClassFooterButton(int32_t type, float x, float y, float angle,
                                   const BattleClassFooterServices &s,
                                   BattleClassFooterCursor &c) {
  if (c.operation &&
      (c.type != type || std::memcmp(&c.x, &x, 4) || std::memcmp(&c.y, &y, 4) ||
       std::memcmp(&c.angle, &angle, 4)))
    return false;
  if (!enter(s, c, 3))
    return false;
  if (!c.phase) {
    c.type = type;
    c.x = x;
    c.y = y;
    c.angle = angle;
    c.scale = k(s, -20472) * angle;
    c.phase = 1;
  }
  if (c.phase == 1) {
    const auto &m = s.raster.math;
    if (!m.rotation)
      return false;
    const auto v = m.rotation(m.user, c.scale, 121);
    if (!v)
      return false;
    c.a = *v;
    c.a[3] += c.x;
    c.a[7] += c.y;
    c.a[11] += k(s, -20500);
    c.phase = 2;
  }
  if (!send(s, c, [&] {
        return request(
            F::TexturePlane2,
            {uint32_t(c.type), word(s, 0x80427440u + uint32_t(-20508)), 0}, {},
            c.a);
      }))
    return false;
  return finish(c);
}
} // namespace ttyd
