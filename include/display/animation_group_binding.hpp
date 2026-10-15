#pragma once
#include "animation_group.hpp"
#include "animation_node_binding.hpp"
#include "animation_render_binding.hpp"
#include "event_address_space.hpp"
#include <functional>
namespace ttyd {
struct NativeNpcGroupBackend {
 void* user=nullptr;
 bool (*node)(void*,EventAddressSpace&,const AnimationGroupNode&)=nullptr;
 bool (*material)(void*,EventAddressSpace&,uint32_t shape,uint32_t current)=nullptr;
 bool (*vivian)(void*,EventAddressSpace&,const AnimationGroupVivian&)=nullptr;
 bool (*callback)(void*,EventAddressSpace&,uint32_t identity,uint32_t data,uint32_t group,uint32_t matrix)=nullptr;
 bool (*render)(void*,EventAddressSpace&,uint32_t shape,uint32_t current)=nullptr;
 AnimationSourceRenderChildren renderChildren;
};
// Same-VM group parent, scalar stores and sequential paired Copy. Borrowed
// backend context outlives all holds. Owned original node/render parents;
// material/effect/GPU services remain explicit.
class AnimationGroupBinding {
public:
 AnimationGroupBinding(EventAddressSpace&,const NativeNpcGroupBackend&,std::function<bool()> current);
 bool advance(const AnimationInnerTraversal&,unsigned visits=128);
private:
 AnimationGroupServices source();bool copy(uint32_t,uint32_t);
 EventAddressSpace& memory_;NativeNpcGroupBackend backend_;std::function<bool()> current_;
 AnimationNodeBinding node_;AnimationRenderBinding render_;
 AnimationGroupCursor cursor_;bool copying_=false,pairLoaded_=false;
 uint32_t copySource_=0,copyDestination_=0,pair_=0;std::array<uint32_t,2> words_{};
};
}
