#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
// Native counterpart of the reward count only. It owns no guest End work and
// does not certify btlseqEnd, victory events, icon drawing or field readiness.
struct NativeHarborRewardCounterServices {
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*writeHalf)(void *, uint32_t, uint16_t) = nullptr;
  // Must retain successful prefixes internally when returning false.
  bool (*sound)(void *) = nullptr;
};
class NativeHarborRewardCounter {
public:
  enum class Phase {
    Admission,
    Ready,
    Counting,
    Cooldown,
    Done,
    ZeroReward,
    LevelUp
  };
  NativeHarborRewardCounter(uint32_t battle, uint32_t pouch,
                            NativeHarborRewardCounterServices);
  // Admit only after the actual native ending owner reaches its reward
  // boundary.
  bool admit();
  // Caller supplies actual completed victory/PointGet readiness before
  // starting.
  bool start();
  // Simulation ticks, with source held-A/B mask0x300. A held tick resumes with
  // exactly its original input. Repeated completed ticks have no side effects.
  bool advance(uint64_t tick, uint32_t held);
  Phase phase() const { return phase_; }
  int32_t total() const { return total_; }
  int32_t remaining() const { return remaining_; }
  int32_t counter() const { return counter_; }

private:
  bool current();
  uint32_t battle_, pouch_;
  NativeHarborRewardCounterServices services_;
  Phase phase_ = Phase::Admission;
  unsigned admission_ = 0, step_ = 0;
  int32_t total_ = 0, remaining_ = 0, counter_ = 0, level_ = 0, exp_ = 0;
  bool invoking_ = false, checking_ = false, levelUp_ = false, hasTick_ = false,
       tickDone_ = false;
  uint64_t tick_ = 0;
  uint32_t held_ = 0;
};
} // namespace ttyd
