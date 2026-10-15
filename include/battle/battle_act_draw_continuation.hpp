#pragma once
#include "battle_act_draw.hpp"
#include <memory>
// Retains one DrawMainMenu invocation, including copied/optional stack slots.
// Keep the service table and owners identical across holds; advance simulation
// only after this invocation and its retained child providers complete.
namespace ttyd {
// One original DrawMainMenu invocation, including its copied stack slots. The
// caller owns simulation cadence: resume this invocation before starting the
// next tick; neither a second screen nor a GPU retry starts another draw.
class BattleActDrawCursor {
public:
  BattleActDrawCursor();
  ~BattleActDrawCursor();
  BattleActDrawCursor(const BattleActDrawCursor &) = delete;
  BattleActDrawCursor &operator=(const BattleActDrawCursor &) = delete;
  bool active() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumePackedBattleActMenuDraw(const BattleActDrawServices &,
                                            BattleActDrawCursor &);
};
// SDK/footer false retains the exact call operands and all prior source writes.
// Child icons/party/pinch providers must retain their own partial invocations:
// repeating an unretained drawPackedBattleActIcons after partial raster effects
// is not safe. Unwritten source positions/matrices remain optional; no identity
// or guessed geometry is supplied to make those child providers acknowledge.
// The service table and its owners must remain identical during a continuation.
bool resumePackedBattleActMenuDraw(const BattleActDrawServices &,
                                   BattleActDrawCursor &);
} // namespace ttyd
