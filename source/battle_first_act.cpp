#include "battle_first_act.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t sw(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
bool packedBattleFirstAct(uint32_t b,const BattleFirstActServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeHalf && s.memory.byte);
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};auto byte=[&](uint32_t a){return s.memory.byte(s.memory.user,a);};
 auto set=[&](uint32_t state){packedBattleSetSequence(b,2,state,m);};auto increment=[&](){packedBattleIncrementSequence(b,2,m);};
 const auto info=word(b+0x2738);const auto first=sw(word(info+8));auto state=packedBattleGetSequence(b,2,m);
 if(state==0x02000000){
  if(first!=0){if(!s.firstAttack)return false;s.firstAttack(s.user,b,first);return true;}
  assert(s.create && s.fieldMarioMotion);store(b+0xefc,0);s.create(s.user,0x80416d70,10,0);const auto npcInfo=word(word(b+0x2738)+0xc);
  for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.memory);if(!u || !word(u+0x2b8))continue;
   const auto flags=word(npcInfo+4);const auto kind=sw(word(u+8));if((flags&0x100000) && (kind==222 || (kind>=224 && kind<231)))continue;
   const auto motion=s.fieldMarioMotion(s.user);uint32_t script;if(motion==26 && sw(word(u+8))==222){assert(s.unitData);script=s.unitData(s.user,u,65);}else script=word(u+0x2b8);
   auto& entry=s.create(s.user,script,10,0);store(u+0x2bc,entry.id);entry.owner=sw(word(u));
  }
  set(0x02000001);return true;
 }
 if(state==0x02000001){const auto remaining=word(b+0xefc)-1;store(b+0xefc,remaining);if(sw(remaining)>0)return true;
  assert(s.fadeEntry && s.sound);s.fadeEntry(s.user,23,200,word(0x80422550));store(b+0x19070,s.sound(s.user,0x802efbc4));set(0x02000002);state=0x02000002;
 }
 if(state==0x02000002){assert(s.fadeFinished && s.soundOff && s.allEventsReady && s.statusEventOn);
  if(s.fadeFinished(s.user)){const auto voice=word(b+0x19070);if(voice!=0xffffffff){s.soundOff(s.user,voice);store(b+0x19070,0xffffffff);}}
  if(s.allEventsReady(s.user,b))s.statusEventOn(s.user);
  if(!s.allEventsReady(s.user,b) || !s.fadeFinished(s.user))return true;
  set(0x02000003);const auto u=battleRegistryUnit(b,sw(word(b+0x420)),s.memory);if(u)store(u+0x1c,word(u+0x1c)&~uint32_t(0x04000000));return true;
 }
 if(state==0x02000003){assert(s.concluded && s.audiencePerAct && s.statusAdjust);if(s.concluded(s.user,b)){s.audiencePerAct(s.user);packedBattleSetSequence(b,0,3,m);packedBattleSetSequence(b,7,0x07000000,m);return true;}
  set(0x02000004);s.statusAdjust(s.user,b);state=0x02000004;
 }
 if(state==0x02000004){assert(s.allEventsReady && s.statusMessage && s.statusAnnounce);if(!s.allEventsReady(s.user,b))return true;const auto message=s.statusMessage(s.user,b),announce=s.statusAnnounce(s.user,b);if(message|announce)return true;increment();state=0x02000005;}
 if(state==0x02000005){assert(s.statusSystemOn && s.consumeReserve && s.audiencePerAct);if(!(word(b+0xef4)&0x30))s.statusSystemOn(s.user);s.consumeReserve(s.user);store(b+0xef4,word(b+0xef4)&~uint32_t(4));s.audiencePerAct(s.user);increment();state=0x02000006;}
 if(state==0x02000006){assert(s.audienceReaction && s.concluded && s.slotReaction);if(uint8_t(s.audienceReaction(s.user)))return true;if(!s.concluded(s.user,b) && uint8_t(s.slotReaction(s.user)))return true;increment();state=0x02000007;}
 if(state==0x02000007){assert(s.statusMessage && s.statusAnnounce);const auto message=s.statusMessage(s.user,b),announce=s.statusAnnounce(s.user,b);if(message|announce)return true;increment();state=0x02000008;}
 if(state==0x02000008){assert(s.allEventsReady && s.wallClose);if(!s.allEventsReady(s.user,b))return true;const auto npc=word(word(b+0x2738)+0xc);if(word(npc+4)&0x10){set(0x02000009);s.wallClose(s.user);}else set(0x0200000a);return true;}
 if(state==0x02000009){assert(s.eventExists);const auto event=word(b+0x182c0);if(event && !s.eventExists(s.user,event))store(b+0x182c0,0);if(word(b+0x182c0))return true;set(0x0200000a);return true;}
 if(state==0x0200000a){assert(s.status && s.create && s.statusAdjust);store(b+0x1905c,0);bool luck=false;
  for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.memory);if(u && byte(u+0x304) && !s.status(s.user,u,27))luck=true;}
  if(luck){auto& event=s.create(s.user,0x80416d40,10,0);event.owner=-5;store(b+0x1905c,event.id);store(b+0x18ff4,word(b+0x18ff4)|0xffffffff);store(b+0x18ff0,word(b+0x18ff0)|0xffffffff);s.statusAdjust(s.user,b);set(0x0200000b);m.storeHalf(m.user,b+0x18ffa,0);}else set(0x0200000c);return true;
 }
 if(state==0x0200000b){assert(s.eventExists && s.allEventsReady && s.statusAnnounce && s.statusMessage);const auto count=m.half(m.user,b+0x18ffa);if(count<300)m.storeHalf(m.user,b+0x18ffa,uint16_t(count+1));bool ready=true;const auto event=word(b+0x1905c);if(event){if(s.eventExists(s.user,event))ready=false;else store(b+0x1905c,0);}if(!s.allEventsReady(s.user,b))ready=false;if(s.statusAnnounce(s.user,b))ready=false;if(s.statusMessage(s.user,b))ready=false;if(ready)set(0x0200000c);return true;}
 if(state==0x0200000c){if(word(b+0xef4)&2){set(0x0200000e);return true;}assert(s.nozzleCheck);s.nozzleCheck(s.user,0);set(0x0200000d);state=0x0200000d;}
 if(state==0x0200000d){assert(s.allEventsReady);if(!s.allEventsReady(s.user,b))return true;set(0x0200000e);state=0x0200000e;}
 if(state==0x0200000e){store(b+0xef4,word(b+0xef4)&~uint32_t(2));packedBattleSetSequence(b,0,2,m);packedBattleSetSequence(b,3,0x03000000,m);}
 return true;
}
}
