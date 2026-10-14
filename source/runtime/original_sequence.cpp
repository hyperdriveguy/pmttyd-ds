#include "original_sequence.hpp"

namespace ttyd {
namespace {
// Original table80304400: Init/Main/Exit, indexed by current, never native Scene.
constexpr uint32_t callbacks[8][3]={
    {0x800874b0,0x80086d6c,0x8008746c},
    {0x80009d30,0x80009cd8,0x80009cf8},
    {0x8000753c,0x800073a4,0x80007390},
    {0x8000874c,0x80007c2c,0x80008724},
    {0x80073f20,0x80073b24,0x80073f00},
    {0x80078a14,0x80078548,0x800789ec},
    {0x800f72b8,0x800f6f68,0x800f727c},
    {0x80252f8c,0x80252818,0x80252f60}
};
bool select(int32_t id,unsigned column,uint32_t& identity){
    // Outside recovered table memory is unavailable, not clamped to a row.
    if(id<0 || id>=8)return false;
    identity=callbacks[id][column];return true;
}
}
void OriginalSequence::request(int32_t id,uint32_t p0,uint32_t p1){
    state.requested=id;state.p0=p0;state.p1=p1;
}
void OriginalSequence::importNativeField(){
    state={};state.current=state.requested=2;state.work[0]=2;
    stage_=0;callback_=pending_=0;
}
void originalFieldSequenceExit(uint32_t& flags){flags&=~2u;}
bool OriginalSequence::main(const OriginalSequenceServices& s){
    for(;;)switch(stage_){
    case 0:{
        const int32_t requested=state.requested,current=state.current;
        stage_=requested==current?8:current==-1?4:1;break;
    }
    case 1:
        pending_=0x80005ce4;
        if(!s.drawSync || !s.drawSync(s.user))return false;
        stage_=2;break;
    case 2:
        pending_=0x8002e228;
        if(!select(state.current,2,callback_))return false;
        stage_=3;break;
    case 3:
        pending_=callback_;
        if(!s.callback || !s.callback(s.user,callback_,state.work))return false;
        stage_=4;break;
    case 4:
        state.previous=state.current;state.work.fill(0);stage_=5;break;
    case 5:{
        const int32_t requested=state.requested;
        const uint32_t p0=state.p0,p1=state.p1;
        state.current=requested;
        // Capture Init before publishing work. These arguments survive a wait.
        pending_=0x8002e228;
        if(!select(requested,0,callback_)){stage_=10;return false;}
        state.work[0]=uint32_t(requested);state.work[1]=0;
        state.work[2]=p0;state.work[3]=p1;stage_=7;break;
    }
    case 7:
        pending_=callback_;
        if(!s.callback || !s.callback(s.user,callback_,state.work))return false;
        stage_=8;break;
    case 8:
        pending_=0x8002e228;
        if(!select(state.current,1,callback_))return false;
        stage_=9;break;
    case 9:
        pending_=callback_;
        if(!s.callback || !s.callback(s.user,callback_,state.work))return false;
        stage_=0;pending_=0;return true;
    default:return false; // Unmapped captured Init table access stays unavailable.
    }
}
}
