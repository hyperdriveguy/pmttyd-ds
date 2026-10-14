#include "battle_timing_display.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
float number(uint32_t n) {
  float v;
  std::memcpy(&v, &n, 4);
  return v;
}
uint32_t bits(float v) {
  uint32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
float single(float v) {
  volatile float out = v;
  return out;
}
} // namespace
bool packedBattleTimingADraw(float x, float y,
                             const BattleTimingDisplayServices &s) {
  const auto &m = s.memory.memory;
  assert(m.word && m.storeWord);
  const auto b = m.word(m.user, 0x8041eb90), a = b + 0x1f20;
  if (!s.camera || !s.camera(s.user, 8))
    return false;
  const bool active = m.word(m.user, b + 0x1f50) & 1;
  const auto base = active ? 0x802f01c8u : 0x802f01d4u;
  BattleTimingIconRequest r;
  r.icon = active ? 109 : 108;
  r.flags = 16;
  const auto offsetX = number(m.word(m.user, 0x80422710));
  const auto offsetY = number(m.word(m.user, 0x80422714));
  r.position[0] = number(m.word(m.user, base));
  const auto positionX = single(offsetX + x);
  r.position[1] = number(m.word(m.user, base + 4));
  const auto positionY = single(offsetY + y);
  r.position[2] = number(m.word(m.user, base + 8));
  r.position[0] = positionX;
  r.position[1] = positionY;
  r.scale = number(m.word(m.user, 0x80422718));
  if (!s.icon || !s.icon(s.user, r))
    return false;
  m.storeWord(m.user, a, m.word(m.user, a) + 1);
  return true;
}
bool packedBattleTimingADisplay(uint32_t b,
                                const BattleTimingDisplayServices &s) {
  const auto &m = s.memory.memory;
  assert(m.word && m.storeWord);
  const auto a = b + 0x1f20;
  const auto state = integer(m.word(m.user, b + 0x1c9c));
  auto load = [&](uint32_t address) { return number(m.word(m.user, address)); };
  auto store = [&](float v) { m.storeWord(m.user, a + 0x14, bits(v)); };
  auto curve = [&](int32_t step, float from, float to) {
    if (!s.interpolate)
      return false;
    const auto v = s.interpolate(s.user, 4, step, 20, from, to);
    if (!v)
      return false;
    store(*v);
    return true;
  };
  if (state == 99 || state == 100) {
    if (state == 99) {
      const auto count = m.word(m.user, a + 0x20);
      const auto from = load(0x8042271c);
      const auto to = load(0x80422720);
      if (!curve(integer(20u - count), from, to))
        return false;
    }
    // Original state99 falls through and overwrites the interpolation result.
    store(load(0x80422720));
    const auto x = load(a + 0x14);
    const auto y = load(a + 0x18);
    if (!packedBattleTimingADraw(x, y, s))
      return false;
    const auto count = m.word(m.user, a + 0x20);
    if (integer(count) > 0)
      m.storeWord(m.user, a + 0x20, count - 1);
  } else if (state == 1000 || state == 1001) {
    const auto count = m.word(m.user, a + 0x20);
    if (integer(count) >= 40) {
      const auto from = load(0x80422720);
      const auto to = load(0x8042271c);
      if (!curve(integer(count - 40), from, to))
        return false;
    } else
      store(load(0x80422720));
    const auto x = load(a + 0x14);
    const auto y = load(a + 0x18);
    if (!packedBattleTimingADraw(x, y, s))
      return false;
    const auto after = m.word(m.user, a + 0x20);
    if (integer(after) < 60)
      m.storeWord(m.user, a + 0x20, after + 1);
  }
  return true;
}
} // namespace ttyd
