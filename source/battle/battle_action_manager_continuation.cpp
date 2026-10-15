#include "battle_action_manager_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
bool same(const BattleActionManagerServices &a,
          const BattleActionManagerServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.pad == b.pad && a.main == b.main &&
         a.display == b.display && a.memory.user == b.memory.user &&
         a.memory.byte == b.memory.byte &&
         a.memory.audienceLifecycle.user == b.memory.audienceLifecycle.user &&
         a.memory.audienceLifecycle.event == b.memory.audienceLifecycle.event &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall;
}
struct Guard {
  bool &v;
  explicit Guard(bool &f) : v(f) { v = true; }
  ~Guard() { v = false; }
};
} // namespace
struct BattleActionManagerCursor::State {
  BattleActionManagerServices owner;
  uint32_t battle = 0, callback = 0, display = 0;
  float priority = 0;
  unsigned phase = 0;
};
BattleActionManagerCursor::BattleActionManagerCursor() = default;
BattleActionManagerCursor::~BattleActionManagerCursor() = default;
bool BattleActionManagerCursor::active() const { return bool(state_); }
bool BattleActionManagerCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
uint32_t BattleActionManagerCursor::pending() const {
  if (!state_)
    return 0;
  return state_->phase == 0   ? 0x800fa5b0
         : state_->phase == 2 ? state_->callback
         : state_->phase == 4 ? 0x80010bc4
                              : 0;
}
bool resumePackedBattleActionCommandManager(
    uint32_t b, const BattleActionManagerServices &s,
    BattleActionManagerCursor &cursor) {
  if (cursor.invoking_)
    return false;
  if (cursor.state_ &&
      (cursor.state_->battle != b || !same(cursor.state_->owner, s)))
    return false;
  const auto &m = s.memory.memory;
  if (!m.word || !m.storeWord)
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleActionManagerCursor::State>();
    cursor.state_->owner = s;
    cursor.state_->battle = b;
  }
  auto &c = *cursor.state_;
  for (;;)
    switch (c.phase) {
    case 0:
      if (!s.pad || !s.pad(s.user, c.battle + 0x1d1c, 0))
        return false;
      c.phase = 1;
      break;
    case 1:
      c.callback = m.word(m.user, c.battle + 0x1ca0);
      if (!c.callback) {
        cursor.state_.reset();
        return true;
      }
      c.phase = 2;
      break;
    case 2: {
      if (!s.main)
        return false;
      const auto result = s.main(s.user, c.callback, c.battle);
      if (!result)
        return false;
      if (*result == 0)
        m.storeWord(m.user, c.battle + 0x1ca0, 0);
      c.phase = 3;
      break;
    }
    case 3: {
      c.display = m.word(m.user, c.battle + 0x1ca8);
      if (!c.display) {
        cursor.state_.reset();
        return true;
      }
      const auto bits = m.word(m.user, 0x80422d68);
      std::memcpy(&c.priority, &bits, 4);
      c.phase = 4;
      break;
    }
    case 4:
      if (!s.display ||
          !s.display(s.user, 8, 1, c.display, c.battle, c.priority))
        return false;
      cursor.state_.reset();
      return true;
    default:
      return false;
    }
}
} // namespace ttyd
