#include "battle_action_help.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
void packedBattleActionHelpSetKey(uint32_t key,const BattleActorServices& s){const auto& m=s.memory;const auto b=currentBattleAddress(s);m.storeWord(m.user,b+0x19008,key);}
void packedBattleActionHelpSetDisplay(int32_t mode,const BattleActorServices& s){
 const auto& m=s.memory;const auto b=currentBattleAddress(s);m.storeWord(m.user,b+0x1900c,uint32_t(mode));
 // WinSetActive independently resolves the current inline audience window.
 const auto current=currentBattleAddress(s);m.storeByte(m.user,current+0x2858+0x1375c,mode==0?1:0);
}
int32_t packedBattleStartActionCommandEvent(EventEntry& e,const BattleUnitEventServices& u,const BattleActorServices& s){
 const auto& m=u.memory;const auto args=e.arguments,b=currentBattleAddress(s);const auto raw=m.word(m.user,args);
 const auto on=u.values.resolve(u.values.user,e,raw<0x80000000u?int32_t(raw):int32_t(int64_t(raw)-0x100000000LL));
 m.storeWord(m.user,b+0x1c9c,100);
 if(on){const auto actor=m.word(m.user,b+0x1c90);if(!(m.word(m.user,actor+0x27c)&0x10))packedBattleActionHelpSetDisplay(2,s);}
 return 2;
}
bool packedBattleActionHelpMain(const BattleActionHelpServices& s) {
    const auto& m=s.actors.memory;assert(m.word);
    const auto b=currentBattleAddress(s.actors);
    if(!m.word(m.user,b+0x19008) || !m.word(m.user,b+0x1900c) || (m.word(m.user,b+0xef4)&2))return true;
    const auto bits=m.word(m.user,0x804272b8);float priority;std::memcpy(&priority,&bits,4);
    return s.display && s.display(s.user,8,1,0x80216728,0,priority);
}
}
