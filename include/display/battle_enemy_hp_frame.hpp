#pragma once
#include "battle_enemy_hp_raster.hpp"
#include "source_icon_number.hpp"
#include <functional>
namespace ttyd {
struct BattleEnemyHpFrameServices {
  // Composed children must be unbound: parent.number/bar and bar.request null.
  // All other actual providers/context lifetimes are retained by the caller.
  BattleEnemyHpDrawServices parent;
  BattleEnemyHpBarServices bar;
  SourceIconNumberServices number;
  BattleTexturePlaneServices raster;
};
// Stable retained whole source HP callback owner. Does not begin/publish/reset
// a GPU batch: actual sinks retain their ordered effects until caller completes
// the whole source queue/frame. No visibility/camera/GPU service is invented.
class BattleEnemyHpFrame {
public:
  BattleEnemyHpFrame() = default;
  ~BattleEnemyHpFrame();
  BattleEnemyHpFrame(const BattleEnemyHpFrame &) = delete;
  BattleEnemyHpFrame &operator=(const BattleEnemyHpFrame &) = delete;
  // Pure allocation predicate verifies the exact real battle/VM allocation and
  // retains service lifetimes. Actual camera/total-memory admission is
  // external; never mutate those owners inside synchronous source
  // reads/providers.
  bool bind(uint64_t allocation, BattleEnemyHpFrameServices,
            std::function<bool()> allocationCurrent);
  bool resume(uint64_t pass, const DisplayDispatchCursor &,
              const DisplayQueue &);
  bool complete(uint64_t pass) const;
  bool active() const;
  uint32_t pending() const;
  void
  retire(); // Native metadata only; safe deferred retirement in a provider.
private:
  struct State;
  State *state_ = nullptr;
};
} // namespace ttyd
