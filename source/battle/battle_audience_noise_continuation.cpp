#include "battle_audience_noise_continuation.hpp"
namespace ttyd {
namespace {
constexpr uint32_t AudienceOffset = 0x2858, SoundOffset = 0x133fc,
                   SoundStride = 0x24, NoiseSlot = 2;
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
bool same(const BattleAudienceNoiseServices &a,
          const BattleAudienceNoiseServices &b) {
  const auto &x = a.actors.memory, &y = b.actors.memory;
  return a.user == b.user && a.current == b.current && a.call == b.call &&
         a.lifetime == b.lifetime && !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event;
}
bool admitted(const BattleAudienceNoiseServices &s) {
  return s.lifetime && s.current && s.actors.byte && s.actors.memory.word &&
         s.actors.memory.storeWord && s.actors.memory.storeByte;
}
} // namespace
struct BattleAudienceNoiseCursor::State {
  BattleAudienceNoiseServices services;
  const BattleAudienceNoisePrivate *incoming = nullptr;
  BattleAudienceNoisePrivate name;
  unsigned phase = 0;
  uint32_t audience = 0, record = 0, right = 0, left = 0, category = 0,
           originalName = 0, pending = 0;
  bool stale = false, done = false;
  std::optional<BattleAudienceNoiseCall> held;
  bool current() {
    if (stale || !services.current(services.user)) {
      stale = true;
      return false;
    }
    return true;
  }
  uint32_t word(uint32_t at) {
    const auto &m = services.actors.memory;
    return m.word(m.user, at);
  }
  void word(uint32_t at, uint32_t v) {
    const auto &m = services.actors.memory;
    m.storeWord(m.user, at, v);
  }
  void byte(uint32_t at, uint8_t v) {
    const auto &m = services.actors.memory;
    m.storeByte(m.user, at, v);
  }
  std::optional<uint32_t> child(uint32_t fn, uint32_t site,
                                std::array<uint32_t, 2> args = {}) {
    if (!held) {
      BattleAudienceNoiseCall q;
      q.function = fn;
      q.callsite = site;
      q.arguments = args;
      q.originalName = originalName;
      if (fn == 0x8026d5fc)
        q.copyOutput = &name;
      if (fn == 0x8019fcec)
        q.name = &name;
      held = q;
    }
    pending = fn;
    if (!services.call)
      return {};
    const auto out = services.call(services.user, *held);
    if (!current() || !out)
      return {};
    held.reset();
    pending = 0;
    return out;
  }
  uint32_t halfCount(bool rightHalf) {
    // Whole RL_Sub201c + PPAudience1fa0 + Exist29c0. In particular, Exist's
    // discarded Base call is separate from its fresh ActorPtr global capture.
    const auto base = [&] {
      return currentBattleAddress(services.actors) + AudienceOffset;
    };
    (void)base();
    uint32_t count = 0;
    for (uint32_t row = 0; row < 10; ++row)
      for (uint32_t column = 0; column < 10; ++column) {
        const auto index = row * 20 + (rightHalf ? 10u : 0u) + column;
        const auto captured = base() + 0x1bc + 0x134 * index;
        (void)base();
        const auto flags = word(base() + 0x1bc + 0x134 * index);
        if (!(flags & 1) || (flags & 0x102))
          continue;
        const auto state =
            services.actors.byte(services.actors.user, captured + 0x19);
        if (state != 12 && state != 13 && state != 17)
          ++count;
      }
    return count;
  }
  bool run() {
    while (!done) {
      if (!current())
        return false;
      switch (phase) {
      case 0:
        audience = currentBattleAddress(services.actors) + AudienceOffset;
        ++phase;
        break;
      case 1:
        record = currentBattleAddress(services.actors) + AudienceOffset +
                 SoundOffset + NoiseSlot * SoundStride;
        ++phase;
        break;
      case 2:
        right = halfCount(true);
        ++phase;
        break;
      case 3:
        left = halfCount(false);
        ++phase;
        break;
      case 4: {
        if (word(record) & 1) {
          done = true;
          break;
        }
        const auto total = right + left;
        if (total == 0)
          byte(record + 8, 0);
        // Half scans can produce only0..200. Four independent source range
        // tests select exactly one name; zero performs no private-name write.
        if (total >= 1 && total <= 49) {
          category = 1;
          originalName = 0x802fa574;
        }
        if (total >= 50 && total <= 99) {
          category = 2;
          originalName = 0x802fa588;
        }
        if (total >= 100 && total <= 149) {
          category = 3;
          originalName = 0x802fa59c;
        }
        if (total >= 150) {
          category = 4;
          originalName = 0x802fa5b0;
        }
        ++phase;
        break;
      }
      case 5:
        if (originalName) {
          constexpr uint32_t CopySites[]{0x801a0bfc, 0x801a0c20, 0x801a0c44,
                                         0x801a0c60};
          if (!child(0x8026d5fc, CopySites[category - 1], {originalName, 0}))
            return false;
          byte(record + 8, uint8_t(category));
        }
        ++phase;
        break;
      case 6: {
        const auto out = child(0x8019fcec, 0x801a0c78, {right, left});
        if (!out)
          return false;
        word(record + 4, *out);
        ++phase;
        break;
      }
      case 7:
        if (word(record + 4) == 0xffffffffu) {
          done = true;
          break;
        }
        word(record, word(record) | 1);
        word(record + 0xc, 0xffffffffu);
        word(record + 0x10, 0);
        // Actual source reads the stored handle again before volume child
        // entry.
        held = BattleAudienceNoiseCall{
            0x800d9558, 0x801a0cac, {word(record + 4), 0},
            nullptr,    nullptr,    originalName};
        ++phase;
        break;
      case 8: {
        const auto out = child(0x800d9558, 0x801a0cac);
        if (!out)
          return false;
        byte(record + 0x1d, uint8_t(*out));
        byte(record + 0x1c, uint8_t(*out));
        word(record + 0x18, 0);
        word(record + 0x14, 0);
        byte(record + 0x1e, 100);
        word(audience, word(audience) | 0x2000);
        done = true;
        break;
      }
      default:
        return false;
      }
    }
    return current();
  }
};
BattleAudienceNoiseCursor::BattleAudienceNoiseCursor() = default;
BattleAudienceNoiseCursor::~BattleAudienceNoiseCursor() = default;
bool BattleAudienceNoiseCursor::active() const {
  return state_ && !state_->done;
}
uint32_t BattleAudienceNoiseCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleAudienceNoiseCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleAudienceNoise(
    const BattleAudienceNoiseServices &s, BattleAudienceNoiseCursor &c,
    const BattleAudienceNoisePrivate *incoming) {
  if (c.invoking_ || !admitted(s))
    return false;
  Guard guard(c.invoking_);
  if (!c.state_) {
    c.state_ = std::make_unique<BattleAudienceNoiseCursor::State>();
    c.state_->services = s;
    c.state_->incoming = incoming;
    if (incoming)
      c.state_->name = *incoming;
  }
  auto &x = *c.state_;
  if (!same(x.services, s) || x.incoming != incoming) {
    x.stale = true;
    return false;
  }
  return x.run();
}
} // namespace ttyd
