#include "animation_group_binding.hpp"
#include "event_ram_access.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}}
AnimationGroupBinding::AnimationGroupBinding(EventAddressSpace& m,const NativeNpcGroupBackend& b,std::function<bool()> current,std::shared_ptr<const CameraManager> cameras):memory_(m),backend_(b),current_(std::move(current)),node_(m,current_),sdk_(b.nativeRenderSdk?std::make_unique<AnimationRenderSdkBinding>(m,std::move(cameras),b.renderChildren,current_):nullptr),render_(m,sdk_?sdk_->services():b.renderChildren,current_){}
bool AnimationGroupBinding::copy(uint32_t from,uint32_t to){
 if(!current_ || !current_())return false;
 if(!copying_){copying_=true;copySource_=from;copyDestination_=to;pair_=0;pairLoaded_=false;}
 else if(from!=copySource_ || to!=copyDestination_)return false;
 while(pair_<6){const auto offset=pair_*8;
  if(!pairLoaded_){
   if(!memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(copySource_+offset,8) && memory_.readWord(copySource_+offset,words_[0]) && memory_.readWord(copySource_+offset+4,words_[1]);}))return false;
   pairLoaded_=true;
  }
  if(!std::isfinite(real(words_[0])) || !std::isfinite(real(words_[1])))return false;
  if(!memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(copyDestination_+offset,8) && memory_.writeWord(copyDestination_+offset,words_[0]) && memory_.writeWord(copyDestination_+offset+4,words_[1]);}))return false;
  ++pair_;pairLoaded_=false;
 }
 copying_=false;return true;
}
AnimationGroupServices AnimationGroupBinding::source(){
 AnimationGroupServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationGroupBinding*>(u);uint32_t w;return b.current_ && b.current_() && b.memory_.readWord(a,w)?std::optional<uint32_t>(w):std::nullopt;};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationGroupBinding*>(u);uint8_t w;return b.current_ && b.current_() && b.memory_.readByte(a,w)?std::optional<uint8_t>(w):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t w){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && b.memory_.writeWord(a,w);};
 s.node=[](void* u,const AnimationGroupNode& q){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && (b.backend_.node?b.backend_.node(b.backend_.user,b.memory_,q):b.node_.advance(q));};
 s.copy=[](void* u,uint32_t from,uint32_t to){return static_cast<AnimationGroupBinding*>(u)->copy(from,to);};
 s.material=[](void* u,uint32_t shape,uint32_t current){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && b.backend_.material && b.backend_.material(b.backend_.user,b.memory_,shape,current);};
 s.vivian=[](void* u,const AnimationGroupVivian& q){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && b.backend_.vivian && b.backend_.vivian(b.backend_.user,b.memory_,q);};
 s.callback=[](void* u,uint32_t fn,uint32_t data,uint32_t group,uint32_t matrix){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && b.backend_.callback && b.backend_.callback(b.backend_.user,b.memory_,fn,data,group,matrix);};
 s.render=[](void* u,uint32_t shape,uint32_t current){auto& b=*static_cast<AnimationGroupBinding*>(u);return b.current_ && b.current_() && (b.backend_.render?b.backend_.render(b.backend_.user,b.memory_,shape,current):b.render_.advance(shape));};
 s.blend=[](void* u,uint32_t k,uint32_t m,uint32_t old)->std::optional<uint32_t>{auto& b=*static_cast<AnimationGroupBinding*>(u);const float factor=real(k),source=real(m),saved=real(old);if(!b.current_ || !b.current_() || !std::isfinite(factor) || !std::isfinite(source) || !std::isfinite(saved))return {};volatile float delta=source-saved;const float result=std::fma(factor,float(delta),saved);return std::isfinite(result)?std::optional<uint32_t>(bits(result)):std::nullopt;};return s;
}
bool AnimationGroupBinding::advance(const AnimationInnerTraversal& q,unsigned visits){return current_ && current_() && resumeAnimationGroup(q,source(),cursor_,visits);}
}
