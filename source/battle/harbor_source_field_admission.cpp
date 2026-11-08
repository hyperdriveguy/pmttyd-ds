#include "harbor_source_field_admission.hpp"
#include "battle_sequence_binding.hpp"
#include "battle_sequence_nested_pool.hpp"
#include "game.hpp"
#include "battle_event_callbacks.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
namespace {
struct Guard{bool&v;explicit Guard(bool&value):v(value){v=true;}~Guard(){v=false;}};
struct Proof {
 EventNativeRuntime*vm=nullptr;
 NativeFacadeRegistration::Proof vmLife;
 std::shared_ptr<const BattleSequenceNestedPoolReceipt>receipt;
 HarborBattleCameraReceipt camera;
 std::shared_ptr<CameraManager>cameras;
 std::shared_ptr<FieldNpcRegistryOwner>npcs;
 std::shared_ptr<const HarborSourceFieldServices>services;
 std::shared_ptr<HarborSourceMapPool>pool;
 mutable bool stale=false,checking=false;
 bool sourceCurrent()const{
  // Intrinsic native registration precedes all borrowed VM references.
  return vmLife.current()&&receipt&&receipt->accepts(*vm)&&npcs&&npcs->aliases(vm->memory)&&
   camera.lifetime&&camera.current&&cameras&&camera.current(*cameras);
 }
 bool current()const{
  if(stale||checking)return false;
  Guard guard(checking);
  if(!sourceCurrent()){stale=true;return false;}
  if(!services->current()||!sourceCurrent()||(pool&&!pool->current())){stale=true;return false;}
  return true;
 }
};
}
struct HarborSourceFieldAdmission::State {
 std::shared_ptr<Proof>proof;
 std::unique_ptr<HarborSourceStartup>startup;
 std::optional<uint32_t>information;
};
HarborSourceFieldAdmission::HarborSourceFieldAdmission():state_(std::make_unique<State>()){}
HarborSourceFieldAdmission::~HarborSourceFieldAdmission()=default;
std::shared_ptr<HarborSourceFieldAdmission> HarborSourceFieldAdmission::prepare(BattleSequenceBinding&binding){
 if(!binding.facadeCurrent()||!binding.harborServices_||!binding.harborServices_->lifetime||!binding.harborServices_->current)return {};
 auto out=std::shared_ptr<HarborSourceFieldAdmission>(new HarborSourceFieldAdmission);
 auto p=std::make_shared<Proof>();p->vm=&binding.vm_;p->vmLife=binding.vmLife_;p->services=binding.harborServices_;
 p->receipt=binding.nestedPoolReceipt();p->camera=binding.battleCameraReceipt();p->cameras=binding.game_.cameraManager;p->npcs=binding.npcs_;
 out->state_->proof=std::move(p);return out->current()?out:nullptr;
}
bool HarborSourceFieldAdmission::current()const{
 return state_->proof&&state_->proof->current()&&(!state_->startup||state_->startup->current());
}
bool HarborSourceFieldAdmission::resumeCold(uint32_t information){
 if(invoking_||!current())return false;
 auto&s=*state_;if(information!=s.proof->npcs->fieldWorkAddress()+0x20)return false;
 if(s.information&&*s.information!=information)return false;
 Guard guard(invoking_);s.information=information;
 if(!s.proof->pool){
  auto adopted=HarborSourceMapPool::adopt(*s.proof->vm,s.proof->receipt);if(!adopted)return false;
  s.proof->pool=std::shared_ptr<HarborSourceMapPool>(std::move(adopted));
 }
 if(!s.startup){
  HarborSourceStartupBinding b;b.vm=s.proof->vm;b.pool=s.proof->pool.get();b.lifetime=s.proof;
  b.current=[weak=std::weak_ptr<Proof>(s.proof)]{const auto p=weak.lock();return p&&p->current();};
  const auto&services=*s.proof->services;b.menu=services.menu;b.menu.memory=nativeBattleActorServices(*s.proof->vm);b.animation=services.animation;b.poseResources=services.poseResources;
  b.coldChild=services.coldChild;b.unitChild=services.unitChild;
  s.startup=HarborSourceStartup::create(std::move(b));if(!s.startup)return false;
 }
 return s.startup->resumeCold(information);
}
bool HarborSourceFieldAdmission::resumeUnitSetup(uint32_t battle){
 if(invoking_||!current()||!state_->startup||!state_->startup->coldComplete())return false;
 Guard guard(invoking_);return state_->startup->resumeUnitSetup(battle);
}
bool HarborSourceFieldAdmission::coldComplete()const{return current()&&state_->startup&&state_->startup->coldComplete();}
bool HarborSourceFieldAdmission::unitSetupComplete()const{return current()&&state_->startup&&state_->startup->unitSetupComplete();}
uint32_t HarborSourceFieldAdmission::battleAddress()const{return current()&&state_->startup?state_->startup->battleAddress():0;}
uint32_t HarborSourceFieldAdmission::pending()const{return current()&&state_->startup?state_->startup->pending():0;}
HarborBattleCameraReceipt HarborSourceFieldAdmission::cameraReceipt()const{return current()?state_->proof->camera:HarborBattleCameraReceipt{};}
std::shared_ptr<HarborSourceMapPool> HarborSourceFieldAdmission::pool()const{return current()?state_->proof->pool:nullptr;}
}
