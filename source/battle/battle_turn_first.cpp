#include "battle_turn_first.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleTurnFirstServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
uint8_t byte(const BattleTurnFirstServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
void storeWord(const BattleTurnFirstServices& s,uint32_t a,uint32_t v) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,a,v);}
void storeByte(const BattleTurnFirstServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v) {return v<128?int32_t(v):int32_t(v)-256;}
int32_t sh(uint16_t v) {return v<32768?int32_t(v):int32_t(v)-65536;}
}
uint32_t battleUnitStatusFlags(uint32_t unit,uint32_t mask,const BattleTurnFirstServices& s) {return unit?word(s,unit+0x138)&mask:0;}
void clearBattleUnitStatusFlags(uint32_t unit,uint32_t mask,const BattleTurnFirstServices& s) {storeWord(s,unit+0x138,word(s,unit+0x138)&~mask);}
int32_t battleFogEndCheck(const BattleTurnFirstServices& s) {
    const uint32_t battle=word(s,0x8041eb90),timer=word(s,battle+0x182b8);if(timer==0)return 1;
    storeWord(s,battle+0x182b8,timer-1);
    if(sw(word(s,battle+0x182b8))>0)return 0;
    const uint32_t effect=word(s,battle+0x182bc);
    if(effect) {assert(s.softDelete);s.softDelete(s.user,effect);storeWord(s,battle+0x182bc,0);}return 1;
}
void battleTurnFirstProcess(uint32_t battle,const BattleTurnFirstServices& s) {
    assert(s.judgeTurnRule && s.memory.half && s.memory.storeHalf);s.judgeTurnRule(s.user);
    const uint16_t old=s.memory.half(s.memory.user,battle);s.memory.storeHalf(s.memory.user,battle,uint16_t(old+1));
    if(sh(s.memory.half(s.memory.user,battle))>=2) {assert(s.fortune);s.fortune(s.user);}
    storeWord(s,battle+0x18ff4,0);storeWord(s,battle+0x18ff0,0);
    const uint8_t count=byte(s,battle+0x16f57);if(count<100)storeByte(s,battle+0x16f57,uint8_t(count+1));
    BattleActorServices actors;actors.memory=s.memory;
    for(int32_t i=0;i<64;++i) {
        const uint32_t unit=battleRegistryUnit(battle,i,actors);if(!unit)continue;
        storeByte(s,unit+0x24,uint8_t(byte(s,unit+0x24)+1));assert(s.resetMove);s.resetMove(s.user,unit);
        const uint32_t kind=word(s,unit+0x10);const int32_t span=sb(byte(s,kind+0x89));assert(s.irand);
        const int32_t random=s.irand(s.user,span),half=sb(byte(s,kind+0x89))/2,base=sb(byte(s,kind+0x88));
        storeWord(s,unit+0x28,uint32_t(base)+uint32_t(random)-uint32_t(half));
        if(battleUnitStatusFlags(unit,0x01000000,s)!=0)clearBattleUnitStatusFlags(unit,0x01000000,s);
    }
    battleFogEndCheck(s); // Caller ignores fog result.
}
}
