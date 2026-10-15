#include "battle_packed_hit_event.hpp"
#include <cassert>
namespace ttyd {
// Delete/clear IDs2b4, 290, 29c, 2a8 in order, even with script0. Nonzero
// script creates priority10/flags20 root, owner=unit ID, stores code278/ID2b4.
// Successful deletion precedes next read; no rollback or per-event map free.
uint32_t packedBattleRunHitEventDirect(uint32_t u,uint32_t code,uint32_t script,const BattleUnitEventServices& s){
 assert(s.memory.word && s.memory.storeWord);
 for(unsigned offset:{0x2b4u,0x290u,0x29cu,0x2a8u}){
  const auto id=s.memory.word(s.memory.user,u+offset);
  if(id){assert(s.removeEventId);s.removeEventId(s.user,id);s.memory.storeWord(s.memory.user,u+offset,0);}
 }
 if(!script)return 0;
 assert(s.createWait);auto& event=s.createWait(s.user,script,10,0x20);
 event.owner=int32_t(s.memory.word(s.memory.user,u));
 s.memory.storeWord(s.memory.user,u+0x278,code);
 s.memory.storeWord(s.memory.user,u+0x2b4,event.id);
 return event.id;
}
// Ordinary wrapper skips code low8==40 without data key46, then skips null
// script2b0. Those routes do NOT invoke Direct, so old events remain untouched.
uint32_t packedBattleRunHitEvent(uint32_t u,uint32_t code,const BattleUnitEventServices& s){
 if((code&255)==40 && !battleUnitHasData(u,46,s.memory))return 0;
 assert(s.memory.word);const auto script=s.memory.word(s.memory.user,u+0x2b0);
 return script?packedBattleRunHitEventDirect(u,code,script,s):0;
}
}
