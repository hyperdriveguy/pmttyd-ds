#pragma once
#include "battle_action_class_table.hpp"
#include <memory>
namespace ttyd {
// Native composition must supply the actual immutable US message resource and
// guest-address mapping owner. lookup returns its mapped original text address,
// preserving control bytes; no generated label or unowned pointer is admitted.
struct BattleActionClassMessageBinding {
  std::shared_ptr<const void> allocation;
  void *user = nullptr;
  std::optional<uint32_t> (*lookup)(void *, uint32_t originalKey) = nullptr;
};
class BattleActionClassTableCursor {
public:
  BattleActionClassTableCursor();
  ~BattleActionClassTableCursor();
  BattleActionClassTableCursor(const BattleActionClassTableCursor &) = delete;
  BattleActionClassTableCursor &
  operator=(const BattleActionClassTableCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend std::optional<uint32_t>
  resumePackedBattleActionClassTable(uint32_t,
                                     const BattleActionClassServices &,
                                     BattleActionClassTableCursor &);
};
// Whole original80124dc8/5b8. Engaged return is the raw original ABI word:
// live published count normally, original pouch pointer when wrapped M==0.
// Providers retain partial work; all owners/identities remain fixed on hold.
std::optional<uint32_t>
resumePackedBattleActionClassTable(uint32_t battle,
                                   const BattleActionClassServices &,
                                   BattleActionClassTableCursor &);
} // namespace ttyd
