#include "platform.hpp"
namespace ttyd {
// Complete CPU-decoded fans are the submission boundary: a provider hold must
// never leave an immediate GPU Begin open. Texture identity includes resource,
// atlas cell, UV scale, generator and matrix, not just the currently bound image.
bool Renderer::finishingAnimationGeometry(const AnimationShapeCall& request){
    AnimationFanTexture texture;
    if(finishingAtlas){texture.resource=finishingAtlas->resource;texture.cell=finishingAtlas->cell;
        if(!finishingAnimationTextureOwner(texture.resource,texture.uvScale))return false;
        texture.generator=finishingTextureGenerator;texture.matrix=finishingTextureMatrix;}
    AnimationFanSubmit submit{this,[](void* user,const AnimationFanBatch& fan){
        auto& renderer=*static_cast<Renderer*>(user);const auto& program=renderer.finishingShadowProgram;
        // ChannelControl[0/1] own color/alpha separately; argument1 is the
        // source lighting-enable gate. This unlit shader rejects lit channels.
        if(!program.seeded || program.chans!=1 || !program.channelControl[0] || !program.channelControl[1] ||
           (*program.channelControl[0])[1] || (*program.channelControl[1])[1])return false; // Normal shader required for actual lit channels.
        if(fan.slots){
            if(!renderer.finishingAtlas || renderer.finishingAtlas->resource!=fan.resource || renderer.finishingAtlas->cell!=fan.texture.cell || program.texture!=ShadowTexture::CharacterAtlas)return false;
            AnimationFanTexture current;current.resource=renderer.finishingAtlas->resource;current.cell=renderer.finishingAtlas->cell;
            if(!renderer.finishingAnimationTextureOwner(current.resource,current.uvScale))return false;
            current.generator=renderer.finishingTextureGenerator;current.matrix=renderer.finishingTextureMatrix;
            if(!(current==fan.texture))return false;
        }
        // Restore acknowledged GPU state, including bound texture and current
        // raster/K0 combiner, before the complete original fan. No immediate
        // Begin survives a provider hold or an outer frame boundary.
        if(!renderer.finishingShadowGpu(program))return false;
        // Native stream is XYZ/UV/normalized RGBA; source byte colors divide
        // by 255 exactly once. Logical GX state was already acknowledged.
        C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
        for(uint32_t i=0;i<fan.count;++i){const auto& v=fan.vertices[i];
            C3D_ImmSendAttrib(v.position[0],v.position[1],v.position[2],1);
            C3D_ImmSendAttrib(v.uv[0],v.uv[1],0,1);
            C3D_ImmSendAttrib(v.color[0]/255.f,v.color[1]/255.f,v.color[2]/255.f,v.color[3]/255.f);
        }
        C3D_ImmDrawEnd();return true;
    }};
    return finishingAnimationFans.request(request,texture,submit);
}
}
