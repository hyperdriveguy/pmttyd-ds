#pragma once
#include "animation_source_render.hpp"
#include "event_address_space.hpp"
#include <functional>
namespace ttyd {
// Raw primitive RAM reads and every child effect share the active producer lease.
// A selected provider owns its incomplete call; there is no alternate fallback.
class AnimationRenderBinding {
public:
 AnimationRenderBinding(EventAddressSpace&,const AnimationSourceRenderChildren&,std::function<bool()> current);
 bool advance(uint32_t shape,unsigned visits=128);
private:
 AnimationSourceRenderServices source();
 // Borrowed owners remain stable until final FIFO and all outer cursors drain.
 EventAddressSpace& memory_;AnimationSourceRenderChildren children_;
 std::function<bool()> current_;AnimationSourceRenderCursor cursor_;
};
}
