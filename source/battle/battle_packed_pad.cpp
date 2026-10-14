#include "battle_packed_pad.hpp"
#include <cassert>
namespace ttyd {
BattlePadWork readPackedBattlePad(uint32_t a,const BattleActorServices& s){assert(s.memory.half && s.memory.word && s.byte);BattlePadWork p;
 auto snapshot=[&](uint32_t at,BattlePadSnapshot& v){v.buttons=s.memory.half(s.memory.user,at);v.mainX=int8_t(s.byte(s.user,at+2));v.mainY=int8_t(s.byte(s.user,at+3));v.subX=int8_t(s.byte(s.user,at+4));v.subY=int8_t(s.byte(s.user,at+5));v.leftTrigger=s.byte(s.user,at+6);v.rightTrigger=s.byte(s.user,at+7);v.analogA=s.byte(s.user,at+8);v.analogB=s.byte(s.user,at+9);v.retainedA=s.byte(s.user,at+10);v.retainedB=s.byte(s.user,at+11);};snapshot(a,p.current);snapshot(a+12,p.previous);
 for(unsigned i=0;i<30;++i){p.triggers.entries[i]=s.memory.word(s.memory.user,a+0x18+4*i);p.repeats.entries[i]=s.memory.word(s.memory.user,a+0x90+4*i);p.held.entries[i]=s.memory.word(s.memory.user,a+0x108+4*i);p.released[i]=s.memory.word(s.memory.user,a+0x180+4*i);}p.repeatPhase=s.byte(s.user,a+0x1f8);p.repeatCounter=s.byte(s.user,a+0x1f9);return p;
}
void writePackedBattlePad(uint32_t a,const BattlePadWork& p,const BattleActorServices& s){assert(s.memory.storeHalf && s.memory.storeWord && s.memory.storeByte);
 auto snapshot=[&](uint32_t at,const BattlePadSnapshot& v){s.memory.storeHalf(s.memory.user,at,v.buttons);const std::array<uint8_t,10> b{{uint8_t(v.mainX),uint8_t(v.mainY),uint8_t(v.subX),uint8_t(v.subY),v.leftTrigger,v.rightTrigger,v.analogA,v.analogB,v.retainedA,v.retainedB}};for(unsigned i=0;i<10;++i)s.memory.storeByte(s.memory.user,at+2+i,b[i]);};snapshot(a,p.current);snapshot(a+12,p.previous);
 for(unsigned i=0;i<30;++i){s.memory.storeWord(s.memory.user,a+0x18+4*i,p.triggers.entries[i]);s.memory.storeWord(s.memory.user,a+0x90+4*i,p.repeats.entries[i]);s.memory.storeWord(s.memory.user,a+0x108+4*i,p.held.entries[i]);s.memory.storeWord(s.memory.user,a+0x180+4*i,p.released[i]);}s.memory.storeByte(s.memory.user,a+0x1f8,p.repeatPhase);s.memory.storeByte(s.memory.user,a+0x1f9,p.repeatCounter);
}
}
