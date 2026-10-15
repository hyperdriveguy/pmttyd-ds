#include "battle_audience_incarnation.hpp"
#include <limits>
namespace ttyd {
BattleAudienceLifecycleHook BattleAudienceIncarnationOwner::hook() {
  return {this, [](void *p, BattleAudienceLifecycleEvent e, uint32_t r,
                   uint32_t related) {
            static_cast<BattleAudienceIncarnationOwner *>(p)->observe(e, r,
                                                                      related);
          }};
}
std::optional<uint32_t>
BattleAudienceIncarnationOwner::actorIndex(uint32_t r) const {
  if (!owner_)
    return {};
  const uint64_t base = uint64_t(allocation_) + 0x1bc;
  if (uint64_t(r) < base)
    return {};
  const auto offset = uint64_t(r) - base;
  if (offset % 0x134 || offset / 0x134 >= actors_.size())
    return {};
  return uint32_t(offset / 0x134);
}
std::optional<uint32_t>
BattleAudienceIncarnationOwner::itemIndex(uint32_t r) const {
  if (!owner_)
    return {};
  const uint64_t base = uint64_t(allocation_) + 0xf25c;
  if (uint64_t(r) < base)
    return {};
  const auto offset = uint64_t(r) - base;
  if (offset % 0x48 || offset / 0x48 >= items_.size())
    return {};
  return uint32_t(offset / 0x48);
}
// Generation0 is unknown. On U64 exhaustion discard all identities and stop
// admitting births until actual Reset; never wrap into a stale generation.
uint64_t BattleAudienceIncarnationOwner::birth() {
  if (exhausted_)
    return 0;
  if (serial_ == std::numeric_limits<uint64_t>::max()) {
    exhausted_ = true;
    actors_ = {};
    items_ = {};
    return 0;
  }
  return ++serial_;
}
// Reset accepts only a nonzero whole 0x13914 guest span and creates a new
// owner token. Exact actor/item record alignment is required for observations.
// Reserved/Clear invalidates actor; ItemReady links the captured holder's then-
// current generation. Observers perform no guest reads or source side effects.
void BattleAudienceIncarnationOwner::observe(BattleAudienceLifecycleEvent event,
                                             uint32_t r, uint32_t related) {
  using E = BattleAudienceLifecycleEvent;
  if (event == E::Reset) {
    owner_.reset();
    allocation_ = 0;
    serial_ = 0;
    exhausted_ = false;
    actors_ = {};
    items_ = {};
    if (r && uint64_t(r) + 0x13914 <=
                 uint64_t(std::numeric_limits<uint32_t>::max()) + 1) {
      allocation_ = r;
      owner_ = std::make_shared<const uint8_t>(0);
    }
    return;
  }
  if (!owner_ || exhausted_)
    return;
  if (event == E::ActorBirth || event == E::ActorReady ||
      event == E::ActorReserved || event == E::ActorClear) {
    const auto i = actorIndex(r);
    if (!i)
      return;
    if (event == E::ActorBirth) {
      const auto generation = birth();
      actors_[*i] = {generation, generation != 0, false};
    } else if (event == E::ActorReady) {
      if (actors_[*i].alive)
        actors_[*i].ready = true;
    } else
      actors_[*i] = {};
  } else {
    const auto i = itemIndex(r);
    if (!i)
      return;
    if (event == E::ItemBirth) {
      const auto generation = birth();
      items_[*i] = {};
      items_[*i].generation = generation;
      items_[*i].alive = generation != 0;
    } else if (event == E::ItemReady) {
      auto &item = items_[*i];
      if (!item.alive)
        return;
      item.ready = true;
      const auto actor = actorIndex(related);
      if (actor && actors_[*actor].alive) {
        item.slot = *actor;
        item.actorGeneration = actors_[*actor].generation;
        item.linked = true;
      }
    } else if (event == E::ItemClear)
      items_[*i] = {};
  }
}
std::optional<uint64_t>
BattleAudienceIncarnationOwner::actorGeneration(uint32_t slot) const {
  if (!owner_ || exhausted_ || slot >= actors_.size() || !actors_[slot].alive ||
      !actors_[slot].ready)
    return {};
  return actors_[slot].generation;
}
std::optional<uint64_t>
BattleAudienceIncarnationOwner::itemGeneration(uint32_t item) const {
  if (!owner_ || exhausted_ || item >= items_.size() || !items_[item].alive ||
      !items_[item].ready)
    return {};
  return items_[item].generation;
}
// Both lifecycles must be observed alive/ready and the item's stored actor
// generation must match NOW. Slot reuse invalidates the old link even when
// source pointers/kind are unchanged. This proves identity, not eligibility.
std::optional<BattleAudienceIncarnationIdentity>
BattleAudienceIncarnationOwner::identity(uint32_t slot, uint32_t item) const {
  const auto actor = actorGeneration(slot), held = itemGeneration(item);
  if (!actor || !held || !items_[item].linked || items_[item].slot != slot ||
      items_[item].actorGeneration != *actor)
    return {};
  return BattleAudienceIncarnationIdentity{owner_, allocation_, slot,
                                           item,   *actor,      *held};
}
} // namespace ttyd
