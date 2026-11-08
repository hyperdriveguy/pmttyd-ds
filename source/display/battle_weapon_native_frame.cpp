#include "battle_weapon_native_frame.hpp"
namespace ttyd {
struct BattleWeaponNativeFrame::State {
 BattleWeaponNativeFrameBinding binding;
 std::shared_ptr<const void> resources,batch;
 BattleWeaponDrawServices original,source;
 BattleWeaponNativeSubmission sink;
 BattleWeaponCameraBridge camera;
 BattleWeaponDrawCursor cursor;
 // Zero means unstarted; sourcePass can complete before submitted/finished pass.
 uint64_t pass=0,completed=0,sourcePass=0;
 const DisplayRecord* record=nullptr;
 bool invalid=false,dispatching=false,begun=false,drawn=false,submitted=false;
 bool current(){if(invalid)return false;if(!binding.allocationCurrent||!binding.allocationCurrent()||!binding.sourceResourceCurrent||!binding.sourceResourceCurrent()){invalid=true;return false;}return true;}
 // Short proof exists only inside the live source dispatch/camera epoch.
 // Long batchCurrent uses allocation/resource history independently of camera;
 // observed source or batch ownership loss poisons this binding.
 bool sourceCurrent(){
  if(!current()||!dispatching||!camera.available())return false;
  if(!original.current||!original.current(original.user)){invalid=true;return false;}
  return current()&&camera.available();
 }
 bool batchCurrent(){if(!current()||!batch)return false;if(!sink.current||!sink.current(sink.user,batch)){invalid=true;return false;}return current();}
 // Preserve original callback userdata while installing State as the guarded
 // facade. Optional results are exposed only after the short proof rechecks.
 void wrap(){
  source=original;source.user=this;
  source.current=[](void*p){return static_cast<State*>(p)->sourceCurrent();};
  source.request=[](void*p,const BattleWeaponDrawRequest&q)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(!s.sourceCurrent()||!s.original.request)return {};const auto r=s.original.request(s.original.user,q);return s.sourceCurrent()?r:std::nullopt;};
  source.truncate=[](void*p,float x)->std::optional<int32_t>{auto&s=*static_cast<State*>(p);if(!s.sourceCurrent()||!s.original.truncate)return {};const auto r=s.original.truncate(s.original.user,x);return s.sourceCurrent()?r:std::nullopt;};
  source.fma=[](void*p,float x,float y,float z)->std::optional<float>{auto&s=*static_cast<State*>(p);if(!s.sourceCurrent()||!s.original.fma)return {};const auto r=s.original.fma(s.original.user,x,y,z);return s.sourceCurrent()?r:std::nullopt;};
  source.identity=[](void*p)->std::optional<BattleActMatrix>{auto&s=*static_cast<State*>(p);if(!s.sourceCurrent()||!s.original.identity)return {};const auto r=s.original.identity(s.original.user);return s.sourceCurrent()?r:std::nullopt;};
 }
};
BattleWeaponNativeFrame::BattleWeaponNativeFrame()=default;
BattleWeaponNativeFrame::~BattleWeaponNativeFrame()=default;
bool BattleWeaponNativeFrame::bind(BattleWeaponNativeFrameBinding b,std::shared_ptr<const void> resources,
 const BattleWeaponDrawServices& source,BattleWeaponNativeSubmission sink){
 if(invoking_||retiring_||state_||!resources||!b.allocationOwner||!b.sourceResourceOwner||!b.allocation||!b.battle||!b.generation||
 !b.cameras||!b.allocationCurrent||!b.sourceResourceCurrent||!b.producerCurrent||!b.allocationCurrent()||!b.sourceResourceCurrent()||
 !source.current||!sink.begin||!sink.seal||!sink.submit||!sink.current)return false;
 auto s=std::make_unique<State>();s->binding=std::move(b);s->resources=std::move(resources);s->original=source;s->sink=sink;
 auto*p=s.get();
 if(!s->camera.bind(s->binding.cameras,s->binding.generation,s->binding.actors,s->binding.addresses,
 [p](BattleClassCameraEpoch e){return p->current()&&p->binding.producerCurrent(e);}))return false;
 s->wrap();state_=std::move(s);return true;
}
// Pin one monotonic Main pass and exact queue record across all retained stages.
// begin/draw/seal/submit/finish are distinct owners; provider false retains its
// own partial effects. No source readiness write substitutes for whole submit.
bool BattleWeaponNativeFrame::resume(uint64_t pass,const CameraMainCursor& main,const CameraDrawCursor& draw,
 const DisplayQueue& queue,uint32_t camera,uint32_t battle){
 if(invoking_||retiring_||!state_)return false;
 invoking_=true;struct Guard{BattleWeaponNativeFrame&f;~Guard(){f.invoking_=false;if(f.retiring_){f.state_.reset();f.retiring_=false;}}}guard{*this};
 auto&s=*state_;const auto&d=draw.display;
 if(!s.current()||!pass||camera!=8||battle!=s.binding.battle||!d.active||!d.invoking||d.camera!=8||
 d.callback!=0x80121ce8||d.userdata!=battle||(s.pass&&s.pass!=pass)||pass<=s.completed)return false;
 // The exact borrowed queue record is fixed, not just equal callback operands.
 // The camera bridge separately rejects queue reset/repopulation by reset epoch.
 if(s.record&&s.record!=queue.current())return false;
 const BattleClassCameraEpoch epoch{s.binding.generation,pass};
 if(!s.camera.begin(epoch,main,draw,queue,battle))return false;
 s.dispatching=true;
 // Reset stage latches only when opening a genuinely new producer pass.
 if(!s.pass){s.pass=pass;s.record=queue.current();s.begun=s.drawn=s.submitted=false;s.batch.reset();}
 if(!s.sourceCurrent())return false;
 // Latch acknowledged begin and source draw independently. Once drawn,
 // waiting for seal/submit cannot restart source setters, glyph RNG or W writes.
 if(!s.begun){const bool ok=s.sink.begin(s.sink.user,pass);if(ok)s.begun=true;if(!ok||!s.sourceCurrent())return false;}
 if(!s.drawn){if(!resumePackedBattleWeaponQueuedDraw(camera,battle,s.source,s.cursor)||!s.sourceCurrent())return false;s.drawn=true;s.sourcePass=pass;}
 // Empty seal is a hold after source completion; the source cursor stays drawn.
 if(!s.batch){auto batch=s.sink.seal(s.sink.user,pass);if(!s.sourceCurrent())return false;if(!batch)return false;s.batch=std::move(batch);}
 if(!s.batchCurrent()||!s.sourceCurrent())return false;
 // Latch real submit success BEFORE post-call guards. A later invalidation
 // cannot permit duplicate GPU submission; finish remains its own barrier.
 if(!s.submitted){const bool accepted=s.sink.submit(s.sink.user,pass,s.batch);if(accepted)s.submitted=true;if(!accepted||!s.batchCurrent()||!s.sourceCurrent())return false;}
 if(!s.camera.finish(epoch))return false;
 s.completed=pass;s.pass=0;s.record=nullptr;s.dispatching=false;return true;
}
const BattleWeaponCameraBridge* BattleWeaponNativeFrame::camera()const{
 return invoking_&&!retiring_&&state_&&state_->dispatching&&state_->current()&&state_->camera.available()?&state_->camera:nullptr;
}
// Source completion proof is available to seal while this camera lease is held;
// it does not itself expose a drawable batch or acknowledge GPU submission.
bool BattleWeaponNativeFrame::sourceComplete(uint64_t pass)const{return !retiring_&&state_&&state_->current()&&pass&&pass==state_->sourcePass&&pass==(state_->pass?state_->pass:state_->completed);}
// Expose only a completely submitted, camera-finished, still-current batch.
// Starting a later pass hides the previous batch; inspection never starts draw.
std::shared_ptr<const void> BattleWeaponNativeFrame::publication()const{
 return !invoking_&&!retiring_&&state_&&state_->completed&&!state_->pass&&state_->batchCurrent()?state_->batch:nullptr;
}
uint64_t BattleWeaponNativeFrame::publishedPass()const{return publication()?state_->completed:0;}
// pending() names only the retained source child frontier. It can be zero while
// seal/submit/finish still holds, so zero does not establish frame completion.
uint32_t BattleWeaponNativeFrame::pending()const{return state_?state_->cursor.pending():0;}
// Invalidate immediately during callbacks, defer destruction until resume's
// guard unwinds. Metadata retirement does not run guest End/free or cleanup.
void BattleWeaponNativeFrame::retire(){if(!state_)return;state_->invalid=true;if(invoking_)retiring_=true;else state_.reset();}
}
