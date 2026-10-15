#include "native_field_animation.hpp"
#include "animation_native_inner.hpp"
#include "field_npc_draw.hpp"
namespace ttyd {
bool NativeFieldAnimation::configureNpcInner(const NativeNpcInnerServices& s){
 if(!ready() || producerPending() || entering_ || releasing_.active)return false;
 npcInnerBinding_.reset();npcInner_=s;return true;
}
bool NativeFieldAnimation::npcDrawCurrent() const{
 const auto* p=pool_->slot(npcDrawHandle_);
 const auto* main=process_->mainOwner();
 const bool ownedMain=!process_->mainPending() || (npcDraw_.active && npcDraw_.phase==12 && main && main->handle()==npcDrawHandle_ && main->pass()==npcPass_);
 return npcOwner_ && npcCurrent_ && npcCurrent_() && ready() &&
  configuredMain_==process_->mainConfiguration() && !marioActive_ && !entering_ &&
  !releasing_.active && !process_->presentationPending() && !process_->selectionPending() && ownedMain && p &&
  p->nativeGeneration==npcDrawGeneration_ && p->resource==npcDrawResource_ &&
  p->sourceBuffers==npcDrawBuffers_;
}
bool NativeFieldAnimation::drawNpc(const FieldNpcDrawRequest& q){
 if(!budget_ || q.args[1]!=0 || (!npcDraw_.active && process_->producerPending()))return false;
 if(!npcDraw_.active){
  if(q.args[0]>=256)return false;
  const auto* p=pool_->slot(q.args[0]);
  if(!p || !p->nativeGeneration || !p->resource || !p->sourceBuffers)return false;
  npcDrawHandle_=q.args[0];npcDrawGeneration_=p->nativeGeneration;
  npcDrawResource_=p->resource;npcDrawBuffers_=p->sourceBuffers;
 }
 if(q.args[0]!=npcDrawHandle_ || !npcDrawCurrent())return false;
 AnimationDrawWrapperServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& n=*static_cast<NativeFieldAnimation*>(u);uint32_t w;return n.npcDrawCurrent() && n.process_->vm().memory.readWord(a,w)?std::optional<uint32_t>(w):std::nullopt;};
 s.inner=[](void* u,const AnimationInnerDrawRequest& request){
  auto& n=*static_cast<NativeFieldAnimation*>(u);
  if(!n.npcDrawCurrent() || !n.npcInner_.request || !n.npcInner_.request(n.npcInner_.user,n.process_->vm().memory,request))return false;
  return !n.process_->producerPending() && n.npcDrawCurrent();
 };
 AnimationDrawWrapperInput input{q.args[0],q.args[2],q.matrix,q.scalars[0],q.scalars[1]};
 npcDrawFrontier_=resumeAnimationDrawWrapper(input,s,npcDraw_,budget_);
 if(npcDrawFrontier_!=AnimationDrawWrapperFrontier::Complete)return false;
 npcDrawResource_.reset();npcDrawBuffers_.reset();return true;
}
}
