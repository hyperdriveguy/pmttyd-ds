#pragma once
#include "battle_class_camera_bridge.hpp"
namespace ttyd {
// HP-only entry to the shared audited native camera engine. No callback IDs
// are rewritten: admission requires real8011fde0 during camera8 Draw dispatch.
class BattleEnemyHpCameraBridge {
public:
  bool bind(std::shared_ptr<const CameraManager>, uint64_t generation,
            BattleActorServices, BattleClassCameraAddressPolicy,
            std::function<bool(BattleClassCameraEpoch)> current);
  bool begin(BattleClassCameraEpoch, const CameraMainCursor &,
             const CameraDrawCursor &, const DisplayQueue &, uint32_t battle);
  bool finish(BattleClassCameraEpoch);
  void retire();
  bool available() const;
  bool owns(uint32_t) const;
  std::optional<uint32_t> camera(std::optional<uint32_t> index = {}) const;
  std::optional<uint32_t> word(uint32_t) const;
  std::optional<BattleScreenPoint>
  project(const BattleScreenPoint &borrowedXYZ) const;
  bool matrix(BattleAudienceMatrixRequest &) const;

private:
  BattleClassCameraBridge engine_;
};
} // namespace ttyd
