#pragma once
#include "battle_enemy_hp_presentation.hpp"
#include "battle_texture_plane.hpp"
namespace ttyd {
class BattleEnemyHpRasterCursor {
public:
  BattleEnemyHpRasterCursor();
  ~BattleEnemyHpRasterCursor();
  BattleEnemyHpRasterCursor(const BattleEnemyHpRasterCursor&)=delete;
  BattleEnemyHpRasterCursor& operator=(const BattleEnemyHpRasterCursor&)=delete;
  bool active() const;
  uint32_t pending() const;
  void reset();
private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumeBattleEnemyHpRaster(const BattleEnemyHpRasterRequest&,
      const BattleTexturePlaneServices&,BattleEnemyHpRasterCursor&);
};
// Original linked source calls emitted by BattleDrawEnemyHPBar: InitRasta
// 80103b48 (+whole InitSub80103c3c), QuadRasta80103918 (+Point801039ec),
// TexPlane801032c8 and LoadPosMtxImm802ba2fc. The borrowed request must remain
// alive and at the same address through suspension. No GPU acknowledgement
// or source camera, matrix, bank, texture, material or FIFO service is invented.
bool resumeBattleEnemyHpRaster(const BattleEnemyHpRasterRequest&,
    const BattleTexturePlaneServices&,BattleEnemyHpRasterCursor&);
}
