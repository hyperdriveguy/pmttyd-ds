#pragma once
#include "camera_manager.hpp"
#include "shadow_collision.hpp"
namespace ttyd {
enum class ShadowGX : uint32_t {
    NumChans=0x802b6f3c,ChanCtrl=0x802b6f78,ChanMaterial=0x802b6e54,
    NumTexGens=0x802b5084,NumTevStages=0x802b8fcc,TevOrder=0x802b8e30,
    TevColorOp=0x802b89e8,TevAlphaOp=0x802b8a50,TevColorIn=0x802b8960,
    TevAlphaIn=0x802b89a4,Fog=0x802b8ff4,Blend=0x802b932c,
    Depth=0x802b93d8,DepthLocation=0x802b940c,Alpha=0x802b8d60,
    CurrentMatrix=0x802ba39c,Cull=0x802b5c8c,Array=0x802b4db0,
    ClearDesc=0x802b4520,Desc=0x802b3c08,Format=0x802b4558,
    TextureBind=0x802b7880,TexCoordGen=0x802b4e04
};
enum class ShadowTexture {N64Texture44,UnloadedDummy};
struct ShadowGXRequest {
    ShadowGX kind=ShadowGX::NumChans;
    std::array<int32_t,7> values{};
    uint32_t color=0;
    std::array<float,4> floats{};
    ShadowTexture texture=ShadowTexture::N64Texture44;
};
enum class ShadowGeometry {Polygon,Cylinder,Texture};
struct ShadowDisplayServices {
    void* user=nullptr;
    const DisplayRecord* (*currentRecord)(void*)=nullptr;
    const CameraState* (*currentCamera)(void*)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    bool (*texture44)(void*,ShadowTexture&)=nullptr;
    // Provider owns complete source per-record matrix/GX/bounding continuation.
    bool (*geometry)(void*,ShadowGeometry,uint32_t,const CameraState&,ShadowRecord&)=nullptr;
    ShadowCollisionServices collision;
};
struct ShadowDisplayCursor {
    bool active=false,requestPending=false,geometryPending=false;
    uint32_t camera=0,userdata=0,phase=0,step=0,index=0,pending=0;
    uint8_t drawClass=0;
    ShadowWork* owner=nullptr;
    ShadowRecord* pool=nullptr;
    const CameraState* view=nullptr;
    ShadowGXRequest request;
    ShadowTexture texture=ShadowTexture::N64Texture44;
    ShadowCollisionCursor collision;
};
// Typed cold-start fog words; later source writers must update these owners.
struct ShadowDisplayGlobals {uint32_t polygonFog=0,textureFog=0;};
bool shadowDisplay(ShadowWork&,uint32_t camera,uint32_t userdata,
                   ShadowDisplayCursor&,const ShadowDisplayGlobals&,const ShadowDisplayServices&);
}
