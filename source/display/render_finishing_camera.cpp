#include "platform.hpp"
#include "battle_pose_inner.hpp"
#include "animation_shape_render.hpp"
#include "map_material_color.hpp"
#include "map_material_tev.hpp"
namespace ttyd {
bool Renderer::finishingCameraPacket(const NativeCameraPacket& p){
    const auto& r=p.rect;
    if(p.kind==NativeCameraCommand::Viewport){
        C3D_SetViewport(r.x,r.y,r.width,r.height);C3D_DepthMap(true,p.depthScale,p.depthOffset);
        // Citro3D viewport disables scissor; GX viewport preserves it. Keep the
        // acknowledged scissor active even when the subsequent source call holds.
        if(const auto& logical=finishingCameraDraw.scissor()){
            const auto scissor=nativeCameraRoundedRect((*logical)[0],(*logical)[1],(*logical)[2],(*logical)[3]);
            if(!scissor)return false;
            C3D_SetScissor(GPU_SCISSOR_NORMAL,scissor->x,scissor->y,scissor->x+scissor->width,scissor->y+scissor->height);
        }
        return true;
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
    auto factor=[](int32_t value){return value==0?GPU_ZERO:value==1?GPU_ONE:value==4?GPU_SRC_ALPHA:value==5?GPU_ONE_MINUS_SRC_ALPHA:value==6?GPU_DST_ALPHA:GPU_ONE_MINUS_DST_ALPHA;};
    const auto source=factor(p.sourceBlend),destination=factor(p.destinationBlend);
    if(p.logicOr)C3D_ColorLogicOp(GPU_LOGICOP_OR);
    else C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,source,destination,source,destination);
    return true;
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
        auto& renderer=*static_cast<Renderer*>(b.pipeline->user);
        if(identity==0x801a38f8){
            BattleAudienceCallbackServices audience;audience.user=&renderer;
            audience.request=[](void* user,const BattleAudienceGXRequest& r){
                ShadowGXRequest request;request.kind=static_cast<ShadowGX>(r.function);request.values=r.arguments;
                return static_cast<Renderer*>(user)->finishingShadowRequest(request);
            };
            return b.game->trioAttack.resumeAudienceCallback(camera,borrowed,audience);
        }
        if(identity==0x801a37f0 || identity==0x801a36c8){
            BattleAudienceAuxServices auxiliary;auxiliary.common.user=&renderer;bindAudienceAuxMathAdapter(auxiliary);
            auxiliary.common.request=[](void* user,const BattleAudienceGXRequest& r){
                ShadowGXRequest request;request.kind=static_cast<ShadowGX>(r.function);request.values=r.arguments;
                return static_cast<Renderer*>(user)->finishingShadowRequest(request);
            };
            struct IconBridge {Renderer* renderer;const Game* game;} iconBridge{&renderer,b.game};
            SourceIconMatrixServices icon;icon.user=&iconBridge;
            icon.word=[](void* u,uint32_t address)->std::optional<uint32_t>{auto& b=*static_cast<IconBridge*>(u);const auto resource=b.renderer->sourceIcons.word(address);return resource?resource:b.game->trioAttack.sourceIconWord(address);};
            icon.half=[](void* u,uint32_t address){return static_cast<IconBridge*>(u)->renderer->sourceIcons.half(address);};
            icon.currentCamera=[](void* u){return static_cast<IconBridge*>(u)->game->trioAttack.sourceIconCamera();};
            icon.matrix=[](void*,BattleAudienceMatrixRequest& r){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,r);};
            SourceIconGXServices gx;gx.user=&iconBridge;gx.word=icon.word;gx.half=icon.half;gx.matrix=icon.matrix;
            gx.call=[](void* u,const SourceIconGXCall& r){auto& b=*static_cast<IconBridge*>(u);return b.renderer->sourceIconCall(*b.game,r);};
            icon.gx=&gx;auxiliary.sourceIcon=&icon;
            return b.game->trioAttack.resumeAudienceAux(camera,identity,borrowed,auxiliary);
        }
        if(identity==0x80101774){
            struct InnerBridge {Renderer* renderer;const Game* game;} bridge{&renderer,b.game};
            BattlePoseInnerServices inner;inner.user=&bridge;
            inner.gx=[](void* user,const BattleAudienceGXRequest& r){ShadowGXRequest q;q.kind=static_cast<ShadowGX>(r.function);q.values=r.arguments;return static_cast<InnerBridge*>(user)->renderer->finishingShadowRequest(q);};
            inner.flush=[](void* user,unsigned index,const BattlePoseInnerContext& context){
                const auto* pool=static_cast<InnerBridge*>(user)->game->trioAttack.battlePosePool();
                if(!pool || pool->outputSelector || index>1)return false;
                const auto* pose=pool->pose(context.handle);if(!pose || !pose->resource || !pose->resource->bank)return false;
                const auto selected=pose->word(0x14);if(selected>=pose->resource->names.size())return false;
                const auto* animation=pose->resource->bank->pose(pose->resource->names[selected].c_str());if(!animation)return false;
                const auto& output=index?pose->shape.normals:pose->shape.positions;
                const auto count=index?animation->normals.size():animation->positions.size();
                if(output.size()!=count)return false;
                static_assert(sizeof(ActorVector)==12,"Source vector flush stride");
                return R_SUCCEEDED(GSPGPU_FlushDataCache(output.data(),uint32_t(12*count)));
            };
            inner.invalidateVertices=[](void*){GPUCMD_AddWrite(GPUREG_VTX_FUNC,1);return true;};
            AnimationShapeServices shape;shape.user=&bridge;
            shape.currentCamera=[](void* user){const auto* cameras=static_cast<InnerBridge*>(user)->game->trioAttack.cameras();return cameras?cameras->current():nullptr;};
            shape.cameraByIndex=[](void* user,uint32_t index){const auto* cameras=static_cast<InnerBridge*>(user)->game->trioAttack.cameras();return cameras?cameras->camera(index):nullptr;};
            shape.concat=[](void*,BattleAudienceMatrixRequest& r){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,r);};
            shape.math=[](void*,AnimationShapeMath& r){return animationShapeNativeMath(r);};
            shape.material=[](void* user,const AnimationShapeCall& r,uint32_t& pending){
                auto& bridge=*static_cast<InnerBridge*>(user);
                if(r.function==0x8003dcfc){
                    if(!r.pose)return false;
                    AnimationTextureServices texture;texture.user=&bridge;
                    texture.get=[](void*,const BattlePose& p,uint32_t index,AnimationTPLImage& out){if(!p.resource || !p.resource->textures)return false;const auto image=p.resource->textures->image(index);if(!image)return false;out=*image;return true;};
                    texture.matrix=[](void*,BattleAudienceMatrixRequest& r){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,r);};
                    texture.gx=[](void* u,const AnimationTextureRequest& q){return static_cast<InnerBridge*>(u)->renderer->finishingAnimationTexture(q);};
                    return bridge.game->trioAttack.resumeTexture(*r.pose,r.draw,texture,pending);
                }
                if(r.function==0x80021b80){
                    if(!r.matrix)return false;
                    MapTevInput input;input.slots=int32_t(r.arguments[0]);input.mode=r.arguments[1];input.flags=r.arguments[2];input.model=*r.matrix;
                    MapTevServices tev;tev.user=bridge.renderer;
                    tev.gx=[](void* u,const ShadowGXRequest& q){return static_cast<Renderer*>(u)->finishingShadowRequest(q);};
                    return bridge.game->trioAttack.resumeMaterialTev(input,tev,pending);
                }
                if(r.function==0x80021860 || r.function==0x800217c8){
                    MapMaterialColorServices material;material.user=&bridge;
                    material.group=[](void* u){return static_cast<InnerBridge*>(u)->game->trioAttack.materialGroup();};
                    material.currentCamera=[](void* u){const auto* cameras=static_cast<InnerBridge*>(u)->game->trioAttack.cameras();return cameras?cameras->current():nullptr;};
                    material.gx=[](void* u,const ShadowGXRequest& q){return static_cast<InnerBridge*>(u)->renderer->finishingShadowRequest(q);};
                    return bridge.game->trioAttack.resumeMaterialColor(r.function,r.arguments[0],r.arguments[1],r.arguments[2],material,pending);
                }
                const auto* work=bridge.game->trioAttack.materialLights();
                if(!work || !work->battleLoaded)return false;
                struct LightBridge {const MapMaterialLightWork* work;Renderer* renderer;} lightBridge{work,bridge.renderer};
                MapMaterialLightServices services;services.user=&lightBridge;
                services.query=[](void* u,MapLightQuery& q,uint32_t& pending){return mapLightBankQuery(*static_cast<LightBridge*>(u)->work,q,pending);};
                services.gx=[](void* u,const ShadowGXRequest& q){return static_cast<LightBridge*>(u)->renderer->finishingShadowRequest(q);};
                return bridge.game->trioAttack.resumeMaterialLight(r.arguments[0],r.origin,services,pending);
            };
            shape.call=[](void* user,const AnimationShapeCall& r){
                auto& renderer=*static_cast<InnerBridge*>(user)->renderer;
                if(r.function==0x802ba34c){if(!r.matrix)return false;renderer.finishingShapeNormal=*r.matrix;return true;}
                if(r.function==0x802ba2fc){if(!r.matrix)return false;ShadowGXRequest q;q.kind=ShadowGX::PositionMatrix;
                    for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)q.matrix[row][col]=(*r.matrix)[4*row+col];
                    return renderer.finishingShadowRequest(q);}
                if(r.function==0x802ba39c || r.function==0x802b5c8c){ShadowGXRequest q;q.kind=static_cast<ShadowGX>(r.function);q.values[0]=int32_t(r.arguments[0]);return renderer.finishingShadowRequest(q);}
                return renderer.finishingAnimationGeometry(r);
            };
            inner.shape=&shape;
            BattlePartDisplayServices part;part.inner=&inner;
            return b.game->trioAttack.resumePartDisplay(camera,borrowed,part);
        }
        return renderer.finishingShadowCallback(*b.game,camera,identity,borrowed);
    };
    // Actual shadow setup; admitted geometry, independent rays and other callbacks retain provider waits.
    game.trioAttack.resumeFinishingDraw(services);
}
}
