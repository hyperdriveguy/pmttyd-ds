#include "battle_status_window.hpp"
#include <cassert>
namespace ttyd {
// Show = !273c && !2740 && (274c || (2748 &&2744)), offsets hex.
// Always query actual isOpen before transition; return desired predicate0/1.
int32_t packedBattleStatusWindowCheck(const BattleStatusWindowServices& s){
 const auto& m=s.units.memory;assert(m.word && s.isOpen);
 const auto b=m.word(m.user,0x8041eb90);
 const bool show=!m.word(m.user,b+0x273c) && !m.word(m.user,b+0x2740) && (m.word(m.user,b+0x274c) || (m.word(m.user,b+0x2748) && m.word(m.user,b+0x2744)));
 const bool current=s.isOpen(s.user);
 if(show && !current){assert(s.open);s.open(s.user);}else if(!show && current){assert(s.close);s.close(s.user);}
 return show?1:0;
}
void packedBattleStatusWindowSet(uint32_t field,bool on,const BattleStatusWindowServices& s){
 const auto& m=s.units.memory;assert(m.word && m.storeWord);const auto b=m.word(m.user,0x8041eb90);
 m.storeWord(m.user,b+field,on?1:0);(void)packedBattleStatusWindowCheck(s);
}
// Capture B before operand resolution. Args0/1 write2748; 2 sets latch2744;
// other values leave storage. Check independently recaptures B afterward.
// Completion2 is not the predicate or a guarantee of same-owner publication.
int32_t battleStatusWindowOnOffEvent(EventEntry& e,const BattleStatusWindowServices& s){
 const auto& m=s.units.memory;const auto& v=s.units.values;assert(m.word && m.storeWord && v.resolve);
 const auto args=e.arguments,b=m.word(m.user,0x8041eb90);const auto raw=m.word(m.user,args);
 const auto argument=v.resolve(v.user,e,raw<=0x7fffffffu?int32_t(raw):int32_t(int64_t(raw)-0x100000000LL));
 if(argument==0 || argument==1)m.storeWord(m.user,b+0x2748,uint32_t(argument));
 else if(argument==2)m.storeWord(m.user,b+0x2744,1);
 (void)packedBattleStatusWindowCheck(s);return 2;
}
}
