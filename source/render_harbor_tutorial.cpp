#include "platform.hpp"
#include "battle_actor_rotation.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
bool Renderer::loadTutorialPresentation() {
    if(tutorialReady)return true;
    C3D_FrameSync();
    // Field event and collision ownership stay suspended while source setup2
    // owns battle presentation. GPU rooms belong to only one native scene.
    for(auto* room:{&plaza,&stage,&trioBackground,&trioStage})freeRoom(*room);
    auto release=[&] {
        freeRoom(harborBattleBackground);freeRoom(harborBattleStage);
        for(auto& a:tutorialActors){if(a.image.imported){Tex3DS_TextureFree(a.image.imported);C3D_TexDelete(&a.image.texture);}a=CineActor{};}
    };
    if(!loadMap("harbor-tutorial-bti_01",harborBattleBackground) || !loadMap("harbor-tutorial-stg_00_0",harborBattleStage)){release();return false;}
    const char* names[]={"crump","mario","goombella"};
    for(unsigned i=0;i<3;++i) {
        auto& actor=tutorialActors[i];const auto base=std::string("romfs:/harbor-tutorial-")+names[i];
        if(!actor.animations.load((base+".pmanim").c_str())){release();return false;}
        FILE* meta=std::fopen((base+".bin").c_str(),"rb");if(!meta){release();return false;}
        char magic[8];const bool ok=std::fread(magic,8,1,meta)==1 && !std::memcmp(magic,"PMHTUV01",8) && std::fread(&actor.uvScale,4,1,meta)==1 && std::fgetc(meta)==EOF;
        std::fclose(meta);
        if(!ok || !(actor.uvScale==1 || actor.uvScale==2 || actor.uvScale==4 || actor.uvScale==8)){release();return false;}
        FILE* texture=std::fopen((base+".t3x").c_str(),"rb");if(!texture){release();return false;}
        actor.image.imported=Tex3DS_TextureImportStdio(texture,&actor.image.texture,nullptr,false);std::fclose(texture);
        if(!actor.image.imported){release();return false;}
        C3D_TexSetFilter(&actor.image.texture,GPU_LINEAR,GPU_LINEAR);
        C3D_TexSetWrap(&actor.image.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    }
    tutorialReady=true;return true;
}
void Renderer::advanceTutorialAnimations(Game& game) {
    if(!tutorialReady || !game.harborTutorial.running())return;
    if(game.harborTutorial.running()){bindTutorialStartupResources(game.harborTutorial);advanceTutorialCurtain(game.harborTutorial);}
    for(unsigned i=0;i<3;++i) {
        const auto& a=game.harborTutorial.actors[i];const auto* pose=tutorialActors[i].animations.pose(a.pose.c_str());
        if(pose && pose->end>pose->start && game.harborTutorial.now>=a.poseAt && float(game.harborTutorial.now-a.poseAt)*.06f>=pose->end)
            game.harborTutorial.animationComplete(a.id);
    }
}
void Renderer::worldTutorial(const Game& g) {
    if(!loadTutorialPresentation())return;
    // Native initial battle framing. Original mode/moveto camera requests are
    // retained by the source owner; its complete camera driver remains unbound.
    C3D_Mtx projection,view;
    Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(42),C3D_AspectRatioTop,.1f,80,false);
    Mtx_LookAt(&view,FVec3_New(0,2.8f,5.5f),FVec3_New(0,1.f,-.4f),FVec3_New(0,1,0),false);
    Mtx_Multiply(&worldTransform,&projection,&view);
    C3D_BindProgram(&program);
    auto* attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);
    AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,4);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* environment=C3D_GetTexEnv(0);C3D_TexEnvSrc(environment,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(environment,C3D_Both,GPU_MODULATE);
    drawMap(true,g,false,&harborBattleBackground);drawMap(true,g,false,&harborBattleStage);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    count=0;
    for(unsigned i=0;i<3;++i) {
        const auto& a=g.harborTutorial.actors[i];if(a.id<0)continue;
        const auto* pose=tutorialActors[i].animations.pose(a.pose.c_str());
        if(!pose || !pose->evaluate(float(g.harborTutorial.now-a.poseAt)*.06f,actorSample) || actorSample.vertices.size()>size_t(12000-count))continue;
        const int first=count;
        const ActorRootTransform root({},i==0?0:180,false);
        const BattleActorRotation rotation(a.rotation,{});
        for(size_t n=0;n<actorSample.vertices.size() && count<12000;++n) {
            const auto& v=actorSample.vertices[n];const auto p=rotation.point(root.point(v));
            vertex(a.position[0]*.01f+p[0],a.position[1]*.01f+p[1],a.position[2]*.01f+p[2],v.u,1-(1-v.v)*tutorialActors[i].uvScale,0xffffffff,actorSample.colors.empty()?nullptr:&actorSample.colors[n]);
        }
        if(count==first)continue;
        GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        C3D_TexBind(0,&tutorialActors[i].image.texture);C3D_DrawArrays(GPU_TRIANGLES,first,count-first);
    }
    drawMap(true,g,true,&harborBattleBackground);drawMap(true,g,true,&harborBattleStage);
    drawTutorialCurtain();
}
}
