#pragma once
#include "source_gx_inherited_state.hpp"
#include <functional>
#include <memory>
#include <vector>
namespace ttyd {
// Long immutable evidence for actual source icon Loads. No GX writes, source
// acknowledgments or native readiness are produced. Reserve storage before the first
// source icon Load so a successful retain never allocates. Capacity is a
// native admission policy, bounded by the queued pass packet budget.
class BattleWeaponIconEvidence {
public:
  static std::shared_ptr<BattleWeaponIconEvidence>
  create(std::shared_ptr<SourceGxInheritedState>, size_t capacity,
         std::shared_ptr<const void> resources,
         std::function<bool()> resourceCurrent);
  bool retain(const std::shared_ptr<SourceGxInheritedState> &actualOwner,
              std::shared_ptr<const SourceGxInheritedSnapshot>);
  bool current() const;
  size_t size() const { return snapshots_.size(); }
  void retire();

private:
  BattleWeaponIconEvidence() = default;
  std::shared_ptr<SourceGxInheritedState> owner_;
  std::shared_ptr<const void> resources_;
  std::function<bool()> resourceCurrent_;
  std::vector<std::shared_ptr<const SourceGxInheritedSnapshot>> snapshots_;
  size_t capacity_ = 0;
  mutable bool checking_ = false, invalid_ = false;
  bool invoking_ = false, retired_ = false;
};
} // namespace ttyd
