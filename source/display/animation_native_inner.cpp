#include "animation_native_inner.hpp"
#include "native_field_animation.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_ram_access.hpp"
#include <cstring>
#include <cmath>
namespace ttyd {
// Native semantic camera reservation: 13 slots,stride280hex at81910000.
// Same range is shared by facing/render adapters; it must remain unmapped.
namespace {constexpr uint32_t CameraBase=0x81910000,CameraSize=13*0x280;}
AnimationNativeInner::AnimationNativeInner(NativeFieldAnimation& n,const NativeNpcInnerBackend& b):native_(n),backend_(b){}
// Outer Draw owner proves same VM/lease/pass/configuration and pose lifetime.
// Guarding a primitive does not transfer backend or VM lifetime to this object.
bool AnimationNativeInner::current() const{return native_.npcDrawCurrent();}
bool AnimationNativeInner::ownsCamera(uint32_t a) const{return a>=CameraBase && uint64_t(a)<uint64_t(CameraBase)+CameraSize;}
// Lazily refresh facing for changed camera bank/Main configuration. Producer
// reconfiguration remains quiescent-only; a held child keeps its original owner.
bool AnimationNativeInner::camera(){
 if(!current() || !native_.cameras_)return false;
 if(facing_ && (facingCameras_!=native_.cameras_ || facingConfiguration_!=native_.configuredMain_))facing_.reset();
 if(!facing_){facingCameras_=native_.cameras_;facingConfiguration_=native_.configuredMain_;AnimationFacingAddressPolicy a;a.base=CameraBase;a.unmapped=[this](uint32_t p,uint32_t n){return native_.process_->vm().memory.unmappedRange(p,n);};
  facing_=AnimationFacingBinding::bind(native_.pool_,native_.process_->vm().memory,*native_.process_->animationMemory(),native_.cameras_,a,[this]{return current();});}
 return facing_ && facing_->available();
}
// Caller matrix is already an owned 48-byte snapshot. Copy six forward 8-byte
// pairs, retaining exact input/destination and a loaded pair through repair.
// This differs from Group's live VM-to-VM Copy with overlapping guest inputs.
bool AnimationNativeInner::copy(const BattleActMatrix& m,uint32_t address){
 if(!current())return false;
 if(!copying_){for(float value:m)if(!std::isfinite(value))return false;copying_=true;copyInput_=m;copyAddress_=address;copyPair_=0;pairLoaded_=false;}
 else if(address!=copyAddress_ || std::memcmp(m.data(),copyInput_.data(),48))return false;
 // Native finite-domain GQR0 float-pair adaptation. Pair loads precede their
 // two BE stores; successful stores survive a later unavailable destination.
 while(copyPair_<6){
  if(!pairLoaded_){for(unsigned i=0;i<2;++i)std::memcpy(&copyWords_[i],&copyInput_[copyPair_*2+i],4);pairLoaded_=true;}
  auto& memory=native_.process_->vm().memory;const uint32_t destination=copyAddress_+copyPair_*8;
  // A paired store is one admitted primitive. Only raw complete writable RAM
  // can supply it; no provider/child executes between the two lane writes.
  if(!memory.withRawRamAccess([&](auto&){return current() && memory.contiguousRam(destination,8) && memory.writeWord(destination,copyWords_[0]) && memory.writeWord(destination+4,copyWords_[1]);}))return false;
  ++copyPair_;pairLoaded_=false;
 }
 copying_=false;return true;
}
AnimationInnerServices AnimationNativeInner::source(){
 AnimationInnerServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationNativeInner*>(u);if(!b.current())return {};if(b.ownsCamera(a)){if(!b.camera())return {};auto f=b.facing_->services();return f.word(f.user,a);}uint32_t w;return b.native_.process_->vm().memory.readWord(a,w)?std::optional<uint32_t>(w):std::nullopt;};
 s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& b=*static_cast<AnimationNativeInner*>(u);uint16_t v;return b.current() && b.native_.process_->vm().memory.readHalf(a,v)?std::optional<uint16_t>(v):std::nullopt;};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationNativeInner*>(u);uint8_t v;return b.current() && b.native_.process_->vm().memory.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t w){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && !b.ownsCamera(a) && b.native_.process_->vm().memory.writeWord(a,w);};
 // No successful GPU/callback/cache defaults. Selected backend false retains
 // its child work; the caller must resume the same context through completion.
 s.gx=[](void* u,const BattleAudienceGXRequest& q){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && b.backend_.gx && b.backend_.gx(b.backend_.user,b.native_.process_->vm().memory,q);};
 s.callback=[](void* u,uint32_t fn,uint32_t pass){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && b.backend_.callback && b.backend_.callback(b.backend_.user,b.native_.process_->vm().memory,fn,pass);};
 s.facing.user=this;s.facing.word=s.word;s.facing.store=s.store;
 s.facing.call=[](void* u,AnimationFacingCall& q){auto& b=*static_cast<AnimationNativeInner*>(u);if(!b.camera())return false;auto f=b.facing_->services();return f.call(f.user,q);};
 s.facing.matrix=[](void* u,BattleAudienceMatrixRequest& q){if(!static_cast<AnimationNativeInner*>(u)->current())return false;BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
 // Inner may pump only its enclosing Draw ROOT handle and SAME lease pass.
 // Late Main remains the process owner; no typed evaluate/draw fallback.
 s.main=[](void* u,uint32_t h){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && h==b.native_.npcDrawHandle_ && b.native_.process_->main(h,b.native_.npcPass_,b.native_.budget_);};
 s.copy=[](void* u,const BattleActMatrix& m,uint32_t a){return static_cast<AnimationNativeInner*>(u)->copy(m,a);};
 s.flush=[](void* u,uint32_t a,uint32_t n){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && b.backend_.flush && b.backend_.flush(b.backend_.user,b.native_.process_->vm().memory,a,n);};
 s.invalidate=[](void* u){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && b.backend_.invalidate && b.backend_.invalidate(b.backend_.user,b.native_.process_->vm().memory);};
 // Optional owned group is exclusive once selected,including on false.
 // Only absence selects external traversal; a hold never changes that choice.
 s.traverse=[](void* u,const AnimationInnerTraversal& q){auto& b=*static_cast<AnimationNativeInner*>(u);return b.current() && (b.group_?b.group_->advance(q,b.native_.budget_):b.backend_.traverse && b.backend_.traverse(b.backend_.user,b.native_.process_->vm().memory,q));};return s;
}
bool AnimationNativeInner::advance(const AnimationInnerDrawRequest& q){
 if(!current())return false;
 // Rebuild group/SDK owners only before a NEW Inner operation when camera
 // owner or Main configuration changes; retained uploads keep local identities.
 if(!cursor_.active && backend_.groups && (!group_ || groupCameras_!=native_.cameras_ || groupConfiguration_!=native_.configuredMain_)){
  groupCameras_=native_.cameras_;groupConfiguration_=native_.configuredMain_;
  group_=std::make_unique<AnimationGroupBinding>(native_.process_->vm().memory,*backend_.groups,[this]{return current();},groupCameras_);
 }
 return resumeAnimationInner(q,source(),cursor_,native_.budget_);
}
// The borrowed facade accepts ONLY this exact source VM address-space owner.
NativeNpcInnerServices AnimationNativeInner::services(){NativeNpcInnerServices s;s.user=this;s.request=[](void* u,EventAddressSpace& m,const AnimationInnerDrawRequest& q){auto& b=*static_cast<AnimationNativeInner*>(u);return &m==&b.native_.process_->vm().memory && b.advance(q);};return s;}
// Replacement waits for source producers,Entry and Release to quiesce; backend
// context outlives all retained Inner/group/material/render/GPU effects.
bool NativeFieldAnimation::configureNpcInnerBackend(const NativeNpcInnerBackend& backend){
 if(!ready() || producerPending() || entering_ || releasing_.active)return false;
 auto next=std::make_unique<AnimationNativeInner>(*this,backend);npcInnerBinding_=std::move(next);npcInner_=npcInnerBinding_->services();return true;
}
}
