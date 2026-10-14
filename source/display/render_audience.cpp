#include "platform.hpp"
#include "battle_camera_frame.hpp"
namespace ttyd {
bool Renderer::prepareAudienceDrawing(){
    if(!c3dReady || !programReady || !top)return false;
    if(audienceVertices && audienceTexture.imported)return true;
    if(!audienceAssets.loadFrames("romfs:"))return false;
    C3D_FrameSync();
    if(!audienceVertices)audienceVertices=static_cast<Vertex*>(linearAlloc(sizeof(Vertex)*3600));
    if(!audienceVertices)return false;
    if(!audienceTexture.imported){FILE* file=std::fopen("romfs:/audience-frames.t3x","rb");if(!file)return false;
        audienceTexture.imported=Tex3DS_TextureImportStdio(file,&audienceTexture.texture,nullptr,false);std::fclose(file);
        if(!audienceTexture.imported)return false;
        C3D_TexSetFilter(&audienceTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&audienceTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    }
    audienceBatch.assets=&audienceAssets;return true;
}
void Renderer::releaseAudienceDrawing(){
    if(c3dReady)C3D_FrameSync();
    if(audienceTexture.imported){Tex3DS_TextureFree(audienceTexture.imported);C3D_TexDelete(&audienceTexture.texture);}
    audienceTexture={};if(audienceVertices)linearFree(audienceVertices);audienceVertices=nullptr;
    audienceVertexCount=audienceOpaqueCount=0;audiencePrepared=false;audienceBatch.begin();
}
void Renderer::drawAudienceScene(const Game& game,bool lower){
    if(!audienceVertices || !audienceTexture.imported)return;
    if(!lower){
        audiencePrepared=false;audienceVertexCount=audienceOpaqueCount=0;audienceBatch.begin();
        struct Context {Renderer* renderer;const Game* game;uint8_t mode=1;std::array<float,6> vectors;} context{this,&game};
        context.vectors={0,280,550,0,100,-40};
        if(game.trioBattleActive && game.scene!=Scene::Battle && game.scene!=Scene::Victory && game.scene!=Scene::Defeat){
            auto camera=game.fieldCameraFrame;camera.eye={0,2.8f,5.5f};camera.at={0,1.f,-.4f};
            const auto* owner=game.trioAttack.cameras()?game.trioAttack.cameras():game.cameraManager.get();
            const auto* published=owner?owner->camera(4):nullptr;
            if(published && published->viewPublished){
                camera.eye={published->eye[0]*.01f,published->eye[1]*.01f,published->eye[2]*.01f};camera.at={published->at[0]*.01f,published->at[1]*.01f,published->at[2]*.01f};
            }else if(game.trioAttack.cameraDepthSnapshot())if(const auto* live=game.trioAttack.cameraFrameState()){
                camera.eye={live->eye[0]*.01f,live->eye[1]*.01f,live->eye[2]*.01f};camera.at={live->at[0]*.01f,live->at[1]*.01f,live->at[2]*.01f};
            }
            context.vectors={camera.eye.x*100,camera.eye.y*100,camera.eye.z*100,camera.at.x*100,camera.at.y*100,camera.at.z*100};
        }
        BattleAudienceDrawServices services;services.user=&context;bindAudienceMathAdapters(services);
        services.camera=[](void*,uint32_t index)->std::optional<uint32_t>{return index==4?std::optional<uint32_t>{4}:std::nullopt;};
        // Independent native world adapter: source world coordinates are kept,
        // then its existing camera/projection applies once on the GPU.
        services.view=[](void*,uint32_t)->std::optional<BattleActMatrix>{return BattleActMatrix{1,0,0,0,0,1,0,0,0,0,1,0};};
        services.vectors=[](void* p,uint32_t)->std::optional<std::array<float,6>>{return static_cast<Context*>(p)->vectors;};
        services.mapMode=[](void* p)->std::optional<uint8_t>{return static_cast<Context*>(p)->mode;};
        services.texture=[](void* p,uint32_t bank,uint8_t index)->std::optional<BattleAudienceTexture>{const auto* f=static_cast<Context*>(p)->renderer->audienceAssets.frame(bank,index);
            if(!f)return {};return BattleAudienceTexture{bank,index,f->width,f->height};};
        services.request=[](void* p,const BattleAudienceGXRequest& r){return static_cast<Context*>(p)->renderer->audienceBatch.request(r);};
        services.quad=[](void* p,const BattleAudienceQuad& q){return static_cast<Context*>(p)->renderer->audienceBatch.emitQuad(q);};
        const auto prepare=[&](){return game.harborTutorial.running()?game.harborTutorial.prepareAudienceDrawing(services):game.trioBattleActive && game.trioAttack.prepareAudienceDrawing(services);};
        if(!prepare())return;audienceOpaqueCount=unsigned(audienceBatch.vertices.size());context.mode=2;
        if(!prepare() || audienceBatch.vertices.size()>3600)return;
        audienceVertexCount=unsigned(audienceBatch.vertices.size());
        for(unsigned i=0;i<audienceVertexCount;++i){const auto& v=audienceBatch.vertices[i];audienceVertices[i]={v.x*.01f,v.y*.01f,v.z*.01f,v.u,v.v,
            float(v.rgba>>24)/255,float((v.rgba>>16)&255)/255,float((v.rgba>>8)&255)/255,float(v.rgba&255)/255};}
        if(audienceVertexCount)GSPGPU_FlushDataCache(audienceVertices,audienceVertexCount*sizeof(Vertex));
        audiencePrepared=true;
    }
    if(!audiencePrepared || !audienceVertexCount)return;
    C3D_BindProgram(&program);auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,audienceVertices,sizeof(Vertex),3,0x210);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
    C3D_CullFace(GPU_CULL_NONE);C3D_TexBind(0,&audienceTexture.texture);
    for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env,C3D_RGB,GPU_ADD);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
    constexpr unsigned rgb=GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE;
    C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITEMASK(rgb|GPU_WRITE_DEPTH));C3D_AlphaTest(true,GPU_GEQUAL,128);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,GPU_ZERO,GPU_ONE,GPU_ZERO);
    if(audienceOpaqueCount)C3D_DrawArrays(GPU_TRIANGLES,0,audienceOpaqueCount);
    C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITEMASK(rgb));C3D_AlphaTest(false,GPU_ALWAYS,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
    if(audienceVertexCount>audienceOpaqueCount)C3D_DrawArrays(GPU_TRIANGLES,audienceOpaqueCount,audienceVertexCount-audienceOpaqueCount);
    // Independent world adapter's normal texture*raster policy for its next draw.
    for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));env=C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_DepthTest(true,GPU_GREATER,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
}
}
