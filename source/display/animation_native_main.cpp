#include "animation_native_main.hpp"
#include "animation_startup_process.hpp"
#include "event_ram_access.hpp"
namespace ttyd {
namespace {constexpr uint32_t CameraSize=13*0x280;}
bool AnimationQuantizationState::write(uint32_t spr,uint32_t value){if(spr<912 || spr>919)return false;words_[spr-912]=value;++writes_;return true;}
std::optional<uint32_t> AnimationQuantizationState::read(unsigned i) const{return i<words_.size()?words_[i]:std::nullopt;}
struct AnimationNativeMain::Frame {
 AnimationNativeMain* owner=nullptr;uint32_t handle=0;uint64_t generation=0;
 std::shared_ptr<const BattlePoseResource> resource;std::shared_ptr<AnimationScratchPose> buffers;
 AnimationMainCursor main;AnimationMainGeometryCursor geometry;
 bool childRequested=false,childDone=false;uint32_t child=0;
};
AnimationNativeMain::AnimationNativeMain(AnimationStartupProcess& p,std::shared_ptr<const CameraManager> cameras,AnimationFacingAddressPolicy addresses,std::function<bool(uint64_t)> current):process_(p),aliases_(*p.animationMemory()),pool_(p.pool_),cameras_(std::move(cameras)),addresses_(std::move(addresses)),current_(std::move(current)){}
AnimationNativeMain::~AnimationNativeMain()=default;
std::unique_ptr<AnimationNativeMain> AnimationNativeMain::bind(AnimationStartupProcess& p,std::shared_ptr<const CameraManager> c,AnimationFacingAddressPolicy a,std::function<bool(uint64_t)> current){
 if(!p.ready() || !a.base || uint64_t(a.base)+CameraSize>0x100000000ull || !a.unmapped || !a.unmapped(a.base,CameraSize) || !current)return {};
 return std::unique_ptr<AnimationNativeMain>(new AnimationNativeMain(p,std::move(c),std::move(a),std::move(current)));
}
bool AnimationNativeMain::owns(uint32_t a) const{return a>=addresses_.base && uint64_t(a)<uint64_t(addresses_.base)+CameraSize;}
bool AnimationNativeMain::stable() const{
 if(!active_ || !current_(pass_) || (!resumeReady_ && !process_.ready()) || process_.animationMemory()!=&aliases_ || !addresses_.unmapped(addresses_.base,CameraSize))return false;
 for(const auto& frame:frames_){const auto* p=pool_->slot(frame->handle);if(!p || p->nativeGeneration!=frame->generation || p->resource!=frame->resource || p->sourceBuffers!=frame->buffers)return false;}
 return true;
}
bool AnimationNativeMain::available() const{return stable() && aliases_.aliases(*pool_,process_.vm().memory);}
std::optional<uint32_t> AnimationNativeMain::word(uint32_t a) const{uint32_t w;return available() && !owns(a) && process_.vm().memory.readWord(a,w)?std::optional<uint32_t>(w):std::nullopt;}
bool AnimationNativeMain::push(uint32_t handle){
 // Named native lifetime admission only. The original parent keeps raw linked
 // IDs/flags and does not acquire an invented source active/depth guard.
 if(handle>=256)return false;
 const auto* p=pool_->slot(handle);if(!p || !p->nativeGeneration || !p->resource || !p->sourceBuffers)return false;
 std::unique_ptr<Frame> frame;if(free_.empty()){frame=std::make_unique<Frame>();++allocations_;}else{frame=std::move(free_.back());free_.pop_back();}
 frame->owner=this;frame->handle=handle;frame->generation=p->nativeGeneration;frame->resource=p->resource;frame->buffers=p->sourceBuffers;frames_.push_back(std::move(frame));return true;
}
bool AnimationNativeMain::linked(Frame& f,uint32_t handle){
 if(!available())return false;
 if(f.childRequested)return f.child==handle && f.childDone;
 if(!push(handle))return false;
 f.childRequested=true;f.child=handle;return false;
}
bool AnimationNativeMain::camera(){
 if(!available() || !cameras_)return false;
 if(!facing_)facing_=AnimationFacingBinding::bind(pool_,process_.vm().memory,aliases_,cameras_,addresses_,[this](){return stable();});
 return facing_ && facing_->available();
}
AnimationFacingServices AnimationNativeMain::facingServices(){
 AnimationFacingServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& n=*static_cast<AnimationNativeMain*>(u);if(!n.owns(a))return n.word(a);if(!n.camera())return {};const auto b=n.facing_->services();return b.word(b.user,a);};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& n=*static_cast<AnimationNativeMain*>(u);return n.available() && !n.owns(a) && n.process_.vm().memory.writeWord(a,v);};
 s.call=[](void* u,AnimationFacingCall& q){auto& n=*static_cast<AnimationNativeMain*>(u);if(!n.camera())return false;const auto b=n.facing_->services();return b.call(b.user,q);};
 s.matrix=[](void* u,BattleAudienceMatrixRequest& q){auto& n=*static_cast<AnimationNativeMain*>(u);if(!n.camera())return false;const auto b=n.facing_->services();return b.matrix(b.user,q);};return s;
}
AnimationMainServices AnimationNativeMain::services(Frame& f){
 AnimationMainServices s;s.user=&f;s.facing=facingServices();
 s.word=[](void* u,uint32_t a){return static_cast<Frame*>(u)->owner->word(a);};
 s.gqr=[](void* u,uint32_t spr,uint32_t value){auto& n=*static_cast<Frame*>(u)->owner;return n.available() && n.process_.quantization().write(spr,value);};
 s.linked=[](void* u,uint32_t handle){auto& f=*static_cast<Frame*>(u);return f.owner->linked(f,handle);};
 s.evaluate=[](void* u,const AnimationMainEvaluation& input){auto& f=*static_cast<Frame*>(u);auto& n=*f.owner;return n.available() && advanceAnimationMainGeometry(input,n.time_->services(),n.replay_->services(),f.geometry);};return s;
}
bool AnimationNativeMain::advance(uint32_t handle,uint64_t pass,unsigned visits){
 // Closed synchronous slice: internal source/numeric/camera services only.
 // Pure current/reservation predicates and stable semantic registries are
 // required. Only backing proofs cache; every source value stays live.
 return process_.vm().memory.withRawRamAccess([&](auto&){
 if(!process_.ready())return false;
 // Tangent/scratch/alias owner identities cannot retire within this closed
 // borrow. Header bytes and pose/pass state are still checked at every service.
 resumeReady_=true;
 struct ResetReady {bool& value;~ResetReady(){value=false;}} resetReady{resumeReady_};
 if(!active_){
  if(!visits || !current_(pass) || !process_.ready() || !addresses_.unmapped(addresses_.base,CameraSize))return false;
  handle_=handle;pass_=pass;if(!push(handle))return false;active_=true;
 }else if(handle_!=handle || pass_!=pass)return false;
 if(!available())return false;
 if(!time_)time_=AnimationMainTimeBinding::bind(pool_,process_.vm().memory,aliases_,[this](){return stable();});
 if(!replay_)replay_=AnimationMainReplayBinding::bind(pool_,process_.vm().memory,aliases_,[this](){return stable();},[this](){return process_.quantization().read(4);},true);
 if(!time_ || !replay_)return false;
 for(unsigned i=0;i<visits;++i){
  if(!available())return false;
  auto& f=*frames_.back();const bool done=advanceAnimationMain(f.handle,services(f),f.main);
  if(!done)continue;
  auto retired=std::move(frames_.back());frames_.pop_back();*retired={};free_.push_back(std::move(retired));if(frames_.empty()){active_=false;return true;}
  frames_.back()->childDone=true;
 }
 return false;
 });
}
uint32_t AnimationNativeMain::phase() const{return frames_.empty()?0:frames_.back()->main.phase;}
AnimationNativeMainProgress AnimationNativeMain::progress() const{
 if(frames_.empty())return {};
 const auto& f=*frames_.back();return {frames_.size(),f.main.phase,f.geometry.phase,f.geometry.cache.phase,f.geometry.cache.replay.phase,f.geometry.interpolation.phase};
}

bool AnimationStartupProcess::configureMain(std::shared_ptr<const CameraManager> cameras,AnimationFacingAddressPolicy addresses,std::function<bool(uint64_t)> current){
 if(producerPending() || entryResource_)return false;
 auto owner=AnimationNativeMain::bind(*this,std::move(cameras),std::move(addresses),current);if(!owner)return false;main_=std::move(owner);presentation_.reset(new AnimationMeshPublication(*this,std::move(current)));++mainConfiguration_;return true;
}
bool AnimationStartupProcess::main(uint32_t handle,uint64_t pass,unsigned visits){return !entryResource_ && !selectionPending() && !presentationPending() && main_ && main_->advance(handle,pass,visits);}

}
