#include "platform.hpp"
#include "opening_title_prompt.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
// PMTIT001 sidecars require UV scale 8 and two poses per bank. Center bank
// has 4 textures, side bank 1; Z_* and A_* retain original authored pose identities.
// Attempt latches prevent repeated failed imports until explicit release.
bool Renderer::loadTitleCurtains(){
    if(titleCurtainsAttempted)return titleCurtainsReady;
    titleCurtainsAttempted=true;
    for(unsigned n=0;n<2;++n){
        auto& actor=titleCurtains[n];const char* stem=n?"title-curtain-sides":"title-curtain-center";char path[96];
        std::snprintf(path,sizeof(path),"romfs:/%s.pmanim",stem);if(!actor.animations.load(path))return false;
        const auto* a=actor.animations.pose(n?"A_1":"Z_2");const auto* z=actor.animations.pose(n?"Z_1":"A_2");
        if(!a || !z || a->start!=0 || a->end!=(n?80:0) || a->keys.size()!=(n?14:1) || z->end!=(n?60:160) || a->loop || z->loop)return false;
        std::snprintf(path,sizeof(path),"romfs:/%s.bin",stem);FILE* file=std::fopen(path,"rb");if(!file)return false;
        char magic[8];uint32_t poses=0,textures=0;
        const bool valid=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMTIT001",8) && std::fread(&actor.uvScale,4,1,file)==1 && std::fread(&poses,4,1,file)==1 && std::fread(&textures,4,1,file)==1 && std::fgetc(file)==EOF;
        std::fclose(file);if(!valid || actor.uvScale!=8 || poses!=2 || textures!=(n?1:4))return false;
        std::snprintf(path,sizeof(path),"romfs:/%s.t3x",stem);file=std::fopen(path,"rb");if(!file)return false;
        actor.image.imported=Tex3DS_TextureImportStdio(file,&actor.image.texture,nullptr,true);std::fclose(file);
        if(!actor.image.imported || !a->evaluate(0,actor.sample))return false;
        C3D_TexSetFilter(&actor.image.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&actor.image.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    }
    titleCurtainsReady=true;return true;
}
// Prompt has independent mask and repeating color pattern UVs. SystemVertex
// uses four attributes(0x3210); TEV preserves mask alpha while tinting its RGB.
bool Renderer::frontTitlePrompt(const OpeningTitle& title){
    if(!frontImage("title-prompt",0,0,0,0) || !frontImage("title-prompt-pattern",0,0,0,0))return false;
    const auto find=[&](const char* name)->FrontTexture*{const auto it=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& t){return t.name==name;});return it==frontTextures.end()?nullptr:&*it;};
    auto* mask=find("title-prompt");auto* pattern=find("title-prompt-pattern");if(!mask || !pattern)return false;
    if(!titlePromptVertices)titlePromptVertices=(SystemVertex*)linearAlloc(sizeof(SystemVertex)*6);
    if(!titlePromptVertices || !messageSystemProgramReady)return false;
    C3D_TexSetWrap(&pattern->image.texture,GPU_REPEAT,GPU_REPEAT);
    constexpr float uv[6][2]={{0,0},{1,0},{1,1},{0,0},{1,1},{0,1}};
    for(unsigned n=0;n<6;++n){const float u=uv[n][0],v=uv[n][1];const auto point=titlePromptSourcePoint(u,v),tex=titlePromptPatternPoint(title.promptCounter,u,v);titlePromptVertices[n]={200+point[0]*.5f,120-point[1]*.5f,0,tex[0],1-tex[1]*float(pattern->size[1])/pattern->size[3],u*float(mask->size[0])/mask->size[2],1-v*float(mask->size[1])/mask->size[3],1,1,1,title.promptAlpha()/255.f};}
    C2D_Flush();C3D_BindProgram(&messageSystemProgram);
    auto* attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,3,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,titlePromptVertices,sizeof(SystemVertex),4,0x3210);
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,messageSystemTransform,&projection);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_GREATER,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* first=C3D_GetTexEnv(0);C3D_TexEnvSrc(first,C3D_Both,GPU_TEXTURE1,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(first,C3D_Both,GPU_REPLACE);
    auto* second=C3D_GetTexEnv(1);C3D_TexEnvSrc(second,C3D_RGB,GPU_PREVIOUS,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(second,C3D_RGB,GPU_MODULATE);
    C3D_TexEnvSrc(second,C3D_Alpha,GPU_PREVIOUS,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(second,C3D_Alpha,GPU_MODULATE);
    C3D_TexBind(0,&pattern->image.texture);C3D_TexBind(1,&mask->image.texture);GSPGPU_FlushDataCache(titlePromptVertices,6*sizeof(SystemVertex));C3D_DrawArrays(GPU_TRIANGLES,0,6);
    C2D_Prepare();C2D_SceneBegin(top);return true;
}
// Source types 33/34 select center A_2/Z_2; 63 selects side A_1, otherwise
// Z_1. Source 64x48 paper projects into native 304x240 centered at(200, 120).
// Pass2 uses alpha>=128; cull 0/1 maps front/back. Final state is reported to
// the outer frame owner, which acknowledges it after GPU completion.
bool Renderer::frontTitleCurtain(OpeningTitle& title,uint32_t& presentedType,float& presentedFrame,float& presentedScale){
    if(!title.state)return true;
    if(!loadTitleCurtains()){title.failed=true;return false;}
    title.admitCurtain(title.curtainType);
    auto& actor=titleCurtains[title.curtainType==34 || title.curtainType==33?0:1];const char* name=title.curtainType==34?"Z_2":title.curtainType==33?"A_2":title.curtainType==63?"A_1":"Z_1";
    const auto* pose=actor.animations.pose(name);const float frame=title.curtainFrame();
    if(actor.sampledPose!=pose || actor.sampledTime!=frame){if(!pose || !pose->evaluate(frame,actor.sample))return false;actor.sampledPose=pose;actor.sampledTime=frame;}
    const auto& sample=actor.sample;if(sample.vertices.size()>size_t(12000-count))return false;
    C2D_Flush();C3D_BindProgram(&program);auto* attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,4);
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-100,100,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
    C3D_SetScissor(GPU_SCISSOR_NORMAL,0,48,240,352);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    const int first=count;
    // disp_maku source64x48 to608x480; explicit native image-plane projection.
    for(size_t n=0;n<sample.vertices.size();++n){const auto& p=sample.vertices[n];const auto color=sample.colors.empty()?ActorColor{{1,1,1,1}}:sample.colors[n];vertex(200+p.x*title.paperScale*304/64,120-p.y*title.paperScale*5,0,p.u,1-(1-p.v)*actor.uvScale,0xffffffff,&color);}
    GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);C3D_TexBind(0,&actor.image.texture);
    for(const auto& batch:sample.batches){
        if(batch.start+batch.count>sample.vertices.size() || batch.pass>2 || batch.cull>3)return false;
        C3D_CullFace(batch.cull==0?GPU_CULL_FRONT_CCW:batch.cull==1?GPU_CULL_BACK_CCW:GPU_CULL_NONE);
        C3D_AlphaTest(true,batch.pass==2?GPU_GEQUAL:GPU_GREATER,batch.pass==2?128:0);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
        C3D_DrawArrays(GPU_TRIANGLES,first+batch.start,batch.count);
    }
    if(!title.curtainFinalPresented && title.curtainDriverFinished()){presentedType=title.curtainType;presentedFrame=frame;presentedScale=title.paperScale;}
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_CullFace(GPU_CULL_NONE);C3D_AlphaTest(true,GPU_GREATER,0);C2D_Prepare();C2D_SceneBegin(top);return true;
}
void Renderer::freeTitleCurtains(){
    for(auto& actor:titleCurtains)if(actor.image.imported){Tex3DS_TextureFree(actor.image.imported);C3D_TexDelete(&actor.image.texture);}
    if(titlePromptVertices){linearFree(titlePromptVertices);titlePromptVertices=nullptr;}
    titleCurtains={};titleCurtainsReady=titleCurtainsAttempted=false;
}
}
