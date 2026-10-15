#include "native_field_animation.hpp"
namespace ttyd {
bool NativeFieldAnimation::release(int32_t handle){
 if(!ready() || entering_ || handle<0 || unsigned(handle)>=256)return false;
 // Only the initial admission may use typed metadata. Source teardown clears
 // model flags before its children finish, temporarily invalidating pose().
 auto* p=pool_->poses_[unsigned(handle)].get();
 if(!releasing_.active){
  if(!p || !pool_->pose(handle) || !p->sourceBuffers || !pool_->cacheMetadataReady())return false;
  releasingGeneration_=p->nativeGeneration;
 }else if(releasing_.handle!=uint32_t(handle) || !p || p->nativeGeneration!=releasingGeneration_)return false;
 BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;auto& m=static_cast<NativeFieldAnimation*>(u)->process_->vm().memory;return m.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<NativeFieldAnimation*>(u)->process_->vm().memory.writeWord(a,v);};
 s.call=[](void* u,BattleSequenceCall& c){
  auto& n=*static_cast<NativeFieldAnimation*>(u);
  // Original fileFree changes handle references; it does not run file GC.
  // Real smart-header ownership is still separate, never a successful noop.
  return c.function==0x8018a260 && n.files_->release(c.args[0]);
 };
 if(!releaseAnimationPose(handle,s,releasing_))return false;
 if(p->word(0))return true; // Borrowed count remains nonnegative: still live.
 // Terminal source flags are authoritative. Retire only native sidecars;
 // retain stale source words, source scratch allocation and file-cache bytes.
 p->sourceBuffers.reset();p->resource.reset();p->shape={};p->cache={};
 for(auto& model:pool_->models_)if(!model.flags())model.resource.reset();
 for(auto& texture:pool_->textures_)if(!texture.flags()){texture.resource.reset();texture.name.clear();}
 if(sampled_==handle)sampled_=-1;
 if(selected_==handle){selected_=-1;uploaded_=UINT64_MAX;}
 return true;
}
}
