#include "animation_native_selection.hpp"
#include "animation_startup_process.hpp"
#include "field_npc_clock.hpp"
namespace ttyd {
AnimationNativeSelection::AnimationNativeSelection(AnimationStartupProcess& p):process_(p),pool_(p.pool_),aliases_(*p.animationMemory()){}
// Exact process/VM-alias,slot generation,resource and scratch-view identities
// must remain current. A ready replacement or recycled numeric handle cannot
// retarget this held invocation; this adapter adds no separate stale latch.
bool AnimationNativeSelection::available() const{
 const auto* p=pool_->slot(handle_);
 return active_ && process_.ready() && process_.animationMemory()==&aliases_ && aliases_.aliases(*pool_,process_.vm().memory) && p && p->nativeGeneration==generation_ && p->resource==resource_ && p->sourceBuffers==buffers_;
}
// Bytewise native strcmp result is unsigned-byte difference. Retain left
// byte while right is missing.256 is per-call work budget,not string length;
// original SDK optimized load order/overread behavior is outside this adapter.
bool AnimationNativeSelection::compare(uint32_t left,uint32_t right,int32_t& result){
 if(!available())return false;
 if(!comparing_){comparing_=true;left_=left;right_=right;offset_=0;leftLoaded_=false;}
 else if(left!=left_ || right!=right_)return false;
 auto& memory=process_.vm().memory;
 // Bound work only, not accepted string length. Preserve captured left byte
 // if right is unavailable; no acknowledged prefix is reread on a retry.
 for(unsigned i=0;i<256;++i){
  if(!available())return false;
  if(!leftLoaded_){if(!memory.readByte(left_+offset_,leftByte_))return false;leftLoaded_=true;}
  uint8_t b;if(!memory.readByte(right_+offset_,b))return false;
  if(leftByte_!=b || !leftByte_){result=int32_t(leftByte_)-int32_t(b);comparing_=false;leftLoaded_=false;return true;}
  ++offset_;leftLoaded_=false;
 }
 return false;
}
AnimationSelectionServices AnimationNativeSelection::services(){
 AnimationSelectionServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& n=*static_cast<AnimationNativeSelection*>(u);uint32_t v;return n.available() && n.process_.vm().memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& n=*static_cast<AnimationNativeSelection*>(u);return n.available() && n.process_.vm().memory.writeWord(a,v);};
 s.compare=[](void* u,uint32_t a,uint32_t b,int32_t& result){return static_cast<AnimationNativeSelection*>(u)->compare(a,b,result);};
 s.divide=[](void* u,uint64_t n,uint32_t d,uint64_t& q){return static_cast<AnimationNativeSelection*>(u)->available() && nativeFieldNpcClockDivide(nullptr,n,d,q);};return s;
}
// Native entry requires handle<256 and a source-backed generated slot. Pin
// all owners until portable setter completes; only then release held resource/
// buffer references. False can mean budget,missing byte or lost current context.
bool AnimationNativeSelection::advance(uint32_t handle,uint32_t selector,uint32_t force,unsigned visits){
 if(!visits)return false;
 if(!active_){
  if(!process_.ready() || handle>=256)return false;
  const auto* p=pool_->slot(handle);if(!p || !p->nativeGeneration || !p->resource || !p->sourceBuffers)return false;
  handle_=handle;selector_=selector;force_=force;generation_=p->nativeGeneration;resource_=p->resource;buffers_=p->sourceBuffers;active_=true;
 }
 if(handle!=handle_ || selector!=selector_ || force!=force_ || !available() || !selectAnimation(handle,selector,force,services(),cursor_,visits))return false;
 active_=false;resource_.reset();buffers_.reset();return true;
}
// Producer exclusion:Entry/Main and unrelated/later presentation phases hold.
// Same-handle presentation phase0 may run its own setup setter; no independent
// pass token distinguishes competing callers,so reserved setup stays exclusive.
bool AnimationStartupProcess::setAnimation(uint32_t handle,uint32_t selector,uint32_t force,unsigned visits){
 if(!visits || !ready() || entryResource_ || mainPending() || (presentationPending() && (presentation_->handle()!=handle || presentation_->phase()!=0)))return false;
 if(!selection_)selection_=std::make_unique<AnimationNativeSelection>(*this);
 return selection_->advance(handle,selector,force,visits);
}
}
