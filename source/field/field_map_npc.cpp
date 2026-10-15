#include "field_map_npc.hpp"
#include <cassert>
namespace ttyd {
uint32_t packedMapNpcTribe(uint32_t name,const PackedMapNpcServices& s){
 for(uint32_t p=0x803316d0;;p+=0x54){const auto text=s.memory.word(s.memory.user,p);if(!text)return 0;if(!s.compare(s.user,text,name))return p;}
}
int32_t packedMapNpcDefinition(uint32_t table,uint32_t name,const PackedMapNpcServices& s){
 for(auto p=table;;p+=8){const auto text=s.memory.word(s.memory.user,p);if(!text)return 0;if(!s.compare(s.user,text,name)){const auto v=s.memory.word(s.memory.user,p+4);return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
}
uint32_t registerPackedMapNpc(const PackedMapNpcInput& i,const PackedMapNpcServices& s){
 const auto& m=s.memory;assert(m.word && m.half && m.storeWord && m.storeHalf && m.storeByte && s.compare && s.copyString && s.entry && s.lookup && s.area && s.battle && s.animation);
 auto word=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};
 const auto tribe=packedMapNpcTribe(i.tribe,s);s.entry(s.user,i.name,word(tribe+4));const auto npc=s.lookup(s.user,i.name);
 m.storeByte(m.user,npc+0x314,uint8_t(i.ordinal));store(npc,word(npc)|4);
 auto triple=[&](uint32_t from,uint32_t to){const auto x=word(from),y=word(from+4);store(to,x);store(to+4,y);store(to+8,word(from+8));};
 triple(i.position,npc+0x8c);store(npc+0x144,word(i.direction));store(npc+0x28,tribe);store(npc+0x14c,word(tribe+0x28));store(npc+0x150,word(tribe+0x2c));store(npc,word(npc)&~2u);
 for(uint32_t ai=0x80338270;;ai+=0x24){const auto name=word(ai);if(!name)break;if(s.compare(s.user,name,i.ai))continue;
  const auto flags=word(npc),mask=word(ai+4);store(npc,flags|mask);const uint32_t offsets[]={0x120,0x124,0x12c,0x130,0x134,0x138,0x13c};for(unsigned n=0;n<7;++n)store(npc+offsets[n],word(ai+8+4*n));break;
 }
 store(npc+0x1f8,uint32_t(packedMapNpcDefinition(0x80416580,i.territory,s)));
 const uint16_t one=1;const auto zero=word(0x804207b4);
 triple(i.territoryBase,npc+0x1fc);triple(i.loiter,npc+0x208);triple(i.homing,npc+0x214);for(unsigned n=0;n<4;++n)store(npc+0x220+4*n,word(i.ranges[n]));
 store(npc+0x1e0,word(tribe+0x40));store(npc+0x1e4,word(tribe+0x44));store(npc+0x1ec,word(tribe+0x4c));store(npc+0x1f0,word(tribe+0x50));m.storeHalf(m.user,npc+0x1e8,m.half(m.user,tribe+0x48));m.storeHalf(m.user,npc+0x1d8,one);store(npc+0x1dc,zero);
 const auto area=s.area(s.user,i.area);int32_t battle=-1;if(s.compare(s.user,i.battle,0x802c3b10))battle=packedMapNpcDefinition(word(area+0xc),i.battle,s);s.battle(s.user,npc,battle);
 auto name=word(tribe+0x10);if(name)s.copyString(s.user,npc+0x4c,name);name=word(tribe+0x14);if(name)s.copyString(s.user,npc+0x6c,name);name=word(tribe+8);if(name){s.copyString(s.user,npc+0x2c,name);const auto handle=word(npc+0x104);s.animation(s.user,handle<=0x7fffffff?int32_t(handle):int32_t(int64_t(handle)-0x100000000LL),npc+0x2c,1);}
 return npc;
}
}
