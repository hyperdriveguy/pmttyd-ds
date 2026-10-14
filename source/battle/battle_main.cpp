#include "battle_main.hpp"
#include "battle_sequence_startup.hpp"
#include <cassert>

namespace ttyd {
std::optional<int32_t> packedBattleMain(const BattleMainServices& s) {
    const auto b=currentBattleAddress(s.actors);
    const auto& m=s.actors.memory;
    assert(s.request && m.word && m.storeWord);
    for(const auto fn:battleMainRequests)
        if(!s.request(s.user,fn,b))return std::nullopt;
    if(packedBattleGetSequence(b,0,m)==4)return 0;
    const auto gp=m.word(m.user,0x80415fd0),flags=m.word(m.user,gp);
    if((flags&0x3000)==0x3000 && packedBattleGetSequence(b,0,m)==2) {
        m.storeWord(m.user,b+0xef4,m.word(m.user,b+0xef4)|0x60);
        packedBattleSetSequence(b,0,3,m);
        packedBattleSetSequence(b,7,0x07000000,m);
    }
    return 1;
}
bool packedBattleMainWrapper(const BattleMainServices& s) {
    const auto b=currentBattleAddress(s.actors);
    if(!b)return true;
    const auto result=packedBattleMain(s);
    if(!result)return false;
    if(*result==0) {
        const auto& m=s.actors.memory;
        m.storeWord(m.user,b+0xef4,m.word(m.user,b+0xef4)|0x80000000);
    }
    return true;
}
bool packedBattleSequenceEndCheck(const BattleActorServices& s) {
    // Source leaf has no null guard; its caller owns battle lifetime.
    const auto b=currentBattleAddress(s);
    return (s.memory.word(s.memory.user,b+0xef4)&0x80000000)!=0;
}
}
