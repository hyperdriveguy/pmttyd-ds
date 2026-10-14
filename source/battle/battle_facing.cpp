#include "battle_facing.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
}
int32_t sb(uint8_t v) { return v < 128 ? int32_t(v) : int32_t(v) - 256; }
std::array<float, 3> xyz(uint32_t at, const EventMemoryServices &m) {
  std::array<float, 3> v{};
  for (unsigned i = 0; i < 3; ++i) {
    const auto bits = m.word(m.user, at + 4 * i);
    std::memcpy(&v[i], &bits, 4);
  }
  return v;
}
} // namespace
int32_t battleCalculateFaceDirectionEvent(EventEntry &e,
                                          const BattleFacingServices &s) {
  const auto &u = s.positions.unit;
  const auto &m = u.memory;
  const auto &v = u.values;
  const auto &bytes = s.positions.memory;
  assert(m.word && bytes.byte && u.translate && u.unit && v.resolve &&
         v.store && s.subtract && s.signedWordFloat && s.positions.add);
  const auto args = e.arguments, b = m.word(m.user, 0x8041eb90);
  auto resolve = [&](unsigned i) {
    return v.resolve(v.user, e, sw(m.word(m.user, args + 4 * i)));
  };
  const auto actorId = u.translate(u.user, e, resolve(0));
  const auto actorPartId = resolve(1), literalX = resolve(2),
             targetPartId = resolve(3);
  const auto mode = m.word(m.user, args + 16);
  const auto output = sw(m.word(m.user, args + 20));
  const auto targetId =
      (mode & 9) ? literalX : u.translate(u.user, e, literalX);
  const auto actor = u.unit(u.user, m.word(m.user, 0x8041eb90), actorId);
  const auto actorPart =
      actorPartId == -1 ? 0 : packedUnitPart(actor, actorPartId, m);
  uint32_t target = 0, targetPart = 0;
  if (mode & 16) {
    target = u.unit(u.user, m.word(m.user, 0x8041eb90), targetId);
    if (targetPartId != -1)
      targetPart = packedUnitPart(target, targetPartId, m);
  }
  auto byte = [&](uint32_t at) { return sb(bytes.byte(bytes.user, at)); };
  int32_t direction = byte(actor + 0x189);
  if (actorPart)
    direction = byte(actorPart + 0xbd);
  if (mode & 4)
    direction = byte(b + 8 * uint32_t(byte(actor + 0xc)) + 0xa);
  if (mode & 8) {
    direction = byte(actor + 0x188);
    if (actorPart)
      direction = byte(actorPart + 0xbc);
  }
  auto x = [&](uint32_t unit, uint32_t part) {
    auto position = xyz(unit + 0x3c, m);
    if (part) {
      if (m.word(m.user, part + 0x1ac) & 0x10000000)
        position = xyz(part + 0x18, m);
      else {
        const auto offset = xyz(part + 0x24, m);
        position[0] =
            s.positions.add(s.positions.numericUser, position[0], offset[0]);
      }
    }
    return position[0];
  };
  auto choose = [&](float delta) {
    if (delta > 0)
      direction = 1;
    else if (delta < 0)
      direction = -1;
  };
  if (mode & 16) {
    const auto own = x(actor, actorPart), other = x(target, targetPart);
    choose(s.subtract(s.user, other, own));
  }
  if (mode & 1) {
    const auto own = x(actor, actorPart);
    const auto point = s.signedWordFloat(s.user, literalX);
    choose(s.subtract(s.user, point, own));
  }
  if (mode & 0x100)
    direction = sw(0u - uint32_t(direction));
  v.store(v.user, e, output, direction);
  return 2;
}
int32_t battleChangeFaceDirectionEvent(EventEntry &e,
                                       const BattleUnitEventServices &s) {
  const auto args = e.arguments;
  const auto actor = s.values.resolve(s.values.user, e,
                                      sw(s.memory.word(s.memory.user, args)));
  const auto id = s.translate(s.user, e, actor);
  const auto direction = s.values.resolve(
      s.values.user, e, sw(s.memory.word(s.memory.user, args + 4)));
  const auto unit =
      s.unit(s.user, s.memory.word(s.memory.user, 0x8041eb90), id);
  s.memory.storeByte(s.memory.user, unit + 0x189, uint8_t(direction));
  return 2;
}
} // namespace ttyd
