#include "native_field_animation.hpp"
#include "field_npc_main_entry_binding.hpp"
namespace ttyd {
bool NativeFieldAnimation::beginNpcPass(
    const void* owner,uint64_t pass,std::shared_ptr<const CameraManager> cameras,
    std::function<bool()> current){
 if(!owner || !current || !current() || !ready() || producerPending() ||
    entering_ || releasing_.active || !configurePresentation(std::move(cameras)))
  return false;
 npcPass_=pass;npcCurrent_=std::move(current);npcOwner_=owner;
 return true;
}
bool NativeFieldAnimation::endNpcPass(const void* owner,uint64_t pass){
 // Child quiescence alone cannot prove the outer callback/queue is drained.
 // The caller owns that boundary and must retain this lease until then.
 if(!owner || npcOwner_!=owner || npcPass_!=pass || !npcCurrent_ ||
    !npcCurrent_() || process_->producerPending() || entering_ || releasing_.active)
  return false;
 npcOwner_=nullptr;npcCurrent_={};return true;
}
FieldNpcAnimationServices NativeFieldAnimation::npcAnimationServices(){
 FieldNpcAnimationServices s;s.user=this;
 s.request=[](void* u,EventAddressSpace& memory,const FieldNpcDrawRequest& q,
              FieldNpcDrawResponse&){
  auto& n=*static_cast<NativeFieldAnimation*>(u);
  if(!n.npcOwner_ || !n.npcCurrent_ || !n.npcCurrent_() ||
     &memory!=&n.process_->vm().memory ||
     n.configuredMain_!=n.process_->mainConfiguration() || n.marioActive_ ||
     n.entering_ || n.releasing_.active || n.process_->presentationPending())
   return false;
  // Process children retain exact raw arguments and partial source effects.
  // The public raw Main wrapper intentionally rejects an active NPC lease.
  if(q.function==0x80041170)
   return n.process_->setAnimation(q.args[0],q.args[1],q.args[2],n.budget_);
  if(q.function==0x8003ed2c)
   return n.process_->main(q.args[0],n.npcPass_,n.budget_);
  return false;
 };
 return s;
}
}
