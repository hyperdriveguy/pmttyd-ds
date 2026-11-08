#include "battle_unit_rotate_offset_event.hpp"
#include "battle_packed_unit_vectors.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, SetRotateOffset = 0x80128604;
int32_t signedWord(uint32_t v) {
  return v <= 0x7fffffff ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
} // namespace
int32_t
battleSetUnitRotateOffsetEvent(EventEntry &e,
                               const BattleUnitRotationEventServices &s) {
  const auto &u = s.units;
  const auto &m = u.memory;
  assert(m.word && u.values.resolve && u.translate && u.unit);
  const auto args = e.arguments;
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto resolve = [&](uint32_t raw) {
    return u.values.resolve(u.values.user, e, signedWord(raw));
  };
  const auto id = u.translate(u.user, e, resolve(word(args)));
  std::array<float, 3> value;
  const auto x = resolve(word(args + 4));
  const auto rawY = word(args + 8);
  value[0] = convertPackedBattleRotationWord(x, s);
  const auto y = resolve(rawY);
  const auto rawZ = word(args + 12);
  value[1] = convertPackedBattleRotationWord(y, s);
  const auto z = resolve(rawZ);
  const auto battle = word(BattleGlobal);
  value[2] = convertPackedBattleRotationWord(z, s);
  const auto unit = u.unit(u.user, battle, id);
  PackedUnitVectorServices leaf;
  leaf.memory = m;
  // Existing complete leaf performs each fresh source sentinel read separately.
  const bool complete = setPackedUnitVector(SetRotateOffset, unit, value, leaf);
  assert(complete);
  (void)complete;
  return 2;
}
} // namespace ttyd
