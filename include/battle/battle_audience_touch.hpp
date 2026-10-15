#pragma once
#include "retained_icon_frame.hpp"
#include <array>
#include <optional>
#include <vector>
// Native lower-screen adaptation uses acknowledged world geometry and actual
// completed displayed pass. It adds no original targeting, visibility or depth
// rule; source physical-X first-eligible behavior remains independent.
namespace ttyd {
// Native input adaptation, not original game targeting rules. The producer must
// retain an actor/item incarnation: reusing a source slot alone is insufficient.
struct BattleAudienceTouchTarget {
  uint32_t slot=0, item=0;
  uint64_t actorGeneration=0, itemGeneration=0;
  bool operator==(const BattleAudienceTouchTarget& other) const;
};
struct BattleAudienceTouchCandidate {
  BattleAudienceTouchTarget target;
  // Explicit producer evidence, never inferred from item-number ranges here.
  bool visible=false, dangerous=false, interruptible=false;
};
// Geometry must be copied from the acknowledged audience world quad, in source
// units, after its actual local/world matrix. Do not supply seat approximations
// or a quad which still includes source view (camera is applied here once).
using BattleAudienceTouchQuad = std::array<std::array<float,3>,4>;
class BattleAudienceTouchFrame {
public:
  void reset();
  // Starts building a scene4 pass with its actual retained source camera.
  bool begin(const SourceIconPass&, const RetainedIconCamera&);
  bool append(const SourceIconPass&, const BattleAudienceTouchTarget&,
              const BattleAudienceTouchQuad&);
  // A held/incomplete draw may remain visible but never becomes tappable.
  bool finish(const SourceIconPass&);
  // Require the currently displayed completed pass, then revalidate source
  // incarnation and eligibility. Ambiguous overlaps deliberately select none.
  // Physical X keeps its separate original first-eligible selection path.
  std::optional<BattleAudienceTouchTarget> hit(
      const SourceIconPass& displayed, float x, float y,
      const std::vector<BattleAudienceTouchCandidate>& live) const;
private:
  struct Quad { BattleAudienceTouchTarget target; std::array<std::array<float,2>,4> pixels; };
  SourceIconPass pass_;
  CameraProjectionMatrix lower_{};
  std::vector<Quad> quads_;
  bool complete_=false, failed_=false;
};
} // namespace ttyd
