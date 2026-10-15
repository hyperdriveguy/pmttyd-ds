#include "battle_weapon_menu_main.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t sw(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}int32_t sb(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}}
bool packedBattleMenuKeyAllowed(uint32_t b,const BattleActorServices& m){assert(m.byte && m.memory.word);if(m.byte(m.user,b+0x1c74))return true;return m.byte(m.user,m.memory.word(m.memory.user,b+0x1c78)+0x14)!=0;}
std::optional<bool> packedBattleMenuAudienceCheck(const BattleMenuAudienceServices& s){
 BattleMenuAudienceCursor cursor;return resumePackedBattleMenuAudienceCheck(s,cursor);
}
bool endPackedBattleWeaponMenu(uint32_t b,const BattleWeaponMenuMainServices& s){assert(s.endUI && s.memory.memory.word && s.memory.memory.storeWord);if(!s.endUI(s.user,b))return false;const auto at=b+0x1720;const auto flags=s.memory.memory.word(s.memory.memory.user,at);s.memory.memory.storeWord(s.memory.memory.user,at,flags&~0x10u);return true;}
std::optional<int32_t> mainPackedBattleWeaponMenu(uint32_t b,uint32_t type,const BattleWeaponMenuMainServices& s){
 const auto& m=s.memory;assert(m.memory.word && m.memory.storeWord && m.byte && m.memory.storeByte && s.unit && s.keyAllowed && s.display);
 auto word=[&](uint32_t at){return m.memory.word(m.memory.user,at);};auto store=[&](uint32_t at,uint32_t v){m.memory.storeWord(m.memory.user,at,v);};auto byte=[&](uint32_t at){return m.byte(m.user,at);};auto putByte=[&](uint32_t at,uint8_t v){m.memory.storeByte(m.memory.user,at,v);};
 const auto menu=b+0x171c,u=s.unit(s.user,b,sw(word(b+0x420)));int32_t result=0;
 auto finish=[&]()->std::optional<int32_t>{s.display(s.user,b);return result;};
 if(!s.keyAllowed(s.user,b))return finish();
 assert(s.cursor && s.privateCursor && s.update && s.trigger && s.repeat && s.sound);s.cursor(s.user,menu,type,s.privateCursor);if(!s.update(s.user,b,type))return std::nullopt;
 auto cursor=[&](){return word(s.privateCursor);};auto row=[&](){return menu+28*word(cursor());};
 if(byte(b+0x1c74)){putByte(b+0x1c74,0);store(b+0x1c70,0);}
 const auto confirmation=s.trigger(s.user,0x100);
 if(confirmation || byte(b+0x1c74)){
  if(word(row()+0x84)){
   s.sound(s.user,2520);if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;
   const auto selected=word(cursor());assert(s.decide && s.targetSetup);if(!s.decide(s.user,b,type,selected) || !s.targetSetup(s.user,b))return std::nullopt;store(menu,10);
   const auto first=word(row()+0x80);if(!(word(first+0x64)&0x200)){assert(s.lightFriend);s.lightFriend(s.user);}
   const auto second=word(row()+0x80),flags=word(second+0x64);if(!(flags&0x4110)){assert(s.lightEnemy);s.lightEnemy(s.user);}result=1;
  }else{
   if(!byte(b+0x1c74))s.sound(s.user,0x802efe88);
   putByte(b+0x1c74,0);
   if(word(row()+0x98)){if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;store(menu,8);store(menu+0x564,word(row()+0x98));result=1;}
  }return finish();
 }
 const auto c=cursor();const auto count=word(c+8),index=word(c);
 if(sw(index)<sw(count-1)){
  if(s.repeat(s.user,0x20000)){const auto fresh=cursor();store(fresh,word(fresh)+1);s.sound(s.user,0x802efe98);return finish();}
 }else if(s.trigger(s.user,0x20000)){store(cursor(),0);s.sound(s.user,0x802efe98);return finish();}
 const auto reverse=cursor();if(sw(word(reverse))>0){if(s.repeat(s.user,0x10000)){const auto fresh=cursor();store(fresh,word(fresh)-1);s.sound(s.user,0x802efe98);return finish();}}
 else if(s.trigger(s.user,0x10000)){const auto fresh=cursor();store(fresh,word(fresh+8)-1);s.sound(s.user,0x802efe98);return finish();}
 if(s.trigger(s.user,0x200)){
  bool special=false;if(type==2){special=byte(u+0x2f3)!=0;if(!special)special=byte(u+0x2f4)!=0;}
  if(special){if(sb(byte(u+0x20))==0){s.sound(s.user,2521);if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;store(menu,15);}else{if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;store(menu,12);}}
  else{s.sound(s.user,2521);if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;store(menu,1);}result=1;return finish();
 }
 assert(s.attackAudience);const auto audience=s.attackAudience(s.user);if(!audience)return std::nullopt;if(*audience){if(!endPackedBattleWeaponMenu(b,s))return std::nullopt;putByte(b+0x1c74,1);}return finish();
}
}
