#include "battle_audience_aux_callback.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float scalar(const BattleActorServices& a,uint32_t address){const auto bits=a.memory.word(a.memory.user,address);float v;std::memcpy(&v,&bits,4);return v;}
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
constexpr BattleAudienceGXRequest setup[]{
    {0x802b5c8c,{0}},{0x802b4520,{}},{0x802b3c08,{9,1}},{0x802b3c08,{11,1}},{0x802b3c08,{13,1}},
    {0x802b4558,{0,9,1,4,0}},{0x802b4558,{0,11,1,5,0}},{0x802b4558,{0,13,1,4,0}},
    {0x802b6f3c,{1}},{0x802b6f78,{4,0,1,1,0,2,2}},
    {0x802b5084,{1}},{0x802b4e04,{0,1,4,60,0,125}},
    {0x802b8fcc,{1}},{0x802b89e8,{0,0,0,0,1,0}},{0x802b8960,{0,15,10,8,15}},
    {0x802b8a50,{0,0,0,0,1,0}},{0x802b89a4,{0,7,5,4,7}},{0x802b8e30,{0,0,0,4}}
};
constexpr BattleAudienceGXRequest apState[]{
    {0x802b932c,{1,4,5,0}},{0x802b940c,{1}},{0x802b8d60,{7,0,0,7,0}},{0x802b93d8,{1,3,0}}
};
}
bool resumePackedBattleAudienceAux(uint32_t callback,const BattleAudienceAuxServices& s,BattleAudienceAuxCursor& c){
    if(callback!=0x801a37f0 && callback!=0x801a36c8)return false;
    const auto& common=s.common;const auto& a=common.actors;const bool ap=callback==0x801a36c8;
    if(!c.active){c={};c.active=true;c.callback=callback;}
    if(c.callback!=callback)return false;
    if(c.phase==0){
        c.pending=0x801a86a0;if(!a.memory.word)return false;
        if(ap){c.owner=base(a);c.phase=1;}else c.phase=2;
    }
    if(c.phase==1){
        c.pending=0x8000f2fc;
        if(!common.camera || !common.camera(common.user,8))return false; // Original return unused.
        c.phase=2;
    }
    if(c.phase==2){(void)base(a);c.phase=3;} // GXInit independently calls BaseGetPtr, return unused.
    if(c.phase==3){
        while(c.step<18){const auto& r=setup[c.step];c.pending=r.function;
            if(!common.request || !common.request(common.user,r))return false;
            ++c.step;}
        c.step=0;c.phase=4;
    }
    if(c.phase==4){
        if(ap)while(c.step<4){const auto& r=apState[c.step];c.pending=r.function;
            if(!common.request || !common.request(common.user,r))return false;
            ++c.step;}
        c.phase=5;
    }
    if(c.phase==5){c.record=ap?c.owner+0x10e7c:base(a)+0xf25c;c.index=0;c.phase=6;}
    while(c.index<100){
        if(c.phase==6){
            const auto flags=a.memory.word(a.memory.user,c.record); // Exactly one fresh load.
            if(ap?!(flags&1):(flags&3)!=3){++c.index;c.record+=ap?0x60:0x48;continue;}
            c.icon={};c.icon.function=ap?0x8001a4bc:0x8001a6e0;
            c.step=0;c.phase=ap?7:10;
        }
        if(c.phase==10){
            c.pending=0x801a37f0;if(!a.memory.half)return false;
            const uint32_t item=a.memory.word(a.memory.user,c.record+0x10);
            const auto row=0x803108a8u+40u*item; // Original mulli/add modulo32.
            c.icon.icon=a.memory.half(a.memory.user,row+0x20);c.phase=7;
        }
        if(c.phase==7){
            const unsigned count=ap?5:6;
            while(c.step<count){
                unsigned target=0;
                if(ap && c.step==1)target=1;
                if(ap && c.step==2)target=2;
                if(!ap && c.step==1)target=1;
                if(!c.matrixPending){
                    auto& r=c.matrix;r={};
                    if(ap){switch(c.step){
                    case 0:r.function=0x8029e2b0;r.xyz={scalar(a,c.record+0x38),scalar(a,c.record+0x3c),scalar(a,c.record+0x40)};break;
                    case 1:r.function=0x8029dff0;r.radians=scalar(a,0x80424988);break;
                    case 2:r.function=0x8029e230;r.xyz={scalar(a,c.record+8),scalar(a,c.record+12),scalar(a,0x80424988)};break;
                    case 3:r.function=0x8029dd64;r.left=c.matrices[0];r.right=c.matrices[1];break;
                    case 4:r.function=0x8029dd64;r.left=c.matrices[2];r.right=c.matrices[0];break;
                    }}else {switch(c.step){
                    case 0:{r.function=0x8029e230;const float zero=scalar(a,0x80424988);r.xyz={zero,scalar(a,0x80424a10),zero};break;}
                    case 1:r.function=0x8029dff0;r.radians=scalar(a,0x80424a14)*scalar(a,c.record+0x24);break;
                    case 2:r.function=0x8029dd64;r.left=c.matrices[1];r.right=c.matrices[0];break;
                    case 3:{r.function=0x8029e264;r.left=c.matrices[0];const float zero=scalar(a,0x80424988);r.xyz={zero,scalar(a,0x80424a18),zero};break;}
                    case 4:{r.function=0x8029e2d8;r.left=c.matrices[0];const float scale=scalar(a,c.record+0x20);r.xyz={scale,scale,scalar(a,0x80424990)};break;}
                    case 5:r.function=0x8029e264;r.left=c.matrices[0];r.xyz={scalar(a,c.record+0x14),scalar(a,c.record+0x18),scalar(a,c.record+0x1c)};break;
                    }}
                    c.matrixPending=true;
                }
                c.pending=c.matrix.function;if(!s.matrix || !s.matrix(s.user,c.matrix))return false;
                c.matrices[target]=c.matrix.output;c.matrixPending=false;++c.step;
            }
            c.icon.matrix=c.matrices[0];
            if(ap){c.icon.flags=16;c.icon.icon=442;c.icon.rgba=a.memory.word(a.memory.user,0x80424978);
                c.phase=9;
            }else c.phase=8;
        }
        if(c.phase==9){c.pending=0x8001a4bc;if(!a.byte)return false;
            c.icon.rgba=(c.icon.rgba&0xffffff00u)|a.byte(a.user,c.record+0x5c);c.phase=8;
        }
        if(c.phase==8){c.pending=c.icon.function;
            if(s.icon){if(!s.icon(s.user,c.icon))return false;}
            else {if(!s.sourceIcon || !resumeSourceIconMatrix(c.icon,*s.sourceIcon,c.iconCursor)){
                if(c.iconCursor.active)c.pending=c.iconCursor.pending;return false;}}
            ++c.index;c.record+=ap?0x60:0x48;c.phase=6;}
    }
    c.active=false;c.pending=0;return true;
}
void bindAudienceAuxMathAdapter(BattleAudienceAuxServices& s){
    s.matrix=[](void*,BattleAudienceMatrixRequest& r){
        for(float v:r.xyz)if(!std::isfinite(v))return false;
        if(!std::isfinite(r.radians))return false;
        for(float v:r.left)if(!std::isfinite(v))return false;
        for(float v:r.right)if(!std::isfinite(v))return false;
        auto out=r.left;
        switch(r.function){
        case 0x8029e230:out={1,0,0,r.xyz[0],0,1,0,r.xyz[1],0,0,1,r.xyz[2]};break;
        case 0x8029e2b0:out={r.xyz[0],0,0,0,0,r.xyz[1],0,0,0,0,r.xyz[2],0};break;
        case 0x8029dd04:out={1,0,0,0,0,1,0,0,0,0,1,0};break;
        case 0x8029dd30:out=r.left;break; // Source caller uses disjoint or exact in-place matrices.
        case 0x8029dff0:{const float sn=std::sin(r.radians),cs=std::cos(r.radians);
            if(r.axis==120)out={1,0,0,0,0,cs,-sn,0,0,sn,cs,0};
            else if(r.axis==121)out={cs,0,sn,0,0,1,0,0,-sn,0,cs,0};
            else if(r.axis==122)out={cs,-sn,0,0,sn,cs,0,0,0,0,1,0};
            else return false;
            break;}

        case 0x8029e264:for(unsigned row=0;row<3;++row)out[4*row+3]=r.left[4*row+3]+r.xyz[row];break;
        case 0x8029e2d8:for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)out[4*row+col]=r.left[4*row+col]*r.xyz[row];break;
        case 0x8029dd64:
            for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col){const unsigned i=4*row;
                const float xy=std::fma(r.left[i+1],r.right[4+col],r.left[i]*r.right[col]);
                const float xyz=std::fma(r.left[i+2],r.right[8+col],xy);
                out[i+col]=col>=2?std::fma(col==3?1.f:0.f,r.left[i+3],xyz):xyz;}
            break;
        default:return false;
        }
        for(float v:out)if(!std::isfinite(v))return false;
        r.output=out;return true;
    };
}
}
