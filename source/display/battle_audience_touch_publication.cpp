#include "battle_audience_touch_publication.hpp"
namespace ttyd {
void BattleAudienceTouchPublication::retire() {
  owner_.reset();
  allocation_ = 0;
  sequence_ = inputSerial_ = 0;
  physical_ = admitted_ = false;
  frame_.reset();
  pending_.reset();
}
void BattleAudienceTouchPublication::bind(std::shared_ptr<const uint8_t> owner,
                                          uint32_t allocation, bool physical) {
  if (!owner || !allocation) {
    retire();
    return;
  }
  if (owner == owner_ && allocation == allocation_) {
    if (physical_ != physical) {
      physical_ = physical;
      admitted_ = false;
      frame_.reset();
      pending_.reset();
    }
    return;
  }
  retire();
  owner_ = std::move(owner);
  allocation_ = allocation;
  physical_ = physical;
}
bool BattleAudienceTouchPublication::matches(
    const BattleAudienceTouchPublicationKey &key) const {
  return frame_ && physical_ && key.owner && key.owner == owner_ &&
         key.allocation == allocation_ && key.sequence == sequence_;
}
bool BattleAudienceTouchPublication::publish(
    const BattleAudienceTouchPublicationKey &key,
    const BattleAudienceTouchSourceFrame &source) {
  if (!physical_ || !owner_ || key.owner != owner_ ||
      key.allocation != allocation_ || !key.sequence ||
      key.sequence <= sequence_ || !source.pass.complete ||
      source.pass.owner != key.owner || source.pass.sequence != key.sequence ||
      source.quads.size() > 200 ||
      source.quads.size() != source.eligibility.size())
    return false;
  BattleAudienceTouchFrame geometry;
  const SourceIconPass building{source.pass.owner, source.pass.sequence, false};
  if (!geometry.begin(building, source.camera))
    return false;
  for (const auto &quad : source.quads) {
    unsigned matches = 0;
    bool identity = false;
    for (const auto &candidate : source.eligibility)
      if (candidate.target.slot == quad.target.slot) {
        ++matches;
        identity = candidate.target == quad.target;
      }
    if (matches != 1 || !identity ||
        !geometry.append(building, quad.target, quad.world))
      return false;
  }
  if (!geometry.finish(source.pass))
    return false;
  auto copy = std::make_shared<BattleAudienceTouchPublishedFrame>();
  copy->key = key;
  copy->source = source;
  copy->geometry = std::move(geometry);
  frame_ = std::move(copy);
  sequence_ = key.sequence;
  admitted_ = false;
  pending_.reset();
  return true;
}
bool BattleAudienceTouchPublication::admit(
    const BattleAudienceTouchPublicationKey &key) {
  if (!matches(key))
    return false;
  admitted_ = true;
  return true;
}
bool BattleAudienceTouchPublication::queueTap(
    const BattleAudienceTouchPublicationKey &key, uint64_t serial, float x,
    float y) {
  if (!admitted_ || !matches(key) || !serial || serial <= inputSerial_)
    return false;
  // Observe even a miss or an edge dropped behind a pending request: the same
  // touch-down must never become a new action on a later frame/eligibility
  // change.
  inputSerial_ = serial;
  if (pending_)
    return false;
  const auto target = frame_->geometry.hit(frame_->source.pass, x, y,
                                           frame_->source.eligibility);
  if (!target)
    return false;
  pending_ = BattleAudienceTouchRequest{key, *target, serial, x, y};
  return true;
}
std::optional<BattleAudienceTouchRequest>
BattleAudienceTouchPublication::consumeTap(
    const BattleAudienceTouchPublicationKey &current, uint64_t serial,
    const std::vector<BattleAudienceTouchCandidate> &live) {
  if (!admitted_ || !matches(current) || !pending_ ||
      pending_->inputSerial != serial)
    return {};
  const auto request = *pending_;
  pending_.reset();
  const auto target =
      frame_->geometry.hit(frame_->source.pass, request.x, request.y, live);
  if (!target || !(*target == request.target))
    return {};
  return request;
}
} // namespace ttyd
