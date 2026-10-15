#include "battle_act_draw_continuation.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t v) {
  int32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
float scalar(uint32_t v) {
  float n;
  std::memcpy(&n, &v, 4);
  return n;
}
bool same(const BattleActDrawServices &a, const BattleActDrawServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.byte == b.memory.byte && x.user == y.user &&
         x.word == y.word && x.half == y.half && x.storeWord == y.storeWord &&
         x.storeHalf == y.storeHalf && x.storeByte == y.storeByte &&
         x.flushBlock == y.flushBlock && x.systemCall == y.systemCall &&
         a.camera == b.camera && a.center == b.center && a.offset == b.offset &&
         a.interpolate == b.interpolate && a.translation == b.translation &&
         a.rotation == b.rotation && a.scale == b.scale &&
         a.concat == b.concat && a.icons == b.icons &&
         a.partyButton == b.partyButton && a.pinchMark == b.pinchMark;
}
// Phases split original SDK boundaries, not simulation states. Acknowledged
// providers/stores never replay; operands are captured before entering a held
// phase while source reads after that boundary remain fresh.
enum class Phase {
  Camera,
  Mode,
  Row,
  Angle,
  Center,
  Offset,
  Branch,
  Spin,
  Scale,
  Translate,
  Concat,
  PostTransform,
  InterpolateX,
  InterpolateY,
  InterpolateParameter,
  FinishParameter,
  CaptureIcons,
  Icons,
  Party,
  Pinch
};
} // namespace
struct BattleActDrawCursor::State {
  BattleActDrawServices owner;
  Phase phase = Phase::Camera;
  uint32_t b = 0, w = 0, c = 0, i = 0, a = 0, loopCount = 0, rowState = 0,
           rows = 0;
  float angle = 0, endpoint = 0, rotation = 0, scale = 0, scaleZ = 0, zero = 0;
  std::array<float, 3> center{}, translation{};
  BattleActMatrix auxiliary{}, matrix{};
  bool selected = false;
  int32_t interpolationMode = 0, step = 0, duration = 0;
  float start = 0, end = 0;
  std::vector<BattleActDrawSlot> slots;
};
BattleActDrawCursor::BattleActDrawCursor() = default;
BattleActDrawCursor::~BattleActDrawCursor() = default;
bool BattleActDrawCursor::active() const { return bool(state_); }
void BattleActDrawCursor::reset() { state_.reset(); }
bool resumePackedBattleActMenuDraw(const BattleActDrawServices &s,
                                   BattleActDrawCursor &cursor) {
  if (cursor.state_ && !same(cursor.state_->owner, s))
    return false;
  const auto &m = s.memory.memory;
  if (!m.word || !m.storeWord || !m.storeByte)
    return false;
  auto word = [&](uint32_t p) { return m.word(m.user, p); };
  auto put = [&](uint32_t p, uint32_t v) { m.storeWord(m.user, p, v); };
  auto real = [&](uint32_t p) { return scalar(word(p)); };
  auto constant = [&](int32_t offset) {
    return real(0x80427440u + uint32_t(offset));
  };
  auto rate = [&]() { return word(word(0x80415fd0) + 4); };
  auto duration = [&](uint32_t n) { return integer(rate() * n) / 60; };
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleActDrawCursor::State>();
    auto &c = *cursor.state_;
    c.owner = s;
    c.b = currentBattleAddress(s.memory);
    c.w = word(c.b + 0x1c78);
    c.c = word(c.w);
  }
  auto &c = *cursor.state_;
  const auto advanceRow = [&]() {
    ++c.i;
    c.phase = Phase::Row;
  };
  const auto prepareInterpolation = [&](int32_t mode, int32_t step,
                                        int32_t ticks, float start, float end) {
    c.interpolationMode = mode;
    c.step = step;
    c.duration = ticks;
    c.start = start;
    c.end = end;
  };
  for (;;) {
    switch (c.phase) {
    case Phase::Camera:
      if (!s.camera || !s.camera(s.user, 8))
        return false;
      c.phase = Phase::Mode;
      break;
    // Same source UI/row state domains as immediate DrawMainMenu. This phase
    // performs mode stores once; a later camera/math/footer hold resumes after
    // them rather than advancing ring animation a second time.
    case Phase::Mode: {
      const auto mode = integer(word(c.w + 0xc));
      if (mode == 0 || mode == 2) {
        const auto count = word(c.c + 8);
        uint32_t n = 0;
        for (uint32_t i = 0; integer(i) < integer(count); ++i)
          if (word(c.w + 0x34 + i * 4) == 3)
            ++n;
        if (n == count)
          put(c.w + 0xc, 1);
      } else if (mode == 10) {
        put(c.w + 0xc, 1);
        m.storeByte(m.user, c.w + 0x14, 1);
      } else if (mode == 1) {
        m.storeByte(m.user, c.w + 0x14, 1);
        const auto live = word(c.w), previous = word(c.w + 0x18),
                   delta = word(live) - previous;
        bool next = delta == 1;
        uint32_t retained = previous;
        if (!next) {
          retained = word(live + 8) - 1;
          next = delta == 0u - retained;
        }
        const bool reverse =
            !next && (delta == uint32_t(-1) || delta == retained);
        if (next || reverse) {
          put(c.w + 0xc, 2);
          m.storeByte(m.user, c.w + 0x14, 0);
          put(c.w + 0x18, word(word(c.w)));
          for (uint32_t i = 0; integer(i) < integer(word(c.c + 8)); ++i) {
            const auto a = c.w + i * 4;
            put(a + 0x34, next ? 4 : 5);
            put(a + 0x48, 0);
            put(a + 0x70, word(a + 0x84));
            put(a + 0x84, word(a + 0x84) + (next ? uint32_t(-1) : 1u));
            if (next) {
              if (word(a + 0x84) == uint32_t(-1))
                put(a + 0x84, word(c.c + 8) - 1);
            } else if (word(a + 0x84) == word(c.c + 8))
              put(a + 0x84, 0);
          }
        }
      }
      put(c.w + 0x1c, 0);
      c.phase = Phase::Row;
      break;
    }
    case Phase::Row:
      c.loopCount = word(c.c + 8);
      if (integer(c.i) >= integer(c.loopCount)) {
        c.phase = Phase::CaptureIcons;
        break;
      }
      c.slots.emplace_back();
      c.a = c.w + c.i * 4;
      c.rowState = word(c.a + 0x34);
      if (c.rowState > 11) {
        advanceRow();
        break;
      }
      if (c.rowState == 0) {
        put(c.a + 0x48, word(c.a + 0x48) - 1);
        if (integer(word(c.a + 0x48)) <= 0) {
          put(c.a + 0x34, 1);
          put(c.a + 0x48, 0);
        }
        advanceRow();
        break;
      }
      if (c.rowState != 1 && c.rowState != 3 && c.rowState != 4 &&
          c.rowState != 5 && c.rowState != 10 && c.rowState != 11) {
        advanceRow();
        break;
      }
      c.phase = Phase::Angle;
      break;
    case Phase::Angle: {
      auto ring = [&](uint32_t count, uint32_t order) {
        return real(0x803635d8 + count * 20 + order * 4 - 60);
      };
      const float zero = constant(-20500), pi = constant(-20492),
                  degrees = constant(-20488);
      auto radians = [&](float angle) { return (pi * angle) / degrees; };
      c.zero = zero;
      c.angle = zero;
      c.endpoint = zero;
      if (c.rowState == 1) {
        put(c.a + 0x20, word(c.a + 0x20) | 1);
        const auto gp = word(0x80415fd0), count = word(c.c + 8),
                   freshRate = word(gp + 4), order = word(c.a + 0x84);
        const auto target = ring(count, order);
        const auto counter = word(c.a + 0x48);
        const float step = constant(-20496) / float(integer(freshRate));
        c.endpoint = radians(target);
        c.angle = std::fma(float(integer(counter)), step, zero);
        if (c.angle >= c.endpoint)
          c.angle = c.endpoint;
      } else if (c.rowState == 3 || c.rowState == 11)
        c.angle = radians(ring(c.rowState == 3 ? c.loopCount : word(c.c + 8),
                               word(c.a + 0x84)));
      else if (c.rowState == 4) {
        const auto order = word(c.a + 0x84), oldOrder = word(c.a + 0x70);
        auto target = ring(c.loopCount, order);
        const auto previous = ring(c.loopCount, oldOrder);
        if (target == zero)
          target = constant(-20476);
        const auto freshRate = rate(), counter = word(c.a + 0x48);
        const float endpoint = radians(target), start = radians(previous),
                    step = constant(-20496) / float(integer(freshRate));
        c.angle = std::fma(float(integer(counter)), step, start);
        if (c.angle >= endpoint) {
          c.angle = endpoint;
          put(c.a + 0x34, 3);
          put(c.a + 0x48, uint32_t(-1));
        }
      } else if (c.rowState == 5) {
        auto previous = ring(c.loopCount, word(c.a + 0x70));
        if (previous == zero)
          previous = constant(-20476);
        const auto gp = word(0x80415fd0), order = word(c.a + 0x84),
                   freshRate = word(gp + 4);
        const auto target = ring(c.loopCount, order);
        const auto counter = word(c.a + 0x48);
        const float start = radians(previous), endpoint = radians(target),
                    step = constant(-20496) / float(integer(freshRate));
        c.angle = std::fma(-float(integer(counter)), step, start);
        if (c.angle <= endpoint) {
          c.angle = endpoint;
          put(c.a + 0x34, 3);
          put(c.a + 0x48, uint32_t(-1));
        }
      } else if (c.rowState == 10) {
        put(c.a + 0x20, word(c.a + 0x20) | 1);
        put(c.a + 0x20, word(c.a + 0x20) | 2);
      }
      if (c.rowState == 11) {
        put(c.a + 0x20, word(c.a + 0x20) | 1);
        put(c.a + 0x20, word(c.a + 0x20) & ~2u);
      }
      c.phase = Phase::Center;
      break;
    }
    case Phase::Center: {
      if (!s.center || !s.offset)
        return false;
      const auto value = s.center(s.user);
      if (!value)
        return false;
      c.center = *value;
      c.phase = Phase::Offset;
      break;
    }
    // Center and offset are separately acknowledged. Preserve offset Y as
    // stack depth before branch-specific transforms; don't recapture center on
    // retry or fill optional slots for states that never wrote them.
    case Phase::Offset: {
      if (!s.offset)
        return false;
      const auto value = s.offset(s.user, c.angle);
      if (!value)
        return false;
      c.center[0] += (*value)[0];
      c.center[1] += (*value)[1];
      c.center[2] = (*value)[1];
      c.slots.back().position = c.center;
      c.phase = Phase::Branch;
      break;
    }
    case Phase::Branch:
      if (c.rowState == 1) {
        c.selected = c.i == word(c.c);
        const auto gp = word(0x80415fd0), counter = word(c.a + 0x48);
        const auto count = integer(word(gp + 4) * 10) / 60;
        float rotation =
            constant(-20484) +
            (constant(-20480) * float(integer(counter))) / float(count);
        if (rotation >= constant(-20476))
          rotation = c.zero;
        c.rotation = constant(-20472) * rotation;
        c.phase = Phase::Spin;
      } else if (c.rowState == 10) {
        if (!s.interpolate)
          return false;
        prepareInterpolation(4, integer(word(c.a + 0x48)), duration(12),
                             constant(-20464), c.center[0]);
        c.phase = Phase::InterpolateX;
      } else if (c.rowState == 11) {
        const auto n = word(c.a + 0x48);
        c.scale = constant(-20456) * float(integer(n));
        if (c.scale >= constant(-20452))
          put(c.a + 0x34, 3);
        if (s.scale && s.concat)
          c.scaleZ = constant(-20452);
        c.phase = Phase::Scale;
      } else {
        c.translation = c.center;
        c.selected = false;
        if (c.rowState == 3) {
          c.selected = c.i == word(c.c);
          if (c.selected)
            c.translation[0] = constant(-20468) + c.translation[0];
        }
        c.phase = Phase::Translate;
      }
      break;
    case Phase::Spin: {
      if (!s.rotation || !s.concat)
        return false;
      const auto value = s.rotation(s.user, c.rotation, 120);
      if (!value)
        return false;
      c.auxiliary = *value;
      c.translation = c.center;
      if (c.selected)
        c.translation[0] = constant(-20468) + c.translation[0];
      c.phase = Phase::Translate;
      break;
    }
    case Phase::Scale: {
      if (!s.scale || !s.concat)
        return false;
      const auto value = s.scale(s.user, c.scale, c.scale, c.scaleZ);
      if (!value)
        return false;
      c.auxiliary = *value;
      c.translation = c.center;
      c.phase = Phase::Translate;
      break;
    }
    case Phase::Translate: {
      if (!s.translation)
        return false;
      const auto value = s.translation(s.user, c.translation);
      if (!value)
        return false;
      c.matrix = *value;
      c.slots.back().matrix = *value;
      c.phase = c.rowState == 1 || c.rowState == 11 ? Phase::Concat
                                                    : Phase::PostTransform;
      break;
    }
    case Phase::Concat: {
      if (!s.concat)
        return false;
      const auto value = s.concat(s.user, c.matrix, c.auxiliary);
      if (!value)
        return false;
      c.slots.back().matrix = *value;
      c.phase = Phase::PostTransform;
      break;
    }
    case Phase::PostTransform:
      if (c.rowState == 1) {
        if (c.selected)
          put(c.a + 0x20, word(c.a + 0x20) | 2);
        c.slots.back().parameter = c.zero;
        if (c.angle >= c.endpoint) {
          const auto gp = word(0x80415fd0), now = word(c.a + 0x48);
          const auto n = integer(word(gp + 4) * 10) / 60;
          if (integer(now) >= n) {
            put(c.a + 0x34, 3);
            put(c.a + 0x48, uint32_t(-1));
          }
        }
        put(c.a + 0x48, word(c.a + 0x48) + 1);
        advanceRow();
      } else if (c.rowState == 3) {
        if (c.selected)
          put(c.a + 0x20, word(c.a + 0x20) | 2);
        c.slots.back().parameter = c.zero;
        advanceRow();
      } else if (c.rowState == 4 || c.rowState == 5) {
        c.slots.back().parameter = c.zero;
        put(c.a + 0x20, word(c.a + 0x20) & ~2u);
        put(c.a + 0x48, word(c.a + 0x48) + 1);
        advanceRow();
      } else if (c.rowState == 10) {
        prepareInterpolation(0, integer(word(c.a + 0x48)), duration(12),
                             constant(-20476), c.zero);
        c.phase = Phase::InterpolateParameter;
      } else {
        c.slots.back().parameter = c.zero;
        put(c.a + 0x48, word(c.a + 0x48) + 1);
        advanceRow();
      }
      break;
    // X/Y/parameter calls retain their exact operands independently. Fresh
    // rate/timer/constants for the NEXT call are captured only after the prior
    // provider succeeds, so a hold is not a whole-row snapshot/re-evaluation.
    case Phase::InterpolateX: {
      if (!s.interpolate)
        return false;
      const auto value = s.interpolate(s.user, c.interpolationMode, c.step,
                                       c.duration, c.start, c.end);
      if (!value)
        return false;
      c.center[0] = *value;
      prepareInterpolation(4, integer(word(c.a + 0x48)), duration(12),
                           constant(-20460), c.center[1]);
      c.phase = Phase::InterpolateY;
      break;
    }
    case Phase::InterpolateY: {
      if (!s.interpolate)
        return false;
      const auto value = s.interpolate(s.user, c.interpolationMode, c.step,
                                       c.duration, c.start, c.end);
      if (!value)
        return false;
      c.center[1] = *value;
      c.slots.back().position = c.center;
      c.translation = c.center;
      c.translation[0] = constant(-20468) + c.translation[0];
      c.phase = Phase::Translate;
      break;
    }
    case Phase::InterpolateParameter: {
      if (!s.interpolate)
        return false;
      const auto value = s.interpolate(s.user, c.interpolationMode, c.step,
                                       c.duration, c.start, c.end);
      if (!value)
        return false;
      c.slots.back().parameter = *value;
      c.phase = Phase::FinishParameter;
      break;
    }
    case Phase::FinishParameter:
      put(c.a + 0x48, word(c.a + 0x48) + 1);
      if (integer(word(c.a + 0x48)) > duration(12))
        put(c.a + 0x34, 3);
      advanceRow();
      break;
    // Capture row pointer only when actual icons provider exists. The slots
    // vector stays owned and stable through icons, party-button and pinch calls;
    // each child must retain its own unfinished GPU/service suffix.
    case Phase::CaptureIcons:
      if (!s.icons)
        return false;
      c.rows = word(c.w + 4);
      c.phase = Phase::Icons;
      break;
    case Phase::Icons:
      if (!s.icons || !s.icons(s.user, c.w, c.rows, c.slots))
        return false;
      c.phase = Phase::Party;
      break;
    case Phase::Party:
      if (!s.partyButton || !s.partyButton(s.user))
        return false;
      c.phase = Phase::Pinch;
      break;
    case Phase::Pinch:
      if (!s.pinchMark || !s.pinchMark(s.user))
        return false;
      cursor.reset();
      return true;
    }
  }
}
} // namespace ttyd
