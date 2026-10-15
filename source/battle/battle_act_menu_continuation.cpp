#include "battle_act_menu_continuation.hpp"
#include <initializer_list>
namespace ttyd {
namespace {
int32_t sw(uint32_t n) {
  return n < 0x80000000u ? int32_t(n) : int32_t(int64_t(n) - 0x100000000LL);
}
int sb(uint8_t n) { return n < 128 ? int(n) : int(n) - 256; }
enum class Entry { Main, Setup, End, Position };
bool same(const BattleActMenuContinuationServices &a,
          const BattleActMenuContinuationServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.request == b.request &&
         a.memory.user == b.memory.user && a.memory.byte == b.memory.byte &&
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
} // namespace
struct BattleActMenuContinuationCursor::State {
  BattleActMenuContinuationServices owner;
  Entry entry = Entry::Main;
  uint32_t b = 0, m = 0, flag = 0, pc = 0, pending = 0, u = 0, row = 0,
           type = 0, j = 0, offset = 0, positionUnit = 0, endReturn = 0,
           positionReturn = 0;
  int32_t result = 0;
  std::optional<uint32_t> cursor;
  std::optional<BattleActMenuContinuationRequest> held;
  uint32_t word(uint32_t a) const {
    return owner.memory.memory.word(owner.memory.memory.user, a);
  }
  void put(uint32_t a, uint32_t v) const {
    owner.memory.memory.storeWord(owner.memory.memory.user, a, v);
  }
  uint8_t byte(uint32_t a) const {
    return owner.memory.byte(owner.memory.user, a);
  }
  void pb(uint32_t a, uint8_t v) const {
    owner.memory.memory.storeByte(owner.memory.memory.user, a, v);
  }
  bool known() {
    if (cursor)
      return true;
    pending = uint32_t(BattleActService::Cursor);
    return false;
  }
  uint32_t selectedRow() const { return m + 20 * word(*cursor); }
  template <class Build>
  std::optional<uint32_t> call(BattleActService fn, Build build) {
    pending = uint32_t(fn);
    if (!held) {
      held = BattleActMenuContinuationRequest{};
      held->function = fn;
      held->callsite = pc;
      build(*held);
    }
    if (!owner.request)
      return {};
    auto out = owner.request(owner.user, *held);
    if (out) {
      held.reset();
      pending = 0;
    }
    return out;
  }
  std::optional<uint32_t> call(BattleActService fn,
                               std::initializer_list<uint32_t> args = {}) {
    return call(fn, [&](BattleActMenuContinuationRequest &r) {
      for (auto a : args)
        r.arguments[r.count++] = a;
    });
  }
  void end(uint32_t next) {
    endReturn = next;
    pc = 0x801220e8;
  }
  void position(uint32_t next) {
    positionReturn = next;
    pc = 0x80122160;
  }
};
BattleActMenuContinuationCursor::BattleActMenuContinuationCursor() = default;
BattleActMenuContinuationCursor::~BattleActMenuContinuationCursor() = default;
bool BattleActMenuContinuationCursor::active() const { return bool(state_); }
uint32_t BattleActMenuContinuationCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleActMenuContinuationCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
struct BattleActMenuContinuationRunner {
  static std::optional<int32_t> run(uint32_t b, uint32_t flag, Entry entry,
                                    const BattleActMenuContinuationServices &s,
                                    BattleActMenuContinuationCursor &cursor) {
    const auto &m = s.memory.memory;
    if (cursor.invoking_ || !m.word || !m.storeWord || !m.storeByte ||
        !s.memory.byte)
      return {};
    if (cursor.state_ &&
        (cursor.state_->b != b || cursor.state_->flag != flag ||
         cursor.state_->entry != entry || !same(cursor.state_->owner, s)))
      return {};
    Running guard(cursor.invoking_);
    if (!cursor.state_) {
      cursor.state_ =
          std::make_unique<BattleActMenuContinuationCursor::State>();
      auto &c = *cursor.state_;
      c.owner = s;
      c.b = b;
      c.m = b + 0x171c;
      c.flag = flag;
      c.entry = entry;
      c.pc = entry == Entry::Setup      ? 0x801227b0
             : entry == Entry::End      ? 0x801220e8
             : entry == Entry::Position ? 0x80122160
                                        : 0x80122328;
    }
    auto &c = *cursor.state_;
    auto finish = [&](int32_t v) -> std::optional<int32_t> {
      cursor.state_.reset();
      return v;
    };
    auto positioned = [&](int32_t v) -> std::optional<int32_t> {
      if (entry == Entry::Position)
        return finish(v);
      c.pc = v ? (c.positionReturn == 0x80122360 ? 0x8012236c : 0x80122740)
               : (c.positionReturn == 0x80122360 ? 0x80122768 : 0x80122778);
      return {};
    };
    using F = BattleActService;
    for (;;)
      switch (c.pc) {
      case 0x801227b0:
        if (!c.call(F::ClassTable, {b}))
          return {};
        c.pc = 0x801227c0;
        break;
      case 0x80122344:
      case 0x801227c0:
        if (!c.call(F::Cursor, [&](BattleActMenuContinuationRequest &r) {
              r.arguments = {c.m, 14};
              r.count = 2;
              r.privateCursorOutput = &c.cursor;
            }))
          return {};
        c.pc = c.pc == 0x80122344 ? 0x8012234c : 0x801227c4;
        break;
      case 0x801227c4:
        if (!c.known())
          return {};
        c.pc = 0x801227d4;
        break;
      case 0x801227d4:
        if (!c.call(F::Init, [&](BattleActMenuContinuationRequest &r) {
              r.arguments = {b, *c.cursor, c.m + 8, c.flag};
              r.count = 4;
            }))
          return {};
        c.put(c.m + 0x560, 0);
        c.put(c.m + 4, c.word(c.m + 4) | 0x01000001);
        if (!c.flag && !c.byte(c.m + 0x558))
          for (auto off : {0x4b0u, 0x4bcu, 0x4c8u, 0x4d4u, 0x4e0u, 0x4ecu,
                           0x4f8u, 0x504u, 0x51cu}) {
            c.put(c.m + off, 0);
            c.put(c.m + off + 4, 0);
          }
        return finish(0);
      case 0x801220e8:
        if (!c.call(F::End, {b}))
          return {};
        c.put(b + 0x1720, c.word(b + 0x1720) & ~1u);
        if (entry == Entry::End)
          return finish(0);
        c.pc = c.endReturn;
        break;
      case 0x80122160: {
        const auto flags = c.word(b + 0xef4);
        if (flags & 0x00800000 || flags & 0x40000000) {
          auto done = positioned(0);
          if (done)
            return done;
        } else
          c.pc = 0x80122184;
        break;
      }
      case 0x80122184:
      case 0x80122240: {
        const bool mario = c.pc == 0x80122184;
        auto out = c.call(mario ? F::Mario : F::Party, {b});
        if (!out)
          return {};
        c.positionUnit = *out;
        if (!*out || sb(c.byte(*out + 0x20))) {
          auto done = positioned(0);
          if (done)
            return done;
          break;
        }
        const auto count = sb(c.byte(*out + 0x23));
        if (count <= 0 || sb(c.byte(*out + 0x22)) != count) {
          auto done = positioned(0);
          if (done)
            return done;
          break;
        }
        c.pc = mario ? 0x801221d8 : 0x80122294;
        break;
      }
      case 0x801221d8:
      case 0x801221ec:
      case 0x80122200:
      case 0x80122214:
      case 0x80122294:
      case 0x801222a8:
      case 0x801222bc:
      case 0x801222d0: {
        const bool mario = c.pc <= 0x80122214;
        const auto i = (c.pc - (mario ? 0x801221d8u : 0x80122294u)) / 20;
        constexpr uint32_t ids[]{1, 2, 9, 27};
        auto out = c.call(F::Status, {c.positionUnit, ids[i]});
        if (!out)
          return {};
        if (*out) {
          auto done = positioned(0);
          if (done)
            return done;
          break;
        }
        if (i != 3) {
          c.pc += 20;
          break;
        }
        if (mario) {
          if (c.word(c.positionUnit + 0x104) & 16) {
            auto done = positioned(0);
            if (done)
              return done;
          } else
            c.pc = 0x80122240;
        } else {
          auto done = positioned(c.byte(c.positionUnit + 0x12b) == 0 ? 1 : 0);
          if (done)
            return done;
        }
        break;
      }
      case 0x80122328: {
        auto out = c.call(F::Unit, [&](BattleActMenuContinuationRequest &r) {
          r.arguments = {b, c.word(b + 0x420)};
          r.count = 2;
        });
        if (!out)
          return {};
        c.u = *out;
        c.pc = 0x80122344;
        break;
      }
      case 0x8012234c:
      case 0x80122720: {
        auto out = c.call(F::Trigger, {0x800});
        if (!out)
          return {};
        if (*out)
          c.position(c.pc == 0x8012234c ? 0x80122360 : 0x80122734);
        else
          c.pc = c.pc == 0x8012234c ? 0x80122768 : 0x80122778;
        break;
      }
      case 0x8012236c:
      case 0x80122740:
        if (!c.call(F::Sound, {2520}))
          return {};
        c.end(c.pc == 0x8012236c ? 0x80122378 : 0x8012274c);
        break;
      case 0x80122378:
      case 0x8012274c:
        c.put(c.m + 0x540, 9);
        c.put(c.m, 29);
        c.result = 1;
        c.pc = c.pc == 0x80122378 ? 0x8012238c : 0x80122778;
        break;
      case 0x8012238c:
      case 0x80122778:
        if (!c.call(F::Display, {b}))
          return {};
        return finish(c.result);
      case 0x80122768: {
        auto out = c.call(F::Allowed, {b});
        if (!out)
          return {};
        if (*out) {
          c.j = 0;
          c.offset = 0;
          c.pc = 0x80122440;
        } else
          c.pc = 0x80122778;
        break;
      }
      case 0x80122440:
        if (!c.known())
          return {};
        if (sw(c.j) >= sw(c.word(*c.cursor + 8))) {
          c.pc = 0x80122454;
          break;
        }
        c.row = c.m + c.offset;
        c.type = c.word(c.row + 8);
        if ((c.type == 0 && (c.word(b + 0xef4) & 0x200)) ||
            (c.type == 1 && (c.word(b + 0xef4) & 0x400)))
          c.pc = 0x801223ec;
        else
          c.pc = 0x80122438;
        break;
      case 0x801223ec:
        if (!c.call(F::WeaponTable, {b, c.type}))
          return {};
        c.pc = 0x801223fc;
        break;
      case 0x801223fc: {
        auto out =
            c.call(F::TargetCount, [&](BattleActMenuContinuationRequest &r) {
              r.arguments = {b, c.u, c.word(c.m + 0x80)};
              r.count = 3;
            });
        if (!out)
          return {};
        if (sw(*out) <= 0) {
          c.put(c.row + 12, 0);
          if (!c.word(c.row + 24))
            c.put(c.row + 24, c.type == 0 ? 4 : 5);
        }
        c.pc = 0x80122438;
        break;
      }
      case 0x80122438:
        ++c.j;
        c.offset += 20;
        c.pc = 0x80122440;
        break;
      case 0x80122454: {
        auto out = c.call(F::Now, {0xffffffff});
        if (!out)
          return {};
        if (*out) {
          c.put(c.m + 0x560, 0);
          c.pc = 0x80122470;
        } else {
          const auto timer = c.word(c.m + 0x560) + 1;
          c.put(c.m + 0x560, timer);
          c.pc = sw(timer) >= 900 ? 0x80122494 : 0x801224a4;
        }
        break;
      }
      case 0x80122470:
      case 0x80122494:
        if (!c.call(F::CameraMode, {0, c.pc == 0x80122470 ? 0u : 6u}))
          return {};
        c.pc = c.pc == 0x80122470 ? 0x801224a4 : 0x801224a0;
        break;
      case 0x801224a0:
        if (!c.call(F::CameraSpeed, {0, 5}))
          return {};
        c.pc = 0x801224a4;
        break;
      case 0x801224a4:
        if (!c.byte(b + 0x1c74)) {
          c.pc = 0x801224fc;
          break;
        }
        c.pc = 0x801224b0;
        break;
      case 0x801224b0: {
        if (!c.known())
          return {};
        const auto t = c.word(c.selectedRow() + 8);
        if ((t == 0 && (c.word(b + 0xef4) & 0x200)) ||
            (t == 1 && (c.word(b + 0xef4) & 0x400))) {
          c.pb(b + 0x1c74, 0);
          c.put(b + 0x1c70, 0);
        }
        c.pc = 0x801224fc;
        break;
      }
      case 0x801224fc: {
        auto out = c.call(F::Trigger, {0x100});
        if (!out)
          return {};
        c.pc = (*out || c.byte(b + 0x1c74)) ? 0x80122514 : 0x80122660;
        break;
      }
      case 0x80122514:
        if (!c.known())
          return {};
        c.pc = c.word(c.selectedRow() + 12) ? 0x80122534 : 0x801225f0;
        break;
      case 0x80122534:
        if (!c.call(F::Sound, {2520}))
          return {};
        c.pc = 0x80122538;
        break;
      case 0x80122538: {
        if (!c.known())
          return {};
        c.put(c.m + 0x540, c.word(c.selectedRow() + 8));
        const auto type = c.word(c.m + 0x540);
        switch (type) {
        case 0:
        case 1:
        case 4:
        case 6:
        case 7:
        case 8:
          c.put(c.m, 5);
          c.result = 1;
          break;
        case 2:
          c.put(c.m, 5);
          if (sb(c.byte(c.u + 0x20)) == 0) {
            c.pb(c.m + 0x550, 1);
            if (c.byte(c.u + 0x2f3) || c.byte(c.u + 0x2f4)) {
              c.put(c.m, 14);
              c.result = 1;
            }
          }
          break;
        case 5:
          c.put(c.m, 19);
          c.result = 1;
          break;
        case 13:
          c.put(c.m, 29);
          c.result = 1;
          break;
        default:
          break;
        }
        c.end(0x80122778);
        break;
      }
      case 0x801225f0:
        c.pc = c.byte(b + 0x1c74) ? 0x80122608 : 0x80122604;
        break;
      case 0x80122604:
        if (!c.call(F::Sound, {0x802efe88}))
          return {};
        c.pc = 0x80122608;
        break;
      case 0x80122608:
        c.pb(b + 0x1c74, 0);
        c.pc = 0x80122610;
        break;
      case 0x80122610:
        if (!c.known())
          return {};
        if (c.word(c.selectedRow() + 24))
          c.end(0x80122634);
        else
          c.pc = 0x80122660;
        break;
      case 0x80122634:
        c.put(c.m, 3);
        c.result = 1;
        c.pc = 0x80122640;
        break;
      case 0x80122640:
        if (!c.known())
          return {};
        c.put(c.m + 0x564, c.word(c.selectedRow() + 24));
        c.pc = 0x80122778;
        break;
      case 0x80122660: {
        auto out = c.call(F::Repeat, {0x80000});
        if (!out)
          return {};
        if (!*out) {
          c.pc = 0x801226bc;
          break;
        }
        c.pc = 0x8012266c;
        break;
      }
      case 0x8012266c: {
        if (!c.known())
          return {};
        const auto ptr = *c.cursor, count = c.word(ptr + 8),
                   index = c.word(ptr);
        if (sw(index) < sw(count - 1)) {
          c.put(ptr, index + 1);
          c.pc = 0x80122694;
        } else
          c.pc = 0x801226a4;
        break;
      }
      case 0x80122694:
      case 0x801226e8:
        if (!c.call(F::Sound, {0x802efef4}))
          return {};
        c.pc = 0x80122778;
        break;
      case 0x801226a4:
        if (!c.call(F::Sound, {0x802efef4}))
          return {};
        c.pc = 0x801226a8;
        break;
      case 0x801226a8:
        if (!c.known())
          return {};
        c.put(*c.cursor, 0);
        c.pc = 0x80122778;
        break;
      case 0x801226bc: {
        auto out = c.call(F::Repeat, {0x40000});
        if (!out)
          return {};
        c.pc = *out ? 0x801226c8 : 0x80122710;
        break;
      }
      case 0x801226c8: {
        if (!c.known())
          return {};
        const auto ptr = *c.cursor, index = c.word(ptr);
        if (sw(index) > 0) {
          c.put(ptr, index - 1);
          c.pc = 0x801226e8;
        } else
          c.pc = 0x801226f8;
        break;
      }
      case 0x801226f8:
        if (!c.call(F::Sound, {0x802efef4}))
          return {};
        c.pc = 0x801226fc;
        break;
      case 0x801226fc:
        if (!c.known())
          return {};
        c.put(*c.cursor, c.word(*c.cursor + 8) - 1);
        c.pc = 0x80122778;
        break;
      case 0x80122710: {
        auto out = c.call(F::Audience);
        if (!out)
          return {};
        c.pc = *out ? 0x80122778 : 0x80122720;
        break;
      }
      default:
        return {};
      }
  }
};
std::optional<int32_t>
resumePackedBattleActMenuMain(uint32_t b,
                              const BattleActMenuContinuationServices &s,
                              BattleActMenuContinuationCursor &c) {
  return BattleActMenuContinuationRunner::run(b, 0, Entry::Main, s, c);
}
bool resumePackedBattleActMenuSetup(uint32_t b, uint32_t flag,
                                    const BattleActMenuContinuationServices &s,
                                    BattleActMenuContinuationCursor &c) {
  return BattleActMenuContinuationRunner::run(b, flag, Entry::Setup, s, c)
      .has_value();
}
bool resumePackedBattleActMenuEnd(uint32_t b,
                                  const BattleActMenuContinuationServices &s,
                                  BattleActMenuContinuationCursor &c) {
  return BattleActMenuContinuationRunner::run(b, 0, Entry::End, s, c)
      .has_value();
}
std::optional<int32_t>
resumePackedBattleChangePosition(uint32_t b,
                                 const BattleActMenuContinuationServices &s,
                                 BattleActMenuContinuationCursor &c) {
  return BattleActMenuContinuationRunner::run(b, 0, Entry::Position, s, c);
}
} // namespace ttyd

namespace ttyd {
struct BattleActUiContinuationCursor::State {
  BattleActUiServices owner;
  uint32_t b = 0, cursor = 0, rows = 0, flag = 0, old = 0, pending = 0;
  uint8_t phase = 0;
  bool ending = false;
};
BattleActUiContinuationCursor::BattleActUiContinuationCursor() = default;
BattleActUiContinuationCursor::~BattleActUiContinuationCursor() = default;
bool BattleActUiContinuationCursor::active() const { return bool(state_); }
uint32_t BattleActUiContinuationCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleActUiContinuationCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
struct BattleActUiContinuationRunner {
  static bool run(uint32_t b, uint32_t cursor, uint32_t rows, uint32_t flag,
                  bool ending, const BattleActUiServices &s,
                  BattleActUiContinuationCursor &hold) {
    const auto &m = s.memory.memory;
    if (hold.invoking_ || !m.word || !m.storeWord || !m.storeByte)
      return false;
    if (hold.state_) {
      const auto &c = *hold.state_;
      BattleActMenuContinuationServices a, d;
      a.memory = c.owner.memory;
      d.memory = s.memory;
      if (c.b != b || c.cursor != cursor || c.rows != rows || c.flag != flag ||
          c.ending != ending || c.owner.user != s.user ||
          c.owner.allocate != s.allocate || c.owner.release != s.release ||
          !same(a, d))
        return false;
    }
    Running guard(hold.invoking_);
    if (!hold.state_) {
      hold.state_ = std::make_unique<BattleActUiContinuationCursor::State>();
      auto &c = *hold.state_;
      c.owner = s;
      c.b = b;
      c.cursor = cursor;
      c.rows = rows;
      c.flag = flag;
      c.ending = ending;
    }
    auto &c = *hold.state_;
    auto word = [&](uint32_t a) { return m.word(m.user, a); };
    auto put = [&](uint32_t a, uint32_t v) { m.storeWord(m.user, a, v); };
    if (c.phase == 0) {
      c.old = word(b + 0x1c78);
      c.phase = 1;
    }
    if (c.phase == 1) {
      if (c.old) {
        c.pending = 0x800f8908;
        if (!s.release || !s.release(s.user, c.old))
          return false;
        if (ending)
          put(b + 0x1c78, 0);
      }
      c.phase = 2;
    }
    if (ending) {
      hold.state_.reset();
      return true;
    }
    if (c.phase == 2) {
      c.pending = 0x800f8934;
      if (!s.allocate)
        return false;
      const auto allocated = s.allocate(s.user, 0x124);
      if (!allocated)
        return false;
      put(b + 0x1c78, *allocated);
      c.phase = 3;
    }
    // No external children remain. Guest reads stay at original use points;
    // W is captured only after publishing the actual allocation result.
    const auto w = word(b + 0x1c78);
    put(w, cursor);
    put(w + 4, rows);
    put(w + 12, (flag & 1) ? 10 : 0);
    m.storeByte(m.user, w + 20, 0);
    put(w + 24, word(cursor));
    if (flag & 1) {
      uint32_t selected = word(cursor), ordinal = 0;
      while (sw(ordinal) < sw(word(cursor + 8))) {
        put(w + 0x34 + 4 * selected, selected == word(cursor) ? 10 : 11);
        put(w + 0x48 + 4 * selected, 0);
        const auto count = word(cursor + 8);
        put(w + 0x84 + 4 * selected, count + ordinal - 1);
        const auto reverse = word(w + 0x84 + 4 * selected),
                   freshCount = word(cursor + 8);
        if (sw(reverse) >= sw(freshCount))
          put(w + 0x84 + 4 * selected, reverse - freshCount);
        put(w + 0x20 + 4 * selected, 0);
        ++selected;
        if (sw(selected) >= sw(word(cursor + 8)))
          selected = 0;
        ++ordinal;
      }
    } else {
      uint32_t selected = word(cursor), ordinal = 0;
      for (;;) {
        const auto count = word(cursor + 8);
        if (sw(ordinal) >= sw(count))
          break;
        ++selected;
        if (sw(selected) >= sw(count))
          selected = 0;
        put(w + 0x34 + 4 * selected, 0);
        const auto gp = word(0x80415fd0), rate = word(gp + 4);
        const auto step = sw(rate * 10) / 60;
        put(w + 0x48 + 4 * selected,
            uint32_t(sw(ordinal * uint32_t(step)) / 2));
        put(w + 0x84 + 4 * selected, ordinal);
        ++ordinal;
        put(w + 0x20 + 4 * selected, 0);
      }
    }
    hold.state_.reset();
    return true;
  }
};
bool resumePackedBattleActUiInit(uint32_t b, uint32_t cursor, uint32_t rows,
                                 uint32_t flag, const BattleActUiServices &s,
                                 BattleActUiContinuationCursor &c) {
  return BattleActUiContinuationRunner::run(b, cursor, rows, flag, false, s, c);
}
bool resumePackedBattleActUiEnd(uint32_t b, const BattleActUiServices &s,
                                BattleActUiContinuationCursor &c) {
  return BattleActUiContinuationRunner::run(b, 0, 0, 0, true, s, c);
}
} // namespace ttyd

namespace ttyd {
struct BattleActMenuAudienceAdmissionCursor::State {
  BattleMenuAudienceServices owner;
  BattleMenuAudienceCursor source;
};
BattleActMenuAudienceAdmissionCursor::BattleActMenuAudienceAdmissionCursor() =
    default;
BattleActMenuAudienceAdmissionCursor::~BattleActMenuAudienceAdmissionCursor() =
    default;
bool BattleActMenuAudienceAdmissionCursor::active() const {
  return bool(state_);
}
uint32_t BattleActMenuAudienceAdmissionCursor::pending() const {
  if (!state_)
    return 0;
  return state_->source.phase == 1   ? 0x801a2d58
         : state_->source.phase == 3 ? 0x801220d4
                                     : 0x8011f524;
}
bool BattleActMenuAudienceAdmissionCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
std::optional<bool> resumeAdmittedBattleActMenuAudience(
    const BattleMenuAudienceServices &s,
    BattleActMenuAudienceAdmissionCursor &cursor) {
  if (cursor.invoking_)
    return {};
  if (cursor.state_) {
    const auto &a = cursor.state_->owner;
    BattleActMenuContinuationServices x, y;
    x.memory = a.memory;
    y.memory = s.memory;
    if (!same(x, y) || a.user != s.user || a.privateItem != s.privateItem ||
        a.trigger != s.trigger || a.mario != s.mario || a.itemOn != s.itemOn ||
        a.setTarget != s.setTarget || a.endActMenu != s.endActMenu ||
        a.endActMenuReady != s.endActMenuReady || a.lightOff != s.lightOff)
      return {};
  }
  // These existing non-optional child interfaces require whole service
  // admission before entry. This guard adds no source query and supplies no
  // fake result.
  const auto &m = s.memory.memory;
  if (!m.word || !m.storeWord || !s.trigger || !s.mario || !s.itemOn ||
      !s.setTarget || (!s.endActMenu && !s.endActMenuReady) || !s.lightOff ||
      !s.privateItem)
    return {};
  Running guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ =
        std::make_unique<BattleActMenuAudienceAdmissionCursor::State>();
    cursor.state_->owner = s;
  }
  const auto result =
      resumePackedBattleMenuAudienceCheck(s, cursor.state_->source);
  if (result)
    cursor.state_.reset();
  return result;
}
} // namespace ttyd
