#pragma once
#include "battle_action_manager.hpp"
#include <memory>
namespace ttyd {
// One complete original manager invocation, including pad and deferred display.
// Providers retain partial child work; actual guest memory/services outlive
// holds.
// Noncopyable invocation owner pins Battle token and exact service identity.
// reset refuses reentry but does not roll back acknowledged guest/provider work.
// pending reports a function token, not an original gameplay result or frame ID.
class BattleActionManagerCursor {
public:
  BattleActionManagerCursor();
  ~BattleActionManagerCursor();
  BattleActionManagerCursor(const BattleActionManagerCursor &) = delete;
  BattleActionManagerCursor &
  operator=(const BattleActionManagerCursor &) = delete;
  bool active() const;
  bool reset();
  uint32_t pending() const;

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool
  resumePackedBattleActionCommandManager(uint32_t,
                                         const BattleActionManagerServices &,
                                         BattleActionManagerCursor &);
};
bool resumePackedBattleActionCommandManager(uint32_t battle,
                                            const BattleActionManagerServices &,
                                            BattleActionManagerCursor &);
} // namespace ttyd
