#include "battle_packed_movement_sound.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t half(const EventMemoryServices& s,uint32_t a) {assert(s.half);const uint16_t v=s.half(s.user,a);return v<32768?v:int32_t(v)-65536;}
void word(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
void half(const EventMemoryServices& s,uint32_t a,int32_t v) {assert(s.storeHalf);s.storeHalf(s.user,a,uint16_t(v));}
std::array<float,3> visible(uint32_t unit,const EventMemoryServices& s) {std::array<float,3> position;for(unsigned i=0;i<3;++i) {const uint32_t bits=word(s,unit+0x3c+4*i);std::memcpy(&position[i],&bits,4);}return position;}
uint32_t trigger(uint32_t unit,uint32_t key,const std::array<float,3>& position,const BattlePackedMovementSoundServices& s) {assert(s.playback.trigger);return s.playback.trigger(s.playback.user,unit,key,-250000000,0,position);}
}
void loadPackedUnitSoundMode(int32_t type,int32_t mode,uint32_t source,uint32_t dest,const EventMemoryServices& s) {
    half(s,dest+0x14,0);word(s,dest+0x18,0xffffffff);
    if(mode!=-2) {
        if(mode==-1 && type>=0 && type<=3) {
            const uint32_t offset=type==0?8:type==1?0x18:type==2?0:0x28;
            word(s,dest,word(s,source+offset));word(s,dest+4,word(s,source+offset+4));
            for(unsigned i=0;i<3;++i) {const int32_t value=type==2?0:half(s,source+offset+8+2*i);half(s,dest+0xc+2*i,value);}
        }else {word(s,dest,0);word(s,dest+4,0);for(unsigned i=0;i<3;++i)half(s,dest+0xc+2*i,0);}
    }
    half(s,dest+0x12,half(s,dest+0xc));
}
void controlPackedMovementSound(uint32_t unit,uint32_t record,uint32_t part,const std::array<float,3>& supplied,const BattlePackedMovementSoundServices& s) {
    const auto& m=s.position.memory.memory;const auto initial=supplied;
    // Original scratch-vector template is read even though all axes are replaced
    // by incoming floats before any key query. Keep those discarded source reads.
    for(unsigned i=0;i<3;++i)word(m,0x802ee494+4*i);
    const uint32_t first=word(m,record);
    if(!first && word(m,record+4)==0)return;
    if(first && word(m,record+4)==0) {
        if(word(m,record+0x18)==0xffffffff)word(m,record+0x18,trigger(unit,first,initial,s));
        const auto current=part?packedPartWorldPosition(part,s.position):visible(unit,m);
        const uint32_t handle=word(m,record+0x18);assert(s.playback.position);s.playback.position(s.playback.user,handle,current);return;
    }
    const int32_t remaining=half(m,record+0x12);if(remaining<0)return;
    half(m,record+0x12,remaining-1);if(half(m,record+0x12)>=0)return;
    half(m,record+0x14,half(m,record+0x14)+1);const bool firstTurn=(uint32_t(half(m,record+0x14))&1)!=0;
    const uint32_t key=word(m,record+(firstTurn?0:4));const int32_t period=half(m,record+(firstTurn?0xe:0x10));half(m,record+0x12,period);
    if(key)trigger(unit,key,initial,s); // Periodic return has no retained-handle store.
}
void endPackedMovementSound(uint32_t record,const BattlePackedMovementSoundServices& s) {const auto& m=s.position.memory.memory;const uint32_t handle=word(m,record+0x18);if(handle==0xffffffff)return;assert(s.playback.off);s.playback.off(s.playback.user,handle);word(m,record+0x18,0xffffffff);}
void controlPackedUnitMovementSound(uint32_t unit,const BattlePackedMovementSoundServices& s) {const auto position=visible(unit,s.position.memory.memory);controlPackedMovementSound(unit,unit+0x18c,0,position,s);}
void endPackedUnitMovementSound(uint32_t unit,const BattlePackedMovementSoundServices& s) {endPackedMovementSound(unit+0x18c,s);}
}
