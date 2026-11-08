#include "animation_main_replay_binding.hpp"
#include "event_address_space.hpp"
namespace ttyd {
AnimationMainReplayBinding::AnimationMainReplayBinding(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr,bool nativeCopies):pool_(std::move(p)),memory_(m),aliases_(a),current_(std::move(current)),gqr4_(std::move(gqr)){nativeCopies_=nativeCopies;}
// Missing quantization THUNK rejects binding. A thunk may currently have no
// GQR value: ordinary word/byte operations remain available until actual use.
std::unique_ptr<AnimationMainReplayBinding> AnimationMainReplayBinding::bind(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m,const AnimationPoolMemory& a,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr,bool nativeCopies){
 if(!p || !current || !gqr)return {};
 auto result=std::unique_ptr<AnimationMainReplayBinding>(new AnimationMainReplayBinding(std::move(p),m,a,std::move(current),std::move(gqr),nativeCopies));return result->available()?std::move(result):nullptr;
}
// Shared pool is retained, but VM/aliases/platform owner are borrowed. Caller's
// pure current predicate must protect the same producer pass on every boundary.
bool AnimationMainReplayBinding::available() const{return current_() && aliases_.aliases(*pool_,memory_);}
AnimationMainReplayServices AnimationMainReplayBinding::services(){
 AnimationMainReplayServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);uint32_t v;return b.available() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<AnimationMainReplayBinding*>(u);return b.available() && b.memory_.writeWord(a,v);};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);uint8_t v;return b.available() && b.memory_.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& b=*static_cast<AnimationMainReplayBinding*>(u);return b.available() && b.memory_.writeByte(a,v);};
 // Optional acceleration only: false has NO effects and admits retained
 // scalar copying. Word helper80005634 ignores low 2 size bits; byte helper
 // 8000519c uses full size. Zero declines to preserve source do/while domains.
 if(nativeCopies_)s.copy=[](void* u,uint32_t function,uint32_t destination,uint32_t source,uint32_t bytes){
  auto& b=*static_cast<AnimationMainReplayBinding*>(u);if(!b.available())return false;
  if(function==0x80005634){bytes=(bytes>>2)*4u;if(!bytes)return false;}else if(function!=0x8000519c || !bytes)return false;
  // Owned scratch excludes external W/R/file/owner storage. The original W
  // root is owned RAM too, so exclude its guest range explicitly.
  // Root80415dc0..c4 must stay invariant through the borrow. copyOwnedRam
  // additionally proves nonwrapping guest/host nonoverlap, owned destination,
  // complete raw source/destination spans and no external-owner alias. It
  // requires an active raw borrow; it neither retains one nor calls providers.
  if(uint64_t(destination)<0x80415dc4ull && 0x80415dc0ull<uint64_t(destination)+bytes)return false;
  if(!b.memory_.copyOwnedRam(destination,source,bytes))return false;
  // Diagnostic counts accepted whole copies, not frames or source refs.
  ++b.bulkCopies_;return true;
 };
 // Require actual GQR4 word00060006 at EACH load: signed8,type6,scale 0
 // in both halves. Decode 128..255 as -128..-1. No tangent/float/native fallback;
 // missing quantization holds here, after any earlier raw reads/copies.
 s.quantized=[](void* u,uint32_t a)->std::optional<float>{auto& b=*static_cast<AnimationMainReplayBinding*>(u);if(!b.available())return {};auto q=b.gqr4_();if(!q || *q!=0x00060006)return {};uint8_t v;if(!b.memory_.readByte(a,v))return {};const int value=v<128?int(v):int(v)-256;return float(value);};return s;
}
}
