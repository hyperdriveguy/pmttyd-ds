#include "battle_weapon_menu_decision.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t sw(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}int32_t sb(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}}
std::optional<int32_t> decidePackedBattleWeapon(uint32_t b,uint32_t type,uint32_t index,const BattleWeaponMenuDecisionServices& s){
 const auto& m=s.memory;assert(m.memory.word && m.memory.half && m.byte && m.memory.storeByte && s.unit && s.build && s.enemyBelong && s.sample);
 auto word=[&](uint32_t at){return m.memory.word(m.memory.user,at);};auto byte=[&](uint32_t at){return m.byte(m.user,at);};
 const auto buffer=b+0x428,u=s.unit(s.user,b,sw(word(b+0x420)));if(!s.build(s.user,b,type))return std::nullopt;
 const auto weapon=word(b+0x179c+28*index);const auto rawEnemy=s.enemyBelong(s.user,u);const auto owner=sb(byte(u+0xc));const auto enemy=sb(uint8_t(rawEnemy));
 const auto actor=sw(word(b+0x420));const auto admission=word(weapon+0x64),attributes=word(weapon+0x68);const auto direction=sb(byte(b+uint32_t(owner)*8+0xa));
 if(!s.sample(s.user,buffer,weapon,actor,enemy,admission,attributes,direction))return std::nullopt;
 m.memory.storeByte(m.memory.user,buffer+0xab7,0);
 if(!(word(weapon+0x68)&0x30000000))return 0;
 assert(s.belong);const auto side=sb(uint8_t(s.belong(s.user,u)));int32_t chosen=-1;
 for(int32_t j=0;j<sb(byte(buffer+0xa6c));++j){const auto permutation=sb(byte(buffer+0xa6d+uint32_t(j)));const auto row=buffer+4+36*uint32_t(permutation);const auto raw=m.memory.half(m.memory.user,row);const auto id=raw<32768?int32_t(raw):int32_t(raw)-65536;const auto candidate=s.unit(s.user,b,id);const auto candidateSide=sb(uint8_t(s.belong(s.user,candidate)));
  if(word(weapon+0x68)&0x10000000){if(candidateSide==side)chosen=j;}else if(candidateSide!=side){chosen=j;break;}
 }
 if(chosen>=0)m.memory.storeByte(m.memory.user,buffer+0xab7,uint8_t(chosen));
 return 0;
}
bool setupPackedBattleWeaponMenu(uint32_t b,uint32_t type,uint32_t flags,const BattleWeaponMenuDecisionServices& s){
 const auto& m=s.memory;assert(m.memory.word && m.memory.storeWord && s.build && s.cursor && s.initialize && s.privateCursor);
 const auto menu=b+0x171c;if(!s.build(s.user,b,type))return false;s.cursor(s.user,menu,type,s.privateCursor);
 const auto cursor=m.memory.word(m.memory.user,s.privateCursor);if(!s.initialize(s.user,b,cursor,menu+0x80,flags))return false;
 const auto old=m.memory.word(m.memory.user,menu+4);m.memory.storeWord(m.memory.user,menu+4,old|0x01000010);return true;
}
}
