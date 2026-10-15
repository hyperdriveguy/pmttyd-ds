#include "battle_audience_item_on.hpp"
#include "battle_audience_population.hpp"
namespace ttyd {
namespace {
uint32_t member(int32_t slot,const BattleActorServices& a){
 return currentBattleAddress(a)+0x2858+0x1bc+uint32_t(slot)*0x134;
}
bool exists(int32_t slot,const BattleActorServices& a){
 (void)currentBattleAddress(a); // Original BaseGetPtr before GetPtr, even though unused.
 const auto flags=a.memory.word(a.memory.user,member(slot,a));
 return (flags&1) && !(flags&0x100) && !(flags&2);
}
}
void packedBattleAudienceSetTarget(int32_t selected,const BattleActorServices& a){
 const auto& m=a.memory;const auto target=member(selected,a);
 m.storeWord(m.user,target,m.word(m.user,target)|0x20u);
 for(int32_t slot=0;slot<200;++slot){
  if(slot==selected || !exists(slot,a))continue;
  const auto actor=member(slot,a);
  if(a.byte(a.user,actor+0x19)!=7)continue;
  const uint16_t raw=m.half(m.user,actor+0x1c);
  const int32_t held=raw<32768?int32_t(raw):int32_t(raw)-65536;
  const auto item=currentBattleAddress(a)+0x2858+0xf25c+uint32_t(held)*0x48;
  m.storeWord(m.user,item,0);
  m.storeHalf(m.user,actor+0x1c,0xffff);
  packedBattleAudienceChangeStatus(slot,0,a);
 }
}
}
