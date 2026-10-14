#include "battle_action_help.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
bool packedBattleActionHelpMain(const BattleActionHelpServices& s) {
    const auto& m=s.actors.memory;assert(m.word);
    const auto b=currentBattleAddress(s.actors);
    if(!m.word(m.user,b+0x19008) || !m.word(m.user,b+0x1900c) || (m.word(m.user,b+0xef4)&2))return true;
    const auto bits=m.word(m.user,0x804272b8);float priority;std::memcpy(&priority,&bits,4);
    return s.display && s.display(s.user,8,1,0x80216728,0,priority);
}
}
