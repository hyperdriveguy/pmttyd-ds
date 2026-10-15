#pragma once
#include "battle_stage_objects.hpp"
#include "battle_stage_work.hpp"
#include <memory>
// Retained stage counterparts preserve source cache/counter stores and exact
// pending child operands across false. Service/lifecycle identity and nonreentry
// are checked; child providers must retain their own unfinished invocations.
namespace ttyd {
class BattleStageObjectCursor {
public:
  BattleStageObjectCursor();
  ~BattleStageObjectCursor();
  BattleStageObjectCursor(const BattleStageObjectCursor &) = delete;
  BattleStageObjectCursor &operator=(const BattleStageObjectCursor &) = delete;
  bool active() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool
  resumePackedBattleStageObjectMain(const BattleStageObjectServices &,
                                    BattleStageObjectCursor &);
};
class BattleStageWorkCursor {
public:
  BattleStageWorkCursor();
  ~BattleStageWorkCursor();
  BattleStageWorkCursor(const BattleStageWorkCursor &) = delete;
  BattleStageWorkCursor &operator=(const BattleStageWorkCursor &) = delete;
  bool active() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool resumePackedBattleStageWorkMain(const BattleStageWorkServices &,
                                              BattleStageWorkCursor &);
};
// False retains the exact pending child and its operands. A child provider owns
// any partial work inside that child. Completed calls begin anew next
// invocation.
bool resumePackedBattleStageObjectMain(const BattleStageObjectServices &,
                                       BattleStageObjectCursor &);
bool resumePackedBattleStageWorkMain(const BattleStageWorkServices &,
                                     BattleStageWorkCursor &);
} // namespace ttyd
