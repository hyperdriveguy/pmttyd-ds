#pragma once
#include "battle_class_raster.hpp"
#include <memory>
namespace ttyd {
// One original circle/bubble invocation. Fixed service binding and
// bit-identical input identity are required across waits. GPU/SDK
// acknowledgements are never replayed. This does not admit missing resources or
// fabricate source matrices.
class BattleClassRasterCursor {
public:
  BattleClassRasterCursor();
  ~BattleClassRasterCursor();
  BattleClassRasterCursor(const BattleClassRasterCursor &) = delete;
  BattleClassRasterCursor &operator=(const BattleClassRasterCursor &) = delete;
  bool active() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend struct BattleClassRasterContinuationAccess;
};
bool resumePackedBattleClassCircle(uint32_t row, const BattleActMatrix &base,
                                   const std::array<float, 3> &position,
                                   float parameter,
                                   const BattleClassRasterServices &,
                                   BattleClassRasterCursor &);
bool resumePackedBattleClassBubble(uint32_t work, uint32_t row,
                                   const BattleActMatrix &base, float parameter,
                                   const BattleClassRasterServices &,
                                   BattleClassRasterCursor &);
} // namespace ttyd
