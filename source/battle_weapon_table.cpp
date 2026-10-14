#include "battle_weapon_table.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
std::optional<int32_t> packedWeaponTargetCount(uint32_t b,uint32_t u,uint32_t w,const BattleWeaponTargetCountServices& s){
 const auto& m=s.memory;assert(m.memory.word);auto word=[&](uint32_t at){return m.memory.word(m.memory.user,at);};
 if(!w || !word(w+0xb0))return 0;
 assert(s.cost);if(!s.cost(s.user,u,w))return -1;
 assert(s.enemyBelong && m.byte && s.sample && s.privateBuffer);const auto enemy=s.enemyBelong(s.user,u);
 const auto actor=word(u),admission=word(w+0x64),attributes=word(w+0x68);const auto raw=m.byte(m.user,b+uint32_t(enemy)*8+0xa);
 auto signedByte=[](uint8_t v){return v<128?int32_t(v):int32_t(v)-256;};
 if(!s.sample(s.user,s.privateBuffer,w,signedWord(actor),signedByte(uint8_t(enemy)),admission,attributes,signedByte(uint8_t(-raw))))return std::nullopt;
 return signedByte(m.byte(m.user,s.privateBuffer+0xa6c));
}
bool updatePackedBattleWeaponTable(uint32_t b,uint32_t type,const BattleWeaponEligibilityServices& s){
 const auto& m=s.memory;assert(m.memory.word && m.memory.storeWord && s.unit && s.cursor && s.cursorSlot && s.targetCount);
 auto word=[&](uint32_t at){return m.memory.word(m.memory.user,at);};auto store=[&](uint32_t at,uint32_t v){m.memory.storeWord(m.memory.user,at,v);};
 const auto u=s.unit(s.user,b,signedWord(word(b+0x420))),menu=b+0x171c;s.cursor(s.user,menu,type,s.cursorSlot);
 const auto count=signedWord(word(word(s.cursorSlot)+8));
 for(int32_t j=0;j<count;++j){const auto row=menu+28*uint32_t(j),w=word(row+0x80);store(row+0x84,0);store(row+0x98,0);if(!w)continue;
  const auto targets=s.targetCount(s.user,b,u,w);if(!targets)return false;
  if(*targets>0){store(row+0x84,1);store(row+0x98,0);}else store(row+0x98,*targets<0?(type==4?2:1):(type==0?4:type==1?5:3));
 }return true;
}
std::optional<int32_t> makePackedBattleWeaponTable(uint32_t b,uint32_t type,const BattleWeaponTableServices& s){
 const auto& m=s.memory;assert(m.memory.word && m.memory.storeWord && m.memory.half && m.memory.storeHalf && m.byte && s.unit && s.pouch);
 auto word=[&](uint32_t at){return m.memory.word(m.memory.user,at);};
 auto store=[&](uint32_t at,uint32_t v){m.memory.storeWord(m.memory.user,at,v);};
 auto half=[&](uint32_t at){return m.memory.half(m.memory.user,at);};
 auto shalf=[&](uint32_t at){const auto v=half(at);return v<32768?int32_t(v):int32_t(v)-65536;};
 auto byte=[&](uint32_t at){const auto v=m.byte(m.user,at);return v<128?int32_t(v):int32_t(v)-256;};
 const auto u=s.unit(s.user,b,signedWord(word(b+0x420))),menu=b+0x171c,p=s.pouch(s.user);
 if(!menu)return 0;
 assert(s.cursor && s.cursorSlot && s.countSlot && s.cursorSlot!=s.countSlot);
 s.cursor(s.user,menu,type,s.cursorSlot);store(s.countSlot,0);
 auto row=[&](){return menu+28*word(s.countSlot);};
 auto increment=[&](){store(s.countSlot,word(s.countSlot)+1);};
 auto name=[&](){const auto w=word(row()+0x80);auto key=word(w);if(!key)key=word(0x803108a8+40*word(w+8)+4);assert(s.message);const auto result=s.message(s.user,key);store(row()+0x88,result);};
 auto fromItem=[&](uint32_t ordinal,uint32_t itemId,uint32_t item){store(row()+0x90,ordinal);store(row()+0x94,itemId);const auto weapon=word(item+0x24);store(row()+0x80,weapon);store(row()+0x84,0);store(row()+0x98,0);};
 if(type==0 || type==1){
  const uint32_t weapons[2][3]{{0x803538a8,0x80353968,0x80353ae8},{0x80353de8,0x80353ea8,0x80354028}};
  for(unsigned level=1;level<=3;++level)if(byte(p+(type==0?0x98:0x99))>=int32_t(level)){
   if(level==1)store(menu+0x90,0xffffffff);else store(row()+0x90,0xffffffff);
   store(row()+0x94,0);store(row()+0x80,weapons[type][level-1]);store(row()+0x84,0);store(row()+0x98,0);
   assert(type==0?s.jumpIcon:s.hammerIcon);const auto icon=type==0?s.jumpIcon(s.user):s.hammerIcon(s.user);m.memory.storeHalf(m.memory.user,row()+0x8c,icon);name();
   if(level==1 && (word(b+0xef4)&(type==0?0x09000000u:0x0a000000u))){const auto id=signedWord(word(b+0x424));if(id!=-1){const auto owner=s.unit(s.user,b,id);const auto replacement=packedUnitData(owner,type==0?68:69,m);store(row()+0x80,replacement);}}
   increment();
  }
  const unsigned count=type==0?8:9;const auto array=type==0?0x80416d80u:0x80416da0u;
  assert(s.equipped);for(unsigned index=0;index<count;++index){const auto badge=word(array+4*index);if(s.equipped(s.user,signedWord(badge))<=0)continue;const auto item=0x803108a8+40*badge;
   fromItem(index,badge,item);const auto w=word(row()+0x80);const auto icon=word(w+8)?half(item+0x20):half(w+4);m.memory.storeHalf(m.memory.user,row()+0x8c,icon);name();increment();
  }
 }
 if(type==2){
  for(unsigned index=0;index<20;++index){const auto id=shalf(p+0x192+2*index);const auto item=0x803108a8+40*uint32_t(id);if(!id || !(half(item+0x10)&2))continue;
   fromItem(index,uint32_t(id),item);if(!word(row()+0x80)){store(row()+0x80,0x80365488);assert(s.message);const auto label=s.message(s.user,word(item+4));store(row()+0x88,label);m.memory.storeHalf(m.memory.user,row()+0x8c,half(item+0x20));}
   else{m.memory.storeHalf(m.memory.user,row()+0x8c,half(item+0x20));name();}
   increment();if(signedWord(word(s.countSlot))>=21)break;
  }
  if(!word(s.countSlot)){store(row()+0x90,0xffffffff);store(row()+0x94,0);store(row()+0x90,0);store(row()+0x80,0x80363e60);store(row()+0x84,0);store(row()+0x98,0);m.memory.storeHalf(m.memory.user,row()+0x8c,362);name();increment();}
 }
 if(type==4){const auto mask=half(p+0x8c);for(unsigned index=0;index<16;++index){if(!(mask&(1u<<index)))continue;const auto table=0x80355ca8+4*index;if(!word(table))continue;
  store(row()+0x90,0xffffffff);store(row()+0x94,0);store(row()+0x80,word(table));store(row()+0x84,0);const auto icon=half(word(table)+4);m.memory.storeHalf(m.memory.user,row()+0x8c,icon);store(row()+0x98,0);name();increment();if(signedWord(word(s.countSlot))>=21)break;
 }}
 if(type==6){const auto callback=packedUnitData(u,1,m);assert(s.party);if(!s.party(s.user,callback,menu,s.countSlot))return std::nullopt;}
 const auto published=word(s.countSlot);if(signedWord(published)<=0)return 0;
 auto cursor=[&](){return word(s.cursorSlot);};const auto destination=cursor();store(destination+8,published);
 const auto selected=cursor();const auto index=signedWord(word(selected)),count=signedWord(word(selected+8));if(index>=count)store(selected,uint32_t(count)-1);
 for(;;){const auto c=cursor();const auto scroll=signedWord(word(c+4)),n=signedWord(word(c+8));if(n>=signedWord(uint32_t(scroll)+6) || scroll<=0)break;const auto decremented=word(c+4)-1;store(c+4,decremented);}
 assert(s.update);if(!s.update(s.user,b,type))return std::nullopt;
 return signedWord(word(s.countSlot));
}
}
