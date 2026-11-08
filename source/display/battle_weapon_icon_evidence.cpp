#include "battle_weapon_icon_evidence.hpp"
#include <cstring>
namespace ttyd {
namespace {
struct Guard {
  bool &b;
  explicit Guard(bool &v) : b(v) { b = true; }
  ~Guard() { b = false; }
};
bool identical(const SourceGxInheritedSnapshot &a,
               const SourceGxInheritedSnapshot &b) {
  return a.stages() == b.stages() && a.size() == b.size() &&
         std::memcmp(a.evidence().data(), b.evidence().data(),
                     sizeof(a.evidence())) == 0;
}
} // namespace
std::shared_ptr<BattleWeaponIconEvidence>
BattleWeaponIconEvidence::create(std::shared_ptr<SourceGxInheritedState> owner,
                                 size_t capacity,
                                 std::shared_ptr<const void> resources,
                                 std::function<bool()> resourceCurrent) {
  if (!owner || !capacity || !resources || !resourceCurrent ||
      !resourceCurrent() || !owner->current())
    return {};
  auto out =
      std::shared_ptr<BattleWeaponIconEvidence>(new BattleWeaponIconEvidence);
  out->owner_ = std::move(owner);
  out->resources_ = std::move(resources);
  out->resourceCurrent_ = std::move(resourceCurrent);
  out->capacity_ = capacity;
  out->snapshots_.reserve(capacity);
  return out;
}
bool BattleWeaponIconEvidence::current() const {
  if (checking_ || retired_ || invalid_ || !owner_)
    return false;
  Guard guard(checking_);
  if (!resources_ || !resourceCurrent_ || !resourceCurrent_() ||
      !owner_->current()) {
    invalid_ = true;
    return false;
  }
  for (const auto &snapshot : snapshots_)
    if (!owner_->ownsSnapshot(snapshot)) {
      invalid_ = true;
      return false;
    }
  return !retired_;
}
bool BattleWeaponIconEvidence::retain(
    const std::shared_ptr<SourceGxInheritedState> &actualOwner,
    std::shared_ptr<const SourceGxInheritedSnapshot> snapshot) {
  if (invoking_ || checking_ || !actualOwner || !snapshot)
    return false;
  Guard guard(invoking_);
  if (!current() || actualOwner.get() != owner_.get() ||
      actualOwner.owner_before(owner_) || owner_.owner_before(actualOwner) ||
      !owner_->ownsSnapshot(snapshot) || retired_)
    return false;
  for (const auto &held : snapshots_)
    if (identical(*held, *snapshot))
      return current();
  if (snapshots_.size() >= capacity_)
    return false;
  // create reserved this exact capacity outside a native source/GPU interval.
  snapshots_.push_back(std::move(snapshot));
  return current();
}
void BattleWeaponIconEvidence::retire() { retired_ = true; }
} // namespace ttyd
