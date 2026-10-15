#pragma once
#include "battle_audience_lifecycle.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
struct BattleAudienceIncarnationIdentity {
  std::shared_ptr<const uint8_t> owner;
  uint32_t audience = 0, slot = 0, item = 0;
  uint64_t actorGeneration = 0, itemGeneration = 0;
};
// Per-VM provenance only. Source Init's actual reset is the sole allocation
// lifetime producer; unobserved births remain unknown even when memory is live.
// No guest reads, writes, population, visibility or clock inference occurs
// here.
class BattleAudienceIncarnationOwner {
public:
  BattleAudienceIncarnationOwner() = default;
  BattleAudienceIncarnationOwner(const BattleAudienceIncarnationOwner &) =
      delete;
  BattleAudienceIncarnationOwner &
  operator=(const BattleAudienceIncarnationOwner &) = delete;
  BattleAudienceLifecycleHook hook();
  void observe(BattleAudienceLifecycleEvent, uint32_t record,
               uint32_t related = 0);
  const std::shared_ptr<const uint8_t> &token() const { return owner_; }
  uint32_t allocation() const { return allocation_; }
  std::optional<uint64_t> actorGeneration(uint32_t slot) const;
  std::optional<uint64_t> itemGeneration(uint32_t item) const;
  // Requires both completed observed lifecycles and the actual ItemReady link
  // to this exact actor generation. It proves identity only, not eligibility.
  std::optional<BattleAudienceIncarnationIdentity>
  identity(uint32_t slot, uint32_t item) const;

private:
  struct Actor {
    uint64_t generation = 0;
    bool alive = false, ready = false;
  };
  struct Item {
    uint64_t generation = 0, actorGeneration = 0;
    uint32_t slot = 0;
    bool alive = false, ready = false, linked = false;
  };
  std::optional<uint32_t> actorIndex(uint32_t) const;
  std::optional<uint32_t> itemIndex(uint32_t) const;
  uint64_t birth();
  std::shared_ptr<const uint8_t> owner_;
  uint32_t allocation_ = 0;
  uint64_t serial_ = 0;
  bool exhausted_ = false;
  std::array<Actor, 200> actors_{};
  std::array<Item, 100> items_{};
};
} // namespace ttyd
