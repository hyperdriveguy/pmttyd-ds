#pragma once
#include <cstdint>
#include <string>
#include <string_view>
namespace ttyd {
// Borrowed original hit names become owned native strings. Pointer/owner changes
// with equal names must not retrigger; no head contact clears the cache.
struct MapObjectHeadCache {
    std::string previous;
    bool publish(std::string_view name) {
        if(name.empty()){previous.clear();return false;}
        if(previous==name)return false;
        previous=name;return true;
    }
};
struct MapObjectKeyControl {
    uint8_t depth=0;
    uint32_t marioFlags=0;
    void off(){marioFlags|=8;depth=uint8_t(depth+1);}
    void on(){depth=uint8_t(depth-1);if(depth==0)marioFlags&=~8u;}
};
struct MapObjectState {
    uint32_t owner=0,flags=0x400085,cause=0;
    int32_t stage=0,eventId=0;
    uint32_t optionalEvent=0,systemEvent=0;
};
struct MapObjectHeadInput {
    uint32_t newOwner=0;
    int32_t character=0,motion=0,systemLevel=0;
};
// This is the recovered save constructor's head producer, not a universal
// attack/partner producer. Other object flags/constructors need their owners.
inline void produceSaveBlockHead(MapObjectState& object,const MapObjectHeadInput& input) {
    if(!(object.flags&1))return;
    if((object.flags&0x400000) && !(object.flags&8) && input.character!=2 && input.newOwner==object.owner && object.owner) {
        object.cause=0x400000;object.flags|=8;
    }
    if((object.flags&0x100) || input.systemLevel!=0 || input.motion==31 || input.motion==32)object.flags&=~8u;
}
struct MapObjectServices {
    void* user=nullptr;
    void (*key)(void*,bool enabled)=nullptr;
    void (*system)(void*,int32_t mode)=nullptr;
    void (*sound)(void*,uint32_t owner,uint32_t id)=nullptr;
    void (*animation)(void*,uint32_t owner,const char* pose,bool restart)=nullptr;
    float (*loopRatio)(void*,uint32_t owner)=nullptr;
    void (*cameraFlag)(void*,uint32_t camera,uint16_t mask,bool enabled)=nullptr;
    int32_t (*eventCheck)(void*,int32_t id)=nullptr;
    int32_t (*eventEntry)(void*,uint32_t script,uint32_t owner,uint8_t priority,uint8_t type)=nullptr;
};
// Valid original event allocation domain only. Callers own VM allocation failure
// handling; original mobjRunEvent does not guard its returned context pointer.
bool runMapObjectEvent(MapObjectState&,uint32_t script,const MapObjectServices&);
// One callback invocation, no same-frame stage fallthrough. Animation Main must
// publish AFTER this call, so stage1 observes the previous publication.
void advanceSaveBlock(MapObjectState&,const MapObjectServices&);
}
