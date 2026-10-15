#pragma once
#include "animation_source_node.hpp"
#include "event_address_space.hpp"
#include <functional>
namespace ttyd {
class AnimationNodeBinding {
public:
 AnimationNodeBinding(EventAddressSpace&,std::function<bool()> current);
 bool advance(const AnimationGroupNode&,unsigned visits=128);
private:
 AnimationSourceNodeServices source();
 bool matrix(const AnimationSourceMatrixCall&,AnimationNodeLocals&);
 bool pair(AnimationNodeRef,unsigned,std::array<uint32_t,2>&,AnimationNodeLocals&);
 EventAddressSpace& memory_;std::function<bool()> current_;AnimationSourceNodeCursor cursor_;
 bool sdkActive_=false;AnimationSourceMatrixCall sdk_;AnimationNodeLocals* locals_=nullptr;
 uint32_t stage_=0,load_=0,store_=0,zero_=0,one_=0;float sine_=0,cosine_=0;
 std::array<uint32_t,2> tail_{};BattleActMatrix left_{},right_{},output_{};
};
}
