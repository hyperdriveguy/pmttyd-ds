#include "harbor_battle_music_owner.hpp"
#include <tuple>
namespace ttyd {
namespace {
struct Guard {
  bool &active;
  explicit Guard(bool &v) : active(v) { active = true; }
  ~Guard() { active = false; }
};
bool sameOwner(const std::shared_ptr<const void> &a,
               const std::shared_ptr<const void> &b) {
  return a && b && a.get() == b.get() && !a.owner_before(b) &&
         !b.owner_before(a);
}
auto identity(const HarborBattleMusicServices &s) {
  return std::make_tuple(s.lifetime.get(), s.user, s.current, s.start, s.valid,
                         s.mix, s.stop);
}
} // namespace
bool HarborBattleMusicOwner::sourceCurrent() const {
  return binding_.runtime && binding_.proof && binding_.current &&
         binding_.current(binding_.user) && services_.lifetime &&
         services_.current && services_.current(services_.user);
}
bool HarborBattleMusicOwner::owns(
    const std::shared_ptr<const void> &runtime) const {
  return sameOwner(retired_ ? retiredRuntime_.lock() : binding_.runtime,
                   runtime);
}
bool HarborBattleMusicOwner::retired(
    const std::shared_ptr<const void> &runtime) const {
  return owns(runtime) && retired_;
}
bool HarborBattleMusicOwner::drive(bool enabled) {
  if (!sourceCurrent())
    return false;
  if (lease_ && lease_->track != desired_) {
    if (!services_.stop(services_.user, *lease_))
      return false;
    lease_.reset();
  }
  if (!sourceCurrent())
    return false;
  if (!lease_) {
    auto lease = services_.start(services_.user, desired_, enabled);
    if (!lease)
      return false;
    lease_ = *lease;
  }
  if (!sourceCurrent() || lease_->track != desired_ || !lease_->generation ||
      !services_.valid(services_.user, *lease_))
    return false;
  if (!sourceCurrent() || !services_.mix(services_.user, *lease_, enabled))
    return false;
  return sourceCurrent();
}
bool HarborBattleMusicOwner::begin(const HarborBattleMusicBinding &b,
                                   const HarborBattleMusicServices &s,
                                   bool victory, bool enabled) {
  if (invoking_ || retired_)
    return false;
  Guard guard(invoking_);
  if (!b.runtime || !b.proof || !b.current || !s.lifetime || !s.current ||
      !s.start || !s.valid || !s.mix || !s.stop)
    return false;
  if (binding_.runtime) {
    if (!sameOwner(binding_.runtime, b.runtime) ||
        !sameOwner(binding_.proof, b.proof) || binding_.user != b.user ||
        binding_.current != b.current || identity(services_) != identity(s) ||
        !sameOwner(services_.lifetime, s.lifetime) || retired_)
      return false;
  } else {
    binding_ = b;
    services_ = s;
  }
  // An actual victory owner cannot be downgraded by a legacy battle-track
  // visit.
  if (victory)
    desired_ = VictoryTrack;
  return drive(enabled);
}
bool HarborBattleMusicOwner::update(const std::shared_ptr<const void> &runtime,
                                    bool enabled) {
  if (invoking_ || !owns(runtime) || retired_)
    return false;
  Guard guard(invoking_);
  return drive(enabled);
}
bool HarborBattleMusicOwner::retire(
    const std::shared_ptr<const void> &runtime) {
  if (invoking_ || !owns(runtime))
    return false;
  Guard guard(invoking_);
  if (retired_)
    return true;
  // Source may already be stale. Only independently pinned native backend is
  // borrowed during teardown; it never reads the old source runtime.
  if (!services_.lifetime || !services_.current ||
      !services_.current(services_.user))
    return false;
  if (lease_) {
    if (!services_.stop(services_.user, *lease_))
      return false;
    lease_.reset();
  }
  retiredRuntime_ = binding_.runtime;
  binding_ = {};
  services_ = {};
  retired_ = true;
  return true;
}
} // namespace ttyd
