#include "animation_main_replay_binding.hpp"
#include "event_address_space.hpp"
namespace ttyd {
AnimationMainReplayBinding::AnimationMainReplayBinding(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr):pool_(std::move(p)),memory_(m),aliases_(a),current_(std::move(current)),gqr4_(std::move(gqr)){}
std::unique_ptr<AnimationMainReplayBinding> AnimationMainReplayBinding::bind(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr){
 if(!p || !current || !gqr)return {};
 auto result=std::unique_ptr<AnimationMainReplayBinding>(new AnimationMainReplayBinding(std::move(p),m,a,std::move(current),std::move(gqr)));return result->available()?std::move(result):nullptr;
}
bool AnimationMainReplayBinding::available() const{return current_() && aliases_.aliases(*pool_,memory_);}
AnimationMainReplayServices AnimationMainReplayBinding::services(){
 AnimationMainReplayServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);uint32_t v;return b.available() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<AnimationMainReplayBinding*>(u);return b.available() && b.memory_.writeWord(a,v);};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);uint8_t v;return b.available() && b.memory_.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& b=*static_cast<AnimationMainReplayBinding*>(u);return b.available() && b.memory_.writeByte(a,v);};
 s.quantized=[](void* u,uint32_t a)->std::optional<float>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);if(!b.available())return {};auto q=b.gqr4_();if(!q || *q!=0x00060006)return {};uint8_t v;if(!b.memory_.readByte(a,v))return {};const int value=v<128?int(v):int(v)-256;return float(value);};return s;
}
}
