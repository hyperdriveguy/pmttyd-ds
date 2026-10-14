#include "battle_action_class_table.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t v) {
  int32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
int32_t signByte(uint8_t n) { return n < 128 ? int32_t(n) : int32_t(n) - 256; }
} // namespace
std::optional<uint32_t>
makePackedBattleActionClassTable(uint32_t b,
                                 const BattleActionClassServices &s) {
  const auto &a = s.memory;
  const auto &m = a.memory;
  assert(m.word && m.half && m.storeWord && m.storeHalf && a.byte && s.unit &&
         s.pouch && s.message);
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto put = [&](uint32_t at, uint32_t n) { m.storeWord(m.user, at, n); };
  auto byte = [&](uint32_t at) { return a.byte(a.user, at); };
  auto half = [&](uint32_t at) { return m.half(m.user, at); };
  const auto actor = s.unit(s.user, b, integer(word(b + 0x420)));
  if (!actor)
    return std::nullopt;
  const auto u = *actor, menu = b + 0x171c;
  const auto pouch = s.pouch(s.user);
  if (!pouch)
    return std::nullopt;
  const auto p = *pouch;
  const auto setup = word(b + 0x2738), definition = word(setup + 0xc);
  if (!menu)
    return p;
  const auto kind = integer(word(u + 8));
  const auto cursor = menu + (kind == 222 ? 0x498u : 0x4a4u);
  uint32_t count = 0;
  auto row = [&] { return menu + 20 * count; };
  auto initialize = [&](uint32_t r, uint32_t type, uint32_t enabled,
                        uint32_t reason) {
    put(r + 8, type);
    put(r + 0xc, enabled);
    put(r + 0x18, reason);
  };
  auto deny = [&](uint32_t r, bool condition) {
    if (condition) {
      put(r + 0xc, 0);
      put(r + 0x18, 3);
    }
  };
  auto label = [&](uint32_t r, uint32_t key) {
    const auto name = s.message(s.user, key);
    if (!name)
      return false;
    put(r + 0x10, *name);
    return true;
  };
  auto icon = [&](uint32_t r, uint16_t value) {
    m.storeHalf(m.user, r + 0x14, value);
  };
  if (kind == 222) {
    const auto r = row();
    initialize(r, 0, 1, 0);
    deny(r, word(u + 0x104) & 0x10);
    if (signByte(byte(p + 0x98)) < 1)
      put(r + 0xc, 0);
    deny(r, byte(u + 0x300) != 0);
    deny(r, integer(word(b + 0x19050)) > 0);
    deny(r, word(definition + 4) & 0x01000000);
    deny(r, word(b + 0xef4) & 0x06800000);
    if (!label(r, 0x802f0018))
      return std::nullopt;
    const auto level = signByte(byte(p + 0x98));
    icon(r, level == 1 ? 334 : level == 2 ? 335 : 336);
    count = 1;
  }
  auto k = integer(word(u + 8));
  if (k >= 224 && k < 231) {
    const auto r = row();
    put(r + 8, 6);
    const bool disabled = signByte(byte(u + 0x12b)) != 0;
    put(r + 0xc, disabled ? 0 : 1);
    put(r + 0x18, disabled ? 3 : 0);
    deny(r, half(u + 0x13c) != 0);
    deny(r, word(b + 0xef4) & 0x04800000);
    if (!label(r, 0x802f002c))
      return std::nullopt;
    ++count;
    icon(r, 343);
  }
  if (integer(word(u + 8)) == 222 && signByte(byte(p + 0x99)) >= 1) {
    const auto r = row();
    initialize(r, 1, 1, 0);
    deny(r, word(u + 0x104) & 0x10);
    if (signByte(byte(p + 0x99)) < 1)
      put(r + 0xc, 0);
    deny(r, byte(u + 0x2ff) != 0);
    deny(r, integer(word(b + 0x19054)) > 0);
    deny(r, word(definition + 4) & 0x02000000);
    deny(r, word(b + 0xef4) & 0x05800000);
    if (!label(r, 0x802f0044))
      return std::nullopt;
    const auto level = signByte(byte(p + 0x99));
    icon(r, level == 1 ? 337 : level == 2 ? 338 : 339);
    ++count;
  }
  k = integer(word(u + 8));
  if (k == 222 || (k >= 224 && k < 231)) {
    const auto r = row();
    put(r + 8, 2);
    const bool disabled = signByte(byte(u + 0x12b)) != 0;
    put(r + 0xc, disabled ? 0 : 1);
    put(r + 0x18, disabled ? 3 : 0);
    if (integer(word(u + 8)) == 222)
      deny(r, integer(word(b + 0x19058)) > 0);
    else
      deny(r, half(u + 0x13e) != 0);
    deny(r, word(b + 0xef4) & 0x1f800000);
    if (!label(r, 0x802eff0c))
      return std::nullopt;
    ++count;
    icon(r, 340);
  }
  if (integer(word(u + 8)) == 222 && half(p + 0x8c) != 0) {
    const auto r = row();
    initialize(r, 4, 1, 0);
    deny(r, word(u + 0x104) & 0x10);
    deny(r, word(b + 0xef4) & 0x1b800000);
    if (!label(r, 0x802f0058))
      return std::nullopt;
    icon(r, 341);
    ++count;
  }
  if (!(word(b + 0xef4) & 0x08000000)) {
    const auto r = row();
    initialize(r, 5, 1, 0);
    deny(r, word(b + 0xef4) & 0x1f800000);
    if (!label(r, 0x802f0074))
      return std::nullopt;
    ++count;
    icon(r, 342);
  }
  if (word(b + 0xef4) & 0x08000000) {
    const auto r = row();
    initialize(r, 13, 1, 0);
    if (!label(r, 0x802f008c))
      return std::nullopt;
    icon(r, 342);
    deny(r, word(b + 0xef4) & 0x10800000);
    ++count;
  }
  put(cursor + 8, count);
  const auto index = word(cursor), liveCount = word(cursor + 8);
  if (integer(index) >= integer(liveCount))
    put(cursor, liveCount - 1);
  return liveCount;
}
} // namespace ttyd
