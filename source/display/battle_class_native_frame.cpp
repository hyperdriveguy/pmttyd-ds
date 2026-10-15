#include "battle_class_native_frame.hpp"
#include "battle_class_footer_provider.hpp"
namespace ttyd {
struct BattleClassNativeFrame::State {
  BattleClassNativeFrameBinding binding;
  std::shared_ptr<const void> resources;
  BattleClassBackend backend;
  BattleClassCameraBridge camera;
  BattleClassFramePipeline frame;
  mutable bool invalid=false;bool dispatching=false;
  uint64_t pass=0,completed=0;
  BattleClassNativeSubmission submission;bool submitted=false;
  bool current() const {
    if(invalid)return false;
    if(!binding.allocationCurrent || !binding.allocationCurrent()){invalid=true;return false;}
    return true;
  }
};
BattleClassNativeFrame::BattleClassNativeFrame()=default;
BattleClassNativeFrame::~BattleClassNativeFrame()=default;
bool BattleClassNativeFrame::bind(BattleClassNativeFrameBinding b,
    std::shared_ptr<const void> resources,const BattleClassDisplayServices& display,
    const BattleClassRasterServices& raster,const BattleClassBackend& backend) {
  if(invoking_ || state_ || !resources || !b.allocationOwner || !b.allocation ||
      !b.generation || !b.cameras || !b.allocationCurrent || !b.producerCurrent ||
      !b.allocationCurrent())return false;
  auto next=std::make_unique<State>();
  next->binding=std::move(b);next->resources=std::move(resources);next->backend=backend;
  auto* p=next.get();
  if(!p->camera.bind(p->binding.cameras,p->binding.generation,p->binding.actors,
      p->binding.addresses,[p](BattleClassCameraEpoch e){
        return p->current() && p->binding.producerCurrent(e);
      }))return false;
  if(!p->frame.bind(p->binding.allocationOwner,p->binding.allocation,display,raster,
      p->backend,[p]{return p->current() && (!p->dispatching || p->camera.available());}))return false;
  state_=std::move(next);return true;
}
bool BattleClassNativeFrame::resume(uint64_t pass,const CameraMainCursor& main,
    const CameraDrawCursor& draw,const DisplayQueue& queue,uint32_t camera,uint32_t battle,
    BattleClassNativeSubmission submission) {
  if(invoking_ || retiring_ || !state_)return false;
  invoking_=true;
  struct Guard {BattleClassNativeFrame& self;~Guard(){self.invoking_=false;
    if(self.retiring_){self.state_.reset();self.retiring_=false;}}} guard{*this};
  auto& s=*state_;
  const auto& call=draw.display;
  // Actual CameraDraw/DisplayQueue membership, reset epoch, source-main
  // completion and producer predicates are further checked by camera.begin.
  if(!s.current() || !pass || camera!=8 || !call.active || !call.invoking ||
      call.camera!=camera || call.callback!=0x8012210c || call.userdata!=battle ||
      (s.pass && s.pass!=pass))return false;
  if(s.pass && (s.submission.user!=submission.user || s.submission.submit!=submission.submit))return false;
  const BattleClassCameraEpoch epoch{s.binding.generation,pass};
  if(!s.camera.begin(epoch,main,draw,queue,battle))return false;
  if(!s.pass){s.submission=submission;s.submitted=false;}
  s.pass=pass;s.dispatching=true;
  const BattleClassPublicationKey key{s.binding.allocationOwner,s.binding.allocation,pass};
  if(!s.frame.resume(key,camera,battle) || !s.current() || !s.camera.available())return false;
  if(s.submission.submit && !s.submitted){
    const auto batch=s.frame.publication().batch();
    if(!batch)return false;
    const bool accepted=s.submission.submit(s.submission.user,*batch);
    if(accepted)s.submitted=true;
    if(!accepted || !s.current() || !s.camera.available())return false;
    if(!s.frame.admit(key))return false;
  }
  if(!s.camera.finish(epoch))return false;
  s.completed=pass;s.pass=0;s.dispatching=false;return true;
}
bool BattleClassNativeFrame::appendFooter(const BattleClassFooterAtlasQuad& packet) {
  if(!invoking_ || retiring_ || !state_ || !state_->dispatching ||
      !state_->current() || !state_->camera.available() || !state_->frame.active())return false;
  return state_->backend.appendFooter(packet);
}
std::optional<uint32_t> BattleClassNativeFrame::footerRasterRequest(
    const BattleClassRasterRequest& request,BattleClassFooterProvider& provider) {
  if(!invoking_ || retiring_ || !state_ || !state_->dispatching ||
      !state_->current() || !state_->camera.available() || !state_->frame.active())return {};
  const auto result=provider.rasterRequest(request,state_->backend);
  return state_->current() && state_->camera.available()?result:std::nullopt;
}
const BattleClassCameraBridge* BattleClassNativeFrame::camera() const {
  return invoking_ && !retiring_ && state_ && state_->current() && state_->camera.available()?
      &state_->camera:nullptr;
}
const BattleClassPublication& BattleClassNativeFrame::publication() const {
  return state_ && !retiring_ && state_->current() && state_->completed &&
      state_->frame.publication().key().sequence==state_->completed?state_->frame.publication():unavailable_;
}
bool BattleClassNativeFrame::admit(const BattleClassPublicationKey& key) {
  return !invoking_ && !retiring_ && state_ && state_->current() && key.sequence==state_->completed && state_->frame.admit(key);
}
std::optional<BattleClassPublicationKey> BattleClassNativeFrame::consumeAdmission() {
  return !invoking_ && !retiring_ && state_ && state_->current() && state_->completed &&
      state_->frame.publication().key().sequence==state_->completed?state_->frame.consumeAdmission():std::nullopt;
}
void BattleClassNativeFrame::retire() {
  if(!state_)return;
  state_->invalid=true;state_->frame.retire();
  // camera callbacks can be in flight, so destroy the context after return.
  if(invoking_)retiring_=true;
  else state_.reset();
}
}
