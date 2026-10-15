#pragma once
#include "battle_action_class_table_continuation.hpp"
#include "message_assets.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
class EventAddressSpace;
struct BattleClassMenuMessageKey {
  uint32_t source;
  const char *id;
};
const std::array<BattleClassMenuMessageKey, 7> &
originalBattleClassMenuMessageKeys();
// Private-VM allocation policy: registration is permanent until VM teardown.
// No detach/rebind/unregister or source writes are admitted for this cache.
// This is not an EventAddressSpace registration-generation/ABA proof.
class BattleClassMenuMessages
    : public std::enable_shared_from_this<BattleClassMenuMessages> {
public:
  static std::shared_ptr<BattleClassMenuMessages>
  map(EventAddressSpace &, uint32_t guestBase,
      std::shared_ptr<const MessageAssets>);
  BattleClassMenuMessages(const BattleClassMenuMessages &) = delete;
  BattleClassMenuMessages &operator=(const BattleClassMenuMessages &) = delete;
  std::optional<uint32_t> lookup(const EventAddressSpace &,
                                 uint32_t sourceKey) const;
  std::shared_ptr<const BattleActionClassMessageBinding>
  binding(const EventAddressSpace &) const;
  uint32_t base() const { return base_; }
  uint32_t size() const { return size_; }

private:
  BattleClassMenuMessages() = default;
  EventAddressSpace *memory_ =
      nullptr; // Borrowed stable actual VM; VM outlives calls.
  std::shared_ptr<const MessageAssets> resources_;
  uint32_t base_ = 0, size_ = 0;
  std::array<uint32_t, 7> pointers_{};
};
} // namespace ttyd
