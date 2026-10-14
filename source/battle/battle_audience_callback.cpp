#include "battle_audience_callback.hpp"
namespace ttyd {
bool resumePackedBattleAudienceCallback(const BattleAudienceCallbackServices& s,BattleAudienceCallbackCursor& c){
    const auto& a=s.actors;
    if(!c.active){c={};c.active=true;}
    if(c.stage==0){
        c.pending=0x801a86a0;
        if(!a.memory.word || !a.byte)return false;
        c.owner=currentBattleAddress(a)+0x2858;c.stage=1;
    }
    if(c.stage==1){
        c.pending=0x8000f2fc;
        if(!s.camera || !(c.camera=s.camera(s.user,4)))return false;
        c.stage=2;
    }
    static const BattleAudienceGXRequest setup[]{
        {0x802b5c8c,{0}},{0x802b4520,{}},{0x802b3c08,{9,1}},{0x802b3c08,{13,1}},
        {0x802b4558,{0,9,1,4,0}},{0x802b4558,{0,13,1,4,0}},
        {0x802b6f3c,{1}},{0x802b6f78,{4,0,1,0,0,2,2}},
        {0x802b5084,{1}},{0x802b4e04,{0,1,4,60,0,125}},
        {0x802b8fcc,{1}},{0x802b89e8,{0,0,0,0,1,0}},
        {0x802b8960,{0,15,12,8,10}},{0x802b8a50,{0,0,0,0,1,0}},
        {0x802b89a4,{0,7,5,4,7}},{0x802b8e30,{0,0,0,4}}
    };
    if(c.stage==2){
        while(c.step<16){const auto& r=setup[c.step];c.pending=r.function;
            if(!s.request || !s.request(s.user,r))return false;++c.step;}
        c.stage=3;
    }
    if(c.stage==3){
        // Original GetPtr(0) independently queries the current battle owner.
        c.record=currentBattleAddress(a)+0x2858+0x1bc;c.stage=4;
    }
    while(c.stage==4 && c.index<200){
        c.pending=0x80010864;
        const auto* entry=s.currentRecord?s.currentRecord(s.user):nullptr;if(!entry)return false;
        const auto alpha=a.byte(a.user,c.record+0x44);
        if(entry->drawClass==2?alpha!=255:alpha==255){
            const auto flags=a.memory.word(a.memory.user,c.record);
            if((flags&5)==5){c.stage=5;break;}
        }
        ++c.index;c.record+=0x134;
    }
    if(c.stage==5){c.pending=0x801a38f8;return false;} // Full admitted-actor continuation still required.
    c.active=false;c.pending=0;return true;
}
}
