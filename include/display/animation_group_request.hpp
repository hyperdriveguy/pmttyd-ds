#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct AnimationGroupRequestServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*archive)(void*,uint32_t)=nullptr;
 std::optional<int32_t> (*compare)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<uint32_t> (*request)(void*,uint32_t type,uint32_t completion,uint32_t format,uint32_t prefix,uint32_t name)=nullptr;
};
struct AnimationGroupRequestCursor {
 AnimationGroupRequestServices owner;uint32_t phase=0,name=0,mode=0,completion=0;
 uint32_t modelResult=0,textureResult=0,work=0,handle=0,holder=0,count=0,pointer=0,offset=0,texture=0;
};
// Full animGroupBaseAsync80039d0c. Native hold is false; completed source0
// resets the cursor and permits the next invocation to resolve fresh operands.
// Scans actual W100 holder bytes in32-byte steps, not a detached pair map.
bool requestAnimationGroup(uint32_t name,uint32_t mode,uint32_t completion,
 const AnimationGroupRequestServices&,AnimationGroupRequestCursor&,uint32_t& result,unsigned visits=8192);
}
