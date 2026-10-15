#include "battle_enemy_hp_camera_bridge.hpp"
namespace ttyd {
bool BattleEnemyHpCameraBridge::bind(
    std::shared_ptr<const CameraManager> bank, uint64_t generation,
    BattleActorServices actors, BattleClassCameraAddressPolicy addresses,
    std::function<bool(BattleClassCameraEpoch)> current) {
  return engine_.bindCallback(std::move(bank), generation, actors,
                              std::move(addresses), std::move(current),
                              0x8011fde0);
}
bool BattleEnemyHpCameraBridge::begin(BattleClassCameraEpoch epoch,
                                      const CameraMainCursor &main,
                                      const CameraDrawCursor &draw,
                                      const DisplayQueue &queue,
                                      uint32_t battle) {
  return engine_.begin(epoch, main, draw, queue, battle);
}
bool BattleEnemyHpCameraBridge::finish(BattleClassCameraEpoch epoch) {
  return engine_.finish(epoch);
}
void BattleEnemyHpCameraBridge::retire() { engine_.retire(); }
bool BattleEnemyHpCameraBridge::available() const {
  return engine_.available();
}
bool BattleEnemyHpCameraBridge::owns(uint32_t a) const {
  return engine_.owns(a);
}
std::optional<uint32_t>
BattleEnemyHpCameraBridge::camera(std::optional<uint32_t> id) const {
  return engine_.camera(id);
}
std::optional<uint32_t> BattleEnemyHpCameraBridge::word(uint32_t a) const {
  return engine_.word(a);
}
std::optional<BattleScreenPoint>
BattleEnemyHpCameraBridge::project(const BattleScreenPoint &xyz) const {
  return engine_.screenPointCopied(xyz);
}
bool BattleEnemyHpCameraBridge::matrix(BattleAudienceMatrixRequest &q) const {
  return engine_.matrix(q);
}
} // namespace ttyd
