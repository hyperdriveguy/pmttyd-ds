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
        C3D_ImmDrawEnd();finishingDisplayState.afterDummy();finishingShadowProgram.afterDummy(); // Logical source state is seeded only by the real dummy, never restoration.
    }
    return true;
}
bool Renderer::finishingDisplayPipeline(const NativeDisplayPipeline& p){
    unsigned mask=(p.color?GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE:0)|(p.alpha?GPU_WRITE_ALPHA:0)|(p.depthWrite?GPU_WRITE_DEPTH:0);
    C3D_DepthTest(p.depth,p.compare==3?GPU_GEQUAL:GPU_ALWAYS,GPU_WRITEMASK(mask));
    // GX location is retained separately. PICA's constant-reference early test
    // is not the GX before-alpha control; admitted class pairings use late test.
    C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);
    C3D_AlphaTest(p.alphaReject,p.alphaReject?GPU_GEQUAL:GPU_ALWAYS,p.alphaReject?128:0);
    auto factor=[](int32_t value){return value==0?GPU_ZERO:value==1?GPU_ONE:value==4?GPU_SRC_ALPHA:GPU_ONE_MINUS_SRC_ALPHA;};
    const auto source=factor(p.sourceBlend),destination=factor(p.destinationBlend);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,source,destination,source,destination);return true;
}
void Renderer::resumeFinishingCamera(const Game& game){
    if(!game.trioBattleActive || !game.trioAttack.finishingDrawPending())return;
    NativeCameraSubmit submit{this,[](void* user,const NativeCameraPacket& p){return static_cast<Renderer*>(user)->finishingCameraPacket(p);}};
    if(!finishingCameraDraw.restore(submit))return;
    NativeDisplaySubmit pipeline{this,[](void* user,const NativeDisplayPipeline& p){return static_cast<Renderer*>(user)->finishingDisplayPipeline(p);}};
    if(!finishingDisplayState.restore(pipeline))return;
    if(finishingShadowProgram.seeded && !finishingShadowGpu(finishingShadowProgram))return;
    CameraDrawServices services;services.user=&submit;
    services.request=[](void* user,const CameraDrawRequest& r){
        const auto& submit=*static_cast<NativeCameraSubmit*>(user);
        return static_cast<Renderer*>(submit.user)->finishingCameraDraw.request(r,submit);
    };
    struct DisplayBridge {NativeDisplaySubmit* pipeline;const Game* game;} displayBridge{&pipeline,&game};
    services.display.user=&displayBridge;
    services.display.state=[](void* user,const DisplayStateRequest& request){
        const auto& pipeline=*static_cast<DisplayBridge*>(user)->pipeline;
        return static_cast<Renderer*>(pipeline.user)->finishingDisplayState.request(request,pipeline);
    };
    services.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){
        const auto& b=*static_cast<DisplayBridge*>(user);
        return static_cast<Renderer*>(b.pipeline->user)->finishingShadowCallback(*b.game,camera,identity,borrowed);
    };
    // Actual shadow setup; admitted geometry, independent rays and other callbacks retain provider waits.
    game.trioAttack.resumeFinishingDraw(services);
}
}
