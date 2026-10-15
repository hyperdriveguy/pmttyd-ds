#include "battle_present_item.hpp"
#include "battle_packed_recovery.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
float scalar(uint32_t n) {
  float v;
  std::memcpy(&v, &n, 4);
  return v;
}
double number(uint32_t hi, uint32_t lo) {
  const uint64_t n = (uint64_t(hi) << 32) | lo;
  double v;
  std::memcpy(&v, &n, 8);
  return v;
}
} // namespace
std::optional<int32_t>
resumePackedBattlePresentItem(const BattlePresentItemServices &s,
                              BattlePresentItemCursor &c) {
  const auto &a = s.actors;
  const auto &m = a.memory;
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto put = [&](uint32_t at, uint32_t n) { m.storeWord(m.user, at, n); };
  auto done = [&](int32_t n) -> std::optional<int32_t> {
    c = {};
    return n;
  };
  if (c.active &&
      (c.owner.user != m.user || c.owner.word != m.word ||
       c.owner.storeWord != m.storeWord || c.owner.storeHalf != m.storeHalf ||
       c.position != s.privatePosition))
    return {};
  if (!c.active) {
    c.active = true;
    c.owner = m;
    c.position = s.privatePosition;
    c.battle = currentBattleAddress(a);
    c.item = integer(word(currentBattleAddress(a) + 0x1603c));
    if (c.item >= 339)
      return done(0);
    const auto target = integer(word(currentBattleAddress(a) + 0x16044));
    if (target < -1 || target >= 64)
      return done(0);
    c.unit = battleRegistryUnit(c.battle, target, a);
    if (c.item == 0)
      return done(0);
    if (c.item != 123 && c.item != 124) {
      if (c.item < 0)
        return done(0);
      m.storeByte(m.user, c.battle + 0x1c74, 1);
      const auto state = word(c.battle + 0x171c);
      if (state == 12 || state == 13)
        m.storeByte(m.user, c.battle + 0x1c74, 2);
      put(c.battle + 0x1720,
          word(c.battle + 0x1720) &
              0xffeeeeeeu); // Whole AllEnd leaf, not UI destruction.
      const auto previous = word(c.battle + 0x1c5c);
      put(c.battle + 0x1c60, previous);
      put(c.battle + 0x1c5c, 12);
      put(c.battle + 0x171c, 29);
      return done(1);
    }
    if (c.item == 123)
      recoverPackedBattleHp(c.unit, 1, a);
    else
      recoverPackedBattleFp(c.unit, 1, a);
    // Exact GetPos load/store order; output aliases can affect later source
    // loads.
    for (unsigned axis = 0; axis < 3; ++axis)
      put(c.position + 4 * axis, word(c.unit + 0x3c + 4 * axis));
    const auto h = m.half(m.user, c.unit + 0xce);
    c.height = h < 32768 ? int32_t(h) : int32_t(h) - 65536;
    c.encoded = number(0x43300000, uint32_t(c.height) ^ 0x80000000u);
    const auto hi = word(0x802efe78), lo = word(0x802efe7c);
    c.bias = number(hi, lo);
    c.scale = scalar(word(c.unit + 0x114));
    c.constant = scalar(word(0x804225f8));
    c.phase = 1;
  }
  if (c.phase == 1) {
    if (!s.subtractDoubleSingle)
      return {};
    const auto n = s.subtractDoubleSingle(s.user, c.encoded, c.bias);
    if (!n)
      return {};
    c.converted = *n;
    c.phase = 2;
  }
  if (c.phase == 2) {
    c.y = scalar(word(c.position + 4));
    c.effectPosition[0] = scalar(word(c.position));
    c.effectPosition[2] = scalar(word(c.position + 8));
    c.phase = 3;
  }
  if (c.phase == 3) {
    if (!s.fma)
      return {};
    const auto n = s.fma(s.user, c.scale, c.converted, c.constant);
    if (!n)
      return {};
    c.offset = *n;
    c.phase = 4;
  }
  if (c.phase == 4) {
    if (!s.add)
      return {};
    const auto n = s.add(s.user, c.y, c.offset);
    if (!n)
      return {};
    c.effectPosition[1] = *n;
    uint32_t raw;
    std::memcpy(&raw, &*n, 4);
    put(c.position + 4, raw);
    c.phase = 5;
  }
  if (c.phase == 5) {
    if (!s.effect ||
        !s.effect(s.user, c.item == 123 ? 0 : 1, 1, c.effectPosition))
      return {};
    put(currentBattleAddress(a) + 0x1603c, 0);
    return done(0);
  }
  return {};
}
} // namespace ttyd
