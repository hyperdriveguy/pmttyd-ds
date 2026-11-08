#include "harbor_source_audience_noise.hpp"
#include "battle_event_callbacks.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, AudienceOffset = 0x2858,
                   ActorOffset = 0x1bc, ActorStride = 0x134,
                   AudienceSlots = 200;
constexpr uint32_t EmptyOriginalName = 0x804218c8;
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
float single(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
double wide(uint64_t v) {
  double f;
  std::memcpy(&f, &v, 8);
  return f;
}
bool same(const EventNativePlatform &a, const EventNativePlatform &b) {
  return a.user == b.user && a.truncate == b.truncate &&
         a.multiplyFloat == b.multiplyFloat && a.divideFloat == b.divideFloat &&
         a.addFloat == b.addFloat && a.subtractDouble == b.subtractDouble &&
         a.roundDoubleFloat == b.roundDoubleFloat;
}
bool bound(const EventNativePlatform &p) {
  return p.truncate && p.multiplyFloat && p.divideFloat && p.addFloat &&
         p.subtractDouble && p.roundDoubleFloat;
}
} // namespace
struct HarborSourceAudienceNoise::State {
  HarborSourceAudienceNoiseBinding binding;
  EventNativePlatform platform;
  std::shared_ptr<const uint8_t> allocation;
  BattleAudienceNoiseServices services;
  BattleAudienceNoiseCursor cursor;
  BattleAudienceNoisePrivate incoming;
  std::optional<BattleAudienceSetupRequest> invocation;
  bool invalid = false, done = false;
  mutable bool checking = false;
  uint32_t pending = 0;
  const BattleAudienceNoiseCall *held = nullptr;
  size_t copied = 0;
  unsigned soundPhase = 0;
  uint32_t key = 0;
  std::array<float, 3> position{};
  uint8_t volume = 0;
  std::optional<int32_t> handle;
  bool current() {
    if (invalid || checking)
      return false;
    Guard guard(checking);
    uint32_t b = 0;
    if (!binding.current() || !binding.pool->current() ||
        binding.pool->allocationLifetime(binding.battle) != allocation ||
        !binding.vm->memory.readWord(BattleGlobal, b) || b != binding.battle ||
        !same(platform, binding.vm->platform) ||
        (!binding.sound->usesRegistry(binding.registry.get()) ||
         !binding.sound->usesRuntime(binding.vm))) {
      invalid = true;
      return false;
    }
    return true;
  }
  std::optional<uint8_t> byte(uint32_t a) {
    uint8_t original = 0, actual = 0;
    if (!current())
      return {};
    pending = a;
    if (!binding.sourceData->readByte(a, original) ||
        !binding.vm->memory.readByte(a, actual))
      return {};
    if (original != actual) {
      invalid = true;
      return {};
    }
    return actual;
  }
  std::optional<uint32_t> word(uint32_t a) {
    uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i) {
      const auto b = byte(a + i);
      if (!b)
        return {};
      v = v << 8 | *b;
    }
    return v;
  }
  std::optional<double> realDouble(uint32_t a) {
    const auto hi = word(a), lo = word(a + 4);
    if (!hi || !lo)
      return {};
    return wide(uint64_t(*hi) << 32 | *lo);
  }
  bool preflight() {
    const uint32_t actorBytes = ActorOffset + ActorStride * AudienceSlots;
    return current() &&
           binding.vm->memory.contiguousRam(binding.battle + AudienceOffset,
                                            actorBytes) &&
           binding.vm->memory.contiguousRam(
               binding.battle + AudienceOffset + 0x133fc + 2 * 0x24, 0x24);
  }
  bool textMatches(const BattleAudienceNoisePrivate &text, uint32_t original) {
    for (size_t i = 0; i < text.bytes.size(); ++i) {
      if (!text.bytes[i]) {
        pending = 0x8019fcec;
        return false;
      }
      const auto actual = byte(original + uint32_t(i));
      if (!actual)
        return false;
      if (*actual != *text.bytes[i]) {
        invalid = true;
        return false;
      }
      if (!*actual)
        return true;
    }
    pending = 0x8019fcec;
    return false;
  }
  bool info(int32_t right, int32_t left) {
    // Preflight every reached immutable scalar before the first numeric effect.
    // The arithmetic callbacks are total in this finite domain; a violated
    // numeric/lifetime admission poisons rather than replaying any prefix.
    const auto z = word(0x80424988), x = word(0x804249c4);
    if (!z || !x)
      return false;
    const auto total = right + left;
    std::optional<uint32_t> scale, divisor, base;
    std::optional<double> bias, unsignedBias;
    if (total) {
      bias = realDouble(0x802fa328);
      scale = word(0x804249c0);
      divisor = word(0x804249b8);
      base = word(0x804249bc);
      if (!bias || !scale || !divisor || !base)
        return false;
    }
    if (left && total) {
      unsignedBias = realDouble(0x802fa330);
      if (!unsignedBias)
        return false;
    }
    auto convert = [&](float f) -> std::optional<int32_t> {
      if (!std::isfinite(f) || f < -2147483648.f || f >= 2147483648.f) {
        invalid = true;
        return {};
      }
      const auto i = platform.truncate(platform.user, f);
      if (!current())
        return {};
      return i;
    };
    auto signedSingle = [&](int32_t i) -> std::optional<float> {
      const auto d = platform.subtractDouble(
          platform.user,
          wide((uint64_t(0x43300000) << 32) | (uint32_t(i) ^ 0x80000000u)),
          *bias);
      if (!current())
        return {};
      const auto f = platform.roundDoubleFloat(platform.user, d);
      if (!current())
        return {};
      return f;
    };
    if (!total)
      volume = 0;
    else {
      const auto widened = signedSingle(total);
      if (!widened)
        return false;
      const auto product =
          platform.multiplyFloat(platform.user, single(*scale), *widened);
      if (!current())
        return false;
      const auto quotient =
          platform.divideFloat(platform.user, product, single(*divisor));
      if (!current())
        return false;
      const auto v = platform.addFloat(platform.user, single(*base), quotient);
      if (!current())
        return false;
      const auto n = convert(v);
      if (!n)
        return false;
      volume = uint8_t(*n);
    }
    if (!left || !total)
      position[0] = single(*x);
    else {
      const auto difference = signedSingle(right - left);
      if (!difference)
        return false;
      const auto product =
          platform.multiplyFloat(platform.user, single(*x), *difference);
      if (!current())
        return false;
      const auto denominator = signedSingle(total);
      if (!denominator)
        return false;
      const auto quotient =
          platform.divideFloat(platform.user, product, *denominator);
      if (!current())
        return false;
      const auto n = convert(quotient);
      if (!n)
        return false;
      const auto d = platform.subtractDouble(
          platform.user, wide((uint64_t(0x43300000) << 32) | uint8_t(*n)),
          *unsignedBias);
      if (!current())
        return false;
      position[0] = platform.roundDoubleFloat(platform.user, d);
      if (!current())
        return false;
    }
    position[1] = single(*z);
    position[2] = single(*z);
    if (!std::isfinite(position[0]) || !std::isfinite(position[1]) ||
        !std::isfinite(position[2])) {
      invalid = true;
      return false;
    }
    return current();
  }
  std::optional<uint32_t> call(const BattleAudienceNoiseCall &q) {
    if (!current())
      return {};
    if (held && held != &q) {
      invalid = true;
      return {};
    }
    held = &q;
    pending = q.function;
    if (q.function == 0x8026d5fc) {
      if (!q.copyOutput || !q.originalName || q.arguments[0] != q.originalName)
        return {};
      for (; copied < q.copyOutput->bytes.size(); ++copied) {
        const auto b = byte(q.originalName + uint32_t(copied));
        if (!b)
          return {};
        q.copyOutput->bytes[copied] = *b;
        if (!*b) {
          copied = 0;
          held = nullptr;
          pending = 0;
          return 0;
        }
      }
      return {};
    }
    if (q.function == 0x8019fcec) {
      if (!q.name)
        return {};
      if (soundPhase == 0) {
        key = q.originalName ? q.originalName : EmptyOriginalName;
        // An unknown private key is refused before numeric/backend effects.
        // Known bytes map to an actually equal original string, never a guest
        // SP token.
        if (!textMatches(*q.name, key))
          return {};
        if (q.arguments[0] > 100 || q.arguments[1] > 100)
          return {};
        if (!info(int32_t(q.arguments[0]), int32_t(q.arguments[1])))
          return {};
        soundPhase = 1;
      }
      if (soundPhase == 1) {
        if (!textMatches(*q.name, key) || !current())
          return {};
        handle = binding.sound->position(key, position);
        if (!current())
          return {};
        if (!handle) {
          pending = binding.sound->pending();
          return {};
        }
        soundPhase = 2;
      }
      pending = 0x800d97d0;
      if (!binding.registry->setVolume(*handle, volume) || !current())
        return {};
      const auto result = uint32_t(*handle);
      soundPhase = 0;
      held = nullptr;
      handle.reset();
      pending = 0;
      return result;
    }
    if (q.function == 0x800d9558) {
      const auto v = binding.registry->getVolume(int32_t(q.arguments[0]));
      if (!current() || !v)
        return {};
      held = nullptr;
      pending = 0;
      return *v;
    }
    return {};
  }
};
HarborSourceAudienceNoise::HarborSourceAudienceNoise() = default;
HarborSourceAudienceNoise::~HarborSourceAudienceNoise() = default;
std::unique_ptr<HarborSourceAudienceNoise>
HarborSourceAudienceNoise::create(HarborSourceAudienceNoiseBinding b) {
  if (!b.vm || !b.pool || !b.battle || !b.lifetime || !b.current ||
      !b.sourceData || !b.sound || !b.registry || !bound(b.vm->platform))
    return {};
  auto owner =
      std::unique_ptr<HarborSourceAudienceNoise>(new HarborSourceAudienceNoise);
  owner->state_ = std::make_unique<State>();
  auto &x = *owner->state_;
  x.binding = std::move(b);
  x.platform = x.binding.vm->platform;
  x.allocation = x.binding.pool->allocationLifetime(x.binding.battle);
  if (!x.allocation || !x.current())
    return {};
  if (x.binding.policy ==
      HarborAudienceNoisePrivatePolicy::EmptyColdPrivateName)
    x.incoming.bytes[0] = 0;
  x.services.actors = nativeBattleActorServices(*x.binding.vm);
  x.services.lifetime = x.binding.lifetime;
  x.services.user = &x;
  x.services.current = [](void *p) {
    return static_cast<State *>(p)->current();
  };
  x.services.call = [](void *p, const BattleAudienceNoiseCall &q) {
    return static_cast<State *>(p)->call(q);
  };
  return owner;
}
bool HarborSourceAudienceNoise::resume(const BattleAudienceSetupRequest &q) {
  if (invoking_ || !state_)
    return false;
  Guard guard(invoking_);
  auto &x = *state_;
  if (!x.current() || q.function != 0x801a0b88 || q.callsite != 0x801a8584 ||
      q.privateCopy)
    return false;
  if (!x.invocation)
    x.invocation = q;
  if (x.invocation->arguments != q.arguments || !x.preflight())
    return false;
  if (x.done)
    return true;
  const auto *initial =
      x.binding.policy == HarborAudienceNoisePrivatePolicy::EmptyColdPrivateName
          ? &x.incoming
          : nullptr;
  if (!resumePackedBattleAudienceNoise(x.services, x.cursor, initial))
    return false;
  x.done = true;
  x.pending = 0;
  return x.current();
}
bool HarborSourceAudienceNoise::complete() const {
  return state_ && state_->current() && state_->done;
}
uint32_t HarborSourceAudienceNoise::pending() const {
  return state_ ? (state_->pending ? state_->pending : state_->cursor.pending())
                : 0;
}
bool HarborSourceAudienceNoise::retire() {
  if (invoking_ || !state_ || state_->checking)
    return false;
  state_->invalid = true;
  return true;
}
} // namespace ttyd
