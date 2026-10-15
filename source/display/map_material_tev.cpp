#include "map_material_tev.hpp"
namespace ttyd {
namespace {
// Stage rows retain GX order/output/RGB/alpha arguments verbatim. Numeric
// input domains and blend/cull conventions: docs/code/display-state.md.
// order255 means no texture/raster input; output0 PREV, 1/2 source registers.
struct Stage {std::array<int32_t,3> order;int32_t out;std::array<int32_t,4> rgb,alpha;};
constexpr Stage first{{0,0,255},0,{15,15,15,8},{7,7,7,4}};
constexpr Stage last{{255,255,4},0,{15,0,10,15},{7,0,5,7}};
// Eight fixed bases cover source modes 1..7 and12; mode 5 has four stages,
// others three. Zero/no-texture/generic bases are selected independently.
constexpr Stage fixed[8][4]{
    {first,{{1,1,255},0,{0,8,9,15},{7,7,7,0}},last},
    {first,{{1,1,255},0,{15,0,9,15},{7,0,4,7}},last},
    {first,{{1,1,255},0,{0,15,9,15},{0,7,4,7}},last},
    {first,{{1,1,255},0,{15,0,8,15},{7,0,4,7}},{{0,0,255},0,{8,0,1,15},{7,7,7,4}}},
    {{{0,0,255},1,{15,15,15,8},{7,7,7,4}},{{1,1,255},2,{15,15,15,8},{7,7,7,4}},{{2,2,255},0,{2,4,9,15},{1,2,4,7}},last},
    {first,{{1,1,255},0,{0,8,1,15},{7,4,0,7}},last},
    {first,{{1,1,255},0,{0,8,9,15},{0,7,4,4}},last},
    {{{0,0,255},0,{15,15,15,8},{7,7,7,7}},{{1,1,255},0,{15,15,15,0},{7,7,7,4}},last}
};
ShadowGXRequest stageRequest(const Stage& row,uint32_t stage,uint32_t step){
    ShadowGXRequest r;const auto n=int32_t(stage);r.values[0]=n;
    switch(step){
    case 0:r.kind=ShadowGX::TevOrder;r.values={n,row.order[0],row.order[1],row.order[2]};break;
    case 1:case 2:r.kind=step==1?ShadowGX::TevColorOp:ShadowGX::TevAlphaOp;r.values={n,0,0,0,1,row.out};break;
    case 3:case 4:{r.kind=step==3?ShadowGX::TevColorIn:ShadowGX::TevAlphaIn;const auto& a=step==3?row.rgb:row.alpha;r.values={n,a[0],a[1],a[2],a[3]};break;}
    default:r.kind=ShadowGX::TevSwapMode;break;
    }return r;
}
}
// Stage setters and optional source blocks acknowledge independently. Queued
// calls hold exact arguments; missing branch providers cannot be skipped.
bool resumeMapMaterialTev(const MapTevInput& input,const MapTevServices& s,MapTevCursor& c){
    if(!c.active){c={};c.active=true;c.input=input;c.counts.maps=c.counts.generators=uint32_t(input.slots);}
    auto emit=[&](const ShadowGXRequest& r){if(!c.queued){c.request=r;c.queued=true;}c.pending=uint32_t(c.request.kind);
        if(!s.gx || !s.gx(s.user,c.request))return false;
        c.queued=false;return true;};
    auto branch=[&](uint32_t entry){c.pending=entry;return s.branch && s.branch(s.user,entry,c.input,c.counts,c.pending);};
    for(;;)switch(c.phase){
    case 0:{
        const auto mode=c.input.mode;const auto n=c.input.slots;
        if(n>0 && (mode==8 || mode==9)){if(!branch(mode==8?0x800229fc:0x80022b00))return false;c.phase=1;break;}
        if(n>0 && mode==0){
            ShadowGXRequest r;const ShadowGX kinds[]{ShadowGX::TevOrder,ShadowGX::TevOp,ShadowGX::TevSwapMode};r.kind=kinds[c.step];
            if(c.step==0)r.values={0,0,0,4};
            if(!emit(r))return false;
            if(++c.step<3)break;
        }else {
            const bool isFixed=n>0 && ((mode>=1 && mode<=7) || mode==12);
            const uint32_t k=isFixed?(mode==5?4:3):1;
            const Stage simple=n<=0?Stage{{255,255,4},0,{15,15,15,10},{7,7,7,5}}:Stage{{0,0,4},0,{15,10,8,15},{7,5,4,7}};
            const Stage& row=isFixed?fixed[mode==12?7:mode-1][c.stage]:simple;
            if(!emit(stageRequest(row,c.stage,c.step)))return false;
            if(++c.step<6)break;
            c.step=0;if(++c.stage<k)break;
            c.counts.stages=k;
        }
        c.step=0;c.stage=0;c.phase=1;break;}
    // Unless flag 10000000(hex), append PREV*K0 stage with selectors 12/28.
    // All eight setters must complete before incrementing the stage counter.
    case 1:
        if(!(c.input.flags&0x10000000)){
            ShadowGXRequest r;const auto k=int32_t(c.counts.stages);
            if(c.step<2){r.kind=c.step==0?ShadowGX::TevKColorSel:ShadowGX::TevKAlphaSel;r.values={k,c.step==0?12:28};}
            else {constexpr Stage row{{255,255,255},0,{15,0,14,15},{7,0,6,7}};r=stageRequest(row,c.counts.stages,c.step-2);}
            if(!emit(r))return false;
            if(++c.step<8)break;
            ++c.counts.stages;
        }
        c.step=0;c.phase=2;break;
    case 2:if((c.input.flags&4) && !branch(0x8002bc7c))return false;++c.phase;break;
    // Flag/block order: 400 projection, 10000 depth, 20000 projected shadow,
    // 40000 alternate depth, 80100000 terminal color, 800000 cosine alpha(hex).
    // Preserve every branch hold before proceeding to the next mask.
    case 3:case 4:case 5:case 6:case 7:case 8:{
        constexpr uint32_t masks[]{0x400,0x10000,0x20000,0x40000,0x80100000,0x800000};
        constexpr uint32_t entries[]{0x80023128,0x80023314,0x80023d58,0x80023e9c,0x800244ac,0x80024654};
        if((c.input.flags&masks[c.phase-3]) && !branch(entries[c.phase-3]))return false;
        ++c.phase;break;}
    // Optional callback0f000000(hex) gets a copied counter set with indirect
    // reset0; only stages/generators/indirect copy back. Texture-map count does
    // not inherit callback modifications. Final setters narrow to low 8 bits.
    case 9:
        if(c.input.flags&0x0f000000){
            if(!c.callbackReady){c.callbackCounts=c.counts;c.callbackCounts.indirect=0;c.callbackReady=true;}
            c.pending=0x800248a8;
            if(!s.branch || !s.branch(s.user,0x800248a8,c.input,c.callbackCounts,c.pending))return false;
            c.counts.stages=c.callbackCounts.stages;c.counts.generators=c.callbackCounts.generators;c.counts.indirect=c.callbackCounts.indirect;
        }
        ++c.phase;break;
    case 10:{
        ShadowGXRequest r;const ShadowGX kinds[]{ShadowGX::NumTexGens,ShadowGX::NumTevStages,ShadowGX::NumIndStages};r.kind=kinds[c.step];
        const uint32_t counts[]{c.counts.generators,c.counts.stages,c.counts.indirect};r.values[0]=int32_t(uint8_t(counts[c.step]));
        if(!emit(r))return false;
            if(++c.step<3)break;
        c.active=false;c.pending=0;return true;}
    default:c.pending=0x80021b80;return false;
    }
}
}
