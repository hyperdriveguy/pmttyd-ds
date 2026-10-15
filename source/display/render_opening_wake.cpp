#include "platform.hpp"
#include "opening_wake_effects.hpp"
namespace ttyd {
// Native 400x240 world fits512x256 texture backing.48 vertices hold eight
// six-vertex taps; the render target borrows this retained texture.
bool Renderer::prepareWakeWorld() {
    if(wakeCaptureReady && wakeWorldTarget && wakeEffectVertices)return true;
    if(!wakeCaptureReady){if(!C3D_TexInitVRAM(&wakeCaptureTexture,512,256,GPU_RGBA8))return false;wakeCaptureReady=true;}
    if(!wakeWorldTarget)wakeWorldTarget=C3D_RenderTargetCreateFromTex(&wakeCaptureTexture,GPU_TEXFACE_2D,0,GPU_RB_DEPTH24_STENCIL8);
    if(!wakeEffectVertices)wakeEffectVertices=static_cast<Vertex*>(linearAlloc(sizeof(Vertex)*48));
    C3D_TexSetFilter(&wakeCaptureTexture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&wakeCaptureTexture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    return wakeWorldTarget && wakeEffectVertices;
}
bool Renderer::frontWakeEffects(const OpeningWake& wake) {
    // The actual current world target is the source capture owner, before
    // eyelids and windows. Ordered GPU passes avoid active-frame CPU readback.
    if(!prepareWakeWorld())return false;
    C2D_Flush();
    C3D_RenderTargetClear(top,C3D_CLEAR_ALL,0,0);
    C3D_FrameDrawOn(top);C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    constexpr unsigned corners[6][2]={{0,0},{1,0},{1,1},{0,0},{1,1},{0,1}};
    for(unsigned tap=0;tap<8;++tap){const auto sample=openingWakeTap(tap,wake.depth);
        for(unsigned n=0;n<6;++n){const float x=corners[n][0],y=corners[n][1];
            wakeEffectVertices[tap*6+n]={48+x*304,y*240,0,(48+(x+sample.u)*304)/512,(1-y-sample.v)*240/256,.125f,.125f,.125f,.125f};
        }
    }
    GSPGPU_FlushDataCache(wakeEffectVertices,sizeof(Vertex)*48);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,wakeEffectVertices,sizeof(Vertex),3,0x210);
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);C3D_TexBind(0,&wakeCaptureTexture);
    C3D_SetScissor(GPU_SCISSOR_NORMAL,0,48,240,352);
    // Eight equally weighted taps(.125 each) accumulate RGB/alpha. First
    // overwrites destination, next taps add. Zero blur depth uses one full-white
    // tap instead; flush again after modifying its CPU-side color.
    const unsigned taps=wake.depth>0?8:1;
    if(taps==1)for(unsigned n=0;n<6;++n)wakeEffectVertices[n].r=wakeEffectVertices[n].g=wakeEffectVertices[n].b=wakeEffectVertices[n].a=1;
    GSPGPU_FlushDataCache(wakeEffectVertices,sizeof(Vertex)*48);
    for(unsigned tap=0;tap<taps;++tap){C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,tap?GPU_ONE:GPU_ZERO,GPU_ONE,tap?GPU_ONE:GPU_ZERO);C3D_DrawArrays(GPU_TRIANGLES,tap*6,6);}
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_AlphaTest(true,GPU_GREATER,0);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    C2D_Prepare();C2D_SceneBegin(top);
    // mabutaDisp writes source signed16 vertices240-WW1 / -240+WW1.
    const float edge=std::trunc(240-wake.eyelid);
    const float height=(240-edge)*.5f;
    return C2D_DrawRectSolid(48,0,.4f,304,height,C2D_Color32(0,0,0,255)) &&
           C2D_DrawRectSolid(48,240-height,.4f,304,height,C2D_Color32(0,0,0,255));
}
void Renderer::freeWakeEffects() {
    if(wakeWorldTarget){C3D_RenderTargetDelete(wakeWorldTarget);wakeWorldTarget=nullptr;}
    if(wakeCaptureReady){C3D_TexDelete(&wakeCaptureTexture);wakeCaptureReady=false;}
    if(wakeEffectVertices){linearFree(wakeEffectVertices);wakeEffectVertices=nullptr;}
}
}
