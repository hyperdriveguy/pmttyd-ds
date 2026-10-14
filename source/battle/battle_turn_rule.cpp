#include "battle_turn_rule.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleActorServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
uint8_t byte(const BattleActorServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
void store(const BattleActorServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
}
uint8_t checkBattleZeroTurnCount(uint32_t sum,const BattleActorServices& s) {
    const uint32_t battle=currentBattleAddress(s),descriptor=word(s,battle+0x2738);
    const uint8_t firstTurn=byte(s,battle+0x16f57),firstLimit=byte(s,descriptor+0x1b);
    if(firstTurn<=firstLimit || firstLimit==255)store(s,descriptor+0x19,sum==0?2:1);
    const uint8_t secondTurn=byte(s,battle+0x16f57),secondLimit=byte(s,descriptor+0x1a);
    if(secondTurn<=secondLimit || secondLimit==255) {
        if(sum==0)store(s,descriptor+0x19,6);
        else if((byte(s,descriptor+0x19)&2)==0)store(s,descriptor+0x19,1);
    }
    return byte(s,descriptor+0x19);
}
void judgeBattleTurnRule(const BattleActorServices& s) {
    const uint32_t battle=currentBattleAddress(s),descriptor=word(s,battle+0x2738);
    const uint8_t rule=byte(s,descriptor+0x18);
    static constexpr uint32_t r60[]{0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f38};
    static constexpr uint32_t r61[]{0x16f43,0x16f56,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f55};
    static constexpr uint32_t r62[]{0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f55,0x16f56,0x16f43,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f38};
    static constexpr uint32_t r63[]{0x16f3a,0x16f39,0x16f41,0x16f38};
    static constexpr uint32_t r64[]{0x16f55,0x16f43};
    static constexpr uint32_t r65[]{0x16f3a,0x16f39,0x16f41,0x16f55,0x16f43,0x16f38};
    const uint32_t* fields=nullptr;unsigned count=0;
    switch(rule) {case 60:fields=r60;count=10;break;case 61:fields=r61;count=8;break;case 62:fields=r62;count=18;break;case 63:fields=r63;count=4;break;case 64:fields=r64;count=2;break;case 65:fields=r65;count=6;break;default:return;}
    uint32_t sum=0;for(unsigned i=0;i<count;++i)sum+=byte(s,battle+fields[i]);
    checkBattleZeroTurnCount(sum,s); // Original caller ignores scalar status.
}
}
