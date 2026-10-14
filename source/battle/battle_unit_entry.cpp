#include "battle_unit_entry.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {int32_t signWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}int32_t signByte(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}}
uint32_t packedBattleUnitEntry(uint32_t b,uint32_t d,const BattleUnitEntryServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.half && m.storeWord && m.storeHalf && s.memory.byte && s.storeByte && s.allocate && s.pouch && s.unary && s.vector && s.bodyPart && s.poseName && s.create);
 auto w=[&](uint32_t a){return m.word(m.user,a);};auto store=[&](uint32_t a,uint32_t v){m.storeWord(m.user,a,v);};auto h=[&](uint32_t a,uint16_t v){m.storeHalf(m.user,a,v);};auto byte=[&](uint32_t a,uint8_t v){s.storeByte(s.user,a,v);};auto readByte=[&](uint32_t a){return s.memory.byte(s.memory.user,a);};
 auto f=[&](uint32_t a){const auto bits=w(a);float v;std::memcpy(&v,&bits,4);return v;};auto vf=[&](uint32_t a){return std::array<float,3>{f(a),f(a+4),f(a+8)};};
 int32_t id=0;while(id<64 && battleRegistryUnit(b,id,s.memory))++id;if(id>=64)return 0;
 const auto u=s.allocate(s.user,0xb34);if(!u)return 0;
 for(uint32_t i=0;i<0xb34;++i)byte(u+i,0);
 setBattleRegistryUnit(b,id,u,s.memory);store(u,uint32_t(id));store(u+0x10,w(d));store(u+0x18,w(w(u+0x10)+0xc0));store(u+4,w(w(d)));store(u+8,w(w(d)));
 for(auto function:{0x8019673cu,0x80253e84u,0x80195d34u})s.unary(s.user,function,u);
 byte(u+0xe,0xff);byte(u+0xc,readByte(d+4));byte(u+0xd,readByte(w(u+0x10)+0xe));const auto kind=signWord(w(u+8));if(kind==222 || (kind>=224 && kind<231))byte(u+0xd,uint8_t(m.half(m.user,s.pouch(s.user)+0x8a)));
 store(u+0x2c,w(d+8));
 for(auto off:{0x294u,0x298u,0x29cu,0x28cu,0x290u,0x2a0u,0x2a8u})store(u+off,0);
 for(auto function:{0x80128c6cu,0x801283d8u,0x801289a0u})s.vector(s.user,function,u,vf(d+0xc));
 const auto zero=f(0x804226a4),one=f(0x804226a8);const std::array<float,3> z={zero,zero,zero},o={one,one,one};
 for(auto function:{0x80128680u,0x801288b4u,0x80128604u})s.vector(s.user,function,u,z);
 for(auto function:{0x80128524u,0x80128574u})s.vector(s.user,function,u,o);
 auto copy3=[&](uint32_t target,uint32_t source){for(unsigned a=0;a<3;++a)store(target+4*a,w(source+4*a));};
 for(auto pair:{std::array<uint32_t,2>{0x84,0x20},{0xb4,0x3c},{0xec,0x5c},{0xf8,0x68}})copy3(u+pair[0],w(u+0x10)+pair[1]);
 h(u+0x10a,m.half(m.user,w(d)+8));h(u+0x108,m.half(m.user,u+0x10a));h(u+0x10c,m.half(m.user,w(d)+8));h(u+0x110,m.half(m.user,w(d)+0xa));h(u+0x10e,m.half(m.user,u+0x110));h(u+0x112,m.half(m.user,w(d)+0xa));store(u+0x114,w(0x804226a8));store(u+0x27c,0);store(u+0x138,0);store(u+0x140,0xffffffff);s.unary(s.user,0x80125fd8,u);
 for(auto pair:{std::array<uint32_t,2>{0xcc,0x16},{0xce,0x18},{0xd0,0x1a},{0xd2,0x1c},{0xd4,0x2c},{0xd6,0x2e}})h(u+pair[0],m.half(m.user,w(u+0x10)+pair[1]));
 copy3(u+0xd8,w(u+0x10)+0x74);store(u+0xe4,w(w(u+0x10)+0x80));store(u+0xe8,w(w(u+0x10)+0x84));byte(u+0x25,readByte(w(u+0x10)+0x8a));byte(u+0x26,readByte(w(u+0x10)+0x8b));s.unary(s.user,0x80127b58,u);
 store(u+0x104,w(w(u+0x10)+0xac));store(u+0x144,w(w(u+0x10)+0xb0));byte(u+0x21,1);byte(u+0x22,1);byte(u+0x188,0);byte(u+0x189,readByte(b+uint32_t(signByte(readByte(u+0xc)))*8+0xa));
 for(unsigned i=0;i<4;++i)store(u+0x218+4*i,w(d+0x1c+4*i));
 const auto count=readByte(w(u+0x10)+0xb4);const auto size=uint32_t(count)*0x500;const auto parts=s.allocate(s.user,size);
 for(uint32_t i=0;i<size;++i)byte(parts+i,0);
 store(u+0x14,parts);assert(count && parts);auto pd=w(w(u+0x10)+0xb8),part=parts;
 for(unsigned i=0;i<count;++i){store(part+4,pd);store(part+0x4ec,u);store(part+0x1ac,w(w(part+4)+0x40));store(part+0x1b0,w(w(part+4)+0x44));if(w(part+0x1ac)&1)store(u+0x2d4,w(w(part+4)));
  store(part+0x1b4,w(w(part+4)+0x38));store(part+0x1b8,w(w(part+4)+0x3c));h(part+0x1a2,m.half(m.user,w(part+4)+0x30));h(part+0x1a4,m.half(m.user,w(part+4)+0x32));store(part+0x78,w(0x804226a8));byte(part+0xbc,0);byte(part+0xbd,readByte(b+uint32_t(signByte(readByte(u+0xc)))*8+0xa));
 copy3(part+0x24,w(part+4)+0xc);
  for(auto function:{0x801284a4u,0x80128470u})s.vector(s.user,function,part,o);
 for(auto function:{0x80128654u,0x801287b4u,0x801285d0u})s.vector(s.user,function,part,z);
  byte(part+0x4fc,0);
  copy3(part+0x170,w(part+4)+0x18);
 for(unsigned j=0;j<3;++j)store(part+0x17c+4*j,w(0x804226a4));
 copy3(part+0x188,w(part+4)+0x24);
 for(unsigned j=0;j<3;++j)store(part+0x194+4*j,w(0x804226a4));
 store(part+0x4f0,w(0x804226a0));h(part+0x1a0,uint16_t(w(d+0x18)));store(part+0x1bc,w(pd+0x48));store(part+0x1c0,0xffffffff);byte(part+0x1c4,0);store(part+0x208,w(0x804226a8));store(part+0x20c,w(0x804226a8));byte(part+0x215,0);byte(part+0x214,0);store(part+0x4e8,0xffffffff);store(part+8,w(w(part+4)+4));store(part,i+1<count?part+0x500:0);part+=0x500;pd+=0x4c;
 }
 const auto body=s.bodyPart(s.user,u);store(u+0x2d8,s.poseName(s.user,body,65));store(u+0x2dc,s.poseName(s.user,body,43));byte(u+0x311,255);byte(u+0x312,255);s.unary(s.user,0x801011fc,u);store(u+0x314,0);store(u+0x318,0);if(signWord(w(u+8))==208)store(u+0x314,s.allocate(s.user,0x44));s.unary(s.user,0x80127134,u);store(u+0x288,0);const auto init=w(w(u+0x10)+0xbc);if(init){auto& event=s.create(s.user,init,10,0);event.owner=signWord(w(u));store(u+0x288,event.id);}return u;
}
}
