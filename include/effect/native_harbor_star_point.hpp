#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
// Finite native instances, never original guest EffectEntry addresses.
struct NativeHarborStarPointRequest {
  int32_t subtype = 0, amount = 0;
  std::array<float, 3> position{};
};
enum class NativeHarborStarMath : uint8_t {
  Add,
  Subtract,
  Multiply,
  Divide,
  MultiplyAdd,
  MultiplySubtract
};
struct NativeHarborStarPointSource {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<int32_t> (*randomRaw)(void *) = nullptr;
  std::optional<float> (*math)(void *, NativeHarborStarMath, float, float,
                               float) = nullptr;
};
struct EventEntry;
struct NativeHarborStarPointServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*value)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<float> (*floating)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint64_t> (*spawn)(
      void *, const NativeHarborStarPointRequest &) = nullptr;
};
class NativeHarborStarPointCursor {
public:
  uint32_t pending() const { return pending_; }

private:
  friend bool resumeNativeHarborStarPoint(EventEntry &,
                                          const NativeHarborStarPointServices &,
                                          NativeHarborStarPointCursor &);
  struct Read {
    uint32_t kind, address, value;
  };
  std::vector<Read> reads_;
  NativeHarborStarPointServices services_;
  const EventEntry *event_ = nullptr;
  uint32_t id_ = 0, command_ = 0, args_ = 0, pending_ = 0, name_ = 0;
  bool invoking_ = false, prepared_ = false, spawned_ = false,
       complete_ = false;
  NativeHarborStarPointRequest request_;
};
// Exact reached evt_eff argument evaluation; finite star_point/subtype0 native
// admission. A nonempty name needs genuine guest naming ownership and holds.
bool resumeNativeHarborStarPoint(EventEntry &,
                                 const NativeHarborStarPointServices &,
                                 NativeHarborStarPointCursor &);
// Original US scalar bank80426f08..3f and signed bias802fe540, loaded from an
// admitted read-only original source pack. Atlas pixels never prove this data.
struct NativeHarborStarPointData {
  std::array<float, 14> scalars{};
  uint64_t signedBias = 0;
  bool load(const char *eventImage);
  bool valid() const;
};
struct NativeHarborStarPointConsumer {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint64_t> (*spawn)(void *, const NativeHarborStarPointRequest &,
                                   const NativeHarborStarPointSource &) =
      nullptr;
  bool (*advance)(void *, uint64_t simulationVisit) = nullptr;
};
class NativeHarborStarPointVisual {
public:
  // Native finite budgets; original unchecked effEntry/heap are not emulated.
  static constexpr size_t Capacity = 4, MaximumParticles = 100;
  static constexpr int32_t LifetimeTicks = 60, FirstParticleDelay = 4,
                           ParticleDelayStride = 3;
  static constexpr uint32_t IconId = 404;
  struct Particle {
    std::array<float, 3> position{}, velocity{};
    float gravity = 0;
    int32_t alpha = 0, delay = 0;
  };
  struct Instance {
    uint64_t id = 0;
    NativeHarborStarPointRequest request;
    int32_t lifetime = LifetimeTicks, age = 0;
    std::vector<Particle> particles;
  };
  NativeHarborStarPointVisual(NativeHarborStarPointData,
                              std::shared_ptr<const void> resourceOwner,
                              std::function<bool()> resourceCurrent);
  ~NativeHarborStarPointVisual();
  NativeHarborStarPointVisual(const NativeHarborStarPointVisual &) = delete;
  NativeHarborStarPointVisual &
  operator=(const NativeHarborStarPointVisual &) = delete;
  bool current() const;
  std::optional<uint64_t> spawn(const NativeHarborStarPointRequest &,
                                const NativeHarborStarPointSource &);
  bool advance(uint64_t simulationVisit);
  const std::array<Instance, Capacity> &instances() const;
  bool retire();

private:
  struct State;
  std::unique_ptr<State> state_;
};
} // namespace ttyd
