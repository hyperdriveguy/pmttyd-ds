#include "battle_action_manager.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
// Always update private padB+0x1d1c/channel 0 first. Null main slot 1ca0 skips
// ALL display work. Nonnull main result 0 clears1ca0, then fresh display slot
//1ca8 can STILL register this visit (camera 8, class 1, B argument, priority 0x80422d68).
// Do not couple display admission to whether main survived its callback.
bool packedBattleActionCommandManager(uint32_t b,
                                      const BattleActionManagerServices &s) {
  const auto &m = s.memory.memory;
  assert(m.word && m.storeWord);
  if (!s.pad || !s.pad(s.user, b + 0x1d1c, 0))
    return false;
  const auto callback = m.word(m.user, b + 0x1ca0);
  if (!callback)
    return true;
  if (!s.main)
    return false;
  const auto result = s.main(s.user, callback, b);
  if (!result)
    return false;
  if (*result == 0)
    m.storeWord(m.user, b + 0x1ca0, 0);
  const auto display = m.word(m.user, b + 0x1ca8);
  if (!display)
    return true;
  const auto bits = m.word(m.user, 0x80422d68);
  float priority;
  std::memcpy(&priority, &bits, 4);
  return s.display && s.display(s.user, 8, 1, display, b, priority);
}
} // namespace ttyd
