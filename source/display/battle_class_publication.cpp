#include "battle_class_publication.hpp"
namespace ttyd {
// Same token/address preserves monotonic sequences; a fresh allocation token
// permits address reuse only after discarding the prior batch/admission state.
void BattleClassPublication::bind(std::shared_ptr<const uint8_t> owner,
                                  uint32_t allocation) {
  if (!owner || !allocation) {
    retire();
    return;
  }
  if (owner == owner_ && allocation == allocation_)
    return;
  retire();
  owner_ = std::move(owner);
  allocation_ = allocation;
}
void BattleClassPublication::retire() {
  owner_.reset();
  allocation_ = 0;
  key_ = {};
  batch_.reset();
  admitted_ = consumed_ = false;
}
// Optional surface lease is a long GPU-generation check. Legacy batches
// without that lease still borrow assets from the caller-owned asset lifetime.
bool BattleClassPublication::matches(const BattleClassPublicationKey &key) const {
  return batch_ && key.owner && key.owner == owner_ &&
         key.allocation == allocation_ && key.sequence == key_.sequence &&
         (!batch_->surfaceLease.generation || batch_->surfaceLease.current());
}
// Reject incomplete/stale candidates without replacing the last valid batch.
// Sequence 0 is unstarted; strictly increasing values are publications, not time.
bool BattleClassPublication::publish(const BattleClassPublicationKey &key,
                                     const BattleClassBackend &source) {
  if (!owner_ || key.owner != owner_ || key.allocation != allocation_ ||
      !key.sequence || key.sequence <= key_.sequence || !source.complete() ||
      !source.assets || !source.assets->ready() ||
      (source.surfaceLease.generation && (!source.surfaceLease.current() ||
       source.assets != source.surfaceLease.assets.get())))
    return false;
  // Do not copy label/random/icon callbacks: their contexts may belong to a
  // completed stack-local source pass. Rendering needs only assets and quads.
  auto copy = std::make_shared<BattleClassBackend>();
  copy->assets = source.assets;
  copy->surfaceLease = source.surfaceLease;
  copy->quads = source.quads;
  batch_ = std::move(copy);
  key_ = key;
  admitted_ = consumed_ = false;
  return true;
}
bool BattleClassPublication::admit(const BattleClassPublicationKey &key) {
  if (!matches(key))
    return false;
  // Render may report the same snapshot repeatedly; consumption stays one-use.
  admitted_ = true;
  return true;
}
std::optional<BattleClassPublicationKey>
BattleClassPublication::consumeAdmission() {
  if (!batch_ || !admitted_ || consumed_ ||
      (batch_->surfaceLease.generation && !batch_->surfaceLease.current()))
    return {};
  // Notify simulation once; this does not mutate any original readiness field.
  consumed_ = true;
  return key_;
}
} // namespace ttyd
