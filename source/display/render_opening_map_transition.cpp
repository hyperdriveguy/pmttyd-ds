#include "platform.hpp"
#include <cstring>
namespace ttyd {
bool Renderer::prepareMapTurn(OpeningMapTransition& state){
    if(!state.requested)return false;
    if(!mapTurnReady){
        if(!mapTurnAnimations.load("romfs:/opening-map-turn.pmanim"))return false;
        const auto* exit=mapTurnAnimations.pose("Z_1");const auto* entry=mapTurnAnimations.pose("A_1");
        if(!exit || !entry || exit->start!=0 || exit->end!=0 || exit->keys.size()!=1 || exit->loop || entry->start!=0 || entry->end!=80 || entry->keys.size()!=45 || entry->loop || exit->bindings.size()!=264 || entry->bindings.size()!=264)return false;
        FILE* f=std::fopen("romfs:/opening-map-turn.bin","rb");if(!f)return false;
        char magic[8];uint32_t count=0;bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMMAP001",8) && std::fread(&count,4,1,f)==1 && count==88;
        for(unsigned n=0;n<88 && ok;++n){uint32_t row[3];ok=std::fread(row,sizeof(row),1,f)==1 && row[0]==(n<44?1u:2u) && row[1]==2 && row[2]==1;}
        ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
        if(!mapTurnVertices)mapTurnVertices=(Vertex*)linearAlloc(264*sizeof(Vertex));
        if(!mapTurnVertices)return false;
        if(!mapTurnTarget){
            if(!C3D_TexInitVRAM(&mapTurnCapture,512,256,GPU_RGBA8))return false;
            mapTurnTarget=C3D_RenderTargetCreateFromTex(&mapTurnCapture,GPU_TEXFACE_2D,0,GPU_RB_DEPTH24_STENCIL8);
            if(!mapTurnTarget){C3D_TexDelete(&mapTurnCapture);return false;}
            C3D_TexSetFilter(&mapTurnCapture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&mapTurnCapture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
        }
        mapTurnReady=true;
    }
    state.admitted(0,80);return true;
}
bool Renderer::frontMapTurn(OpeningMapTransition& state,unsigned type,float& presented){
    if(!prepareMapTurn(state) || !state.captureReady || (type!=49 && type!=50))return false;
    if(type==50 && state.entryComplete)return true;
    const float frame=type==49?0:state.entryFrame;
    const auto* pose=mapTurnAnimations.pose(type==49?"Z_1":"A_1");
    if(!pose || !pose->evaluate(frame,mapTurnSample) || mapTurnSample.vertices.size()!=264)return false;
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-200,200,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
    for(unsigned n=0;n<264;++n){
        const auto& p=mapTurnSample.vertices[n];const unsigned cell=n<132?1:0;
        const float u=(p.u*1024-cell*128-2)/124,v=((1-p.v)*1024-2)/124;
        const auto color=mapTurnSample.colors.empty()?ActorColor{{1,1,1,1}}:mapTurnSample.colors[n];
        mapTurnVertices[n]={200+p.x*304/64,120-p.y*5,-p.z*304/64,(48+u*304)/512,(1-v)*240/256,color[0],color[1],color[2],color[3]};
    }
    GSPGPU_FlushDataCache(mapTurnVertices,264*sizeof(Vertex));auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,mapTurnVertices,sizeof(Vertex),3,0x210);
    C3D_SetScissor(GPU_SCISSOR_NORMAL,0,48,240,352);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_CullFace(GPU_CULL_BACK_CCW);C3D_AlphaTest(true,GPU_GEQUAL,128);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,GPU_ZERO,GPU_ONE,GPU_ZERO);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    // Kind1 binds actual capture0 first. The following kind2 has no admitted
    // capture1 in source49/50 and inherits this sampler/UV state. Preserve
    // original44+44 triangle order instead of sorting by material kind.
    C3D_TexBind(0,&mapTurnCapture);C3D_DrawArrays(GPU_TRIANGLES,0,132);C3D_DrawArrays(GPU_TRIANGLES,132,132);
    if((type==49 && !state.exitComplete) || (type==50 && frame==80 && !state.entryComplete))presented=frame;
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_CullFace(GPU_CULL_NONE);C3D_AlphaTest(true,GPU_GREATER,0);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);C2D_Prepare();C2D_SceneBegin(top);return true;
}
void Renderer::freeMapTurn(){
    if(mapTurnTarget){C3D_RenderTargetDelete(mapTurnTarget);mapTurnTarget=nullptr;C3D_TexDelete(&mapTurnCapture);}
    if(mapTurnVertices){linearFree(mapTurnVertices);mapTurnVertices=nullptr;}
    mapTurnAnimations={};mapTurnSample={};mapTurnReady=false;
}
}
