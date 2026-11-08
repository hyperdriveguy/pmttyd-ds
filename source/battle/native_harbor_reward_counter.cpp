#include "native_harbor_reward_counter.hpp"
#include <algorithm>
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t StockExp = 0xf00, FortuneMode = 0x18ff9, PouchLevel = 0x8a,
                   PouchExp = 0x96;
constexpr int32_t MaxReward = 100, MaxLevel = 99, FortuneDouble = 3;
constexpr int32_t CountStride = 4, SoundStride = 8, Accelerated = 1000;
constexpr int32_t FastCooldown = 10, SlowThreshold = 120, SlowEnd = 140,
                  NormalCooldown = 20;
constexpr uint32_t PhysicalConfirmMask = 0x300;
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
NativeHarborRewardCounter::NativeHarborRewardCounter(
    uint32_t b, uint32_t p, NativeHarborRewardCounterServices s)
    : battle_(b), pouch_(p), services_(std::move(s)) {}
bool NativeHarborRewardCounter::current() {
  if (checking_)
    return false;
  Guard guard(checking_);
  return battle_ && pouch_ && services_.lifetime && services_.current &&
         services_.current(services_.user);
}
bool NativeHarborRewardCounter::admit() {
  if (invoking_)
    return false;
  Guard guard(invoking_);
  if (!current() || !services_.word || !services_.half || !services_.byte ||
      !services_.writeHalf || !services_.sound)
    return false;
  if (phase_ != Phase::Admission)
    return true;
  // Original GetStockExp floor/cap, fortune doubling, level gate, final cap.
  if (admission_ == 0) {
    auto n = services_.word(services_.user, battle_ + StockExp);
    if (!n)
      return false;
    total_ = std::clamp<int32_t>(int32_t(*n), 1, MaxReward);
    ++admission_;
  }
  if (admission_ == 1) {
    if (!current())
      return false;
    auto n = services_.byte(services_.user, battle_ + FortuneMode);
    if (!n)
      return false;
    if (*n == FortuneDouble)
      total_ *= 2;
    ++admission_;
  }
  if (admission_ == 2) {
    if (!current())
      return false;
    auto n = services_.half(services_.user, pouch_ + PouchLevel);
    if (!n)
      return false;
    level_ = int16_t(*n);
    if (level_ >= MaxLevel)
      total_ = 0;
    total_ = std::min(total_, MaxReward);
    remaining_ = total_;
    ++admission_;
  }
  phase_ = total_ ? Phase::Ready : Phase::ZeroReward;
  return true;
}
bool NativeHarborRewardCounter::start() {
  if (invoking_)
    return false;
  Guard guard(invoking_);
  if (!current())
    return false;
  if (phase_ == Phase::Ready) {
    phase_ = Phase::Counting;
    counter_ = 0;
    return true;
  }
  return phase_ == Phase::Counting;
}
bool NativeHarborRewardCounter::advance(uint64_t tick, uint32_t held) {
  if (invoking_)
    return false;
  Guard guard(invoking_);
  if (!current())
    return false;
  if (phase_ == Phase::Done || phase_ == Phase::LevelUp)
    return true;
  if (phase_ != Phase::Counting && phase_ != Phase::Cooldown)
    return false;
  if (hasTick_) {
    if (tick < tick_ || (!tickDone_ && (tick != tick_ || held != held_)))
      return false;
    if (tick == tick_ && tickDone_)
      return true;
  }
  // A held tick must resume its retained prefix, including completed stores.
  if (!hasTick_ || tick > tick_) {
    tick_ = tick;
    held_ = held;
    hasTick_ = true;
    tickDone_ = false;
    step_ = 0;
  }
  if (phase_ == Phase::Cooldown) {
    if (--counter_ <= 0)
      phase_ = levelUp_ ? Phase::LevelUp : Phase::Done;
    tickDone_ = true;
    return true;
  }
  if (step_ == 0) {
    if (remaining_ <= 0) {
      counter_ = counter_ >= Accelerated    ? FastCooldown
                 : counter_ < SlowThreshold ? SlowEnd - counter_
                                            : NormalCooldown;
      phase_ = Phase::Cooldown;
      tickDone_ = true;
      return true;
    }
    ++counter_;
    if ((held_ & PhysicalConfirmMask) && counter_ < Accelerated)
      counter_ += Accelerated;
    step_ = 1;
  }
  if (step_ == 1) {
    if (!(counter_ % CountStride) || counter_ >= Accelerated) {
      if (!current())
        return false;
      auto n = services_.half(services_.user, pouch_ + PouchExp);
      if (!n)
        return false;
      exp_ = int16_t(*n) + 1;
      step_ = 2;
    } else
      step_ = 4;
  }
  if (step_ == 2) {
    if (!current() ||
        !services_.writeHalf(services_.user, pouch_ + PouchExp, uint16_t(exp_)))
      return false;
    --remaining_;
    step_ = 3;
  }
  if (step_ == 3) {
    if (!(counter_ % SoundStride) &&
        (!current() || !services_.sound(services_.user)))
      return false;
    step_ = 4;
  }
  // Source reads the fresh pouch even on visits which do not transfer a point.
  if (step_ == 4) {
    if (!current())
      return false;
    auto n = services_.half(services_.user, pouch_ + PouchExp);
    if (!n)
      return false;
    exp_ = int16_t(*n);
    step_ = 5;
  }
  if (step_ == 5) {
    if (exp_ >= MaxReward) {
      if (!current() ||
          !services_.writeHalf(services_.user, pouch_ + PouchExp, 0))
        return false;
      levelUp_ = true;
      step_ = 6;
    } else
      step_ = 7;
  }
  if (step_ == 6) {
    if (!current())
      return false;
    auto n = services_.half(services_.user, pouch_ + PouchLevel);
    if (!n)
      return false;
    if (int16_t(*n) + 1 >= MaxLevel)
      remaining_ = 0;
    step_ = 7;
  }
  tickDone_ = true;
  return true;
}
} // namespace ttyd
