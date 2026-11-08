#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct HarborBattleMusicBinding {
  std::shared_ptr<const void> runtime, proof;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
};
struct HarborBattleMusicLease {
  unsigned slot = 0;
  uint64_t generation = 0;
  int32_t track = -1;
  bool operator==(const HarborBattleMusicLease &o) const {
    return slot == o.slot && generation == o.generation && track == o.track;
  }
};
struct HarborBattleMusicServices {
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  // Atomic native admission: empty means no new stream was queued.
  std::optional<HarborBattleMusicLease> (*start)(void *, int32_t track,
                                                 bool enabled) = nullptr;
  bool (*valid)(void *, const HarborBattleMusicLease &) = nullptr;
  bool (*mix)(void *, const HarborBattleMusicLease &, bool enabled) = nullptr;
  bool (*stop)(void *, const HarborBattleMusicLease &) = nullptr;
};
// Native stream-ownership policy, not source BGMMain/envelope completion.
// Explicit begin mints an actual stream lease; matching track numbers cannot
// adopt an old/foreign stream. Muting retains real PCM progression at zero mix.
class HarborBattleMusicOwner {
public:
  static constexpr int32_t BattleTrack = 12, VictoryTrack = 2;
  bool begin(const HarborBattleMusicBinding &,
             const HarborBattleMusicServices &, bool victory, bool enabled);
  bool update(const std::shared_ptr<const void> &actualRuntime, bool enabled);
  bool retire(const std::shared_ptr<const void> &actualRuntime);
  bool busy() const { return invoking_; }
  bool active() const { return bool(binding_.runtime) && !retired_; }
  bool victory() const { return active() && desired_ == VictoryTrack; }
  bool owns(const std::shared_ptr<const void> &runtime) const;
  bool retired(const std::shared_ptr<const void> &runtime) const;
  bool protects(const HarborBattleMusicLease &lease) const {
    return lease_ && *lease_ == lease;
  }

private:
  HarborBattleMusicBinding binding_;
  std::weak_ptr<const void> retiredRuntime_;
  HarborBattleMusicServices services_;
  std::optional<HarborBattleMusicLease> lease_;
  int32_t desired_ = BattleTrack;
  bool invoking_ = false, retired_ = false;
  bool sourceCurrent() const;
  bool drive(bool enabled);
};
} // namespace ttyd
