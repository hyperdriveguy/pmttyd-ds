#include "battle_act_icon_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t v) {
  int32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
bool same(const BattleActIconDrawServices &a,
          const BattleActIconDrawServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.byte == b.memory.byte && x.user == y.user &&
         x.word == y.word && x.half == y.half && x.storeWord == y.storeWord &&
         x.storeHalf == y.storeHalf && x.storeByte == y.storeByte &&
         x.flushBlock == y.flushBlock && x.systemCall == y.systemCall &&
         a.bubble == b.bubble && a.circle == b.circle;
}
// Unavailable is a sticky source-stack/bounds frontier until reset; adding
// guessed slots or resuming with a different vector does not repair this call.
enum class Phase { Positions, Sort, Row, Bubble, Circle, Unavailable };
} // namespace
struct BattleActIconDrawCursor::State {
  BattleActIconDrawServices owner;
  const std::vector<BattleActDrawSlot> *slots = nullptr;
  uint32_t work = 0, rows = 0, cursor = 0, count = 0, index = 0, row = 0,
           rowAddress = 0, flags = 0;
  Phase phase = Phase::Positions;
  std::vector<float> depths;
  std::vector<uint32_t> order;
  BattleActMatrix matrix{};
  std::array<float, 3> position{};
  float parameter = 0;
};
BattleActIconDrawCursor::BattleActIconDrawCursor() = default;
BattleActIconDrawCursor::~BattleActIconDrawCursor() = default;
bool BattleActIconDrawCursor::active() const { return bool(state_); }
void BattleActIconDrawCursor::reset() { state_.reset(); }
bool resumePackedBattleActIcons(uint32_t work, uint32_t rows,
                                const std::vector<BattleActDrawSlot> &slots,
                                const BattleActIconDrawServices &s,
                                BattleActIconDrawCursor &cursor) {
  if (cursor.state_) {
    const auto &c = *cursor.state_;
    if (!same(c.owner, s) || c.slots != &slots || c.work != work ||
        c.rows != rows)
      return false;
  }
  const auto &m = s.memory.memory;
  if (!m.word)
    return false;
  auto word = [&](uint32_t p) { return m.word(m.user, p); };
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleActIconDrawCursor::State>();
    auto &c = *cursor.state_;
    c.owner = s;
    c.slots = &slots;
    c.work = work;
    c.rows = rows;
    const auto b = currentBattleAddress(s.memory),
               u = battleRegistryUnit(b, integer(word(b + 0x420)), s.memory);
    c.cursor = b + (word(u + 8) == 222 ? 0x1bb4 : 0x1bc0);
    c.count = word(c.cursor + 8);
  }
  auto &c = *cursor.state_;
  for (;;)
    switch (c.phase) {
    case Phase::Positions:
      for (uint32_t i = 0; integer(i) < integer(c.count); ++i) {
        if (i >= slots.size() || !slots[i].position) {
          c.phase = Phase::Unavailable;
          return false;
        }
        c.depths.push_back((*slots[i].position)[1]);
        c.order.push_back(i);
      }
      c.phase = Phase::Sort;
      break;
    // Depth is slot Y; strict< swaps descending order. Equal or unordered
    // comparisons do not swap at that comparison; do not claim a stable sort.
    // Count is reloaded during sorting; growth beyond captured depths holds.
    case Phase::Sort:
      for (uint32_t i = 0;; ++i) {
        const auto count = word(c.cursor + 8);
        if (integer(i) >= integer(count - 1))
          break;
        for (uint32_t j = i + 1; integer(j) < integer(count); ++j) {
          if (i >= c.depths.size() || j >= c.depths.size()) {
            c.phase = Phase::Unavailable;
            return false;
          }
          if (c.depths[i] < c.depths[j]) {
            std::swap(c.depths[i], c.depths[j]);
            std::swap(c.order[i], c.order[j]);
          }
        }
      }
      c.phase = Phase::Row;
      break;
    // Fresh flags precede borrowing this row's actual matrix/parameter/position.
    // Once bubble/circle phase starts, those operands are pinned across retries;
    // acknowledged siblings are skipped and later rows still observe changes.
    case Phase::Row: {
      const auto count = word(c.cursor + 8);
      if (integer(c.index) >= integer(count)) {
        cursor.reset();
        return true;
      }
      if (c.index >= c.order.size()) {
        c.phase = Phase::Unavailable;
        return false;
      }
      c.row = c.order[c.index];
      c.flags = word(c.work + 0x20 + c.row * 4);
      if (!(c.flags & 1)) {
        ++c.index;
        break;
      }
      // The actual stack remains borrowed. An acknowledged earlier child may
      // have changed a later slot; source observes it here, after its fresh
      // flag read.
      if (c.row >= slots.size() || !slots[c.row].matrix ||
          !slots[c.row].parameter) {
        c.phase = Phase::Unavailable;
        return false;
      }
      c.matrix = *slots[c.row].matrix;
      c.parameter = *slots[c.row].parameter;
      c.rowAddress = c.rows + 20 * c.row;
      if (c.flags & 2)
        c.phase = Phase::Bubble;
      else {
        if (!slots[c.row].position) {
          c.phase = Phase::Unavailable;
          return false;
        }
        c.position = *slots[c.row].position;
        c.phase = Phase::Circle;
      }
      break;
    }
    case Phase::Bubble:
      if (!s.bubble ||
          !s.bubble(s.user, c.work, c.rowAddress, c.matrix, c.parameter))
        return false;
      ++c.index;
      c.phase = Phase::Row;
      break;
    case Phase::Circle:
      if (!s.circle || !s.circle(s.user, c.work, c.rowAddress, c.matrix,
                                 c.position, c.parameter))
        return false;
      ++c.index;
      c.phase = Phase::Row;
      break;
    case Phase::Unavailable:
      return false;
    }
}
} // namespace ttyd
