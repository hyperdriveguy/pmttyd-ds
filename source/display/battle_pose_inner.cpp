#include "battle_pose_inner.hpp"
namespace ttyd {
// Optional false is an acknowledged opaque result; nullopt is unavailable.
// Pose40(hex) enables primary alpha; secondary always participates. Map flags
// 2/4 enable colors unless pose 800/1000 suppress them; each query is fresh.
std::optional<bool> battlePoseMapTranslucent(uint32_t flags,uint32_t primary,uint32_t secondary,const BattlePoseInnerServices& s,BattlePoseMapCursor& c){
    if(c.step==0){
        if(((flags&0x40) && uint8_t(primary)!=255) || uint8_t(secondary)!=255){c.step=4;return true;}
        c.step=1;
    }
    if(c.step==1){
        if(!(flags&0x800)){const auto g=s.mapGroup?s.mapGroup(s.user):std::nullopt;if(!g)return {};if((g->flags&2) && g->primaryAlpha!=255){c.step=4;return true;}}
        c.step=2;
    }
    if(c.step==2){
        if(!(flags&0x1000)){const auto g=s.mapGroup?s.mapGroup(s.user):std::nullopt;if(!g)return {};if((g->flags&4) && g->secondaryAlpha!=255){c.step=4;return true;}}
        c.step=3;
    }
    return c.step==4;
}
// Pose+14 selects descriptor,+28/+2c current/next frame,+30 fraction.
// Capture source context and secondary THEN primary colors before material
// predicate; live flags are read later at the actual mapTestXLU boundary.
bool resumeBattlePoseInner(BattlePosePool& pool,uint32_t mode,std::optional<uint32_t>& workPass,const BattlePartDrawRequest& input,const BattlePoseInnerServices& s,BattlePoseInnerCursor& c){
    if(!c.active){
        auto* p=pool.pose(input.handle);if(!p || !p->resource || !p->resource->modelFlags)return false;
        const auto index=p->word(0x14);
        // Converted metadata cannot prove an original null descriptor. Missing
        // selected data stays a provider boundary rather than an empty draw.
        if(index>=p->resource->names.size() || !p->resource->bank || !p->resource->bank->pose(p->resource->names[index].c_str()))return false;
        c={};c.active=true;c.input=input;
        c.context={input.handle,index,p->word(0x28),p->word(0x2c),input.pass,p->scalar(0x30),p->resource,input.matrix};
        if((mode && !p->word(0xc)) || (!mode && p->word(0xc)==1)){c.active=false;return true;}
        const auto flags=*p->resource->modelFlags;
        const uint32_t shift=(input.pass&32)?0:1u<<(input.pass&31); // literal PPC slw
        if(!(input.pass==2 && (flags&1)) && !(flags&shift)){c.active=false;return true;}
        c.secondary=p->word(0xf4);c.primary=p->word(0xf0);c.phase=1;
    }
    if(c.input.handle!=input.handle || c.input.pass!=input.pass)return false;
    auto* p=pool.pose(c.input.handle);if(!p){c.pending=0x8003bc68;return false;}
    auto matrix=[&](auto prepare){
        if(!c.requestPending){c.matrix={};prepare();c.requestPending=true;}
        c.pending=c.matrix.function;
        if(!s.matrix || !s.matrix(s.user,c.matrix))return false;
        c.requestPending=false;return true;
    };
    while(c.active){switch(c.phase){
    case 1:{c.pending=0x8001ca2c;
        // Flags are the live input at the actual leaf invocation; colors were
        // captured beforehand, in secondary-then-primary source order.
        if(!c.requestPending){c.mapFlags=p->word(0xe8);c.requestPending=true;}
        const auto translucent=battlePoseMapTranslucent(c.mapFlags,c.primary,c.secondary,s,c.map);if(!translucent)return false;
        c.effectivePass=*translucent?3:c.input.pass;c.requestPending=false;c.materialCallback=p->word(0xe0);c.phase=c.materialCallback?2:3;c.step=0;break;}
    case 2:c.pending=c.materialCallback;if(!s.callback || !s.callback(s.user,c.materialCallback,c.effectivePass))return false;c.phase=4;break;
    // Built-in passes0..3 install source Z/blend/alpha state; 3 translucent
    // disables depth writes, 0/2 alpha>=128. A material callback owns its own
    // work-pass publication; only the built-in branch updates workPass here.
    case 3:
        if(c.effectivePass<=3){while(c.step<4){
            if(!c.requestPending){c.gx={};const bool translucent=c.effectivePass==3;
                switch(c.step){
                case 0:c.gx.function=0x802b93d8;c.gx.arguments={1,3,translucent?0:1};break;
                case 1:c.gx.function=0x802b932c;c.gx.arguments={translucent?1:0,4,5,0};break;
                case 2:c.gx.function=0x802b940c;c.gx.arguments={c.effectivePass==1 || translucent?1:0};break;
                case 3:c.gx.function=0x802b8d60;c.gx.arguments=c.effectivePass==0 || c.effectivePass==2?std::array<int32_t,7>{6,128,1,0,0}:std::array<int32_t,7>{7,0,0,7,0};break;
                }c.requestPending=true;
            }
            c.pending=c.gx.function;if(!s.gx || !s.gx(s.user,c.gx))return false;c.requestPending=false;++c.step;
        }}
        workPass=c.effectivePass;c.phase=4;break;
    case 4:
        if(p->word(0)&8){c.phase=99;break;} // Source camera4 facing graph is separate.
        c.phase=c.input.angle!=0?5:7;break;
    case 5:
        if(!matrix([&](){c.matrix.function=0x8029dff0;c.matrix.axis=121;c.matrix.radians=c.input.angle*.01745329238474369f;}))return false;
        c.operand=c.matrix.output;c.phase=6;break;
    case 6:
        if(!matrix([&](){c.matrix.function=0x8029dd64;c.matrix.left=c.context.model;c.matrix.right=c.operand;}))return false;
        c.context.model=c.matrix.output;c.phase=7;break;
    case 7:c.phase=c.input.scale!=1?8:10;break;
    case 8:
        if(!matrix([&](){c.matrix.function=0x8029e2b0;c.matrix.xyz={c.input.scale,c.input.scale,c.input.scale};}))return false;
        c.operand=c.matrix.output;c.phase=9;break;
    case 9:
        if(!matrix([&](){c.matrix.function=0x8029dd64;c.matrix.left=c.context.model;c.matrix.right=c.operand;}))return false;
        c.context.model=c.matrix.output;c.phase=10;break;
    // Pose+3c ffffffff(hex) requests original Main. Linked shared-ring flag
    // +4 bit 2 and facing flag 8 hold at separate real owners, not empty draws.
    case 10:
        if(!c.requestPending){
            if(p->word(0x3c)!=0xffffffff){c.phase=11;break;}
            if(p->word(4)&2){c.phase=98;break;} // Source shared-ring preparation is not bound.
            c.requestPending=true;
        }
        c.pending=0x8003ed2c;if(!s.main || !s.main(s.user,c.input.handle))return false;
        c.requestPending=false;c.phase=11;break;
    case 11:
        // Modeling base Copy precedes source W18/1c/20 publication.
        if(!matrix([&](){c.matrix.function=0x8029dd30;c.matrix.left=c.context.model;}))return false;
        c.context.model=c.matrix.output;c.phase=12;break;
    case 12:c.pending=0x8003c210;if(!s.publish || !s.publish(s.user,c.context))return false;c.phase=13;c.step=0;break;
    // Flush positions then normals, then invalidate GPU vertices, then traverse.
    // Each flush resolves current output/count; do not capture both before hold.
    case 13:
        while(c.step<2){c.pending=0x80296a54;if(!s.flush || !s.flush(s.user,c.step,c.context))return false;++c.step;}
        c.phase=14;break;
    case 14:c.pending=0x802b4df4;if(!s.invalidateVertices || !s.invalidateVertices(s.user))return false;c.phase=15;break;
    case 15:c.pending=0x8003c4bc;if(!s.traverse || !s.traverse(s.user,c.context))return false;c.active=false;c.pending=0;return true;
    case 98:c.pending=0x8003bc68;return false;
    case 99:c.pending=0x8000f2d0;return false;
    default:return false;
    }}return true;
}
}
