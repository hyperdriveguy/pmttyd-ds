#include "platform.hpp"
#include <cmath>
namespace ttyd {
bool Renderer::finishingAnimationTexture(const AnimationTextureRequest& r){
    if(r.coordinate!=0)return false; // Multitexture shader/array ownership remains separate.
    switch(r.function){
    case 0x802b72b0:
        if(!r.pose || !r.pose->resource || r.image.paletteHeader || r.image.format!=14 || r.wrapS || r.wrapT || r.image.minLOD || r.image.maxLOD)return false;
        finishingTextureObject=r;finishingTextureObjectReady=true;finishingTextureLODReady=false;return true;
    case 0x802b7544:
        if(!finishingTextureObjectReady || r.image.index!=finishingTextureObject.image.index || r.image.minFilter!=1 || r.image.magFilter!=1 || r.image.bias!=0 || r.image.edge || r.image.minLOD || r.image.maxLOD)return false;
        finishingTextureLODReady=true;return true;
    case 0x802b7880:{
        if(!finishingTextureObjectReady || !finishingTextureLODReady || !r.pose || !r.pose->resource || r.image.index!=finishingTextureObject.image.index)return false;
        const auto cell=animationTextureCell(r);if(!cell)return false;
        auto* texture=r.pose->resource==trioPartnerAnimations?&trioPartnerTexture:r.pose->resource==trioMarioAnimations?&trioMarioTexture:nullptr;
        const float uvScale=r.pose->resource==trioPartnerAnimations?trioPartnerUVScale:trioMarioUVScale;
        if((uvScale!=1 && uvScale!=2) || !texture || !texture->imported || texture->texture.fmt!=GPU_RGBA8 || texture->texture.width!=512 || texture->texture.height!=unsigned(512/uvScale))return false;
        const auto previous=finishingAtlas;finishingAtlas=FinishingAtlas{r.pose->resource,*cell};
        C3D_TexSetFilter(&texture->texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&texture->texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
        ShadowGXRequest request;request.kind=ShadowGX::TextureBind;request.texture=ShadowTexture::CharacterAtlas;
        if(!finishingShadowRequest(request)){finishingAtlas=previous;return false;}return true;}
    case 0x802b4e04:
        if(r.arguments!=std::array<int32_t,7>{0,1,4,30,0,125,0} && r.arguments!=std::array<int32_t,7>{0,1,4,60,0,125,0})return false;
        finishingTextureGenerator=uint32_t(r.arguments[3]);return true;
    case 0x802ba3d0:
        if(r.arguments!=std::array<int32_t,7>{30,1,0,0,0,0,0})return false;
        for(float v:r.matrix)if(!std::isfinite(v))return false;
        finishingTextureMatrix=r.matrix;return true;
    default:return false; // CI/TLUT and other GPU domains need actual providers.
    }
}
}
