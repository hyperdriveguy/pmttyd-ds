#include "battle_weapon_menu_main.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
std::optional<bool> resumePackedBattleMenuAudienceCheck(const BattleMenuAudienceServices& s,BattleMenuAudienceCursor& c){
 const auto& m=s.memory.memory;
 assert(m.word && m.storeWord && s.trigger);
 if(c.active && (c.owner.user!=m.user || c.owner.word!=m.word || c.owner.storeWord!=m.storeWord || c.output!=s.privateItem))return {};
 auto word=[&](uint32_t at){return m.word(m.user,at);};
 auto put=[&](uint32_t at,uint32_t value){m.storeWord(m.user,at,value);};
 auto finish=[&](bool value)->std::optional<bool>{c={};return value;};
 if(!c.active){
  c.active=true;c.owner=m;c.output=s.privateItem;c.battle=word(0x8041eb90);
  if(!s.trigger(s.user,0x400) || (word(c.battle+0xef4)&0x40000000u))return finish(false);
  assert(s.mario);const auto mario=s.mario(s.user,c.battle);
  if(word(mario+0x104)&0x10)return finish(false);
  c.phase=1;
 }
 if(c.phase==1){
  assert(s.itemOn && c.output);
  if(!s.itemOn(s.user,c.output,0,0,0,0))return {};
  const auto raw=word(c.output);std::memcpy(&c.item,&raw,4);
  if(c.item==-1)return finish(false);
  c.phase=2;
 }
 if(c.phase==2){assert(s.setTarget);s.setTarget(s.user,c.item);c.phase=3;}
 if(c.phase==3){
  if(s.endActMenuReady){if(!s.endActMenuReady(s.user,c.battle))return {};}
  else {assert(s.endActMenu);s.endActMenu(s.user,c.battle);}
  c.phase=4;
 }
 if(c.phase==4){
  const auto previous=word(c.battle+0x1c5c);
  put(c.battle+0x1c60,previous);put(c.battle+0x1c5c,11);put(c.battle+0x171c,29);
  assert(s.lightOff);s.lightOff(s.user,0,1);
  put(c.battle+0xef4,word(c.battle+0xef4)|4u);
  return finish(true);
 }
 return {};
}
}
