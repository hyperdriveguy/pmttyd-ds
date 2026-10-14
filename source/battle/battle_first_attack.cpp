#include "battle_first_attack.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleActorServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
uint32_t packedBattlePartnerUnit(uint32_t battle,uint32_t unit,const BattleActorServices& s) {
    return word(s,unit+8)==222?battlePartyUnit(battle,s):battleMarioUnit(battle,s);
}
uint32_t packedFirstAttackPair(uint32_t battle,uint32_t* attacker,uint32_t* target,const BattleActorServices& s) {
    uint32_t candidate=0;
    for(int32_t i=0;i<64;++i) {candidate=battleRegistryUnit(battle,i,s);if(candidate) {assert(s.byte);if(s.byte(s.user,candidate+0xc)==1)break;}}
    const uint32_t setup=word(s,battle+0x2738);const int32_t mode=sw(word(s,setup+8));
    const uint32_t mario=battleMarioUnit(battle,s);uint32_t first,second,raw;
    if(mode>=1 && mode<=6) {first=mario;second=candidate;raw=mario;}
    else if(mode==7 || mode==8) {first=packedBattlePartnerUnit(battle,mario,s);second=candidate;raw=first;}
    else {first=candidate;second=mario;raw=mario;}
    assert(attacker && target);*attacker=first;*target=second;return raw;
}
int32_t packedFirstAttackTargetEvent(EventEntry& e,const BattleFirstAttackServices& s) {
    const uint32_t args=e.arguments,battle=currentBattleAddress(s.memory);
    const int32_t first=sw(word(s.memory,args)),second=sw(word(s.memory,args+4));
    uint32_t attacker=0,target=0;assert(s.pair);s.pair(s.user,battle,&attacker,&target);
    const int32_t part=packedBodyPartId(target,s.memory.memory);
    const int32_t id=sw(word(s.memory,target)); // Target slot reread AFTER body traversal;no null guard.
    assert(s.values.store);s.values.store(s.values.user,e,first,id);s.values.store(s.values.user,e,second,part);return 2;
}
}
