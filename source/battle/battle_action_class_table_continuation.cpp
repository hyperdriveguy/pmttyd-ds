#include "battle_action_class_table_continuation.hpp"
// Field offsets and masks in these notes are hex; counts, strides, states
// and icon IDs are decimal. Original addresses remain32-bit source identities.
namespace ttyd {
namespace {
int32_t sw(uint32_t n) {
  return n < 0x80000000u ? int32_t(n) : int32_t(int64_t(n) - 0x100000000LL);
}
int sb(uint8_t n) { return n < 128 ? int(n) : int(n) - 256; }
bool same(const BattleActionClassServices &a,
          const BattleActionClassServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.unit == b.unit && a.pouch == b.pouch &&
         a.message == b.message && a.memory.user == b.memory.user &&
         a.memory.byte == b.memory.byte &&
         a.memory.audienceLifecycle.user == b.memory.audienceLifecycle.user &&
         a.memory.audienceLifecycle.event == b.memory.audienceLifecycle.event &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall;
}
struct Running {
  bool &v;
  explicit Running(bool &f) : v(f) { v = true; }
  ~Running() { v = false; }
};
enum class Step {
  Entry,
  Unit,
  Pouch,
  Jump,
  JumpMessage,
  Party,
  PartyMessage,
  Hammer,
  HammerMessage,
  Item,
  ItemMessage,
  Special,
  SpecialMessage,
  Operation,
  OperationMessage,
  Lecture,
  LectureMessage,
  Publish
};
} // namespace
struct BattleActionClassTableCursor::State {
  BattleActionClassServices owner;
  uint32_t b = 0, m = 0, u = 0, p = 0, definition = 0, cursor = 0, count = 0,
           row = 0, pending = 0;
  int32_t id = 0, kind = 0;
  Step step = Step::Entry;
  uint32_t word(uint32_t a) const {
    return owner.memory.memory.word(owner.memory.memory.user, a);
  }
  uint16_t half(uint32_t a) const {
    return owner.memory.memory.half(owner.memory.memory.user, a);
  }
  uint8_t byte(uint32_t a) const {
    return owner.memory.byte(owner.memory.user, a);
  }
  void put(uint32_t a, uint32_t v) const {
    owner.memory.memory.storeWord(owner.memory.memory.user, a, v);
  }
  void icon(uint16_t v) const {
    owner.memory.memory.storeHalf(owner.memory.memory.user, row + 20, v);
  }
  // row=M+20*n is an anchor8 bytes BEFORE the actual20-byte record:
  // type+8,enabled+c,label+10,icon half+14,reason+18 (hex). Preserve padding.
  // deny writes0/reason3; a missing equipment level only clears enabled.
  void initialize(uint32_t type, uint32_t enabled, uint32_t reason) {
    row = m + 20 * count;
    put(row + 8, type);
    put(row + 12, enabled);
    put(row + 24, reason);
  }
  void deny(bool value) const {
    if (value) {
      put(row + 12, 0);
      put(row + 24, 3);
    }
  }
  // Actual original msgSearch key/address, not English substitute text.
  // Capture row/count through this hold; completed scalar0 is stored unchanged.
  // Message callbacks may change later live gates/equipment levels.
  bool label(uint32_t key) {
    pending = 0x80081b8c;
    if (!owner.message)
      return false;
    const auto value = owner.message(owner.user, key);
    if (!value)
      return false;
    put(row + 16, *value);
    pending = 0;
    return true;
  }
};
BattleActionClassTableCursor::BattleActionClassTableCursor() = default;
BattleActionClassTableCursor::~BattleActionClassTableCursor() = default;
bool BattleActionClassTableCursor::active() const { return bool(state_); }
uint32_t BattleActionClassTableCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleActionClassTableCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
std::optional<uint32_t>
resumePackedBattleActionClassTable(uint32_t b,
                                   const BattleActionClassServices &s,
                                   BattleActionClassTableCursor &cursor) {
  const auto &m = s.memory.memory;
  if (cursor.invoking_ || !m.word || !m.half || !m.storeWord || !m.storeHalf ||
      !s.memory.byte)
    return {};
  if (cursor.state_ &&
      (cursor.state_->b != b || !same(cursor.state_->owner, s)))
    return {};
  Running guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleActionClassTableCursor::State>();
    cursor.state_->owner = s;
    cursor.state_->b = b;
  }
  auto &c = *cursor.state_;
  auto finish = [&](uint32_t value) -> std::optional<uint32_t> {
    cursor.state_.reset();
    return value;
  };
  for (;;)
    switch (c.step) {
    case Step::Entry:
      c.id = sw(c.word(b + 0x420));
      c.step = Step::Unit;
      break;
    case Step::Unit: {
      c.pending = 0x800f88cc;
      if (!s.unit)
        return {};
      auto value = s.unit(s.user, b, c.id);
      if (!value)
        return {};
      c.u = *value;
      c.m = b + 0x171c;
      c.step = Step::Pouch;
      break;
    }
    // M=B171c wraps32 bits. Setup/definition reads still precede the M0
    // branch, whose raw return is captured pouch P rather than a row count.
    // First kind selects retained class cursor M498(Mario222)/M4a4(other).
    case Step::Pouch: {
      c.pending = 0x800d5eb0;
      if (!s.pouch)
        return {};
      auto value = s.pouch(s.user);
      if (!value)
        return {};
      c.p = *value;
      c.pending = 0;
      const auto setup = c.word(b + 0x2738);
      c.definition = c.word(setup + 12);
      if (!c.m)
        return finish(c.p);
      c.kind = sw(c.word(c.u + 8));
      c.cursor = c.m + (c.kind == 222 ? 0x498 : 0x4a4);
      c.count = 0;
      c.step = Step::Jump;
      break;
    }
    // Original type0 Jump is admitted for initial kind222 even without boots.
    // U104 bit10,status U300,B19050,D4 bit01000000,Bef4 mask06800000
    // deny in source order; do not short-circuit after an earlier denial.
    case Step::Jump:
      if (c.kind != 222) {
        c.step = Step::Party;
        break;
      }
      c.initialize(0, 1, 0);
      c.deny(c.word(c.u + 0x104) & 16);
      if (sb(c.byte(c.p + 0x98)) < 1)
        c.put(c.row + 12, 0);
      c.deny(c.byte(c.u + 0x300) != 0);
      c.deny(sw(c.word(b + 0x19050)) > 0);
      c.deny(c.word(c.definition + 4) & 0x01000000);
      c.deny(c.word(b + 0xef4) & 0x06800000);
      c.step = Step::JumpMessage;
      break;
    // Fresh signed boot level AFTER message:1/2/otherwise -> icons334/335/336.
    // This default is not an equipment-level clamp or a cached pre-message level.
    case Step::JumpMessage: {
      if (!c.label(0x802f0018))
        return {};
      const auto level = sb(c.byte(c.p + 0x98));
      c.icon(level == 1 ? 334 : level == 2 ? 335 : 336);
      c.count = 1;
      c.step = Step::Party;
      break;
    }
    // Fresh kinds224..230 admit type6; signed U12b,U16 U13c and Bef4
    // mask04800000 gate enabled/reason only. Row admission survives denial.
    case Step::Party: {
      const auto kind = sw(c.word(c.u + 8));
      if (kind < 224 || kind >= 231) {
        c.step = Step::Hammer;
        break;
      }
      c.row = c.m + 20 * c.count;
      c.put(c.row + 8, 6);
      const bool disabled = sb(c.byte(c.u + 0x12b)) != 0;
      c.put(c.row + 12, disabled ? 0 : 1);
      c.put(c.row + 24, disabled ? 3 : 0);
      c.deny(c.half(c.u + 0x13c) != 0);
      c.deny(c.word(b + 0xef4) & 0x04800000);
      c.step = Step::PartyMessage;
      break;
    }
    case Step::PartyMessage:
      if (!c.label(0x802f002c))
        return {};
      ++c.count;
      c.icon(343);
      c.step = Step::Hammer;
      break;
    // Type1 Hammer differs from Jump: kind222 AND signed hammer level>=1
    // admit the row. Fresh later P99 can clear enabled without removing it.
    // U2ff/B19054/D4 bit02000000/Bef4 mask05800000 retain source order.
    case Step::Hammer:
      if (sw(c.word(c.u + 8)) != 222 || sb(c.byte(c.p + 0x99)) < 1) {
        c.step = Step::Item;
        break;
      }
      c.initialize(1, 1, 0);
      c.deny(c.word(c.u + 0x104) & 16);
      if (sb(c.byte(c.p + 0x99)) < 1)
        c.put(c.row + 12, 0);
      c.deny(c.byte(c.u + 0x2ff) != 0);
      c.deny(sw(c.word(b + 0x19054)) > 0);
      c.deny(c.word(c.definition + 4) & 0x02000000);
      c.deny(c.word(b + 0xef4) & 0x05800000);
      c.step = Step::HammerMessage;
      break;
    case Step::HammerMessage: {
      if (!c.label(0x802f0044))
        return {};
      const auto level = sb(c.byte(c.p + 0x99));
      c.icon(level == 1 ? 337 : level == 2 ? 338 : 339);
      ++c.count;
      c.step = Step::Item;
      break;
    }
    // Type2 Item uses fresh Mario/party kind admission, not inventory count.
    // A later kind reread chooses Mario B19058 versus U16 U13e restriction;
    // no second party-kind range validation is added after that reread.
    case Step::Item: {
      const auto kind = sw(c.word(c.u + 8));
      if (kind != 222 && !(kind >= 224 && kind < 231)) {
        c.step = Step::Special;
        break;
      }
      c.row = c.m + 20 * c.count;
      c.put(c.row + 8, 2);
      const bool disabled = sb(c.byte(c.u + 0x12b)) != 0;
      c.put(c.row + 12, disabled ? 0 : 1);
      c.put(c.row + 24, disabled ? 3 : 0);
      if (sw(c.word(c.u + 8)) == 222)
        c.deny(sw(c.word(b + 0x19058)) > 0);
      else
        c.deny(c.half(c.u + 0x13e) != 0);
      c.deny(c.word(b + 0xef4) & 0x1f800000);
      c.step = Step::ItemMessage;
      break;
    }
    case Step::ItemMessage:
      if (!c.label(0x802eff0c))
        return {};
      ++c.count;
      c.icon(340);
      c.step = Step::Special;
      break;
    // Type4 Special requires fresh Mario kind and nonzero UNSIGNED P8c half.
    // Type5 Operations and type13 Lecture below independently reload flag08000000;
    // message-visible mutation can admit both or neither, not an else pair.
    case Step::Special:
      if (sw(c.word(c.u + 8)) != 222 || !c.half(c.p + 0x8c)) {
        c.step = Step::Operation;
        break;
      }
      c.initialize(4, 1, 0);
      c.deny(c.word(c.u + 0x104) & 16);
      c.deny(c.word(b + 0xef4) & 0x1b800000);
      c.step = Step::SpecialMessage;
      break;
    case Step::SpecialMessage:
      if (!c.label(0x802f0058))
        return {};
      c.icon(341);
      ++c.count;
      c.step = Step::Operation;
      break;
    case Step::Operation:
      if (c.word(b + 0xef4) & 0x08000000) {
        c.step = Step::Lecture;
        break;
      }
      c.initialize(5, 1, 0);
      c.deny(c.word(b + 0xef4) & 0x1f800000);
      c.step = Step::OperationMessage;
      break;
    case Step::OperationMessage:
      if (!c.label(0x802f0074))
        return {};
      ++c.count;
      c.icon(342);
      c.step = Step::Lecture;
      break;
    case Step::Lecture:
      if (!(c.word(b + 0xef4) & 0x08000000)) {
        c.step = Step::Publish;
        break;
      }
      c.initialize(13, 1, 0);
      c.step = Step::LectureMessage;
      break;
    case Step::LectureMessage:
      if (!c.label(0x802f008c))
        return {};
      c.icon(342);
      c.deny(c.word(b + 0xef4) & 0x10800000);
      ++c.count;
      c.step = Step::Publish;
      break;
    // Publish count,then read index and LIVE count; signed upper correction
    // only. Negative index stays unchanged,zero count can produce index-1.
    // Return live count, including a callback-visible publication mutation.
    case Step::Publish: {
      c.put(c.cursor + 8, c.count);
      const auto index = c.word(c.cursor), count = c.word(c.cursor + 8);
      if (sw(index) >= sw(count))
        c.put(c.cursor, count - 1);
      return finish(count);
    }
    }
}
} // namespace ttyd
