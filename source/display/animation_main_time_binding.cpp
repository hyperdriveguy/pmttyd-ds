#include "animation_main_time_binding.hpp"
#include "event_address_space.hpp"
#include "field_npc_clock.hpp"
#include "effect_maya_animation.hpp"
#include "animation_sample_time.hpp"
namespace ttyd {
AnimationMainTimeBinding::AnimationMainTimeBinding(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current):pool_(std::move(p)),memory_(m),aliases_(a),current_(std::move(current)){}
// Retain pool identity; borrow VM/aliases. Bind requires current admission but
// does not validate one particular active handle or retain the producer pass.
std::unique_ptr<AnimationMainTimeBinding> AnimationMainTimeBinding::bind(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current){
 if(!p || !current)return {};
 auto result=std::unique_ptr<AnimationMainTimeBinding>(new AnimationMainTimeBinding(std::move(p),m,a,std::move(current)));return result->available()?std::move(result):nullptr;
}
// Recheck both pure producer/lifetime predicate and SAME pool/VM aliases at
// EVERY primitive. No detached typed-state fallback when a guard is absent.
bool AnimationMainTimeBinding::available() const{return current_() && aliases_.aliases(*pool_,memory_);}
AnimationMainTimeServices AnimationMainTimeBinding::services(){
 AnimationMainTimeServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationMainTimeBinding*>(u);uint32_t v;return b.available() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<AnimationMainTimeBinding*>(u);return b.available() && b.memory_.writeWord(a,v);};
 // One synchronous BE64 bias read (HIGH then LOW), no suspension between
 // halves. fffffff8 is the last nonwrapping 8-byte start; exclusive caller
 // ownership keeps the memory map stable across these two facade reads.
 s.doubleWord=[](void* u,uint32_t a)->std::optional<uint64_t>{auto& b=*static_cast<AnimationMainTimeBinding*>(u);uint32_t hi,lo;if(!b.available() || a>0xfffffff8 || !b.memory_.readWord(a,hi) || !b.memory_.readWord(a+4,lo))return {};return uint64_t(hi)<<32|lo;};
 // Signed raw64 clock quotient rejects divisor 0. Elapsed conversion below
 // is instead UNSIGNED through binary64 then binary32. fctiwz adapter admits
 // finite [-2^31,2^31); nonfinite/overflow holds rather than inventing integer 0.
 s.divide=[](void* u,uint64_t n,uint32_t d,uint64_t& q){return static_cast<AnimationMainTimeBinding*>(u)->available() && nativeFieldNpcClockDivide(nullptr,n,d,q);};
 s.convertUnsigned=[](void* u,uint64_t n,float& f){return static_cast<AnimationMainTimeBinding*>(u)->available() && effectMayaConvertUnsigned(nullptr,n,f);};
 s.truncate=[](void* u,float f)->std::optional<int32_t>{return static_cast<AnimationMainTimeBinding*>(u)->available()?nativeAnimationLoopConversion(nullptr,f):std::nullopt;};return s;
}
// Timeline borrows exactly the same guarded word/store identity as time.
AnimationMainTimelineServices AnimationMainTimeBinding::timelineServices(){
 const auto s=services();return {s.user,s.word,s.store};
}

}
