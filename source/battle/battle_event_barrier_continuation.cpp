#include "battle_event_barrier_continuation.hpp"
#include "battle_attack_selection.hpp"
#include <cassert>
namespace ttyd {
namespace {
bool same(const BattleEventBarrierServices &a,
          const BattleEventBarrierServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  return a.user == b.user && a.eventExists == b.eventExists &&
         a.removeItem == b.removeItem && a.concluded == b.concluded &&
         a.create == b.create && a.actors.user == b.actors.user &&
         a.actors.byte == b.actors.byte &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall;
}
struct Guard {
  bool &flag;
  explicit Guard(bool &v) : flag(v) { flag = true; }
  ~Guard() { flag = false; }
};
enum class Entry { All, Unit, Reaction, Consume };
enum class Phase {
  UnitFind,
  UnitField,
  UnitCheck,
  AfterStart,
  AfterFind,
  ConsumeStart,
  ConsumeRead,
  ConsumeCall,
  Concluded,
  ReleaseMissing,
  ReleasePresent,
  Data,
  Create,
  OuterRead,
  OuterCheck
};
constexpr uint32_t unitFields[]{0x29c, 0x2a8, 0x2b4, 0x2bc};
constexpr uint32_t outerFields[]{0x182b4, 0x182c4, 0x180fc, 0x182c8, 0x182c0};
} // namespace
void releasePackedBattleAfterReaction(int32_t unit, uint32_t key,
                                      const BattleActorServices &s) {
  const auto &m = s.memory;
  assert(m.word && m.storeWord);
  const auto queue = currentBattleAddress(s) + 0x16f5c;
  for (int32_t i = 0; i < 64; ++i) {
    const auto r = queue + 8 * uint32_t(i);
    const auto id = m.word(m.user, r);
    if (id == 0xffffffff || id != uint32_t(unit))
      continue;
    if (m.word(m.user, r + 4) != key)
      continue;
    for (int32_t j = i; j < 63; ++j) {
      const auto dest = queue + 8 * uint32_t(j);
      m.storeWord(m.user, dest, m.word(m.user, dest + 8));
      m.storeWord(m.user, dest + 4, m.word(m.user, dest + 12));
    }
    m.storeWord(m.user, queue + 504, 0xffffffff);
    m.storeWord(m.user, queue + 508, 0);
    --i;
  }
}
struct BattleEventBarrierCursor::State {
  BattleEventBarrierServices owner;
  Entry entry = Entry::All;
  Phase phase = Phase::UnitFind;
  uint32_t passed = 0, unit = 0, field = 0, event = 0, reaction = 0, key = 0,
           script = 0, consume = 0, item = 0, pending = 0;
  int32_t queueUnit = 0;
  unsigned slot = 0, index = 0, reserved = 0;
  bool ready = true;
};
BattleEventBarrierCursor::BattleEventBarrierCursor() = default;
BattleEventBarrierCursor::~BattleEventBarrierCursor() = default;
bool BattleEventBarrierCursor::active() const { return bool(state_); }
bool BattleEventBarrierCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
uint32_t BattleEventBarrierCursor::pending() const {
  return state_ ? state_->pending : 0;
}
struct BattleEventBarrierRunner {
  static std::optional<int32_t> run(uint32_t b, Entry entry,
                                    const BattleEventBarrierServices &s,
                                    BattleEventBarrierCursor &cursor) {
    if (cursor.invoking_)
      return {};
    if (cursor.state_) {
      const auto &c = *cursor.state_;
      if (c.passed != b || c.entry != entry || !same(c.owner, s))
        return {};
    }
    const auto &m = s.actors.memory;
    if (!m.word || !m.storeWord)
      return {};
    Guard guard(cursor.invoking_);
    if (!cursor.state_) {
      cursor.state_ = std::make_unique<BattleEventBarrierCursor::State>();
      auto &c = *cursor.state_;
      c.owner = s;
      c.passed = b;
      c.entry = entry;
      c.phase = entry == Entry::Consume    ? Phase::ConsumeStart
                : entry == Entry::Reaction ? Phase::AfterStart
                                           : Phase::UnitFind;
    }
    auto &c = *cursor.state_;
    auto word = [&](uint32_t a) { return m.word(m.user, a); };
    auto put = [&](uint32_t a, uint32_t n) { m.storeWord(m.user, a, n); };
    auto finish = [&](int32_t n) -> std::optional<int32_t> {
      cursor.state_.reset();
      return n;
    };
    auto after = [&](int32_t result) -> std::optional<int32_t> {
      if (entry == Entry::Reaction)
        return finish(result);
      if (result)
        return finish(0);
      if (entry == Entry::Unit)
        return finish(1);
      c.index = 0;
      c.phase = Phase::OuterRead;
      return {};
    };
    for (;;)
      switch (c.phase) {
      case Phase::UnitFind:
        if (c.slot == 64) {
          if (!c.ready)
            return finish(0);
          c.phase = Phase::AfterStart;
          break;
        }
        c.unit = battleRegistryUnit(c.passed, int32_t(c.slot), s.actors);
        if (!c.unit) {
          ++c.slot;
          break;
        }
        c.index = 0;
        c.phase = Phase::UnitField;
        break;
      case Phase::UnitField:
        if (c.index == 4) {
          ++c.slot;
          c.phase = Phase::UnitFind;
          break;
        }
        c.field = c.unit + unitFields[c.index];
        c.event = word(c.field);
        if (!c.event) {
          ++c.index;
          break;
        }
        c.phase = Phase::UnitCheck;
        break;
      case Phase::UnitCheck: {
        c.pending = 0x80036604;
        if (!s.eventExists)
          return {};
        const auto result = s.eventExists(s.user, c.event);
        if (!result)
          return {};
        if (*result)
          c.ready = false;
        else
          put(c.field, 0);
        ++c.index;
        c.phase = Phase::UnitField;
        break;
      }
      case Phase::AfterStart:
        c.reaction = currentBattleAddress(s.actors);
        c.index = 0;
        c.phase = Phase::AfterFind;
        break;
      case Phase::AfterFind: {
        if (c.index == 64) {
          const auto result = after(0);
          if (result)
            return result;
          break;
        }
        const auto pair = c.reaction + 0x16f5c + 8 * c.index;
        const auto raw = word(pair);
        if (raw == 0xffffffff) {
          ++c.index;
          break;
        }
        c.queueUnit = raw < 0x80000000 ? int32_t(raw)
                                       : int32_t(int64_t(raw) - 0x100000000LL);
        c.key = word(pair + 4);
        c.unit = battleRegistryUnit(c.reaction, c.queueUnit, s.actors);
        c.phase = c.unit ? Phase::ConsumeStart : Phase::ReleaseMissing;
        break;
      }
      case Phase::ReleaseMissing:
        releasePackedBattleAfterReaction(c.queueUnit, c.key, s.actors);
        c.phase = Phase::AfterStart;
        break;
      case Phase::ConsumeStart:
        c.consume = currentBattleAddress(s.actors);
        c.reserved = 0;
        c.phase = Phase::ConsumeRead;
        break;
      case Phase::ConsumeRead:
        if (c.reserved == 4) {
          if (entry == Entry::Consume)
            return finish(0);
          c.phase = Phase::Concluded;
          break;
        }
        c.field = c.consume + 0x19060 + 4 * c.reserved;
        c.item = word(c.field);
        if (!c.item) {
          put(c.field, 0);
          ++c.reserved;
          break;
        }
        c.phase = Phase::ConsumeCall;
        break;
      case Phase::ConsumeCall: {
        c.pending = 0x800d4c38;
        if (!s.removeItem)
          return {};
        const auto result = s.removeItem(s.user, c.item);
        if (!result)
          return {};
        put(c.field, 0);
        ++c.reserved;
        c.phase = Phase::ConsumeRead;
        break;
      }
      case Phase::Concluded: {
        c.pending = 0x8011aa34;
        if (!s.concluded)
          return {};
        const auto result = s.concluded(s.user, c.reaction);
        if (!result)
          return {};
        if (*result) {
          const auto done = after(0);
          if (done)
            return done;
        } else
          c.phase = Phase::ReleasePresent;
        break;
      }
      case Phase::ReleasePresent:
        releasePackedBattleAfterReaction(c.queueUnit, c.key, s.actors);
        c.phase = Phase::Data;
        break;
      case Phase::Data:
        c.script = packedUnitData(c.unit, c.key, s.actors);
        c.phase = Phase::Create;
        break;
      case Phase::Create: {
        c.pending = 0x800374ac;
        if (!s.create)
          return {};
        const auto event = s.create(s.user, c.script, 10, 0);
        if (!event)
          return {};
        if (*event) {
          put(c.unit + 0x2a8, word(*event + 0x15c));
          put(*event + 0x160, word(c.unit));
        }
        return after(1);
      }
      case Phase::OuterRead:
        if (c.index == 5)
          return finish(1);
        c.field = c.passed + outerFields[c.index];
        c.event = word(c.field);
        if (!c.event) {
          ++c.index;
          break;
        }
        c.phase = Phase::OuterCheck;
        break;
      case Phase::OuterCheck: {
        c.pending = 0x80036604;
        if (!s.eventExists)
          return {};
        const auto result = s.eventExists(s.user, c.event);
        if (!result)
          return {};
        if (*result)
          return finish(0);
        put(c.field, 0);
        ++c.index;
        c.phase = Phase::OuterRead;
        break;
      }
      }
  }
};
std::optional<int32_t>
resumeBattleAllEventBarrier(uint32_t b, const BattleEventBarrierServices &s,
                            BattleEventBarrierCursor &c) {
  return BattleEventBarrierRunner::run(b, Entry::All, s, c);
}
std::optional<int32_t>
resumeBattleUnitEventBarrier(uint32_t b, const BattleEventBarrierServices &s,
                             BattleEventBarrierCursor &c) {
  return BattleEventBarrierRunner::run(b, Entry::Unit, s, c);
}
std::optional<int32_t>
resumeBattleAfterReaction(const BattleEventBarrierServices &s,
                          BattleEventBarrierCursor &c) {
  return BattleEventBarrierRunner::run(0, Entry::Reaction, s, c);
}
bool resumeBattleConsumeReservedItems(const BattleEventBarrierServices &s,
                                      BattleEventBarrierCursor &c) {
  return BattleEventBarrierRunner::run(0, Entry::Consume, s, c).has_value();
}
} // namespace ttyd
