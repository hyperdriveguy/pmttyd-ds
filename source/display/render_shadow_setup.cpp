#include "platform.hpp"
#include <cstdio>
namespace ttyd {
bool Renderer::prepareShadowTexture44(){
    if(shadowTexture44.imported)return true;
    FILE* file=std::fopen("romfs:/shadow-texture44.t3x","rb");if(!file)return false;
    shadowTexture44.imported=Tex3DS_TextureImportStdio(file,&shadowTexture44.texture,nullptr,false);std::fclose(file);
    if(!shadowTexture44.imported)return false;
    if(shadowTexture44.texture.width!=16 || shadowTexture44.texture.height!=16 || shadowTexture44.texture.fmt!=GPU_RGBA8 || shadowTexture44.texture.maxLevel!=0 || shadowTexture44.texture.minLevel!=0 || shadowTexture44.texture.lodBias!=0){
        Tex3DS_TextureFree(shadowTexture44.imported);C3D_TexDelete(&shadowTexture44.texture);shadowTexture44.imported=nullptr;return false;
    }
    C3D_TexSetFilter(&shadowTexture44.texture,GPU_LINEAR,GPU_LINEAR);
    C3D_TexSetWrap(&shadowTexture44.texture,GPU_REPEAT,GPU_REPEAT);return true;
}
bool Renderer::finishingShadowGpu(const NativeShadowProgram& p){
    if(!p.seeded)return false;
    if(p.texture){
        if(*p.texture==ShadowTexture::CharacterAtlas){
            if(!finishingAtlas)return false;
            auto* texture=finishingAtlas->resource==trioPartnerAnimations?&trioPartnerTexture:finishingAtlas->resource==trioMarioAnimations?&trioMarioTexture:nullptr;
            if(!texture || !texture->imported)return false;
            C3D_TexBind(0,&texture->texture);
        }else{
            if(*p.texture!=ShadowTexture::N64Texture44 || !shadowTexture44.imported)return false;
            C3D_TexBind(0,&shadowTexture44.texture);
        }
    }
    C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    // Source direct/indexed declarations are decoded by the geometry provider
    // into the native world shader layout. No primitive is emitted by setup.
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    // No local audience material/texture write exists. Preserve the physical
    // raster constant when its source owner is unknown; this bounded callback
    // never emits an admitted actor with unknown material or texture.
    const auto inheritedRGB=C3D_GetTexEnv(0)->color&0x00ffffff;
    const auto inheritedAlpha=C3D_GetTexEnv(1)->color&0xff000000;
    for(int i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
    const uint32_t rgba=p.rasterRGB?p.material.value_or(0):0;
    const uint32_t abgr=((rgba>>8)&255)<<16 | ((rgba>>16)&255)<<8 | (rgba>>24);
    auto* env=C3D_GetTexEnv(0);
    C3D_TexEnvColor(env,(p.audienceColor && !p.material?inheritedRGB:abgr) | (uint32_t(p.registerAlpha)<<24));
    if(p.textureRGB){C3D_TexEnvSrc(env,C3D_RGB,p.materialVertex?GPU_PRIMARY_COLOR:GPU_CONSTANT,GPU_TEXTURE0);C3D_TexEnvFunc(env,C3D_RGB,GPU_MODULATE);}
    else if(p.audienceColor){C3D_TexEnvSrc(env,C3D_RGB,GPU_TEXTURE0,GPU_CONSTANT);C3D_TexEnvFunc(env,C3D_RGB,GPU_ADD);}
    else {C3D_TexEnvSrc(env,C3D_RGB,p.rasterRGB && p.materialVertex?GPU_PRIMARY_COLOR:GPU_CONSTANT);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);}
    C3D_TexEnvSrc(env,C3D_Alpha,p.alpha==NativeShadowProgram::TextureRaster?GPU_TEXTURE0:GPU_CONSTANT);
    C3D_TexEnvFunc(env,C3D_Alpha,GPU_REPLACE);
    if(p.alpha!=NativeShadowProgram::Register){
        if(!p.material && !p.audienceColor && !p.alphaMaterialVertex)return false;
        env=C3D_GetTexEnv(1);C3D_TexEnvColor(env,p.material?uint32_t(uint8_t(*p.material))<<24:inheritedAlpha);
        C3D_TexEnvSrc(env,C3D_RGB,GPU_PREVIOUS);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);
        C3D_TexEnvSrc(env,C3D_Alpha,GPU_PREVIOUS,p.alphaMaterialVertex?GPU_PRIMARY_COLOR:GPU_CONSTANT);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
    }
    if(p.cull)C3D_CullFace(*p.cull==0?GPU_CULL_NONE:*p.cull==1?GPU_CULL_BACK_CCW:GPU_CULL_FRONT_CCW);
    if(p.fogOff){C3D_FogGasMode(GPU_NO_FOG,GPU_PLAIN_DENSITY,false);C3D_FogColor(*p.fogColor>>8);}
    return true;
}
bool Renderer::finishingShadowRequest(const ShadowGXRequest& request){
    if(request.kind==ShadowGX::PositionMatrix){
        if(request.values!=std::array<int32_t,7>{})return false;
        NativeCameraSubmit submit{this,[](void* user,const NativeCameraPacket& p){return static_cast<Renderer*>(user)->finishingCameraPacket(p);}};
        return finishingCameraDraw.position(request.matrix,submit);
    }
    NativeDisplaySubmit pipeline{this,[](void* user,const NativeDisplayPipeline& p){return static_cast<Renderer*>(user)->finishingDisplayPipeline(p);}};
    DisplayStateRequest state{};bool display=true;
    switch(request.kind){
    case ShadowGX::Blend:state.kind=DisplayGXState::BlendMode;break;
    case ShadowGX::Depth:state.kind=DisplayGXState::ZMode;break;
    case ShadowGX::DepthLocation:state.kind=DisplayGXState::ZCompareLocation;break;
    case ShadowGX::Alpha:state.kind=DisplayGXState::AlphaCompare;break;
    case ShadowGX::ColorUpdate:state.kind=DisplayGXState::ColorUpdate;break;
    case ShadowGX::AlphaUpdate:state.kind=DisplayGXState::AlphaUpdate;break;
    default:display=false;break;
    }
    if(display){for(unsigned i=0;i<5;++i)state.values[i]=request.values[i];return finishingDisplayState.request(state,pipeline);}
    auto next=finishingShadowProgram.request(request);if(!next)return false;
    if(!finishingShadowGpu(*next))return false;
    finishingShadowProgram=*next;return true;
}
ShadowDisplayServices Renderer::finishingShadowServices(){
    ShadowDisplayServices services;services.user=this;
    services.pass=[](void* user,ShadowPassRequest& r){
        auto& renderer=*static_cast<Renderer*>(user);
        NativeCameraSubmit submit{user,[](void* user,const NativeCameraPacket& p){return static_cast<Renderer*>(user)->finishingCameraPacket(p);}};
        switch(r.kind){
        case ShadowPassKind::InvalidateTextures:
            // Installed Citro3D uses the same isolated cache-clear byte. This
            // queues actual PICA work; CPU texture flush is a separate service.
            GPUCMD_AddMaskedWrite(GPUREG_TEXUNIT_CONFIG,0x4,BIT(16));return true;
        case ShadowPassKind::ReadViewport:
            if(!renderer.finishingCameraDraw.viewport())return false;
            r.viewport=*renderer.finishingCameraDraw.viewport();return true;
        case ShadowPassKind::ReadScissor:
            if(!renderer.finishingCameraDraw.scissor())return false;
            r.scissor=*renderer.finishingCameraDraw.scissor();return true;
        case ShadowPassKind::Viewport:return renderer.finishingCameraDraw.viewport(r.viewport,submit,true);
        case ShadowPassKind::Scissor:return renderer.finishingCameraDraw.scissor(r.scissor,submit,true);
        case ShadowPassKind::Projection:
            return r.projectionKind<=1 && renderer.finishingCameraDraw.projection(r.projection,submit);
        }
        return false;
    };
    services.gx=[](void* user,const ShadowGXRequest& request){return static_cast<Renderer*>(user)->finishingShadowRequest(request);};
    services.texture44=[](void* user,ShadowTexture& texture){
        // Explicit native eager resource adapter: publish loaded only AFTER
        // the exact private texture is imported. Not inferred from record gates.
        if(!static_cast<Renderer*>(user)->prepareShadowTexture44())return false;
        texture=ShadowTexture::N64Texture44;return true;
    };
    services.cylinder.enabled=true;services.cylinder.user=this;
    services.cylinder.list=[](void* user,uint32_t address,uint32_t bytes){
        auto& renderer=*static_cast<Renderer*>(user);const auto& p=renderer.finishingShadowProgram;
        if(!renderer.shadowGeometry.loaded || bytes!=64 || p.arrays[0]!=0x80304ce0 || p.descriptors[0]!=2 || p.descriptors[1]!=2 || p.formats[0]!=4 || p.formats[1]!=5)return false;
        const uint32_t addresses[]{0x80304b60,0x80304ba0,0x80304be0,0x80304c20,0x80304c60};
        unsigned index=5;for(unsigned i=0;i<5;++i)if(addresses[i]==address)index=i;
        if(index==5 || (p.arrays[1]!=0x80415140 && p.arrays[1]!=0x80415160))return false;
        const auto& list=renderer.shadowGeometry.lists[index];const auto& palette=renderer.shadowGeometry.palettes[p.arrays[1]==0x80415160?1:0];
        // Preserve original fan/strip order and all INDEX8 pairs. Prepared
        // position matrix already includes view/radius/height/collision lift.
        C3D_ImmDrawBegin(list.primitive==0xa0?GPU_TRIANGLE_FAN:GPU_TRIANGLE_STRIP);
        for(const auto& pair:list.pairs){const auto& v=renderer.shadowGeometry.positions[pair[0]];const auto& c=palette[pair[1]];
            C3D_ImmSendAttrib(v.x,v.y,v.z,1);C3D_ImmSendAttrib(0,0,0,1);
            C3D_ImmSendAttrib(float(c[0])/255,float(c[1])/255,float(c[2])/255,float(c[3])/255);
        }
        C3D_ImmDrawEnd();return true;
    };
    // Runtime owns independent rays; cube resources/camera1 bounds, polygon/
    // texture geometry and nonzero camera2 image suffix remain unavailable.
    return services;
}
bool Renderer::finishingShadowCallback(const Game& game,uint32_t camera,uint32_t identity,uint32_t userdata){
    if(identity!=0x8002d3d8)return false;
    return game.trioAttack.resumeShadowDisplay(camera,userdata,finishingShadowServices());
}
}
