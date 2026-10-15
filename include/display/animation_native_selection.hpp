#pragma once
#include "animation_selection.hpp"
#include "animation_pool_memory.hpp"
namespace ttyd {
class AnimationStartupProcess;
// One exclusive native setter owner, retired before VM/aliases. Native strcmp
// is a retained bytewise adapter, not a claim about SDK strcmp word-load order.
class AnimationNativeSelection {
public:
 explicit AnimationNativeSelection(AnimationStartupProcess&);
 // Same inputs and producer ownership across calls. This sets source selection/
 // timestamp/cache words only; it neither evaluates geometry nor publishes a draw.
 bool advance(uint32_t handle,uint32_t selector,uint32_t force,unsigned visits);
 bool pending() const{return active_;}
 uint32_t phase() const{return cursor_.phase;}
private:
 bool available() const;
 bool compare(uint32_t,uint32_t,int32_t&);
 AnimationSelectionServices services();
 AnimationStartupProcess& process_;std::shared_ptr<BattlePosePool> pool_;
 const AnimationPoolMemory& aliases_;bool active_=false;
 uint32_t handle_=0,selector_=0,force_=0;uint64_t generation_=0;
 std::shared_ptr<const BattlePoseResource> resource_;std::shared_ptr<AnimationScratchPose> buffers_;
 AnimationSelectionCursor cursor_;
 bool comparing_=false,leftLoaded_=false;uint32_t left_=0,right_=0,offset_=0;uint8_t leftByte_=0;
};
}
