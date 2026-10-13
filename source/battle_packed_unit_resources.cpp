#include "battle_packed_unit_resources.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
void revisePackedUnitResources(uint32_t u,const PackedUnitResourceServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeHalf && s.memory.byte && s.pouch);const auto p=s.pouch(s.user);const auto kind=signedWord(m.word(m.user,u+8));
 auto half=[&](uint32_t a){return int32_t(int16_t(m.half(m.user,a)));};auto store=[&](uint32_t a,int32_t v){m.storeHalf(m.user,a,uint16_t(v));};auto badge=[&](uint32_t a){return int32_t(s.memory.byte(s.memory.user,a));};auto cap=[&](uint32_t current,uint32_t maximum){const auto c=half(u+current),max=half(u+maximum);if(c>max)store(u+current,max);};
 if(kind==222){store(u+0x10a,half(p+0x8e));store(u+0x108,half(p+0x8e)+5*badge(u+0x2f1));store(u+0x110,half(p+0x90));store(u+0x10e,half(p+0x90)+5*badge(u+0x2f2));cap(0x10c,0x108);cap(0x112,0x10e);}
 else if(kind>=224 && kind<231){assert(s.partyIndex);const auto row=p+uint32_t(s.partyIndex(s.user,kind))*14;store(u+0x10a,half(row+4));store(u+0x108,half(row+4)+5*badge(u+0x2f1));store(u+0x10c,half(row+6));cap(0x10c,0x108);}
 else{store(u+0x108,half(u+0x10a)+5*badge(u+0x2f1));cap(0x10c,0x108);store(u+0x10e,half(u+0x110)+5*badge(u+0x2f2));cap(0x112,0x10e);}
}
void importPackedUnitResources(uint32_t b,uint32_t u,const PackedUnitResourceServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeHalf && m.storeByte && s.pouch);const auto p=s.pouch(s.user);auto w=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};const auto kind=signedWord(w(u+8));
 if(kind==222){m.storeHalf(m.user,u+0x10c,m.half(m.user,p+0x70));m.storeHalf(m.user,u+0x112,m.half(m.user,p+0x74));revisePackedUnitResources(u,s);}
 else if(kind>=224 && kind<231){assert(s.partyIndex);const auto index=s.partyIndex(s.user,kind);m.storeHalf(m.user,u+0x10c,m.half(m.user,p+uint32_t(index)*14+6));revisePackedUnitResources(u,s);
  const auto saved=b+0x162a0+uint32_t(signedWord(w(u+8))-224)*0x2c;store(u+0x114,w(saved));store(u+0x138,w(saved+0x24));for(unsigned i=0;i<7;++i)store(u+0x118+4*i,w(saved+4+4*i));m.storeHalf(m.user,u+0x134,m.half(m.user,saved+0x20));m.storeHalf(m.user,u+0x13c,m.half(m.user,saved+0x28));m.storeHalf(m.user,u+0x13e,m.half(m.user,saved+0x2a));
  if(int16_t(m.half(m.user,u+0x10c))<=0)m.storeByte(m.user,u+0x135,1); // Original SetStatus27 duration1, strength1 branch.
 }
}
}
