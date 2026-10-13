#include "battle_sequence_startup.hpp"
#include <cassert>
namespace ttyd {
namespace {constexpr uint32_t offsets[]{4,0xf0c,0xf10,0xf14,0xf18,0xf1c,0xf20,0xf24};}
uint32_t packedBattleGetSequence(uint32_t b,uint32_t channel,const EventMemoryServices& m){if(channel>7)return 0;assert(m.word);return m.word(m.user,b+offsets[channel]);}
void packedBattleSetSequence(uint32_t b,uint32_t channel,uint32_t v,const EventMemoryServices& m){if(channel>7)return;assert(m.storeWord);m.storeWord(m.user,b+offsets[channel],v);}
void packedBattleIncrementSequence(uint32_t b,uint32_t channel,const EventMemoryServices& m){packedBattleSetSequence(b,channel,packedBattleGetSequence(b,channel,m)+1,m);}
bool packedBattleUnitInitReady(uint32_t b,const BattleStartupServices& s){assert(s.eventExists);const auto& m=s.memory.memory;assert(m.word && m.storeWord);bool ready=true;
 for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(b,id,s.memory);if(!u)continue;const auto event=m.word(m.user,u+0x288);if(!event)continue;if(s.eventExists(s.user,event))ready=false;else m.storeWord(m.user,u+0x288,0);}return ready;
}
void packedBattleSequenceInit(uint32_t b,const BattleStartupServices& s){
 const auto& m=s.memory.memory;assert(m.word && m.storeWord);auto state=packedBattleGetSequence(b,1,m);
 if(state==0x01000000){packedBattleIncrementSequence(b,1,m);state=0x01000001;}
 if(state==0x01000001){assert(s.mario && s.marioPartyId && s.party && s.unitSetup);const auto mario=s.mario(s.user);m.storeWord(m.user,mario+4,m.word(m.user,mario+4)|1);const auto party=s.party(s.user,s.marioPartyId(s.user));if(party)m.storeWord(m.user,party+4,m.word(m.user,party+4)|1);s.unitSetup(s.user,b);packedBattleSetSequence(b,1,0x01000002,m);return;}
 if(state==0x01000002){if(packedBattleUnitInitReady(b,s))packedBattleSetSequence(b,1,0x01000003,m);return;}
 if(state==0x01000003){assert(s.paperReady && s.niceReady);if(!s.paperReady(s.user,0x802efbe4,1))return;if(!s.paperReady(s.user,0x802efbec,1))return;if(!s.paperReady(s.user,0x802efbf8,1))return;if(!s.niceReady(s.user,1))return;packedBattleSetSequence(b,1,0x01000004,m);return;}
 if(state==0x01000004){packedBattleIncrementSequence(b,1,m);state=0x01000005;}
 if(state==0x01000005){assert(s.statusEventOn && s.statusSystemOn);s.statusEventOn(s.user);s.statusSystemOn(s.user);packedBattleSetSequence(b,0,1,m);packedBattleSetSequence(b,2,0x02000000,m);}
}
void packedBattleSequenceManager(uint32_t b,const BattleSequenceManagerServices& s){
 switch(packedBattleGetSequence(b,0,s.memory)){
 case 0:assert(s.init && s.pinch);s.init(s.user,b);s.pinch(s.user,b,1);break;
 case 1:assert(s.firstAct && s.pinch);s.pinch(s.user,b,0);s.firstAct(s.user,b);break;
 case 2:assert(s.turn && s.pinch);s.pinch(s.user,b,0);s.turn(s.user,b);break;
 case 3:assert(s.end);s.end(s.user,b);break;
 default:break;
 }
}
bool packedBattleNiceReady(int32_t mode,const BattleNiceAdmissionServices& s){
 const auto& m=s.memory.memory;assert(m.word && s.animationReady);bool ready=true;
 for(unsigned i=0;i<7;++i){const auto gp=m.word(m.user,0x80415fd0),language=m.word(m.user,gp+0x16c),row=m.word(m.user,0x80417af8+4*language),key=m.word(m.user,row+4*i);if(key && !s.animationReady(s.user,key,mode,0))ready=false;}return ready;
}
}
