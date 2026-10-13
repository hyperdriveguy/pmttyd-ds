#include "battle_packed_hit_precheck.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {uint32_t word(const BattlePackedHitPrecheckServices& s,uint32_t a){return s.memory.memory.word(s.memory.memory.user,a);}uint8_t byte(const BattlePackedHitPrecheckServices& s,uint32_t a){return s.memory.byte(s.memory.user,a);}bool status(const BattlePackedHitPrecheckServices& s,uint32_t u,int32_t id){assert(s.status);return s.status(s.user,u,id);}int32_t random(const BattlePackedHitPrecheckServices& s){assert(s.random);return s.random(s.user,100);}}
int32_t packedBattleHitPrecheck(uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t flags,const BattlePackedHitPrecheckServices& s){
 if(!d || !p)return 1;
 if(word(s,w+0x74)&4)s.memory.memory.storeWord(s.memory.memory.user,a+0x27c,word(s,a+0x27c)|8);
 if((word(s,p+0x1ac)&0x4000) && !(word(s,w+0x74)&0x40000))return 2;
 if(word(s,w+0x74)&8)return 1;
 if(status(s,d,18) || (word(s,d+0x104)&0x18))return 4;
 if((word(s,p+0x1b0)&2) && !(word(s,w+0x78)&4) && !byte(s,a+0x302))return 5;
 if(word(s,p+0x1ac)&0x40)return 4;
 if(flags&0x100000)return 1;
 for(unsigned i=0;i<byte(s,d+0x2e1);++i)if(random(s)>=90)return 3;
 for(unsigned i=0;i<byte(s,d+0x2e2);++i)if(random(s)>=75)return 3;
 const auto hp=int16_t(s.memory.memory.half(s.memory.memory.user,d+0x10c));
 if(hp<=int32_t(byte(s,word(s,d+0x10)+0xc)))for(unsigned i=0;i<byte(s,d+0x2e0);++i)if(random(s)>=67)return 3;
 if(status(s,d,7) && !status(s,d,1) && !status(s,d,2))if(random(s)>=50)return 6;
 int32_t accuracy=byte(s,w+0x10);if(status(s,a,3))accuracy=accuracy*50/100;
 const auto battle=currentBattleAddress(s.memory);if(word(s,battle+0x182bc)){assert(s.multiply && s.truncate);const auto bits=word(s,0x804221c4);float coefficient;std::memcpy(&coefficient,&bits,4);accuracy=s.truncate(s.user,s.multiply(s.user,float(accuracy),coefficient));}
 return random(s)<accuracy?1:2;
}
}
