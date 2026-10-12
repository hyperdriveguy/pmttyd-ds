#include "battle_actor.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleActorServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
uint8_t byte(const BattleActorServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
}
uint32_t currentBattleAddress(const BattleActorServices& s) {return word(s,0x8041eb90);}
uint32_t battleRegistryUnit(uint32_t b,int32_t id,const BattleActorServices& s) {if(id<0 || id>=64)return 0;return word(s,b+0x20+4u*uint32_t(id));}
void setBattleRegistryUnit(uint32_t b,int32_t id,uint32_t u,const BattleActorServices& s) {if(id<64) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,b+0x20+4u*uint32_t(id),u);}}
uint32_t battleMarioUnit(uint32_t b,const BattleActorServices& s) {
    for(int32_t i=0;i<64;++i) {const uint32_t u=battleRegistryUnit(b,i,s);if(u && sw(word(s,u+8))==222 && byte(s,u+0xc)==0)return u;}
    return 0;
}
uint32_t battlePartyUnit(uint32_t b,const BattleActorServices& s) {
    for(int32_t i=0;i<64;++i) {
        const uint32_t u=battleRegistryUnit(b,i,s);if(!u || (word(s,u+0x1c)&8))continue;
        const int32_t kind=sw(word(s,u+8));if(kind>=224 && kind<231 && byte(s,u+0xc)==0)return u;
    }
    return 0;
}
uint32_t battleSystemUnit(uint32_t b,const BattleActorServices& s) {
    for(int32_t i=0;i<64;++i) {const uint32_t u=battleRegistryUnit(b,i,s);if(u && sw(word(s,u+8))==220)return u;}
    return 0;
}
int32_t translateBattleActor(EventEntry* e,int32_t selector,const BattleActorServices& s) {
    const uint32_t b=currentBattleAddress(s); // Unconditional,including direct IDs.
    if(selector>=0)return selector;
    switch(selector) {
    case -1:return -1;
    case -2:assert(e);return e->owner;
    case -3:return sw(word(s,battleMarioUnit(b,s)));
    case -4: {const uint32_t u=battlePartyUnit(b,s);return u?sw(word(s,u)):-1;}
    case -5:return sw(word(s,battleSystemUnit(b,s)));
    case -6:return sw(word(s,b+0x424));
    default:return selector;
    }
}
}
