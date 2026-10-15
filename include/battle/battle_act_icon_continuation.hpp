#pragma once
#include "battle_act_draw.hpp"
#include <memory>
// Cursor borrows the slots VECTOR OBJECT by identity, not just its bytes.
// Same work/rows/providers must persist. Initial depths are captured, but later
// slot values/counts/flags remain source-visible at their original read points.
namespace ttyd {
// One whole original DrawMainIcon invocation. The source stack-slot container
// is borrowed by object identity and must outlive the cursor. Depths/order and
// the chosen child operands are retained; later source row/flag reads stay
// fresh.
class BattleActIconDrawCursor {
public:
  BattleActIconDrawCursor();
  ~BattleActIconDrawCursor();
  BattleActIconDrawCursor(const BattleActIconDrawCursor &) = delete;
  BattleActIconDrawCursor &operator=(const BattleActIconDrawCursor &) = delete;
  bool active() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumePackedBattleActIcons(uint32_t, uint32_t,
                                         const std::vector<BattleActDrawSlot> &,
                                         const BattleActIconDrawServices &,
                                         BattleActIconDrawCursor &);
};
// A child which returns false must retain its own partial raster invocation.
// This outer cursor never repeats an acknowledged sibling. Unwritten source
// stack frontiers stay unavailable rather than manufacturing missing geometry.
bool resumePackedBattleActIcons(uint32_t work, uint32_t rows,
                                const std::vector<BattleActDrawSlot> &,
                                const BattleActIconDrawServices &,
                                BattleActIconDrawCursor &);
} // namespace ttyd
