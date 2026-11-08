#include "battle_unit_back_item_event.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, Attributes = 0x1c,
                   ReturnItemMask = 0x40000000, HeldItem = 0x308,
                   Information = 0x2738, Setup = 0xc, ReturnItems = 0x7c;
constexpr uint32_t ReturnItemSlots = 8;
int32_t signedWord(uint32_t v) {
  return v <= 0x7fffffff ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
} // namespace
int32_t battleGetBackItemEvent(EventEntry &e,
                               const BattleUnitEventServices &s) {
  const auto &m = s.memory;
  assert(m.word && m.storeWord && s.values.resolve && s.translate && s.unit);
  const auto args = e.arguments;
  const auto battle = m.word(m.user, BattleGlobal);
  const auto actor =
      s.values.resolve(s.values.user, e, signedWord(m.word(m.user, args)));
  const auto id = s.translate(s.user, e, actor);
  const auto unit = s.unit(s.user, battle, id);
  if (m.word(m.user, unit + Attributes) & ReturnItemMask) {
    const auto item = m.word(m.user, unit + HeldItem);
    if (item) {
      const auto info = m.word(m.user, battle + Information);
      const auto setup = m.word(m.user, info + Setup);
      uint32_t count = 0;
      for (uint32_t i = 0; i < ReturnItemSlots; ++i) {
        if (m.word(m.user, setup + ReturnItems + 4 * i) == 0)
          break;
        ++count;
      }
      // Source intentionally stores at index8 when all eight scanned slots are
      // occupied. Mapped writable storage, not a native capacity clamp, admits
      // it.
      m.storeWord(m.user, setup + ReturnItems + 4 * count, item);
      m.storeWord(m.user, unit + HeldItem, 0);
    }
  }
  return 2;
}
} // namespace ttyd
