#include "battle_part_attribute_event.hpp"
#include "battle_packed_part.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, PartAttributes = 0x1ac;
int32_t signedWord(uint32_t v) {
  return v <= 0x7fffffff ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
} // namespace
int32_t battleOnPartAttributeEvent(EventEntry &e,
                                   const BattleUnitEventServices &s) {
  const auto &m = s.memory;
  assert(m.word && m.storeWord && s.values.resolve && s.translate && s.unit);
  const auto args = e.arguments;
  const auto actor =
      s.values.resolve(s.values.user, e, signedWord(m.word(m.user, args)));
  const auto id = s.translate(s.user, e, actor);
  const auto partId =
      s.values.resolve(s.values.user, e, signedWord(m.word(m.user, args + 4)));
  const auto mask = m.word(m.user, args + 8);
  const auto battle = m.word(m.user, BattleGlobal);
  const auto unit = s.unit(s.user, battle, id);
  const auto part = packedUnitPart(unit, partId, m);
  const auto attributes = m.word(m.user, part + PartAttributes);
  m.storeWord(m.user, part + PartAttributes, attributes | mask);
  return 2;
}
} // namespace ttyd
