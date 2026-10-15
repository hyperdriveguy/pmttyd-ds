#include "battle_class_publication.hpp"
namespace ttyd {
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
bool BattleClassPublication::matches(const BattleClassPublicationKey &key) const {
  return batch_ && key.owner && key.owner == owner_ &&
         key.allocation == allocation_ && key.sequence == key_.sequence;
}
bool BattleClassPublication::publish(const BattleClassPublicationKey &key,
                                     const BattleClassBackend &source) {
  if (!owner_ || key.owner != owner_ || key.allocation != allocation_ ||
      !key.sequence || key.sequence <= key_.sequence || !source.complete() ||
      !source.assets || !source.assets->ready())
    return false;
  // Do not copy label/random/icon callbacks: their contexts may belong to a
  // completed stack-local source pass. Rendering needs only assets and quads.
  auto copy = std::make_shared<BattleClassBackend>();
  copy->assets = source.assets;
  copy->quads = source.quads;
  batch_ = std::move(copy);
  key_ = key;
  admitted_ = consumed_ = false;
  return true;
}
bool BattleClassPublication::admit(const BattleClassPublicationKey &key) {
  if (!matches(key))
    return false;
  admitted_ = true;
  return true;
}
std::optional<BattleClassPublicationKey>
BattleClassPublication::consumeAdmission() {
  if (!batch_ || !admitted_ || consumed_)
    return {};
  consumed_ = true;
  return key_;
}
} // namespace ttyd
