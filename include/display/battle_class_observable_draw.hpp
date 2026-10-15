#pragma once
#include "battle_class_draw_continuation.hpp"
namespace ttyd {
class BattleClassObservableDrawCursor {
public:
  BattleClassObservableDrawCursor();
  ~BattleClassObservableDrawCursor();
  BattleClassObservableDrawCursor(const BattleClassObservableDrawCursor&)=delete;
  BattleClassObservableDrawCursor& operator=(const BattleClassObservableDrawCursor&)=delete;
  bool active() const;
  // No rollback/resource retirement; abandon only with the actual pass owner.
  void reset();
private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumeBattleClassObservableDraw(uint32_t,uint32_t,
      const std::vector<BattleActDrawSlot>&,
      const BattleClassDrawContinuationServices&,BattleClassObservableDrawCursor&);
};
// Explicit PORT observable-raster policy, NOT original stack/FPSCR equivalence.
// Complete positions use the exact original retained sorter. Missing positions
// can be ignored with zero/one active row or finite, pairwise-distinct produced
// active Ys: visible raster order is then uniquely defined. A lone bubble needs
// no position operand; a circle always needs its actual produced position.
// Missing operands/tied or unordered active Ys remain unavailable; no positions
// are fabricated. Matrix/parameter and circle position must actually exist.
// Count/acting ownership/visibility/type flags must stay stable; sparse active
// operands must also stay stable. Revalidated at resume/provider boundaries.
// Boundary-only changes hold without replay; mutation inside a provider latches
// the invocation invalid even if storage is later restored. Providers must not
// mutate these inputs during a child. Guest reads must be ordinary pure
// storage reads; this policy does not reproduce inactive garbage comparisons.
// Slots retain borrowed object identity/lifetime. FontString retains the
// explicit ownership contract of BattleClassDrawContinuationServices.
// Does not begin/clear/publish/admit batches, write guest state or force Init1.
bool resumeBattleClassObservableDraw(uint32_t work,uint32_t rows,
    const std::vector<BattleActDrawSlot>&,
    const BattleClassDrawContinuationServices&,BattleClassObservableDrawCursor&);
}
