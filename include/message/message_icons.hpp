#pragma once
#include <cstdint>
#include <vector>
namespace ttyd {
struct MessageIconFrame {uint32_t delay=0,width=0,height=0;float u0=0,v0=0,u1=0,v1=0;};
struct MessageIcon {uint32_t id=0,loop=0;std::vector<MessageIconFrame> frames;const MessageIconFrame& frame(uint64_t ticks)const;};
bool loadMessageIcons(const char* path);
const MessageIcon* messageIcon(uint32_t id);
}
