#include "platform.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
// PMSHP001 contains120 triangles/360 bindings; capture IDs 1/2, pass 2, cull 1
// match original ship paper. A_1 is nonlooping 0..60, not simulation duration.
bool Renderer::loadShipPaper(){
 if(shipPaperAttempted)return shipPaperReady;
 shipPaperAttempted=true;
 FILE* f=std::fopen("romfs:/opening-ship-paper.bin","rb");if(!f)return false;char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMSHP001",8) && std::fread(&count,4,1,f)==1 && count==120;
 if(ok)shipPaperSurfaces.resize(count);
 if(ok)ok=std::fread(shipPaperSurfaces.data(),sizeof(DemoPaperSurface),count,f)==count && std::fgetc(f)==EOF;
 std::fclose(f);for(const auto& s:shipPaperSurfaces)ok=ok && (s.capture==1 || s.capture==2) && s.pass==2 && s.cull==1;
 if(!ok || !shipPaperAnimations.load("romfs:/opening-ship-paper.pmanim"))return false;
 const auto* pose=shipPaperAnimations.pose("A_1");shipPaperReady=pose && pose->bindings.size()==360 && pose->start==0 && pose->end==60 && !pose->loop;return shipPaperReady;
}
// Called after submitted-frame synchronization. Display transfer writes
// linear 240x400 RGBA8; invalidate CPU cache before reading GPU output.
bool Renderer::captureOpeningShip(const OpeningSequence& opening){
 if(!shipCaptureTarget || !opening.harbor.paperCapturePending)return false;
 if(!shipCapturePixels)shipCapturePixels=static_cast<uint8_t*>(linearAlloc(240*400*4));
 if(!shipCapturePixels)return false;
 if(!shipCaptureReady && !C3D_TexInit(&shipCaptureTexture,512,256,GPU_RGBA8))return false;
 shipCaptureReady=true;
 constexpr uint32_t flags=GX_TRANSFER_FLIP_VERT(0)|GX_TRANSFER_OUT_TILED(0)|GX_TRANSFER_RAW_COPY(0)|GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)|GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8)|GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);
 C3D_SyncDisplayTransfer(static_cast<uint32_t*>(shipCaptureTarget->frameBuf.colorBuf),GX_BUFFER_DIM(240,400),reinterpret_cast<uint32_t*>(shipCapturePixels),GX_BUFFER_DIM(240,400),flags);GSPGPU_InvalidateDataCache(shipCapturePixels,240*400*4);
 auto* target=static_cast<uint8_t*>(shipCaptureTexture.data);
 // Transpose tilted framebuffer src=x*240+y into 512x256 texture backing.
 // PICA uses 8x8 Morton tiles: interleave3 low bits, 64 tiles per row, flipY.
 // Clamp padding to last valid399/239 pixel to avoid filtered edge garbage.
 for(unsigned y=0;y<256;++y)for(unsigned x=0;x<512;++x){const size_t dst=(((255-y)/8)*64+x/8)*64+([&](){unsigned m=0;for(unsigned b=0;b<3;++b){m|=((x>>b)&1)<<(2*b);m|=(((255-y)>>b)&1)<<(2*b+1);}return m;})();const size_t src=(std::min(x,399u)*240+std::min(y,239u))*4;std::memcpy(target+dst*4,shipCapturePixels+src,4);}
 C3D_TexSetFilter(&shipCaptureTexture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&shipCaptureTexture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);C3D_TexFlush(&shipCaptureTexture);opening.harbor.captureComplete();return true;
}
// Source 64x48 paper fills native 400x240. Undo1024 atlas cells(128 stride,
// 2 px border, 124 px content), then crop400/512 and 240/256 capture UVs.
// Return sampled frame to outer owner; completion requires post-frame sync.
bool Renderer::frontShipPaper(const OpeningSequence& opening,float& presented){
 if(!opening.harbor.paperLive)return true;
 if(!loadShipPaper() || !shipCaptureReady)return false;
 const auto* pose=shipPaperAnimations.pose("A_1");const float frame=opening.harbor.paperFrame();if(!pose || !pose->evaluate(frame,shipPaperSample) || shipPaperSample.vertices.size()!=360)return false;
 C2D_Flush();C3D_BindProgram(&program);auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1000,1000,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
 C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_GREATER,0);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
 for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
 auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
 const int first=count;
 for(unsigned i=0;i<360;++i){const auto& v=shipPaperSample.vertices[i];const float cell=shipPaperSurfaces[i/3].capture==1?1:0;const float u=(1024*v.u-cell*128-2)/124,vv=((1-v.v)*1024-2)/124;vertex((v.x+32)*400/64,(24-v.y)*240/48,-v.z*400/64,u*400/512,1-vv*240/256,0xffffffff);}
 GSPGPU_FlushDataCache(vertices+first,360*sizeof(Vertex));auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);C3D_CullFace(GPU_CULL_BACK_CCW);C3D_TexBind(0,&shipCaptureTexture);C3D_DrawArrays(GPU_TRIANGLES,first,360);presented=frame;C3D_CullFace(GPU_CULL_NONE);C2D_Prepare();C2D_SceneBegin(top);return true;
}
void Renderer::freeShipCapture(){if(shipCaptureReady){C3D_TexDelete(&shipCaptureTexture);shipCaptureReady=false;}if(shipCaptureTarget){C3D_RenderTargetDelete(shipCaptureTarget);shipCaptureTarget=nullptr;}if(shipCapturePixels){linearFree(shipCapturePixels);shipCapturePixels=nullptr;}}
}
