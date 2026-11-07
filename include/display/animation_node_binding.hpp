#pragma once
#include "animation_sdk_matrix_binding.hpp"
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
 EventAddressSpace& memory_;std::function<bool()> current_;AnimationSourceNodeCursor cursor_;
 AnimationSdkMatrixBinding sdk_;
};
}
