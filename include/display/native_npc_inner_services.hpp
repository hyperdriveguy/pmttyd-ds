#pragma once
#include "animation_draw_wrapper.hpp"
namespace ttyd {
class EventAddressSpace;
struct NativeNpcInnerServices {
 void* user=nullptr;
 // Original Inner8003bc68 on SAME VM, not typed pose/mesh success. Provider
 // owns every original capture/cache/material/SDK/traversal/GPU child and
 // retains progress across false; no successful default or GPU fixture here.
 bool (*request)(void*,EventAddressSpace&,const AnimationInnerDrawRequest&)=nullptr;
};
}
