#include "message_icons.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
namespace ttyd {
namespace {std::vector<MessageIcon> icons;}
// Loop modulo total delay; nonlooping icons latch their last frame. All-zero
// durations select first frame without modulo/division. Zero-duration frames
// otherwise consume no time. Cadence belongs to the caller, not this lookup.
const MessageIconFrame& MessageIcon::frame(uint64_t ticks)const {
    uint64_t length=0;for(const auto& f:frames)length+=f.delay;
    if(!length)return frames[0];
    if(loop)ticks%=length;
    for(const auto& f:frames){if(ticks<f.delay)return f;ticks-=f.delay;}
    return frames.back();
}
// Native pack limits: 1..1023 unique icons, 1..256 frames, boolean loop;
// descriptor dimensions <=512x128 and finite normalized UVs with inverted V.
// fread uses native struct layout on supported little-endian hosts/3DS; this is
// a converted pack, not original big-endian ISO bytes. Require exact EOF and
// publish atomically; failure preserves catalog, success invalidates old views.
bool loadMessageIcons(const char* path) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;char magic[8];uint32_t count=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMICON1",8) && std::fread(&count,4,1,f)==1 && count>0 && count<1024;
    std::vector<MessageIcon> next;
    for(uint32_t i=0;ok && i<count;++i) {
        MessageIcon icon;uint32_t frames=0;
        ok=std::fread(&icon.id,4,1,f)==1 && std::fread(&icon.loop,4,1,f)==1 && std::fread(&frames,4,1,f)==1 && frames>0 && frames<=256 && icon.loop<=1;
        for(uint32_t j=0;ok && j<frames;++j) {
            MessageIconFrame frame;ok=std::fread(&frame,sizeof(frame),1,f)==1 && frame.width>0 && frame.width<=512 && frame.height>0 && frame.height<=128 &&
                std::isfinite(frame.u0) && std::isfinite(frame.v0) && std::isfinite(frame.u1) && std::isfinite(frame.v1) &&
                frame.u0>=0 && frame.u1>frame.u0 && frame.u1<=1 && frame.v1>=0 && frame.v0>frame.v1 && frame.v0<=1;
            icon.frames.push_back(frame);
        }
        for(const auto& prior:next)if(prior.id==icon.id)ok=false;
        next.push_back(std::move(icon));
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;icons=std::move(next);return true;
}
const MessageIcon* messageIcon(uint32_t id){for(const auto& icon:icons)if(icon.id==id)return &icon;return nullptr;}
static_assert(sizeof(MessageIconFrame)==28,"Portable icon frame layout");
}
