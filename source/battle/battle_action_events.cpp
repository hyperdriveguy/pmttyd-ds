#include "battle_action_events.hpp"
namespace ttyd {
// Captured B: callback1ca4 or discarded result 1cb8&2 read, then FRESH result
// mask 1 gates retry0. Callback scalar is ignored. Ready result hides help via
// helper that independently recaptures current owners, then returns2.
std::optional<int32_t>
packedBattleResultActionEvent(EventEntry &,
                              const BattleActionEventServices &s) {
  const auto &m = s.memory.memory;
  const auto b = currentBattleAddress(s.memory);
  const auto callback = m.word(m.user, b + 0x1ca4);
  if (callback) {
    if (!s.evaluate || !s.evaluate(s.user, b, callback))
      return std::nullopt;
  } else {
    (void)(m.word(m.user, b + 0x1cb8) & 2);
  }
  // The callback's scalar return (or null-callback result&2) is discarded.
  if (m.word(m.user, b + 0x1cb8) & 1)
    return 0;
  packedBattleActionHelpSetDisplay(0, s.memory);
  return 2;
}
// Remove callback1cac executes if present before help 0; this leaf does not
// clear callback storage itself. Read helper uses count 1cb4/result 1cb8 as signed
// words; no operand resolution or output-sentinel shortcut is added.
std::optional<int32_t>
packedBattleStopActionEvent(EventEntry &, const BattleActionEventServices &s) {
  const auto &m = s.memory.memory;
  const auto b = currentBattleAddress(s.memory);
  const auto callback = m.word(m.user, b + 0x1cac);
  if (callback && (!s.remove || !s.remove(s.user, b, callback)))
    return std::nullopt;
  packedBattleActionHelpSetDisplay(0, s.memory);
  return 2;
}
int32_t packedBattleReadActionEvent(EventEntry &e, bool count,
                                    const BattleActionEventServices &s) {
  const auto &m = s.memory.memory;
  const auto args = e.arguments, b = currentBattleAddress(s.memory),
             raw = m.word(m.user, args),
             value = m.word(m.user, b + (count ? 0x1cb4 : 0x1cb8));
  auto signedWord = [](uint32_t v) {
    return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
  };
  s.values.store(s.values.user, e, signedWord(raw), signedWord(value));
  return 2;
}
} // namespace ttyd
