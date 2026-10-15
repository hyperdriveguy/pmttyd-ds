#pragma once
#include <cstdint>
#include <vector>
namespace ttyd {
// Native PMICON1 pack: 3 little-endian words + 4 binary32 UVs (28 bytes).
// Delay is the original descriptor duration in caller-supplied icon ticks;
// it is not a wall-clock duration or renderer frame count established here.
struct MessageIconFrame {uint32_t delay=0,width=0,height=0;float u0=0,v0=0,u1=0,v1=0;};
// frame() requires nonempty frames (guaranteed for successfully loaded icons).
// Returned frame/icon views borrow the process catalog until successful reload.
struct MessageIcon {uint32_t id=0,loop=0;std::vector<MessageIconFrame> frames;const MessageIconFrame& frame(uint64_t ticks)const;};
bool loadMessageIcons(const char* path);
const MessageIcon* messageIcon(uint32_t id);
}
