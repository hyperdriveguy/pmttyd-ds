#include "battle_conclusion.hpp"
#include <cassert>
namespace ttyd {
namespace {
uint32_t word(const BattleConclusionServices& s,uint32_t a){assert(s.actors.memory.word);return s.actors.memory.word(s.actors.memory.user,a);}
void word(const BattleConclusionServices& s,uint32_t a,uint32_t v){assert(s.actors.memory.storeWord);s.actors.memory.storeWord(s.actors.memory.user,a,v);}
int32_t belong(const BattleConclusionServices& s,uint32_t u){assert(s.actors.byte);const auto b=s.actors.byte(s.actors.user,u+0xc);return b<128?int32_t(b):int32_t(b)-256;}
int32_t status(const BattleConclusionServices& s,uint32_t u){assert(s.checkStatus);return s.checkStatus(s.user,u,27);}
}
int32_t battleCheckConcluded(uint32_t b,const BattleConclusionServices& s){
 assert(s.actors.memory.half);unsigned count=0;
 for(unsigned group=0;group<3;++group){word(s,b+12+group*8,0);const auto config=s.actors.memory.half(s.actors.memory.user,b+8+group*8);
  bool concluded=config==0;
  if(config&1){bool found=false;for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.actors);if(!u || belong(s,u)!=int32_t(group))continue;
   if(status(s,u)!=0)continue;
   const auto attributes=word(s,u+0x104);
   if(attributes&(0x20000|0x10000000))continue;
   found=true;break;}concluded=!found;}
  if(!concluded && (s.actors.memory.half(s.actors.memory.user,b+8+group*8)&2)){bool found=false;for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.actors);if(!u || belong(s,u)!=int32_t(group))continue;
   if(!(word(s,u+0x104)&0x40000))continue;
   if(status(s,u)!=0)continue;
   if(word(s,u+0x104)&0x10000000)continue;
   found=true;break;}concluded=!found;}
  if(concluded){word(s,b+12+group*8,1);++count;}
 }
 return count>=2 || (word(s,b+0xef4)&0x30)?1:0;
}
int32_t battleUnitEventBarrier(uint32_t b,const BattleConclusionServices& s){assert(s.eventExists && s.afterReaction);int32_t ready=1;
 for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.actors);if(!u)continue;
  for(unsigned offset:{0x29cu,0x2a8u,0x2b4u,0x2bcu}){const auto event=word(s,u+offset);if(!event)continue;if(s.eventExists(s.user,event))ready=0;else word(s,u+offset,0);}}
 if(!ready)return 0;
 return s.afterReaction(s.user)!=0?0:1;
}
int32_t battleAllEventBarrier(uint32_t b,const BattleConclusionServices& s){if(!battleUnitEventBarrier(b,s))return 0;
 for(unsigned offset:{0x182b4u,0x182c4u,0x180fcu,0x182c8u,0x182c0u}){const auto event=word(s,b+offset);if(!event)continue;if(s.eventExists(s.user,event))return 0;word(s,b+offset,0);}return 1;
}
std::optional<int32_t> battleInitialEndResult(uint32_t b,const BattleConclusionServices& s){
 const auto mario=battleMarioUnit(b,s.actors);const int32_t own=belong(s,mario);assert(s.enemyBelong);const auto enemy=s.enemyBelong(s.user,mario);const auto flags=word(s,b+0xef4);
 if(flags&0x40)return 4;
 if(flags&0x20)return 5;
 if(word(s,b+12+8u*uint32_t(own)))return 3;
 if(word(s,b+12+8u*uint32_t(enemy)))return 1;
 if(flags&0x10)return 4;
 return {};
}
}
