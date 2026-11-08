#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
struct EventEntry;
// Explicit native instance, never an original guest EffectEntry address.
struct NativeHarborSmokeRequest {
  std::array<float, 3> position{};
  float scale = 0;
};
struct NativeHarborSmokeServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*value)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<float> (*floating)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint64_t> (*spawn)(void *,
                                   const NativeHarborSmokeRequest &) = nullptr;
};
class NativeHarborSmokeCursor {
public:
  uint32_t pending() const { return pending_; }

private:
  friend bool resumeNativeHarborSmoke(EventEntry &,
                                      const NativeHarborSmokeServices &,
                                      NativeHarborSmokeCursor &);
  struct Read {
    uint32_t kind, address, value;
  };
  std::vector<Read> reads_;
  NativeHarborSmokeServices services_;
  const EventEntry *event_ = nullptr;
  uint32_t id_ = 0, command_ = 0, args_ = 0, pending_ = 0;
  bool invoking_ = false, prepared_ = false, complete_ = false;
  NativeHarborSmokeRequest request_;
};
// Exact evt_eff source argument-evaluation prefix for the finite original
// kemuri_test/type4/unnamed path. Other domains hold; no guest pool/name write
// or whole original effect-driver completion is asserted.
bool resumeNativeHarborSmoke(EventEntry &, const NativeHarborSmokeServices &,
                             NativeHarborSmokeCursor &);
struct NativeHarborSmokeData {
  std::array<float, 10> angles{}, radii{};
  float quarterTurn = 0, tau = 0, fullTurn = 0, radiusScale = 0, decay = 0;
  bool load(const char *eventImage);
  bool valid() const;
};
struct NativeHarborSmokeSource {
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*random)(void *, uint32_t) = nullptr;
  std::optional<float> (*sin)(void *, float) = nullptr;
  std::optional<float> (*cos)(void *, float) = nullptr;
};
;
struct NativeHarborSmokeConsumer {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint64_t> (*spawn)(void *, const NativeHarborSmokeRequest &,
                                   const NativeHarborSmokeSource &) = nullptr;
  bool (*advance)(void *, uint64_t simulationVisit) = nullptr;
};
// Fixed native budgets are port policy, distinct from original effEntry pool.
class NativeHarborSmokeVisual {
public:
  static constexpr size_t Capacity = 4, Particles = 10;
  static constexpr uint32_t AnimationFrames = 16, LifetimeTicks = 32,
                            DelayBound = 30;
  struct Particle {
    float x = 0, y = 0, vx = 0, vy = 0;
    uint32_t delay = 0, age = 0;
    uint8_t frame = 0, nextFrame = 0;
  };
  struct Instance {
    uint64_t id = 0;
    NativeHarborSmokeRequest request;
    std::array<Particle, Particles> particles{};
  };

  NativeHarborSmokeVisual(NativeHarborSmokeData, std::shared_ptr<const void>,
                          std::function<bool()> gpuCurrent);
  // Constructor continuation retains each successful real RNG/math boundary.
  std::optional<uint64_t> spawn(const NativeHarborSmokeRequest &,
                                const NativeHarborSmokeSource &);
  bool advance(uint64_t simulationVisit);
  bool current() const;
  void retire();
  const std::array<Instance, Capacity> &instances() const { return instances_; }

private:
  NativeHarborSmokeData data_;
  NativeHarborSmokeSource services_;
  std::shared_ptr<const void> gpuLifetime_;
  std::function<bool()> gpuCurrent_;
  std::array<Instance, Capacity> instances_{};
  Instance candidate_;
  size_t candidateSlot_ = Capacity, particle_ = 0;
  unsigned stage_ = 0;
  float angle_ = 0, radius_ = 0;
  uint64_t next_ = 1, last_ = 0;
  bool visited_ = false, invoking_ = false, retired_ = false;
  mutable bool checking_ = false;
};
} // namespace ttyd
