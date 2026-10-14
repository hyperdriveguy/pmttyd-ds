#include "field_battle_information.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t signedHalf(uint16_t v){return v<0x8000?v:int32_t(v)-65536;}
int32_t signedByte(uint8_t v){return v<128?v:int32_t(v)-256;}
int32_t add(int32_t a,int32_t b){return signedWord(uint32_t(a)+uint32_t(b));}
int32_t subtract(int32_t a,int32_t b){return signedWord(uint32_t(a)-uint32_t(b));}
}
void setupPackedNpcBattleInformation(uint32_t npc,uint32_t c,const PackedNpcBattleSetupServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeByte && s.memory.byte);
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto byte=[&](uint32_t a){return s.memory.byte(s.memory.user,a);};
 auto clear=[&](uint32_t a,unsigned n){for(unsigned i=0;i<n;++i)m.storeByte(m.user,a+i,0);};
 auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};auto random=[&](int32_t bound){assert(s.random);return s.random(s.user,bound);};
 const auto b=npc+0x230;clear(b,200);clear(b+0x7c,32);
 if(!c || !word(c))return;
 store(b+0xac,word(c+0x1c));const auto audience=signedWord(word(b+0xac));
 if(audience<=0 || audience>=3)for(unsigned i=0;i<16;++i){const auto first=byte(c+0x20+2*i),last=byte(c+0x21+2*i);const auto draw=random(signedByte(last)-signedByte(first)+1);m.storeByte(m.user,b+0xb0+i,uint8_t(uint32_t(first)+uint32_t(draw)));}
 (void)word(c+0x18);store(b+4,0);store(b+8,word(c+8));store(b+0xc,0);store(b+0x10,0);store(b+0x14,0);store(b+0xc0,word(c+0x40));
 const auto selector=signedWord(word(c+0xc));uint32_t list;
 if(selector>=signedWord(0xf8405b80) && selector<=signedWord(0xf8d9f200)){assert(s.savedFlag);list=word(c+(s.savedFlag(s.user,add(selector,130000000))?0x10:0x14));}else list=word(c+0x14);
 store(b+0x18,0);
 if(list){int32_t total=0;for(auto p=list;;p+=12){const auto weight=signedWord(word(p));if(weight<=0)break;total=add(total,weight);}
  if(total>0){auto draw=random(total);for(auto p=list;;p+=12){draw=subtract(draw,signedWord(word(p)));if(draw<0){store(b+0x18,word(p+4));store(b,word(p+8));break;}}}
 }
 const auto group=word(b+0x18);
 if(group){const auto count=signedWord(word(group));const auto units=word(group+4);clear(b+0x1c,32);
  for(int32_t i=0;i<count;++i){const auto unit=units+0x30u*uint32_t(i),table=word(unit+0x2c);if(!table)continue;int32_t total=0;
   for(auto p=table;;p+=8){const auto weight=signedHalf(m.half(m.user,p+4));if(weight<=0 && word(p)==0)break;total=add(total,weight);}
   if(total>0){auto draw=random(total);for(auto p=table;;p+=8){draw=subtract(draw,signedHalf(m.half(m.user,p+4)));if(draw<0){store(b+0x1c+4u*uint32_t(i),word(p));store(b+0x3c+4u*uint32_t(i),word(unit+0x2c));break;}}}
  }
 }
 m.storeByte(m.user,npc+0x316,0);
 if(group)m.storeByte(m.user,npc+0x316,byte(word(word(group+4))+0x12));
 store(npc,word(npc)|0x10000000);
}
void initializePackedBattleInformation(uint32_t i,const EventMemoryServices& m){
 assert(m.storeWord && m.storeByte);
 // Actual SetMode/Party/FirstAttack/Result call order followed by local writes.
 for(unsigned offset:{0u,4u,8u,0x10u,0xcu,0x14u})m.storeWord(m.user,i+offset,0);
 for(unsigned offset:{0x18u,0x19u,0x1au,0x1bu})m.storeByte(m.user,i+offset,0);
}
uint32_t preparePackedFieldBattleInformation(uint32_t f,uint32_t npc,uint32_t pouch,const PackedFieldBattleInformationServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeByte && s.memory.byte && s.marioPartyId && s.party && s.partyKind);
 const auto info=f+0x20;initializePackedBattleInformation(info,m);m.storeWord(m.user,info,0);
 const auto party=s.party(s.user,s.marioPartyId(s.user));
 if(party){const auto id=signedByte(s.memory.byte(s.memory.user,party+0x31));const auto kind=s.partyKind(s.user,id);if(!(m.half(m.user,pouch+14u*uint32_t(id))&2))m.storeWord(m.user,info+4,uint32_t(kind));}
 uint32_t first=0;
 if(!(m.word(m.user,f+0x1c)&1))switch(m.word(m.user,f+0x10)){
  case 0x20000:first=1;break;case 0x40000:first=2;break;case 0x80000:first=3;break;case 0x100000:first=4;break;case 0x200000:first=5;break;case 0x400000:first=6;break;case 0x800000:first=7;break;case 0x1000000:first=8;break;case 0x10000000:first=m.word(m.user,f+0x14);break;default:break;
 }
 m.storeWord(m.user,info+8,first);m.storeWord(m.user,info+0xc,npc+0x230);
 m.storeByte(m.user,info+0x18,s.memory.byte(s.memory.user,npc+0x2f4));m.storeByte(m.user,info+0x1a,s.memory.byte(s.memory.user,npc+0x2f5));m.storeByte(m.user,info+0x1b,s.memory.byte(s.memory.user,npc+0x2f6));return info;
}
}
