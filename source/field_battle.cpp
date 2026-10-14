#include "field_battle.hpp"

namespace ttyd {
namespace {
int32_t signedDepth(uint8_t depth) {return depth<128?int32_t(depth):int32_t(depth)-256;}
}
int32_t fieldMarioControlOff(FieldMarioBattleState& mario) {
    mario.flags|=2;
    mario.controlDepth=uint8_t(unsigned(mario.controlDepth)+1);
    return signedDepth(mario.controlDepth);
}
int32_t fieldMarioControlOn(FieldMarioBattleState& mario) {
    mario.controlDepth=uint8_t(unsigned(mario.controlDepth)-1);
    if(mario.controlDepth==0)mario.flags&=~2u;
    return signedDepth(mario.controlDepth);
}
void restoreFieldMarioAfterBattle(FieldMarioBattleState& mario) {mario.secondaryFlags&=~1u;}
int32_t fieldMarioControlOffCheck(const FieldMarioBattleState& mario) {return signedDepth(mario.controlDepth);}
int32_t fieldMarioAnimationId(const FieldMarioBattleState& mario) {
    if(mario.secondaryFlags&0x10000000)return 2;
    return (mario.secondaryFlags&0x80000000)?1:0;
}
int32_t startFieldBattle(int32_t name,bool firstCall,const FieldBattleServices& s) {
    auto& work=s.work(s.user);
    const auto npc=s.npc(s.user,name); // Both lookups repeat on resumed calls.
    if(!firstCall)return 2;
    work.npc=npc;
    s.prepareMario(s.user);
    s.marioControl(s.user,false);
    const auto id=s.partyId(s.user);
    const auto party=s.party(s.user,id);
    if(party)s.partyControl(s.user,party,false);
    work.result=0;
    s.soundFlag(s.user,0x10);
    s.setSequence(s.user,4);
    return 0;
}
int32_t waitFieldBattleEnd(const FieldBattleServices& s) {
    auto& work=s.work(s.user);
    if(s.sequence(s.user)!=2 || work.result==0)return 0;
    s.marioControl(s.user,true);
    const auto id=s.partyId(s.user);
    const auto party=s.party(s.user,id);
    if(party)s.partyControl(s.user,party,true);
    s.stopBattleBgm(s.user);
    s.postMario(s.user);
    return 2;
}
}
