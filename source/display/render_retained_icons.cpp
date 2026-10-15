#include "platform.hpp"
#include "battle_scene_views.hpp"
namespace ttyd {
// Capture a complete logical packet, including camera/pipeline/texture and
// viewport/scissor/depth. Append may hold; do not publish a partial quad.
bool Renderer::captureSourceIcon(const Game& game,const SourceIconGXQuad& quad){
    const auto pass=game.trioAttack.sourceIconPass();retainedIcons.observe(pass);
    const auto* cameras=game.trioAttack.cameras();const auto camera=cameras?cameras->currentIndex():std::nullopt;
    const auto* pipeline=finishingDisplayState.state();
    if(!camera || !pipeline || !sourceIconPlan || !boundSourceIcon || !sourceIconTexture() ||
       !iconTransform || !iconViewport || !iconScissor || finishingShadowProgram.texture!=ShadowTexture::SourceIcon)return false;
    RetainedIconPacket packet;packet.quad=quad;packet.material=*sourceIconPlan;packet.pipeline=*pipeline;
    packet.transform=iconTransform->transform;packet.viewport=iconViewport->rect;packet.scissor=iconScissor->rect;
    packet.depthScale=iconViewport->depthScale;packet.depthOffset=iconViewport->depthOffset;
    packet.image=*boundSourceIcon;packet.camera=*camera;
    return retainedIcons.append(pass,packet);
}
// Camera 4 icons support lower-view continuation; other camera domains stay
// on the top target. Both retained passes pin texture residency until retired.
void Renderer::drawSourceIcons(uint32_t camera,bool lower){
    if(lower && camera!=4)return;
    const auto& scene=retainedIcons.sceneCamera();
    if(camera==4 && !scene)return;
    // Replay only GPU state. Never rewrite logical GX owners, source cursors,
    // texture bindings, clocks or random state while presenting either screen.
    for(const auto& packet:retainedIcons.published()){
        if(packet.camera!=camera)continue;
        C3D_Tex* texture=nullptr;
        for(auto& cell:sourceIconCells)if(cell.index==packet.image && cell.gpu.imported){texture=&cell.gpu.texture;break;}
        const auto image=sourceIcons.image(packet.image);
        if(!texture || !image)continue; // Residency is pinned by both retained passes.
        auto matrix=packet.transform;
        if(camera==4)matrix=iconSceneTransform(matrix,*scene);
        if(lower){
            matrix=lowerBattleSceneMatrix(iconViewportTransform(matrix,packet.viewport));
            C3D_SetViewport(0,0,240,320);C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,320);
        }else{
            const auto& v=packet.viewport;const auto& s=packet.scissor;
            C3D_SetViewport(v.x,v.y,v.width,v.height);
            C3D_SetScissor(GPU_SCISSOR_NORMAL,s.x,s.y,s.x+s.width,s.y+s.height);
        }
        C3D_DepthMap(true,packet.depthScale,packet.depthOffset);
        publishSourceIconProgram(packet.material,*texture);finishingDisplayPipeline(packet.pipeline);
        C3D_Mtx transform{};
        for(unsigned i=0;i<4;++i){auto& v=transform.r[i];v.x=matrix[i][0];v.y=matrix[i][1];v.z=matrix[i][2];v.w=matrix[i][3];}
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
        // Four-vertex source quad becomes a native fan. Each unscaled source
        // image has2px gutter inside padded backing; divide by actual GPU
        // dimensions and flip V. White vertex color leaves the plan in control.
        C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
        for(unsigned i=0;i<4;++i){
            const auto& p=packet.quad.positions[i];const auto& uv=packet.quad.uv[i];
            C3D_ImmSendAttrib(p[0],p[1],p[2],1);
            C3D_ImmSendAttrib((2.f+uv[0]*image->width)/texture->width,1.f-(2.f+uv[1]*image->height)/texture->height,0,1);
            C3D_ImmSendAttrib(1,1,1,1);
        }
        C3D_ImmDrawEnd();
    }
    // Subsequent C2D/HUD/fade setup assumes the full target raster domain.
    C3D_SetViewport(0,0,240,lower?320:400);C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,lower?320:400);
    C3D_DepthMap(true,-1,0);
}
} // namespace ttyd
