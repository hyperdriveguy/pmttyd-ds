#include "positional_sfx_registry.hpp"
namespace ttyd {
void PositionalSfxRegistry::initializePublicSlots() {
    for(auto& slot:slots){slot.key=-1;slot.engine=-1;slot.flags=0;}
}
std::optional<int32_t> PositionalSfxRegistry::check(int32_t handle)const {
    if(handle==-1)return -1;
    const auto index=uint32_t(handle)&255;if(index>=slots.size())return {};
    const auto& slot=slots[index];
    if(((uint32_t(handle)>>8)&65535)!=slot.generation)return -1;
    return slot.key==-1?-1:0;
}
std::optional<uint8_t> PositionalSfxRegistry::getVolume(int32_t handle)const {
    if(handle==-1)return uint8_t(0);
    const auto index=uint32_t(handle)&255;if(index>=slots.size())return {};
    const auto& slot=slots[index];return slot.key==-1?uint8_t(0):slot.volume;
}
bool PositionalSfxRegistry::setVolume(int32_t handle,int32_t value) {
    if(handle==-1)return true;
    const auto index=uint32_t(handle)&255;if(index>=slots.size())return false;
    auto& slot=slots[index];if(slot.key!=-1)slot.volume=uint8_t(value);return true;
}
bool PositionalSfxRegistry::setDistance(int32_t handle,int32_t value) {
    if(handle==-1)return true;
    const auto index=uint32_t(handle)&255;if(index>=slots.size())return false;
    auto& slot=slots[index];if(slot.key!=-1)slot.distance=uint16_t(value);return true;
}
bool PositionalSfxRegistry::off(int32_t handle,const PositionalSfxStopServices& s) {
    if(handle==-1)return true;
    const auto index=uint32_t(handle)&255;if(index>=slots.size())return false;
    auto& slot=slots[index];const auto key=slot.key;
    if(key==-1)return true;
    if(!(slot.flags&1) && slot.engine!=-1) {
        const auto engine=slot.engine;
        if(!s.descriptor || !s.stop)return false;
        const auto descriptor=s.descriptor(s.user,uint32_t(key)&0x1fff);if(!descriptor)return false;
        const auto kind=(*descriptor&0x80000000)?PositionalSfxStopKind::Stream:
            (*descriptor&0x40000000)?PositionalSfxStopKind::Song:PositionalSfxStopKind::Effect;
        if(!s.stop(s.user,kind,engine))return false;
    }
    slot.key=-1;slot.engine=-1;return true;
}
}
