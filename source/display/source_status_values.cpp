#include "source_status_values.hpp"
namespace ttyd {
namespace {
bool owner(const std::shared_ptr<const void> &a,
           const std::shared_ptr<const void> &b) {
  return a == b && !a.owner_before(b) && !b.owner_before(a);
}
bool same(const SourceStatusValueOutput &a, const SourceStatusValueOutput &b) {
  return a.user == b.user && owner(a.lifetime, b.lifetime) &&
         a.half == b.half && a.storeHalf == b.storeHalf;
}
bool same(const SourceStatusValueServices &a,
          const SourceStatusValueServices &b) {
  return a.user == b.user && owner(a.lifetime, b.lifetime) &&
         a.current == b.current && a.word == b.word && a.half == b.half &&
         a.byte == b.byte && a.call == b.call;
}
int32_t signedHalf(uint32_t v) {
  return v <= 0x7fffu ? int32_t(v) : int32_t(v) - 65536;
}
struct Running {
  bool &value;
  explicit Running(bool &v) : value(v) { value = true; }
  ~Running() { value = false; }
};
// The common four pouch children appear in both arms, in exactly this order.
constexpr uint32_t commonCalls[]{0x800d3c44, 0x800d3bb8, 0x800d3424,
                                 0x800d3e64};
constexpr uint32_t commonOffsets[]{0x12, 0x14, 0x0e, 0x10};
constexpr uint32_t fieldCalls[]{0x800d3d30, 0x800d3d24, 0x800d3c98, 0x800d3c8c};
} // namespace
struct SourceStatusValuesCursor::State {
  SourceStatusValueServices services;
  SourceStatusValueOutput output;
  uint32_t phase = 0, pending = 0, gp = 0, battle = 0, mario = 0, party = 0,
           pouch = 0, value = 0, index = 0;
  bool invalid = false, battleMode = false;
  SourceStatusValueCall request{};
  bool valid() {
    if (invalid)
      return false;
    if (!services.current || !services.current(services.user))
      invalid = true;
    return !invalid;
  }
  bool word(uint32_t pc, uint32_t a, uint32_t &v) {
    pending = pc;
    if (!services.word)
      return false;
    auto x = services.word(services.user, a);
    if (!valid() || !x)
      return false;
    v = *x;
    return true;
  }
  bool half(uint32_t pc, uint32_t a, uint32_t &v) {
    pending = pc;
    if (!services.half)
      return false;
    auto x = services.half(services.user, a);
    if (!valid() || !x)
      return false;
    v = *x;
    return true;
  }
  bool call(uint32_t pc, uint32_t function, uint32_t &v, bool argument = false,
            uint32_t a = 0) {
    pending = function;
    request = {function, pc, {a}, uint8_t(argument ? 1 : 0)};
    if (!services.call)
      return false;
    auto x = services.call(services.user, request);
    if (!valid() || !x)
      return false;
    v = *x;
    return true;
  }
  bool store(uint32_t pc, uint32_t offset) {
    pending = pc;
    if (!output.storeHalf)
      return false;
    const bool done = output.storeHalf(output.user, offset, uint16_t(value));
    return valid() && done;
  }
  bool outputHalf(uint32_t pc) {
    pending = pc;
    if (!output.half)
      return false;
    auto x = output.half(output.user, 8);
    if (!valid() || !x)
      return false;
    index = *x;
    return true;
  }
};
SourceStatusValuesCursor::SourceStatusValuesCursor() = default;
SourceStatusValuesCursor::~SourceStatusValuesCursor() = default;
bool SourceStatusValuesCursor::active() const { return bool(state_); }
bool SourceStatusValuesCursor::stale() const {
  return state_ && state_->invalid;
}
uint32_t SourceStatusValuesCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool SourceStatusValuesCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumeSourceStatusValues(const SourceStatusValueOutput &o,
                              const SourceStatusValueServices &s,
                              SourceStatusValuesCursor &cursor) {
  if (cursor.invoking_)
    return false;
  Running running(cursor.invoking_);
  if (!cursor.state_) {
    if (!s.lifetime || !o.lifetime || !s.current || !s.current(s.user))
      return false;
    cursor.state_ = std::make_unique<SourceStatusValuesCursor::State>();
    cursor.state_->services = s;
    cursor.state_->output = o;
  }
  auto &c = *cursor.state_;
  if (!same(c.services, s) || !same(c.output, o) || !c.valid())
    return false;
  for (;;) {
    switch (c.phase) {
    case 0:
      if (!c.word(0x8013c9c8, 0x80415fd0, c.gp))
        return false;
      c.phase = 1;
      break;
    case 1:
      if (!c.word(0x8013c9cc, c.gp + 0x14, c.value))
        return false;
      c.battleMode = c.value != 0;
      c.phase = c.battleMode ? 20 : 2;
      break;
    // Field HP/maxHP/FP/maxFP are genuine separate source children and stores.
    case 2:
    case 4:
    case 6:
    case 8: {
      const auto i = (c.phase - 2) / 2;
      if (!c.call(0x8013c9d8 + 8 * i, fieldCalls[i], c.value))
        return false;
      ++c.phase;
      break;
    }
    case 3:
    case 5:
    case 7:
    case 9: {
      const auto i = (c.phase - 3) / 2;
      if (!c.store(0x8013c9dc + 8 * i, 2 * i))
        return false;
      c.phase = c.phase == 9 ? 40 : c.phase + 1;
      break;
    }
    // Battle pointer and both unit results are captured before the first LHA.
    case 20:
      if (!c.word(0x8013ca74, 0x8041eb90, c.battle))
        return false;
      c.phase = 21;
      break;
    case 21:
      if (!c.call(0x8013ca7c, 0x800f87a8, c.mario, true, c.battle))
        return false;
      c.phase = 22;
      break;
    case 22:
      if (!c.call(0x8013ca88, 0x800f8728, c.party, true, c.battle))
        return false;
      c.phase = 23;
      break;
    case 23:
    case 25:
    case 27:
    case 29: {
      const auto i = (c.phase - 23) / 2;
      constexpr uint32_t fields[]{0x10c, 0x108, 0x112, 0x10e};
      constexpr uint32_t pcs[]{0x8013ca8c, 0x8013ca98, 0x8013caa0, 0x8013caa8};
      if (!c.half(pcs[i], c.mario + fields[i], c.value))
        return false;
      ++c.phase;
      break;
    }
    case 24:
    case 26:
    case 28:
    case 30: {
      const auto i = (c.phase - 24) / 2;
      constexpr uint32_t pcs[]{0x8013ca94, 0x8013ca9c, 0x8013caa4, 0x8013caac};
      if (!c.store(pcs[i], 2 * i))
        return false;
      c.phase = c.phase == 30 ? 40 : c.phase + 1;
      break;
    }
    case 40:
    case 42:
    case 44:
    case 46: {
      const auto i = (c.phase - 40) / 2;
      const auto pc = (c.battleMode ? 0x8013cab0u : 0x8013c9f8u) + 8 * i;
      if (!c.call(pc, commonCalls[i], c.value))
        return false;
      ++c.phase;
      break;
    }
    case 41:
    case 43:
    case 45:
    case 47: {
      const auto i = (c.phase - 41) / 2;
      const auto pc = c.battleMode
                          ? (i == 3 ? 0x8013cad0u : 0x8013cab4u + 8 * i)
                          : 0x8013c9fcu + 8 * i;
      if (!c.store(pc, commonOffsets[i]))
        return false;
      c.phase = c.phase == 47 ? (c.battleMode ? (c.party ? 70 : 80) : 50)
                              : c.phase + 1;
      break;
    }
    case 50:
      if (!c.call(0x8013ca18, 0x8005c164, c.value))
        return false;
      c.phase = c.value ? 53 : 51;
      break;
    case 51:
      if (!c.call(0x8013ca24, 0x801410e8, c.value))
        return false;
      c.phase = 55;
      break;
    case 53:
      if (!c.call(0x8013ca30, 0x8005e0c0, c.mario))
        return false;
      c.phase = 54;
      break;
    case 54: {
      c.pending = 0x8013ca34;
      if (!s.byte)
        return false;
      auto v = s.byte(s.user, c.mario + 0x247);
      if (!c.valid() || !v)
        return false;
      c.value = *v < 128 ? *v : uint32_t(int32_t(*v) - 256);
      c.phase = 55;
      break;
    }
    case 55:
      if (!c.store(0x8013ca3c, 8))
        return false;
      c.phase = 56;
      break;
    case 56:
      if (!c.call(0x8013ca40, 0x800d5eb0, c.pouch))
        return false;
      c.phase = 57;
      break;
    case 57:
      if (!c.outputHalf(0x8013ca44))
        return false;
      c.phase = 58;
      break;
    case 58:
      if (!c.half(0x8013ca50, c.pouch + uint32_t(signedHalf(c.index)) * 14 + 6,
                  c.value))
        return false;
      c.phase = 59;
      break;
    case 59:
      if (!c.store(0x8013ca54, 0x0a))
        return false;
      c.phase = 60;
      break;
    case 60:
      if (!c.call(0x8013ca58, 0x800d5eb0, c.pouch))
        return false;
      c.phase = 61;
      break;
    case 61:
      if (!c.outputHalf(0x8013ca5c))
        return false;
      c.phase = 62;
      break;
    case 62:
      if (!c.half(0x8013ca68, c.pouch + uint32_t(signedHalf(c.index)) * 14 + 2,
                  c.value))
        return false;
      c.phase = 63;
      break;
    case 63:
      if (!c.store(0x8013ca6c, 0x0c))
        return false;
      c.phase = 90;
      break;
    case 70:
      if (!c.word(0x8013cad8, c.party + 8, c.value))
        return false;
      c.phase = 71;
      break;
    case 71:
      if (!c.call(0x8013cadc, 0x800f8444, c.value, true, c.value))
        return false;
      c.phase = 72;
      break;
    case 72:
      if (!c.store(0x8013cae0, 8))
        return false;
      c.phase = 73;
      break;
    case 73:
      if (!c.half(0x8013cae4, c.party + 0x10c, c.value))
        return false;
      c.phase = 74;
      break;
    case 74:
      if (!c.store(0x8013cae8, 0x0a))
        return false;
      c.phase = 75;
      break;
    case 75:
      if (!c.half(0x8013caec, c.party + 0x108, c.value))
        return false;
      c.phase = 76;
      break;
    case 76:
      if (!c.store(0x8013caf0, 0x0c))
        return false;
      c.phase = 90;
      break;
    case 80:
      c.value = 0;
      c.phase = 81;
      break;
    case 81:
      if (!c.store(0x8013cafc, 8))
        return false;
      c.phase = 82;
      break;
    case 82:
      if (!c.store(0x8013cb00, 0x0a))
        return false;
      c.phase = 83;
      break;
    case 83:
      if (!c.store(0x8013cb04, 0x0c))
        return false;
      c.phase = 90;
      break;
    case 90:
      cursor.state_.reset();
      return true;
    default:
      return false;
    }
  }
}
} // namespace ttyd
