#pragma once
#include "camera_manager.hpp"
#include "shadow_collision.hpp"
namespace ttyd {
enum class ShadowGX : uint32_t {
    NumChans=0x802b6f3c,ChanCtrl=0x802b6f78,ChanMaterial=0x802b6e54,ChanAmbient=0x802b6d6c,
    NumTexGens=0x802b5084,NumTevStages=0x802b8fcc,TevOrder=0x802b8e30,
    TevColorOp=0x802b89e8,TevAlphaOp=0x802b8a50,TevColorIn=0x802b8960,
    TevAlphaIn=0x802b89a4,Fog=0x802b8ff4,Blend=0x802b932c,
    Depth=0x802b93d8,DepthLocation=0x802b940c,Alpha=0x802b8d60,ColorUpdate=0x802b9380,
    CurrentMatrix=0x802ba39c,Cull=0x802b5c8c,Array=0x802b4db0,
    ClearDesc=0x802b4520,Desc=0x802b3c08,Format=0x802b4558,
    TextureBind=0x802b7880,TexCoordGen=0x802b4e04,PositionMatrix=0x802ba2fc,TevColor=0x802b8ab8,AlphaUpdate=0x802b93ac
};
enum class ShadowTexture {N64Texture44,UnloadedDummy,CharacterAtlas};
struct ShadowGXRequest {
    ShadowGX kind=ShadowGX::NumChans;
    std::array<int32_t,7> values{};
    uint32_t color=0;
    std::array<float,4> floats{};
    ShadowTexture texture=ShadowTexture::N64Texture44;
    CameraViewMatrix matrix{};
};
enum class ShadowGeometry {Polygon,Cylinder,Texture,Projection};
enum class ShadowPassKind : uint32_t {
    InvalidateTextures=0x802b7b54,ReadViewport=0x802ba5b4,ReadScissor=0x802ba6bc,
    Viewport=0x802ba56c,Scissor=0x802ba644,Projection=0x802ba184
};
struct ShadowPassRequest {
    ShadowPassKind kind=ShadowPassKind::InvalidateTextures;
    std::array<float,6> viewport{};
    std::array<uint32_t,4> scissor{};
    CameraProjectionMatrix projection{};
    uint32_t projectionKind=0;
};
struct ShadowCylinderBounds {
    uint32_t function=0;int16_t slot=0;std::array<uint16_t,4> values{};
};
struct ShadowCylinderServices {
    bool enabled=false;void* user=nullptr;
    bool (*list)(void*,uint32_t sourceAddress,uint32_t bytes)=nullptr;
    bool (*bounds)(void*,ShadowCylinderBounds&)=nullptr;
};
struct ShadowCylinderCursor {
    bool active=false,requestPending=false;
    uint32_t camera=0,stage=0,pass=0,step=0,listIndex=0,listCount=0;
    ShadowRecord* record=nullptr;const CameraState* view=nullptr;
    std::array<uint32_t,4> lists{},bytes{};
    ShadowGXRequest request;ShadowCylinderBounds bounds;
};
struct ShadowDisplayServices {
    void* user=nullptr;
    const DisplayRecord* (*currentRecord)(void*)=nullptr;
    const CameraState* (*currentCamera)(void*)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    bool (*texture44)(void*,ShadowTexture&)=nullptr;
    // Provider owns complete source per-record matrix/GX/bounding continuation.
    bool (*geometry)(void*,ShadowGeometry,uint32_t,const CameraState&,ShadowRecord&)=nullptr;
    ShadowCollisionServices collision;
    const CameraState* (*cameraByIndex)(void*,uint32_t)=nullptr;
    // Read requests publish output only on success; all setters perform actual
    // GPU work. Mutable current-camera queries remain distinct source calls.
    bool (*pass)(void*,ShadowPassRequest&)=nullptr;
    CameraState* (*mutableCurrentCamera)(void*)=nullptr;
    std::optional<uint32_t> (*gp14)(void*)=nullptr;
    ShadowCylinderServices cylinder;
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
    ShadowProfile* profile=nullptr;
    bool profileTracking=false,passPending=false;
    uint32_t size=0,gpColor=0;
    uint8_t projectionAlpha=0;
    CameraDepthPoint capturedEye{};
    ShadowPassRequest passRequest;
    ShadowAuxiliaryState* auxiliary=nullptr;
    bool auxiliarySetupStarted=false;
    ShadowCylinderCursor cylinder;
};
// Typed cold-start fog words; later source writers must update these owners.
struct ShadowDisplayGlobals {
    uint32_t polygonFog=0,textureFog=0,projectionFog=0,cylinderTevRGB=0,cylinderZeroMaterial=0;
    CameraViewMatrix savedView{};
    std::array<float,6> savedViewport{};
    std::array<uint32_t,4> savedScissor{};
};
bool shadowCylinder(ShadowWork&,uint32_t camera,const CameraState&,ShadowRecord&,
                    ShadowCylinderCursor&,const ShadowDisplayGlobals&,const ShadowDisplayServices&,uint32_t& pending);
// Camera2 setup and counter-gated End restoration.
bool shadowProjectionPass(ShadowWork&,ShadowDisplayCursor&,ShadowDisplayGlobals&,const ShadowDisplayServices&);
bool shadowDisplay(ShadowWork&,uint32_t camera,uint32_t userdata,
                   ShadowDisplayCursor&,ShadowDisplayGlobals&,const ShadowDisplayServices&);
}
