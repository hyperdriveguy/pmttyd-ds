#pragma once
#include "battle_class_backend.hpp"
#include <memory>
namespace ttyd {
// Native ownership metadata, never an original guest field or elapsed clock.
struct BattleClassPublicationKey {
  std::shared_ptr<const uint8_t> owner;
  // Source allocation identity; zero means unbound, not a usable guest address.
  uint32_t allocation = 0;
  uint64_t sequence = 0;
};
// Simulation publishes only after a successful complete source draw. Render
// reads copied geometry and reports admission; it never executes a controller.
// The actual renderer asset owner must outlive every published frame.
class BattleClassPublication {
public:
  // Simulation-owned UI allocation lifetime. Rebinding the same identity does
  // not restart sequences. A fresh token distinguishes reused guest addresses.
  void bind(std::shared_ptr<const uint8_t> owner, uint32_t allocation);
  void retire();
  bool publish(const BattleClassPublicationKey &, const BattleClassBackend &);
  const std::shared_ptr<const BattleClassBackend> &batch() const { return batch_; }
  const BattleClassPublicationKey &key() const { return key_; }
  // Call only after native drawClassMenu returned true for this exact snapshot.
  bool admit(const BattleClassPublicationKey &);
  // Simulation consumes at most once. This notification itself writes no source
  // readiness byte: original DrawMainMenu remains the owner of its guest writes.
  std::optional<BattleClassPublicationKey> consumeAdmission();
private:
  bool matches(const BattleClassPublicationKey &) const;
  std::shared_ptr<const uint8_t> owner_;
  uint32_t allocation_ = 0;
  BattleClassPublicationKey key_;
  std::shared_ptr<const BattleClassBackend> batch_;
  // Reset on replacement/retirement, not by duplicate admit() calls.
  bool admitted_ = false, consumed_ = false;
};
} // namespace ttyd
