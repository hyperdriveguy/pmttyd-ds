#pragma once
#include "animation_source_node.hpp"
#include "event_address_space.hpp"
#include <functional>
namespace ttyd {
// US revision0 SDK identities; these are source ABI addresses, not native PCs.
namespace animation_sdk {
constexpr uint32_t Identity=0x8029dd04,Concat=0x8029dd64,InverseTranspose=0x8029df28;
constexpr uint32_t RotateRadians=0x8029dff0,Translate=0x8029e230,Scale=0x8029e2b0,MultiplyPoint=0x8029e6a4;
// SDK constants have actual mutable VM backing. Matrices contain 12 binary32
// lanes (3x4); pair width8 and last start40 are byte extents,not element counts.
constexpr uint32_t ConcatTail=0x8041e458,Zero=0x8042935c,One=0x80429358;
constexpr unsigned PairBytes=2*sizeof(uint32_t),MatrixBytes=12*sizeof(float),LastPairOffset=MatrixBytes-PairBytes;
}

// Stable storage adapters: Node's ten matrices and Render's optional locals
// share one SDK engine without copying live inputs or inventing guest RAM.
struct AnimationSdkLocals {
 void* user=nullptr;unsigned count=0;
 bool (*pair)(void*,uint32_t slot,unsigned offset,std::array<uint32_t,2>&)=nullptr;
 bool (*store)(void*,uint32_t slot,const BattleActMatrix&)=nullptr;
};
struct AnimationSdkGuestPairs {
 void* user=nullptr;
 bool (*owns)(void*,uint32_t address)=nullptr;
 bool (*pair)(void*,uint32_t address,std::array<uint32_t,2>&)=nullptr;
};
// Retains request/local adapter descriptors,borrows VM and external storage.
// Keep the stable binding,local owners and producer pass alive through pending.
class AnimationSdkMatrixBinding {
public:
 AnimationSdkMatrixBinding(const AnimationSdkMatrixBinding&)=delete;
 AnimationSdkMatrixBinding& operator=(const AnimationSdkMatrixBinding&)=delete;
 AnimationSdkMatrixBinding(EventAddressSpace&,std::function<bool()> current,AnimationSdkGuestPairs={});
 bool pending() const{return sdkActive_;}
 bool advance(const AnimationSourceMatrixCall&,const AnimationSdkLocals&);
 // Reached primitive pair, also reused by source MultVec. Reserved semantic
 // ranges never fall through to RAM, including a pair crossing their edge.
 bool pair(AnimationNodeRef,unsigned,std::array<uint32_t,2>&,const AnimationSdkLocals&);
private:
 EventAddressSpace& memory_;std::function<bool()> current_;AnimationSdkGuestPairs guest_;
 bool sdkActive_=false;AnimationSourceMatrixCall sdk_;AnimationSdkLocals locals_;
 uint32_t stage_=0,load_=0,store_=0,zero_=0,one_=0;float sine_=0,cosine_=0;
 std::array<uint32_t,2> tail_{};BattleActMatrix left_{},right_{},output_{};
};
}
