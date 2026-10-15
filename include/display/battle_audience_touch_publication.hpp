#pragma once
#include "battle_audience_touch.hpp"
#include <memory>
namespace ttyd {
// Native audience-allocation lifetime and publication identity, never original
// guest counters. A reused guest address requires a fresh lifetime token.
struct BattleAudienceTouchPublicationKey {
  std::shared_ptr<const uint8_t> owner;
  uint32_t allocation = 0;
  uint64_t sequence = 0;
};
struct BattleAudienceTouchPublishedQuad {
  BattleAudienceTouchTarget target;
  BattleAudienceTouchQuad world;
};
// The producer supplies one actual completed source scene4 pass. Geometry is
// copied from acknowledged world quads, with the same retained source camera.
// Eligibility comes from real source state and visibility ownership, never seat
// estimates or item-number heuristics. All item-holder geometry capable of
// overlapping a target must be represented; non-holder occlusion must already
// be resolved by the actual visibility owner. Missing knowledge stays unbound.
struct BattleAudienceTouchSourceFrame {
  SourceIconPass pass;
  RetainedIconCamera camera;
  std::vector<BattleAudienceTouchPublishedQuad> quads;
  std::vector<BattleAudienceTouchCandidate> eligibility;
};
struct BattleAudienceTouchPublishedFrame {
  BattleAudienceTouchPublicationKey key;
  BattleAudienceTouchSourceFrame source;
  BattleAudienceTouchFrame geometry;
};
// Data-only request for the simulation's existing retained interruption owner.
// Do not translate it to synthetic X: original X may select a different holder.
struct BattleAudienceTouchRequest {
  BattleAudienceTouchPublicationKey frame;
  BattleAudienceTouchTarget target;
  uint64_t inputSerial = 0;
  float x = 0, y = 0;
};
class BattleAudienceTouchPublication {
public:
  // physicalAlternativeBound acknowledges an actual independent physical-X
  // provider. False prohibits touch publication, and never fabricates that
  // path.
  void bind(std::shared_ptr<const uint8_t> owner, uint32_t allocation,
            bool physicalAlternativeBound);
  void retire();
  bool physicalSelectionAvailable() const { return physical_; }
  bool publish(const BattleAudienceTouchPublicationKey &,
               const BattleAudienceTouchSourceFrame &);
  const std::shared_ptr<const BattleAudienceTouchPublishedFrame> &
  frame() const {
    return frame_;
  }
  // Render calls this only after the exact lower frame was actually drawn.
  // Publishing a replacement invalidates prior admission and pending taps.
  bool admit(const BattleAudienceTouchPublicationKey &);
  // Queue one actual touch-down edge against the displayed frame. Monotonic
  // native input serials are edge identities, not simulation/render durations.
  // This changes only presentation metadata; it writes no simulation state.
  bool queueTap(const BattleAudienceTouchPublicationKey &, uint64_t inputSerial,
                float x, float y);
  // Simulation consumes once, revalidating the exact current allocation/frame,
  // current item/actor incarnations, visibility and harmful eligibility. An
  // ineligible request is discarded; it cannot activate after eligibility
  // flips.
  std::optional<BattleAudienceTouchRequest>
  consumeTap(const BattleAudienceTouchPublicationKey &current,
             uint64_t inputSerial,
             const std::vector<BattleAudienceTouchCandidate> &live);

private:
  bool matches(const BattleAudienceTouchPublicationKey &) const;
  std::shared_ptr<const uint8_t> owner_;
  uint32_t allocation_ = 0;
  uint64_t sequence_ = 0, inputSerial_ = 0;
  bool physical_ = false, admitted_ = false;
  std::shared_ptr<const BattleAudienceTouchPublishedFrame> frame_;
  std::optional<BattleAudienceTouchRequest> pending_;
};
} // namespace ttyd
