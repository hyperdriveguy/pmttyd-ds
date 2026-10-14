#include "platform.hpp"
namespace ttyd {
bool Renderer::finishingCameraPacket(const NativeCameraPacket& p){
    const auto& r=p.rect;
    if(p.kind==NativeCameraCommand::Viewport){
        C3D_SetViewport(r.x,r.y,r.width,r.height);C3D_DepthMap(true,p.depthScale,p.depthOffset);return true;
    }
    if(p.kind==NativeCameraCommand::Scissor){
        C3D_SetScissor(GPU_SCISSOR_NORMAL,r.x,r.y,r.x+r.width,r.y+r.height);return true;
    }
    C3D_BindProgram(&program);C3D_Mtx transform{};
    for(unsigned i=0;i<4;++i){auto& v=transform.r[i];v.x=p.transform[i][0];v.y=p.transform[i][1];v.z=p.transform[i][2];v.w=p.transform[i][3];}
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
    if(p.kind==NativeCameraCommand::Projection)return true;
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    // One constant TEV stage; later PICA stages pass through. GX channels0,
    // unused texture order and register1 RGBA00000081 become this constant.
    for(int i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_CONSTANT);
    C3D_TexEnvFunc(env,C3D_Both,GPU_REPLACE);C3D_TexEnvColor(env,0x81000000);
    C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITEMASK(0));
    C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ZERO,GPU_ONE,GPU_ZERO,GPU_ONE);
    C3D_AlphaTest(true,GPU_GEQUAL,128);
    // Source does not change cull/fog. Frame restoration changes only state;
    // this branch emits the actual original XYZ/UV triangle exactly once.
    if(p.kind==NativeCameraCommand::DummyDraw){
        C3D_ImmDrawBegin(GPU_TRIANGLES);
        for(const auto& xyz:std::array<std::array<float,3>,3>{{{0,0,0},{20,0,0},{20,20,0}}}){
            C3D_ImmSendAttrib(xyz[0],xyz[1],xyz[2],1);
            C3D_ImmSendAttrib(0,0,0,1);C3D_ImmSendAttrib(0,0,0,0);
        }
        C3D_ImmDrawEnd();
    }
    return true;
}
void Renderer::resumeFinishingCamera(const Game& game){
    if(!game.trioBattleActive || !game.trioAttack.finishingDrawPending())return;
    NativeCameraSubmit submit{this,[](void* user,const NativeCameraPacket& p){return static_cast<Renderer*>(user)->finishingCameraPacket(p);}};
    if(!finishingCameraDraw.restore(submit))return;
    CameraDrawServices services;services.user=&submit;
    services.request=[](void* user,const CameraDrawRequest& r){
        const auto& submit=*static_cast<NativeCameraSubmit*>(user);
        return static_cast<Renderer*>(submit.user)->finishingCameraDraw.request(r,submit);
    };
    game.trioAttack.resumeFinishingDraw(services);
}
}
