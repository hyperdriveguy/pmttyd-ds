#include "battle_unit_setup.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<0x80000000?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
int32_t packedBattleUnitSetup(uint32_t b,const BattleUnitSetupServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeHalf && s.memory.byte && s.storeByte && s.pouch && s.entry && s.equip && s.pouchParameters);
 auto w=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};auto half=[&](uint32_t a,uint16_t v){m.storeHalf(m.user,a,v);};auto byte=[&](uint32_t a,uint8_t v){s.storeByte(s.user,a,v);};
 const auto npc=w(w(b+0x2738)+0xc),group=w(npc+0x18),pouch=s.pouch(s.user);
 half(b+8,2);half(b+0x10,1);half(b+0x18,0);byte(b+0xa,1);byte(b+0x12,0xff);byte(b+0x1a,1);store(b+0xc,0);store(b+0x14,0);store(b+0x1c,0);
 auto u=s.entry(s.user,0x80416a80);s.equip(s.user,u,3,0);u=s.entry(s.user,0x80416a50);s.equip(s.user,u,3,0);s.pouchParameters(s.user,u);
 const auto party=signedWord(w(w(b+0x2738)+4));if(party){assert(s.basePosition && s.setBasePosition && s.setPosition);u=s.entry(s.user,0x80363790+uint32_t(party-224)*0x30);const auto old=s.basePosition(s.user,u);auto constant=[&](uint32_t a){const auto bits=w(a);float v;std::memcpy(&v,&bits,4);return v;};const std::array<float,3> pos={constant(0x80422178),old[1],constant(0x8042217c)};s.setBasePosition(s.user,u,pos);s.setPosition(s.user,u,pos);s.equip(s.user,u,5,0);s.pouchParameters(s.user,u);}
 auto definition=w(group+4);uint32_t index=0;for(int32_t n=0;n<signedWord(w(group));++n,index+=4,definition+=0x30){u=s.entry(s.user,definition);byte(u+0xe,uint8_t(n));store(u+0x308,0);
  const auto assigned=signedWord(int8_t(s.memory.byte(s.memory.user,u+0xe)));const auto held=w(npc+0x5c+uint32_t(assigned)*4);
  if(held){store(u+0x308,held);store(u+0x1c,w(u+0x1c)|0x40000000);}else{store(u+0x308,w(npc+index+0x1c));s.equip(s.user,u,1,signedWord(w(npc+index+0x1c)));}
  store(u+0x30c,w(npc+index+0x3c));half(u+0x10c,m.half(m.user,u+0x108));half(u+0x112,m.half(m.user,u+0x10e));
 }
 store(b+0x420,0xffffffff);store(b+0x163f4,0);
 constexpr uint32_t bits[12]={1,2,4,8,16,128,0,256,1024,2048,512,4096};
 for(unsigned i=0;i<200;++i){const auto item=uint32_t(int32_t(int16_t(m.half(m.user,pouch+0x38a+2*i))));const auto offset=item-308;if(offset<12)store(b+0x163f4,w(b+0x163f4)|bits[offset]);}
 return 1;
}
}
