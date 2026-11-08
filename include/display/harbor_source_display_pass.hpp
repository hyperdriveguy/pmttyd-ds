#pragma once
#include "battle_class_camera_bridge.hpp"
#include "battle_display_footer.hpp"
#include "battle_enemy_hp_draw.hpp"
#include <functional>
#include <memory>
namespace ttyd {
// A receipt supplied ONLY by completed source8000e95c(4,2) in the actual
// field-battle initializer. Neither mode==2 nor source camera-work Init is one.
struct HarborBattleCameraReceipt {
  std::shared_ptr<const void> lifetime;
  std::function<bool(const CameraManager &)> current;
};
struct HarborSourceDisplayInvocation {
  uint64_t generation, pass;
  uint32_t camera, callback, battle;
  const CameraMainCursor &main;
  const CameraDrawCursor &draw;
  const DisplayQueue &queue;
};
struct HarborSourceDisplayBinding {
  uint32_t battle = 0;
  uint64_t generation = 0;
  std::weak_ptr<const void> runtime;
  std::shared_ptr<CameraManager> cameras;
  HarborBattleCameraReceipt cameraReceipt;
  // Pure allocation/source-global/VM proof. Called only under locked runtime.
  std::function<bool()> current;
  // Exactly original CommandDisplay registration, not whole BattleDrawMain.
  std::function<bool(DisplayQueue &, BattleFooterCursor &)> registerCommands;
  CameraMainServices main;
  CameraDrawServices
      draw; // display.callback must be null; owner dispatches it.
  BattleEnemyHpDrawServices hp; // camera/monosiri must be null; owner composes.
  BattleMonosiriServices monosiri;
  // Other actual registered callbacks. Retain each child's own partial work.
  // Borrowed invocation object itself must not escape this call.
  std::function<bool(const HarborSourceDisplayInvocation &)> callback;
};
// Retains the ACTUAL shared camera Main/Draw and queue. Does not schedule
// source Main, acknowledge whole BattleDrawMain, import GPU assets or sample
// controls.
class HarborSourceDisplayPass {
public:
  static std::unique_ptr<HarborSourceDisplayPass>
      create(HarborSourceDisplayBinding);
  ~HarborSourceDisplayPass();
  HarborSourceDisplayPass(const HarborSourceDisplayPass &) = delete;
  HarborSourceDisplayPass &operator=(const HarborSourceDisplayPass &) = delete;
  // A strictly newer native pass begins collection; no queue reset on a hold.
  // One-time late admission at the actual completed source child. Never
  // replaces an already bound receipt or changes source camera state.
  bool admitCameraReceipt(HarborBattleCameraReceipt);
  bool begin(uint64_t pass);
  bool registerCommands();
  // Other ORIGINAL registration children may append only while collecting.
  // The borrowed queue cannot be retained/mutated after sealRegistrations().
  DisplayQueue *registrationQueue();
  bool sealRegistrations();
  bool resume(); // completed successful CameraMain, then CameraDraw, reset LAST
  // Metadata queries never establish a successful source-camera receipt.
  std::shared_ptr<CameraManager> cameraManager() const;
  uint32_t battleAddress() const;
  uint64_t generation() const;
  bool runtimeIs(const std::shared_ptr<const void>&) const;
  bool acceptsInvocation(const HarborSourceDisplayInvocation&) const;
  bool current() const;
  bool complete(uint64_t pass) const;
  bool cameraMainCurrent(BattleClassCameraEpoch) const;
  uint32_t pending() const;
  uint64_t pass() const;
  void retire(); // metadata only, safe during a provider; never source End
private:
  HarborSourceDisplayPass();
  struct State;
  std::unique_ptr<State> state_;
  mutable bool invoking_ = false, checking_ = false;
};
} // namespace ttyd
