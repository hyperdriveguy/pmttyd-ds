#include "platform.hpp"
#include "battle_pose_inner.hpp"
#include "animation_shape_render.hpp"
#include "map_material_color.hpp"
#include "map_material_tev.hpp"
namespace ttyd {
// Physical packet publication and source callback execution are separate.
// Save icon projection/viewport/scissor only when the corresponding camera
// state is admitted; native matrix rows are copied, not guest pointers cast.
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
        iconViewport=p;return true;
    }
    if(p.kind==NativeCameraCommand::Scissor){
        C3D_SetScissor(GPU_SCISSOR_NORMAL,r.x,r.y,r.x+r.width,r.y+r.height);iconScissor=p;return true;
    }
    C3D_BindProgram(&program);C3D_Mtx transform{};
    for(unsigned i=0;i<4;++i){auto& v=transform.r[i];v.x=p.transform[i][0];v.y=p.transform[i][1];v.z=p.transform[i][2];v.w=p.transform[i][3];}
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
    if(p.kind==NativeCameraCommand::Projection){iconTransform=p;return true;}
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
    // Dummy triangle coordinates(0, 0),(20, 0),(20, 20), constant alpha 129,
    // cutoff 128 and write-mask 0 reproduce source initialization side effects.
    // Only the real draw seeds logical owners; a frame restore must not reseed.
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
// Source compare1/3/6 -> native GREATER/GEQUAL/LEQUAL in reversed depth.
// Blend tokens0/1/2/4/5/6/7 are ZERO/ONE/SRCCLR/SRCALPHA/INVSRCALPHA/
// DSTALPHA/INVDSTALPHA. Color/alpha/depth write enables remain independent.
bool Renderer::finishingDisplayPipeline(const NativeDisplayPipeline& p){
    unsigned mask=(p.color?GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE:0)|(p.alpha?GPU_WRITE_ALPHA:0)|(p.depthWrite?GPU_WRITE_DEPTH:0);
    const auto compare=p.compare==1?GPU_GREATER:p.compare==3?GPU_GEQUAL:p.compare==6?GPU_LEQUAL:GPU_ALWAYS;
    C3D_DepthTest(p.depth,compare,GPU_WRITEMASK(mask));
    // GX location is retained separately. PICA's constant-reference early test
    // is not the GX before-alpha control; admitted class pairings use late test.
    C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);
    C3D_AlphaTest(p.alphaReject,p.alphaReject?GPU_GEQUAL:GPU_ALWAYS,p.alphaReject?128:0);
    auto factor=[](int32_t value){return value==0?GPU_ZERO:value==1?GPU_ONE:value==2?GPU_SRC_COLOR:value==4?GPU_SRC_ALPHA:value==5?GPU_ONE_MINUS_SRC_ALPHA:value==6?GPU_DST_ALPHA:GPU_ONE_MINUS_DST_ALPHA;};
    const auto source=factor(p.sourceBlend),destination=factor(p.destinationBlend);
    if(p.logicOr)C3D_ColorLogicOp(GPU_LOGICOP_OR);
    else C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,source,destination,p.sourceBlend==2?GPU_SRC_ALPHA:source,destination);
    return true;
}
// Restore acknowledged physical state before resuming the exact source
// callback cursor. A false provider result retains the wait, not success.
// Callback tokens below are original US identities (see docs/code/render-adapters.md);
// borrowed values identify source work and never become native pointers.
void Renderer::resumeFinishingCamera(const Game& game){
    if(!game.trioBattleActive || !game.trioAttack.finishingDrawPending())return;
    NativeCameraSubmit submit{this,[](void* user,const NativeCameraPacket& p){return static_cast<Renderer*>(user)->finishingCameraPacket(p);}};
    if(!finishingCameraDraw.restore(submit))return;
    NativeDisplaySubmit pipeline{this,[](void* user,const NativeDisplayPipeline& p){return static_cast<Renderer*>(user)->finishingDisplayPipeline(p);}};
    if(!finishingDisplayState.restore(pipeline))return;
    if(finishingShadowProgram.seeded && !finishingShadowGpu(finishingShadowProgram))return;
    struct CameraBridge {NativeCameraSubmit* submit;const Game* game;} cameraBridge{&submit,&game};
    CameraDrawServices services;services.user=&cameraBridge;
    services.request=[](void* user,const CameraDrawRequest& r){
        const auto& b=*static_cast<CameraBridge*>(user);auto& renderer=*static_cast<Renderer*>(b.submit->user);
        if(!renderer.finishingCameraDraw.request(r,*b.submit))return false;
        if(r.kind==CameraDrawRequestKind::Projection && r.index==4 && r.camera && r.camera->viewPublished){
            const auto& projection=renderer.finishingCameraDraw.projectionMatrix();
            if(!projection || !renderer.iconViewport)return false;
            const auto pass=b.game->trioAttack.sourceIconPass();renderer.retainedIcons.observe(pass);
            if(!renderer.retainedIcons.camera(pass,{*projection,r.camera->matrices.view,renderer.iconViewport->rect,r.camera->aspect}))return false;
        }
        return true;
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
        // Original callbacks: BattleDrawEnemyHP; letterbox; stage dark/light;
        // audience; reel/stockEXP/audience auxiliaries; unit parts; finally shadow.
        if(identity==0x8011fde0)return b.game->trioAttack.resumeEnemyHpDraw(camera,borrowed,{});
        if(identity==0x8000e564){
            CameraLetterboxDrawServices letterbox;letterbox.user=&renderer;
            letterbox.pass=[](void* u,LetterboxPassRequest& r){return static_cast<Renderer*>(u)->finishingLetterboxPass(r);};
            letterbox.gx=[](void* u,const ShadowGXRequest& r){return static_cast<Renderer*>(u)->finishingShadowRequest(r);};
            letterbox.begin=[](void* u){return static_cast<Renderer*>(u)->finishingLetterboxBegin();};
            letterbox.vertex=[](void* u,uint32_t i,const std::array<int16_t,3>& r){return static_cast<Renderer*>(u)->finishingLetterboxVertex(i,r);};
            return b.game->trioAttack.resumeLetterboxDraw(camera,borrowed,letterbox);
        }
        if(identity==0x80138da8 || identity==0x801389a0){
            BattleStageDisplayServices stage;stage.user=&renderer;
            stage.gx=[](void* u,const ShadowGXRequest& r){return static_cast<Renderer*>(u)->finishingShadowRequest(r);};
            stage.primitive=[](void* u,const BattleStagePrimitive& r){return static_cast<Renderer*>(u)->finishingStagePrimitive(r);};
            stage.math=[](void*,BattleStageMath& r){return nativeBattleStageMath(r);};
            return b.game->trioAttack.resumeStageDisplay(camera,identity,borrowed,stage);
        }
        if(identity==0x801a38f8){
            BattleAudienceCallbackServices audience;audience.user=&renderer;
            audience.request=[](void* user,const BattleAudienceGXRequest& r){
                ShadowGXRequest request;request.kind=static_cast<ShadowGX>(r.function);request.values=r.arguments;
                return static_cast<Renderer*>(user)->finishingShadowRequest(request);
            };
            return b.game->trioAttack.resumeAudienceCallback(camera,borrowed,audience);
        }
        if(identity==0x802036d4 || identity==0x80100258 || identity==0x801a37f0 || identity==0x801a36c8){
            BattleAudienceAuxServices auxiliary;auxiliary.common.user=&renderer;bindAudienceAuxMathAdapter(auxiliary);
            auxiliary.common.request=[](void* user,const BattleAudienceGXRequest& r){
                ShadowGXRequest request;request.kind=static_cast<ShadowGX>(r.function);request.values=r.arguments;
                return static_cast<Renderer*>(user)->finishingShadowRequest(request);
            };
            struct IconBridge {Renderer* renderer;const Game* game;} iconBridge{&renderer,b.game};
            SourceIconMatrixServices icon;icon.user=&iconBridge;
            icon.word=[](void* u,uint32_t address)->std::optional<uint32_t>{auto& b=*static_cast<IconBridge*>(u);const auto resource=b.renderer->sourceIcons.word(address);return resource?resource:b.game->trioAttack.sourceIconWord(address);};
            icon.half=[](void* u,uint32_t address)->std::optional<uint16_t>{auto& b=*static_cast<IconBridge*>(u);
                // Synthetic map-work identity 85000000(hex), flags halfword+4;
                // actual source icon constants remain in their own address map.
                if(address==0x85000004){const auto* group=b.game->trioAttack.materialGroup();return group?std::optional<uint16_t>(group->flags):std::nullopt;}
                return b.renderer->sourceIcons.half(address);};
            icon.currentCamera=[](void* u){return static_cast<IconBridge*>(u)->game->trioAttack.sourceIconCamera();};
            icon.matrix=[](void*,BattleAudienceMatrixRequest& r){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,r);};
            SourceIconGXServices gx;gx.user=&iconBridge;gx.word=icon.word;gx.half=icon.half;gx.matrix=icon.matrix;
            gx.call=[](void* u,const SourceIconGXCall& r){auto& b=*static_cast<IconBridge*>(u);return b.renderer->sourceIconCall(*b.game,r);};
            gx.quad=[](void* u,const SourceIconGXQuad& q){auto& b=*static_cast<IconBridge*>(u);return b.renderer->captureSourceIcon(*b.game,q);};
            icon.gx=&gx;auxiliary.sourceIcon=&icon;
            if(identity==0x802036d4){
                BattleBreakSlotDrawServices slot;slot.user=&renderer;slot.sourceIcon=&icon;
                slot.word=[](void* u,uint32_t a){return static_cast<Renderer*>(u)->sourceIcons.word(a);};
                slot.half=[](void* u,uint32_t a){return static_cast<Renderer*>(u)->sourceIcons.half(a);};
                slot.gx=[](void* u,const ShadowGXRequest& r){return static_cast<Renderer*>(u)->finishingShadowRequest(r);};
                slot.scissor=[](void* u,const std::array<uint32_t,4>& r){auto& renderer=*static_cast<Renderer*>(u);
                    NativeCameraSubmit submit{&renderer,[](void* u,const NativeCameraPacket& p){return static_cast<Renderer*>(u)->finishingCameraPacket(p);}};
                    return renderer.finishingCameraDraw.scissor(r,submit,true);};
                return b.game->trioAttack.resumeBreakSlotDraw(camera,borrowed,slot);
            }
            if(identity==0x80100258)return b.game->trioAttack.resumeStockExpDraw(camera,borrowed,icon);
            return b.game->trioAttack.resumeAudienceAux(camera,identity,borrowed,auxiliary);
        }
        if(identity==0x80101774){
            struct InnerBridge {Renderer* renderer;const Game* game;} bridge{&renderer,b.game};
            BattlePoseInnerServices inner;inner.user=&bridge;
            inner.gx=[](void* user,const BattleAudienceGXRequest& r){ShadowGXRequest q;q.kind=static_cast<ShadowGX>(r.function);q.values=r.arguments;return static_cast<InnerBridge*>(user)->renderer->finishingShadowRequest(q);};
            // Selector 0 owns current positions/normals; index 0/1 chooses
            // position/normal arrays. Pose+14(hex) selects the source descriptor.
            // Vector stride12 is three binary32 components; flush real CPU data.
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
                // animSetMaterial_Texture; mapSetMaterialTev below; then
                // mapSetMaterialLastStageBlend/Fog. Each leaf keeps its own
                // provider cursor and validates resources before publication.
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
                // GXLoadNrmMtxImm retains normal matrix; GXLoadPosMtxImm
                // copies3x4 position rows. CurrentMtx/CullMode route
                // through their logical state owners; geometry handles the rest.
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
    retainedIcons.observe(game.trioAttack.sourceIconPass());
}
}
