#include "battle_act_ui.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
} // namespace
bool endPackedBattleActUi(uint32_t b, const BattleActUiServices &s) {
  const auto &m = s.memory.memory;
  const auto old = m.word(m.user, b + 0x1c78);
  if (old) {
    if (!s.release || !s.release(s.user, old))
      return false;
    m.storeWord(m.user, b + 0x1c78, 0);
  }
  return true;
}
bool initializePackedBattleActUi(uint32_t b, uint32_t cursor, uint32_t rows,
                                 uint32_t flag, const BattleActUiServices &s) {
  const auto &m = s.memory.memory;
  assert(m.word && m.storeWord && m.storeByte);
  const auto old = m.word(m.user, b + 0x1c78);
  if (old && (!s.release || !s.release(s.user, old)))
    return false;
  if (!s.allocate)
    return false;
  const auto allocated = s.allocate(s.user, 0x124);
  if (!allocated)
    return false;
  m.storeWord(m.user, b + 0x1c78, *allocated);
  // The source captures W after publishing the allocation; later B pointer
  // changes do not retarget these arrays.
  const auto w = m.word(m.user, b + 0x1c78);
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto put = [&](uint32_t at, uint32_t n) { m.storeWord(m.user, at, n); };
  put(w, cursor);
  put(w + 4, rows);
  put(w + 0xc, (flag & 1) ? 10 : 0);
  m.storeByte(m.user, w + 0x14, 0);
  put(w + 0x18, word(cursor));
  if (flag & 1) {
    uint32_t selected = word(cursor), ordinal = 0;
    while (integer(ordinal) < integer(word(cursor + 8))) {
      put(w + 0x34 + 4 * selected, selected == word(cursor) ? 10 : 11);
      put(w + 0x48 + 4 * selected, 0);
      const auto count = word(cursor + 8);
      put(w + 0x84 + 4 * selected, count + ordinal - 1);
      const auto reverse = word(w + 0x84 + 4 * selected),
                 freshCount = word(cursor + 8);
      if (integer(reverse) >= integer(freshCount))
        put(w + 0x84 + 4 * selected, reverse - freshCount);
      put(w + 0x20 + 4 * selected, 0);
      ++selected;
      if (integer(selected) >= integer(word(cursor + 8)))
        selected = 0;
      ++ordinal;
    }
  } else {
    uint32_t selected = word(cursor), ordinal = 0;
    for (;;) {
      const auto count = word(cursor + 8);
      if (integer(ordinal) >= integer(count))
        break;
      ++selected;
      if (integer(selected) >= integer(count))
        selected = 0;
      put(w + 0x34 + 4 * selected, 0);
      const auto gp = word(0x80415fd0), rate = word(gp + 4);
      const auto step = integer(rate * 10) / 60;
      const auto delay = integer(ordinal * uint32_t(step)) / 2;
      put(w + 0x48 + 4 * selected, uint32_t(delay));
      put(w + 0x84 + 4 * selected, ordinal);
      ++ordinal;
      put(w + 0x20 + 4 * selected, 0);
    }
  }
  return true;
}
} // namespace ttyd
