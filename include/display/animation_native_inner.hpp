#pragma once
#include "animation_inner.hpp"
#include "animation_group_binding.hpp"
#include "animation_facing_binding.hpp"
#include "native_npc_inner_services.hpp"
namespace ttyd {
class NativeFieldAnimation;
struct NativeNpcInnerBackend {
 void* user=nullptr;
 // Present selects owned actual-VM dispProc; backend context outlives holds.
 std::optional<NativeNpcGroupBackend> groups;
 bool (*gx)(void*,EventAddressSpace&,const BattleAudienceGXRequest&)=nullptr;
 bool (*callback)(void*,EventAddressSpace&,uint32_t identity,uint32_t effectivePass)=nullptr;
 bool (*flush)(void*,EventAddressSpace&,uint32_t sourceAddress,uint32_t bytes)=nullptr;
 bool (*invalidate)(void*,EventAddressSpace&)=nullptr;
 bool (*traverse)(void*,EventAddressSpace&,const AnimationInnerTraversal&)=nullptr;
};
// Owned by NativeFieldAnimation; borrows only backend context across holds.
// CPU source work is real same-VM state; GPU/material/traversal have no defaults.
class AnimationNativeInner {
public:
 AnimationNativeInner(NativeFieldAnimation&,const NativeNpcInnerBackend&);
 NativeNpcInnerServices services();
private:
 bool advance(const AnimationInnerDrawRequest&);
 bool current() const;
 bool ownsCamera(uint32_t) const;
 bool camera();
 bool copy(const BattleActMatrix&,uint32_t);
 AnimationInnerServices source();
 std::unique_ptr<AnimationGroupBinding> group_;
 std::shared_ptr<const CameraManager> groupCameras_;uint64_t groupConfiguration_=0;
 NativeFieldAnimation& native_;NativeNpcInnerBackend backend_;AnimationInnerCursor cursor_;
 std::unique_ptr<AnimationFacingBinding> facing_;
 std::shared_ptr<const CameraManager> facingCameras_;uint64_t facingConfiguration_=0;
 bool copying_=false,pairLoaded_=false;uint32_t copyAddress_=0,copyPair_=0;
 BattleActMatrix copyInput_{};std::array<uint32_t,2> copyWords_{};
};
}
