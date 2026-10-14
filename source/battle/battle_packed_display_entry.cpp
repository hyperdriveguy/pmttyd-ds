#include "battle_packed_display_entry.hpp"
#include <cassert>
namespace ttyd {
uint32_t searchPackedBattlePoseTable(uint32_t table,int32_t type,const EventMemoryServices& m){if(!table)return 0;assert(m.word);for(unsigned i=0;i<28;++i,table+=8){const auto tag=m.word(m.user,table);if(tag==uint32_t(type) || tag==28)return m.word(m.user,table+4);}return 0;}
void initializePackedBattleDisplay(uint32_t u,const PackedBattleDisplayEntryServices& s){const auto& m=s.memory.memory;assert(m.word && m.storeWord && m.storeByte && s.memory.byte);auto w=[&](uint32_t a){return m.word(m.user,a);};auto part=w(u+0x14);
 while(part){const auto table=w(part+0x1bc);if(!table)m.storeWord(m.user,part+0x1c0,0xffffffff);else{const auto bank=w(w(part+4)+8);if(!bank)m.storeWord(m.user,part+0x1c0,0xffffffff);else{const auto pose=searchPackedBattlePoseTable(table,28,m);assert(pose && s.poseEntry);uint32_t i=0;for(;;++i){const auto c=s.memory.byte(s.memory.user,pose+i);m.storeByte(m.user,part+0x1c4+i,c);if(!c)break;}m.storeWord(m.user,part+0x1c0,uint32_t(s.poseEntry(s.user,bank,1)));}}part=w(part);}
}
}
