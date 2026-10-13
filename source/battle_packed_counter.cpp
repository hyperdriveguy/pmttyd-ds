#include "battle_packed_counter.hpp"
#include <cassert>
namespace ttyd {
void BattleCounterWork::init(){words={};words[0]=1;}
int32_t packedBattleCheckCounter(BattleCounterWork& work,uint32_t a,uint32_t d,
 uint32_t p,uint32_t w,uint32_t& flags,const BattlePackedCounterServices& s){
 if(!a)return 0;
 assert(s.memory.memory.word && s.memory.byte && s.status);
 auto word=[&](uint32_t at){return s.memory.memory.word(s.memory.memory.user,at);};
 auto byte=[&](uint32_t at){return s.memory.byte(s.memory.user,at);};
 auto status=[&](uint32_t u,int32_t type){return s.status(s.user,u,type);};
 auto& x=work.words;x[10]=0;
 auto hit=[&](uint32_t code){x[8]=code;x[9]=code|0x100;flags|=0x100;};
 auto block=[&](uint32_t code,uint32_t pattern){hit(code);++x[10];x[11]=pattern;x[0]=0;};
 const auto partFlags=word(p+0x1b0);
 if((partFlags&1) && !byte(a+0x302) && !(word(w+0x78)&2)){block(43,31);}
 else if((partFlags&2) && !byte(a+0x302) && !(word(w+0x78)&4)){block(44,32);}
 else if((partFlags&4) && !byte(a+0x302) && !(word(w+0x78)&8)){block(45,33);}
 else if((partFlags&0x30) && !byte(a+0x301) && !(word(w+0x78)&0x10)){block(46,34);if(word(p+0x1b0)&0x20)x[2]=1;}
 else if((partFlags&0xc0) && !(word(w+0x78)&0x20)){block(47,35);if(word(p+0x1b0)&0x80)x[5]=1;}
 else if((partFlags&0xc00) && !(word(w+0x78)&1) && !status(a,6)){block(48,36);if(word(p+0x1b0)&0x800)x[4]=1;}
 else {
  if((word(p+0x1b0)&0x1000) && !(word(w+0x78)&0x80)){
   const auto element=byte(w+0x6c);
   if(element==3 || element==4 || element==1 || ((word(w+0x74)&0x400) && status(a,8))){hit(49);++x[10];}
  }
  if((word(p+0x1b0)&0x2000) && !(word(w+0x78)&0x100)){hit(50);++x[10];}
  if((status(d,6) || (word(p+0x1b0)&0xc00)) && !(word(w+0x78)&1) && !status(a,6)){
   hit(52);++x[10];if(word(p+0x1b0)&0x800)x[4]=1;
  }
  if(status(d,21) && !(word(w+0x78)&0x200)){hit(53);x[1]=1;}
  if((word(p+0x1b0)&0x300) && !(word(w+0x78)&0x40)){hit(51);x[1]=1;if(word(p+0x1b0)&0x200)x[3]=1;}
  if(status(d,22) && !(word(w+0x78)&0x400)){hit(54);x[6]=1;}
  if(byte(d+0x2f0) && !(word(w+0x78)&0x200)){hit(53);x[7]=1;}
 }
 constexpr std::array<int32_t,12> results{{8,7,9,10,11,0,12,13,15,14,16,17}};
 const uint32_t index=x[8]-43;return index<results.size()?results[index]:0;
}
}
