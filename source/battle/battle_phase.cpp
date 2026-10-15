#include "battle_phase.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
namespace ttyd {
namespace {
uint32_t word(const BattlePhaseServices& s,uint32_t a){return s.actors.memory.word(s.actors.memory.user,a);}
void word(const BattlePhaseServices& s,uint32_t a,uint32_t v){s.actors.memory.storeWord(s.actors.memory.user,a,v);}
uint8_t byte(const BattlePhaseServices& s,uint32_t a){return s.actors.byte(s.actors.user,a);}
void byte(const BattlePhaseServices& s,uint32_t a,uint8_t v){s.actors.memory.storeByte(s.actors.memory.user,a,v);}
int32_t sw(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v){return v<128?v:int32_t(v)-256;}
uint32_t unit(uint32_t b,int32_t id,const BattlePhaseServices& s){return battleRegistryUnit(b,id,s.actors);}
}
// Battle phase IDs120 (64 words), target rows220 (64 ID/key pairs). Reset
//1c70/1c74 and markers311=ff; eligible phase tag unit2c equals battle f18.
// Skip state10 unless status20, or attributes104 mask80000. Part handle1c0
//!=-1 marks part4e4=1. Selected unit140 resets-1; no guessed pose is created.
void buildBattlePhaseTables(uint32_t b,const BattlePhaseServices& s) {
    word(s,b+0x1c70,0);byte(s,b+0x1c74,0);for(unsigned i=0;i<64;++i)word(s,b+0x120+4*i,0xffffffff);
    unsigned count=0;
    for(unsigned i=0;i<64;++i){const auto u=unit(b,i,s);if(!u)continue;byte(s,u+0x311,255);
        if(byte(s,u+0x20)==10 && !s.status(s.actors.user,u,20))continue;
        if(word(s,u+0x2c)!=word(s,b+0xf18) || (word(s,u+0x104)&0x80000))continue;
        word(s,b+0x120+4*count++,word(s,u));word(s,u+0x140,0xffffffff);
        for(auto p=word(s,u+0x14);p;p=word(s,p))if(word(s,p+0x1c0)!=0xffffffff)byte(s,p+0x4e4,1);
    }
    // Two STABLE bubble sorts: abs(home X at unit30) ascending, then signed
    // initiative28 descending. Final ties retain X order then registry order.
    for(unsigned pass=0;pass<count;++pass)for(unsigned i=0;i+1<count-pass;++i){const auto a=b+0x120+4*i,x=word(s,a),y=word(s,a+4);
        if(std::fabs(s.real(s.actors.user,unit(b,x,s)+0x30))>std::fabs(s.real(s.actors.user,unit(b,y,s)+0x30))){word(s,a,y);word(s,a+4,x);}}
    for(unsigned pass=0;pass<count;++pass)for(unsigned i=0;i+1<count-pass;++i){const auto a=b+0x120+4*i,x=word(s,a),y=word(s,a+4);
        if(sw(word(s,unit(b,x,s)+0x28))<sw(word(s,unit(b,y,s)+0x28))){word(s,a,y);word(s,a+4,x);}}
    // Target table ignores phase tag/state: all nonexcluded actors. Negative X
    // uses abs(X), nonnegative X uses X+1000, then source-like truncation and
    // stable signed ascending sort. This is side-prioritized order, not distance.
    for(unsigned i=0;i<64;++i){word(s,b+0x220+8*i,0xffffffff);word(s,b+0x224+8*i,0);}count=0;
    for(unsigned i=0;i<64;++i){const auto u=unit(b,i,s);if(!u || (word(s,u+0x104)&0x80000))continue;const float x=s.real(s.actors.user,u+0x30);word(s,b+0x220+8*count,i);word(s,b+0x224+8*count++,uint32_t(int32_t(x>=0?x+1000:std::fabs(x))));}
    for(unsigned pass=0;pass<count;++pass)for(unsigned i=0;i+1<count-pass;++i){const auto a=b+0x220+8*i;if(sw(word(s,a+4))>sw(word(s,a+12))){const auto id=word(s,a),key=word(s,a+4);word(s,a,word(s,a+8));word(s,a+4,word(s,a+12));word(s,a+8,id);word(s,a+12,key);}}
}
// End test requires state!=10, positive SIGNED remaining byte22, no status27.
// Selector differs: attribute10000 can admit status27; state1 clears to0 and
// skips this selection visit. Do not replace these distinct predicates.
bool battlePhaseEnded(uint32_t b,const BattlePhaseServices& s){for(unsigned i=0;i<64;++i){const auto u=unit(b,sw(word(s,b+0x120+4*i)),s);if(u && byte(s,u+0x20)!=10 && sb(byte(s,u+0x22))>0 && !s.status(s.actors.user,u,27))return false;}return true;}
int32_t selectBattlePhaseActor(uint32_t b,const BattlePhaseServices& s){for(unsigned i=0;i<64;++i){const int32_t id=sw(word(s,b+0x120+4*i));const auto u=unit(b,id,s);if(!u || sb(byte(s,u+0x22))<=0 || sb(byte(s,u+0x20))==10)continue;
    if(s.status(s.actors.user,u,27) && !(word(s,u+0x104)&0x10000))continue;
    if(byte(s,u+0x20)==1){byte(s,u+0x20,0);continue;}return id;}
    std::abort(); // Original region dereferences its invalid selection; no normal no-actor path.
}
// State graph5->6->7->0, 8->9->0; 3/4 reset0 without same-call decrement.
// At0, status138 mask02000000 clears instead of spending a move; otherwise
// remaining byte22 decrements modulo256. Battle ef4 mask2000 forces0.
// Exhaustion marks state10; unit27c mask2000 preserves turn byte24 once.
// Eligible alliance0/phase04000002 publishes marker311=96 while phase remains;
//96 is a source byte, not measured render-frame timing.
void consumeBattleActorMove(uint32_t b,uint32_t u,const BattlePhaseServices& s){int32_t state=sb(byte(s,u+0x20));switch(state){case 5:state=6;break;case 6:state=7;break;case 7:case 9:state=0;break;case 8:state=9;break;default:break;}byte(s,u+0x20,uint8_t(state));
    if(state==0){if(word(s,u+0x138)&0x02000000)word(s,u+0x138,word(s,u+0x138)&~0x02000000);else byte(s,u+0x22,uint8_t(byte(s,u+0x22)-1));if(word(s,b+0xef4)&0x2000)byte(s,u+0x22,0);
        if(sb(byte(s,u+0x22))<=0){byte(s,u+0x20,10);if(!(word(s,u+0x27c)&0x2000))byte(s,u+0x24,0);else word(s,u+0x27c,word(s,u+0x27c)&~0x2000);
            if(sb(byte(s,u+0xc))==0 && word(s,u+0x2c)==0x04000002 && !battlePhaseEnded(b,s) && !(word(s,b+0xef4)&0x2000))byte(s,u+0x311,96);}}
    else if(state==3 || state==4)byte(s,u+0x20,0);
}
}
