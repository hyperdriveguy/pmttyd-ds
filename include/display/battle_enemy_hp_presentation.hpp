#pragma once
#include "battle_actor.hpp"
#include "battle_enemy_hp_draw.hpp"
#include "display_queue.hpp"
#include <memory>
#include <vector>
namespace ttyd {
struct BattleEnemyHpRasterRequest {
  uint32_t function = 0;
  std::vector<uint32_t> integers;
  std::vector<float> reals;
  std::optional<CameraViewMatrix> matrix;
};
struct BattleEnemyHpBarServices {
  BattleActorServices actors;
  void *user = nullptr;
  std::optional<uint32_t> (*camera)(void *, uint32_t) = nullptr;
  // Real retained InitRasta, LoadPosMtxImm, btlDispTexPlane and QuadRasta
  // owners. Acknowledgement is not a no-op or a claim that native pixels were
  // drawn.
  bool (*request)(void *, const BattleEnemyHpRasterRequest &) = nullptr;
};
class BattleEnemyHpBarCursor {
public:
  BattleEnemyHpBarCursor();
  ~BattleEnemyHpBarCursor();
  BattleEnemyHpBarCursor(const BattleEnemyHpBarCursor &) = delete;
  BattleEnemyHpBarCursor &operator=(const BattleEnemyHpBarCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumeBattleEnemyHpBar(std::array<float, 3> &, uint32_t,
                                     const uint32_t &,
                                     const BattleEnemyHpBarServices &,
                                     BattleEnemyHpBarCursor &);
};
// Whole BattleDrawEnemyHPBar8011fc3c/1a4. Position/RGBA are borrowed original
// locals, not native pointers cast from guest addresses. Unit ratios stay live.
bool resumeBattleEnemyHpBar(std::array<float, 3> &position, uint32_t unit,
                            const uint32_t &rgba,
                            const BattleEnemyHpBarServices &,
                            BattleEnemyHpBarCursor &);
struct QueuedBattleEnemyHpServices {
  BattleEnemyHpDrawServices parent;
  BattleEnemyHpBarServices bar;
};
class QueuedBattleEnemyHpCursor {
public:
  QueuedBattleEnemyHpCursor();
  ~QueuedBattleEnemyHpCursor();
  QueuedBattleEnemyHpCursor(const QueuedBattleEnemyHpCursor &) = delete;
  QueuedBattleEnemyHpCursor &
  operator=(const QueuedBattleEnemyHpCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumeQueuedBattleEnemyHp(const DisplayDispatchCursor &,
                                        const QueuedBattleEnemyHpServices &,
                                        QueuedBattleEnemyHpCursor &);
};
// Actual invoking source queue record is required. Reuses complete parent
// callback/checker providers, replaces its unbound bar with the retained body.
// Original number mode1, status/Tattle and projection owners remain required.
bool resumeQueuedBattleEnemyHp(const DisplayDispatchCursor &,
                               const QueuedBattleEnemyHpServices &,
                               QueuedBattleEnemyHpCursor &);
} // namespace ttyd
