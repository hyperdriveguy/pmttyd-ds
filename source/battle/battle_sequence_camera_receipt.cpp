#include "battle_sequence_camera_receipt.hpp"
#include "battle_sequence_binding.hpp"
#include "harbor_source_display_pass.hpp"
#include "game.hpp"
namespace ttyd {
namespace {constexpr uint32_t BattleSceneCamera=4;constexpr int16_t BattleSceneMode=2;}
struct BattleSequenceCameraReceipt::State {
 BattleSequenceBinding*binding=nullptr;
 NativeFacadeRegistration::Proof bindingLife;
 std::weak_ptr<const uint8_t>epoch;
 std::shared_ptr<CameraManager>cameras;
 bool stale=false,checking=false;
 bool current(const CameraManager&expected){
  if(stale||checking)return false;
  if(!bindingLife.current()||epoch.expired()){stale=true;return false;}
  // Weak facade/epoch checks precede the first borrowed binding/Game/VM access.
  checking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{checking};
  if(!binding->facadeCurrent()||binding->game_.cameraManager!=cameras){stale=true;return false;}
  const auto*camera=cameras->camera(BattleSceneCamera);
  if(!camera||camera->mode!=BattleSceneMode){stale=true;return false;}
  return cameras.get()==&expected;
 }
};
BattleSequenceCameraReceipt::BattleSequenceCameraReceipt():state_(std::make_shared<State>()){}
BattleSequenceCameraReceipt::~BattleSequenceCameraReceipt()=default;
std::shared_ptr<BattleSequenceCameraReceipt> BattleSequenceCameraReceipt::capture(BattleSequenceBinding&binding){
 if(!binding.facadeCurrent()||!binding.game_.cameraManager)return {};
 const auto*camera=binding.game_.cameraManager->camera(BattleSceneCamera);
 if(!camera||camera->mode!=BattleSceneMode)return {};
 auto receipt=std::shared_ptr<BattleSequenceCameraReceipt>(new BattleSequenceCameraReceipt);
 auto&s=*receipt->state_;s.binding=&binding;s.bindingLife=binding.facade_.proof();s.cameras=binding.game_.cameraManager;
 // A fresh identity is issued only at the completed actual source mode setter.
 // The binding alone owns the current epoch; old publications cannot pin it.
 binding.cameraEpoch_=std::make_shared<const uint8_t>(0);s.epoch=binding.cameraEpoch_;
 return receipt;
}
HarborBattleCameraReceipt BattleSequenceCameraReceipt::publication()const{
 if(!state_->current(*state_->cameras))return {};
 HarborBattleCameraReceipt out;out.lifetime=state_;
 out.current=[weak=std::weak_ptr<State>(state_)](const CameraManager&camera){const auto s=weak.lock();return s&&s->current(camera);};
 return out;
}
}
