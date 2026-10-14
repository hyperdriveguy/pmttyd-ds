#include "battle_movement_sound.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const uint8_t* p) {return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
void word(uint8_t* p,uint32_t v) {p[0]=v>>24;p[1]=v>>16;p[2]=v>>8;p[3]=v;}
uint16_t half(const uint8_t* p) {return uint16_t(p[0])<<8|p[1];}
void half(uint8_t* p,uint16_t v) {p[0]=v>>8;p[1]=v;}
int32_t signedHalf(const uint8_t* p) {const uint16_t v=half(p);return v<0x8000?v:int32_t(v)-0x10000;}
uint32_t trigger(MarioJumpUnit& u,uint32_t key,JumpVector position,const MovementPlaybackServices& s) {
    assert(s.trigger);return s.trigger(s.user,u,key,-250000000,0,position);
}
}
void controlMovementSound(MarioJumpUnit& u,uint8_t* r,void* part,JumpVector supplied,
                          const MovementPlaybackServices& s) {
    assert(r);const auto initialPosition=supplied;
    const uint32_t first=word(r),second=word(r+4);
    if(!first && !second)return;
    if(first && !second) {
        if(word(r+0x18)==0xffffffff)word(r+0x18,trigger(u,first,initialPosition,s));
        JumpVector position;
        if(part) {assert(s.partPosition);position=s.partPosition(s.user,part);}
        else {assert(s.unitPosition);position=s.unitPosition(s.user,u);}
        assert(s.position);s.position(s.user,word(r+0x18),position);return;
    }
    if(signedHalf(r+0x12)<0)return;
    half(r+0x12,static_cast<uint16_t>(half(r+0x12)-1));
    if(signedHalf(r+0x12)>=0)return;
    half(r+0x14,static_cast<uint16_t>(half(r+0x14)+1));
    const bool firstTurn=(half(r+0x14)&1)!=0;
    const uint32_t key=firstTurn?first:second;
    half(r+0x12,half(r+(firstTurn?0xe:0x10)));
    if(key)trigger(u,key,initialPosition,s);
}
void endMovementSound(uint8_t* r,const MovementPlaybackServices& s) {
    assert(r);const uint32_t handle=word(r+0x18);
    if(handle==0xffffffff)return;
    assert(s.off);s.off(s.user,handle);word(r+0x18,0xffffffff);
}
void controlUnitMovementSound(MarioJumpUnit& u,const MovementPlaybackServices& s) {
    assert(s.unitPosition);const auto position=s.unitPosition(s.user,u);
    controlMovementSound(u,u.soundMode.data(),nullptr,position,s);
}
void endUnitMovementSound(MarioJumpUnit& u,const MovementPlaybackServices& s) {
    endMovementSound(u.soundMode.data(),s);
}
MovementSoundServices movementSoundAdapter(MovementPlaybackServices& s) {
    MovementSoundServices adapter;adapter.user=&s;
    adapter.control=[](void* user,MarioJumpUnit& u) {controlUnitMovementSound(u,*static_cast<MovementPlaybackServices*>(user));};
    adapter.end=[](void* user,MarioJumpUnit& u) {endUnitMovementSound(u,*static_cast<MovementPlaybackServices*>(user));};
    return adapter;
}
}
