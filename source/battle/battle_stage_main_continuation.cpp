#include "battle_stage_main_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
bool same(const BattleActorServices &a, const BattleActorServices &b) {
  const auto &x = a.memory;
  const auto &y = b.memory;
  return a.user == b.user && a.byte == b.byte && x.user == y.user &&
         x.word == y.word && x.half == y.half && x.storeWord == y.storeWord &&
         x.storeHalf == y.storeHalf && x.storeByte == y.storeByte &&
         x.flushBlock == y.flushBlock && x.systemCall == y.systemCall &&
         a.audienceLifecycle.user == b.audienceLifecycle.user &&
         a.audienceLifecycle.event == b.audienceLifecycle.event;
}
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
float number(uint32_t w) {
  float n;
  std::memcpy(&n, &w, 4);
  return n;
}
int32_t integer(uint32_t w) {
  int32_t n;
  std::memcpy(&n, &w, 4);
  return n;
}
} // namespace
struct BattleStageObjectCursor::State {
  BattleStageObjectServices owner;
  uint32_t b = 0, r = 0, layer = 0, name = 0;
  unsigned row = 0, phase = 0;
  BattleStageObjectServices::Point point{};
  bool hidden = false;
};
BattleStageObjectCursor::BattleStageObjectCursor() = default;
BattleStageObjectCursor::~BattleStageObjectCursor() = default;
bool BattleStageObjectCursor::active() const { return bool(state_); }
bool BattleStageObjectCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleStageObjectMain(const BattleStageObjectServices &s,
                                       BattleStageObjectCursor &cursor) {
  if (cursor.invoking_)
    return false;
  if (cursor.state_) {
    const auto &o = cursor.state_->owner;
    if (!same(o.actors, s.actors) || o.user != s.user ||
        o.translate != s.translate || o.rotate != s.rotate ||
        o.groupFlags != s.groupFlags || o.groupFlag != s.groupFlag)
      return false;
  }
  const auto &m = s.actors.memory;
  if (!m.word || !m.half || !m.storeWord)
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleStageObjectCursor::State>();
    cursor.state_->owner = s;
    cursor.state_->b = currentBattleAddress(s.actors);
  }
  auto &c = *cursor.state_;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  auto real = [&](uint32_t a) { return number(word(a)); };
  auto name = [&]() { return word(word(c.r + 0x64)); };
  auto point = [&](uint32_t a) {
    return BattleStageObjectServices::Point{real(a), real(a + 4), real(a + 8)};
  };
  auto different = [&](uint32_t a, uint32_t cache) {
    for (unsigned i = 0; i < 3; ++i)
      if (real(a + 4 * i) != real(cache + 4 * i))
        return true;
    return false;
  };
  auto copy = [&](uint32_t a, uint32_t cache) {
    const auto x = word(a), y = word(a + 4);
    m.storeWord(m.user, cache, x);
    m.storeWord(m.user, cache + 4, y);
    m.storeWord(m.user, cache + 8, word(a + 8));
  };
  for (;;)
    switch (c.phase) {
    case 0: {
      if (c.row == 32) {
        cursor.state_.reset();
        return true;
      }
      c.r = c.b + 0x1715c + 0x7c * c.row;
      const auto ordinal = word(c.r);
      if (!ordinal || ordinal >= 0x80000000) {
        ++c.row;
        break;
      }
      bool dirty = false;
      for (const auto offset : {4u, 0x10u, 0x28u, 0x1cu})
        if (different(c.r + offset, c.r + offset + 0x30)) {
          copy(c.r + offset, c.r + offset + 0x30);
          dirty = true;
        }
      const auto flags = word(c.r + 0x68);
      if (flags & 2) {
        m.storeWord(m.user, c.r + 0x68, flags & ~2u);
        dirty = true;
      }
      const auto descriptor = word(c.r + 0x64);
      if (m.half(m.user, descriptor + 6) == 2 &&
          different(c.b + 0x180e0, c.b + 0x180ec)) {
        copy(c.b + 0x180e0, c.b + 0x180ec);
        dirty = true;
      }
      c.layer = m.half(m.user, word(c.r + 0x64) + 6);
      if (dirty && c.layer < 8) {
        for (unsigned i = 0; i < 3; ++i) {
          const float first = real(c.r + 4 + 4 * i) + real(c.r + 0x10 + 4 * i);
          c.point[i] = real(c.r + 0x28 + 4 * i) + first;
        }
        c.name = name();
        c.phase = 1;
      } else
        c.phase = 8;
      break;
    }
    case 1:
      if (!s.translate || !s.translate(s.user, c.name, c.point))
        return false;
      c.phase = (c.layer == 2 || c.layer == 3) ? 2 : 4;
      break;
    case 2:
      c.point = {0, -real(c.r + (c.layer == 2 ? 0x14 : 8)), 0};
      c.name = name();
      c.phase = 3;
      break;
    case 3:
      if (!s.translate || !s.translate(s.user, c.name, c.point))
        return false;
      c.phase = 4;
      break;
    case 4:
      c.point = point(c.layer == 2 ? c.b + 0x180e0 : c.r + 0x1c);
      c.name = name();
      c.phase = 5;
      break;
    case 5:
      if (!s.rotate || !s.rotate(s.user, c.name, c.point))
        return false;
      c.phase = (c.layer == 2 || c.layer == 3) ? 6 : 8;
      break;
    case 6:
      c.point = {0, real(c.r + (c.layer == 2 ? 0x14 : 8)), 0};
      c.name = name();
      c.phase = 7;
      break;
    case 7:
      if (!s.translate || !s.translate(s.user, c.name, c.point))
        return false;
      c.phase = 8;
      break;
    case 8:
      c.hidden = (word(c.r + 0x68) & 1) != 0;
      c.name = name();
      c.phase = 9;
      break;
    case 9: {
      if (!s.groupFlags)
        return false;
      const auto actual = s.groupFlags(s.user, c.name);
      if (!actual)
        return false;
      if (((*actual & 1) != 0) != c.hidden)
        c.phase = 10;
      else {
        ++c.row;
        c.phase = 0;
      }
      break;
    }
    case 10:
      c.name = name();
      c.phase = 11;
      break;
    case 11:
      if (!s.groupFlag || !s.groupFlag(s.user, c.name, 1, c.hidden))
        return false;
      ++c.row;
      c.phase = 0;
      break;
    default:
      return false;
    }
}
struct BattleStageWorkCursor::State {
  BattleStageWorkServices owner;
  uint32_t w = 0, e = 0, allocation = 0;
  unsigned row = 0, index = 0, phase = 0;
  int32_t mode = 0, elapsed = 0, duration = 0;
  float start = 0, end = 0, value = 0;
};
BattleStageWorkCursor::BattleStageWorkCursor() = default;
BattleStageWorkCursor::~BattleStageWorkCursor() = default;
bool BattleStageWorkCursor::active() const { return bool(state_); }
bool BattleStageWorkCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleStageWorkMain(const BattleStageWorkServices &s,
                                     BattleStageWorkCursor &cursor) {
  if (cursor.invoking_)
    return false;
  if (cursor.state_) {
    const auto &o = cursor.state_->owner;
    if (!same(o.actors, s.actors) || o.user != s.user ||
        o.spotlightMain != s.spotlightMain || o.activeEntry != s.activeEntry ||
        o.freeScratch != s.freeScratch || o.interpolate != s.interpolate ||
        o.truncate != s.truncate)
      return false;
  }
  const auto &m = s.actors.memory;
  if (!m.word || !m.storeWord || !m.storeByte || !s.actors.byte)
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleStageWorkCursor::State>();
    cursor.state_->owner = s;
    cursor.state_->w = currentBattleAddress(s.actors) + 0x163fc;
  }
  auto &c = *cursor.state_;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  struct Counter {
    uint32_t count, start, end, mode, duration, output;
  };
  constexpr Counter counters[] = {{0xaf0, 0xaea, 0xaec, 0xb00, 0xaf8, 0xae8},
                                  {0xaf4, 0xaeb, 0xaed, 0xb01, 0xafc, 0xae9},
                                  {0xb10, 0xb0a, 0xb0c, 0xb20, 0xb18, 0xb08},
                                  {0xb14, 0xb0b, 0xb0d, 0xb21, 0xb1c, 0xb09}};
  for (;;)
    switch (c.phase) {
    case 0:
      if (!s.spotlightMain || !s.spotlightMain(s.user))
        return false;
      c.phase = 1;
      break;
    case 1:
      if (c.row == 12) {
        c.phase = 4;
        break;
      }
      c.e = c.w + 4 + 0xe8 * c.row;
      if (word(c.e) & 1)
        c.phase = 2;
      else {
        c.allocation = word(c.e + 0xe4);
        if (c.allocation)
          c.phase = 3;
        else
          ++c.row;
      }
      break;
    case 2:
      if (!s.activeEntry || !s.activeEntry(s.user, c.e))
        return false;
      ++c.row;
      c.phase = 1;
      break;
    case 3:
      if (!s.freeScratch || !s.freeScratch(s.user, c.allocation))
        return false;
      m.storeWord(m.user, c.e + 0xe4, 0);
      ++c.row;
      c.phase = 1;
      break;
    case 4: {
      if (c.index == 4) {
        cursor.state_.reset();
        return true;
      }
      const auto &v = counters[c.index];
      const auto old = word(c.w + v.count);
      if (integer(old) <= 0) {
        ++c.index;
        break;
      }
      m.storeWord(m.user, c.w + v.count, old - 1);
      c.start = s.actors.byte(s.actors.user, c.w + v.start);
      c.end = s.actors.byte(s.actors.user, c.w + v.end);
      const auto mode = s.actors.byte(s.actors.user, c.w + v.mode);
      c.mode = mode < 128 ? mode : int32_t(mode) - 256;
      c.duration = integer(word(c.w + v.duration));
      const auto remaining = word(c.w + v.count);
      c.elapsed = integer(uint32_t(c.duration) - remaining);
      c.phase = 5;
      break;
    }
    case 5: {
      if (!s.interpolate)
        return false;
      const auto value =
          s.interpolate(s.user, c.mode, c.elapsed, c.duration, c.start, c.end);
      if (!value)
        return false;
      c.value = *value;
      c.phase = 6;
      break;
    }
    case 6: {
      if (!s.truncate)
        return false;
      const auto n = s.truncate(s.user, c.value);
      if (!n)
        return false;
      m.storeByte(m.user, c.w + counters[c.index].output,
                  uint8_t(uint32_t(*n)));
      ++c.index;
      c.phase = 4;
      break;
    }
    default:
      return false;
    }
}
} // namespace ttyd
