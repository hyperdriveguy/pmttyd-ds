#include "battle_packed_hit_precheck.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {uint32_t word(const BattlePackedHitPrecheckServices& s,uint32_t a){return s.memory.memory.word(s.memory.memory.user,a);}uint8_t byte(const BattlePackedHitPrecheckServices& s,uint32_t a){return s.memory.byte(s.memory.user,a);}bool status(const BattlePackedHitPrecheckServices& s,uint32_t u,int32_t id){assert(s.status);return s.status(s.user,u,id);}int32_t random(const BattlePackedHitPrecheckServices& s){assert(s.random);return s.random(s.user,100);}}
// Null defender/part ->1. Weapon74 mask4 first marks attacker27c mask8.
// Ordered early results: part1ac4000 without weapon40000 ->2; weapon8 ->1;
// status18/attributes18 ->4; part1b0 mask2 without protection ->5; part40 ->4.
// Caller flags100000 bypass ONLY probabilistic suffix, returning1.
int32_t packedBattleHitPrecheck(uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t flags,const BattlePackedHitPrecheckServices& s){
 if(!d || !p)return 1;
 if(word(s,w+0x74)&4)s.memory.memory.storeWord(s.memory.memory.user,a+0x27c,word(s,a+0x27c)|8);
 if((word(s,p+0x1ac)&0x4000) && !(word(s,w+0x74)&0x40000))return 2;
 if(word(s,w+0x74)&8)return 1;
 if(status(s,d,18) || (word(s,d+0x104)&0x18))return 4;
 if((word(s,p+0x1b0)&2) && !(word(s,w+0x78)&4) && !byte(s,a+0x302))return 5;
 if(word(s,p+0x1ac)&0x40)return 4;
 if(flags&0x100000)return 1;
 // Per-copy dodge loops use fresh badge counts2e1/2e2/2e0 and100 rolls,
 // thresholds>=90/>=75/>=67 ->3 (last only HP<=kind+c danger threshold).
 // Status7 without1/2 rolls>=50 ->6. Accuracy weapon10 halves for attacker
 // status3; active fog handleB182bc multiplies source coefficient804221c4,
 // truncates, then final100 draw<accuracy ->1, else2. No HP application here.
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
