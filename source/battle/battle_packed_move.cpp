#include "battle_packed_move.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint8_t byte(const BattleActorServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
void store(const BattleActorServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
int32_t signedByte(uint8_t b) {return b<128?int32_t(b):int32_t(b)-256;}
}
// Reset20=0, copy21->22; status19(duration12f) doubles U8 count first.
// Status20(130) halves signed count>=2, otherwise needs turn threshold4 when
// signed base<1, else2. Failure sets state10/count0. Copy22->23 last; only
// signed snapshot==0 forces exhaustion (negative is not treated as zero).
void resetPackedBattleMoveStatus(uint32_t u,const BattleActorServices& s) {
    store(s,u+0x20,0);store(s,u+0x22,byte(s,u+0x21));
    if(byte(s,u+0x12f)!=0)store(s,u+0x22,uint8_t(uint32_t(byte(s,u+0x22))<<1));
    if(byte(s,u+0x130)!=0) {
        const int32_t count=signedByte(byte(s,u+0x22));
        if(count>=2)store(s,u+0x22,uint8_t(count/2));
        else {
            const int32_t threshold=signedByte(byte(s,u+0x21))<1?4:2;
            if(signedByte(byte(s,u+0x24))>=threshold) {store(s,u+0x22,1);store(s,u+0x20,0);}
            else {store(s,u+0x22,0);store(s,u+0x20,10);}
        }
    }
    store(s,u+0x23,byte(s,u+0x22));
    if(signedByte(byte(s,u+0x23))==0) {store(s,u+0x22,0);store(s,u+0x20,10);}
}
}
