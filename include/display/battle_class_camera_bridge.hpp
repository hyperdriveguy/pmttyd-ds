#pragma once
#include "battle_act_ring_position.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_screen_point.hpp"
#include "camera_manager.hpp"
#include <functional>
#include <memory>
namespace ttyd {
// Native ownership/publication identity, never an original guest pointer.
struct BattleClassCameraEpoch {uint64_t generation=0,pass=0;};
struct BattleClassCameraAddressPolicy {
  uint32_t base=0;
  // Pure, non-reentrant predicate. Must prove the WHOLE reserved range has no actual guest storage, including
  // shared file-cache mappings. Rechecked at every provider boundary.
  std::function<bool(uint32_t base,uint32_t bytes)> unmapped;
};
// Stable borrowed service context. The shared bank outlives the binding; actual
// Main/Draw cursors and queue outlive each begin..finish interval. Guest memory
// service owners outlive the binding (the epoch guard should retain that owner).
// No copies or
// moves: callbacks may retain this object's address.
class BattleClassCameraBridge {
public:
  BattleClassCameraBridge()=default;
  BattleClassCameraBridge(const BattleClassCameraBridge&)=delete;
  BattleClassCameraBridge& operator=(const BattleClassCameraBridge&)=delete;
  // Pure, non-reentrant current predicate must validate allocation generation AND this exact successful
  // CameraManager::main producer pass. A default inactive cursor is not proof.
  bool bind(std::shared_ptr<const CameraManager>,uint64_t generation,
            BattleActorServices,BattleClassCameraAddressPolicy,
            std::function<bool(BattleClassCameraEpoch)> current);
  bool begin(BattleClassCameraEpoch,const CameraMainCursor&,
             const CameraDrawCursor&,const DisplayQueue&,uint32_t passedBattle);
  // Call within the completed source callback, before Draw advances cameras.
  bool finish(BattleClassCameraEpoch);
  void retire(); // Metadata only; never invokes source End or modifies bank/VM.
  bool available() const;
  // Owns the complete native address reservation even while unavailable.
  // A caller MUST NOT fall through to actor memory inside this range.
  bool owns(uint32_t address) const;
  std::optional<uint32_t> camera(std::optional<uint32_t> index={}) const;
  // Source +114 yaw, +11c affine view, +15c projection. Only slots4/8 are
  // admitted. Tokens are interpreted here, never dereferenced as host pointers.
  std::optional<uint32_t> word(uint32_t address) const;
  std::optional<BattleScreenPoint> screenPoint(uint32_t guestPosition) const;
  std::optional<std::array<float,3>> ringCenter() const;
  std::optional<std::array<float,2>> ringOffset(float angle) const;
  bool matrix(BattleAudienceMatrixRequest&) const;
private:
  friend class BattleEnemyHpCameraBridge;
  friend class BattleWeaponCameraBridge;
  bool bindCallback(std::shared_ptr<const CameraManager>,uint64_t,
      BattleActorServices,BattleClassCameraAddressPolicy,
      std::function<bool(BattleClassCameraEpoch)>,uint32_t callback);
  BattleScreenPointServices screenServices() const;
  std::optional<BattleScreenPoint> screenPointCopied(const BattleScreenPoint&) const;
  uint32_t callback_=0x8012210c;
  uint64_t queueEpoch_=0;
  std::shared_ptr<const CameraManager> bank_;
  BattleActorServices actors_;
  BattleClassCameraAddressPolicy addresses_;
  std::function<bool(BattleClassCameraEpoch)> current_;
  uint64_t generation_=0,lastPass_=0;
  BattleClassCameraEpoch epoch_;
  uint32_t battle_=0;
  const CameraMainCursor* main_=nullptr;
  const CameraDrawCursor* draw_=nullptr;
  const DisplayQueue* queue_=nullptr;
  std::array<CameraDepthSnapshot,2> snapshots_{};
  std::array<float,2> yaw_{};
  mutable bool invalid_=false;
  bool held_=false;
  bool dispatchCurrent() const;
  bool transform(uint32_t,const BattleScreenPoint&,BattleScreenPoint&,bool project) const;
};
}
