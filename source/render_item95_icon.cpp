#include "platform.hpp"
#include "message_icons.hpp"
namespace ttyd {
void Renderer::releaseItem95Icons() {
 if(item95IconVertices){C3D_FrameSync();linearFree(item95IconVertices);item95IconVertices=nullptr;item95IconCount=0;}
}
bool Renderer::item95Icon(const FieldItemIcon& icon,uint32_t cameraID,const C3D_Mtx& nativeWorldToClip,
                         const CameraViewport& viewport,float yaw,uint32_t counter) {
 if(!icon.visible(messageIconTexture.imported!=nullptr))return true;
 if(cameraID!=5 || cameraID!=icon.camera() || item95IconCount>=FieldItemRegistry::Capacity)return false;
 const auto* original=messageIcon(icon.id);if(!original || original->frames.empty())return false;
 const auto& frame=original->frame(counter);FieldItemIconDraw draw;
 if(!item95IconDraw(icon,yaw,frame.width,frame.height,draw))return false;
 if(!item95IconVertices)item95IconVertices=(Vertex*)linearAlloc(sizeof(Vertex)*6*FieldItemRegistry::Capacity);
 if(!item95IconVertices)return false;
 const unsigned start=item95IconCount++*6;constexpr unsigned order[]={0,1,2,0,2,3};
 for(unsigned n=0;n<6;++n) {
  const unsigned q=order[n];const auto& p=draw.vertices[q];const auto& uv=draw.uv[q];const uint32_t c=draw.color;
  item95IconVertices[start+n]={p[0],p[1],p[2],frame.u0+(frame.u1-frame.u0)*uv[0],frame.v0+(frame.v1-frame.v0)*uv[1],
      (c>>24)/255.f,((c>>16)&255)/255.f,((c>>8)&255)/255.f,(c&255)/255.f};
 }
 C2D_Flush();GSPGPU_FlushDataCache(item95IconVertices+start,sizeof(Vertex)*6);
 C3D_Mtx world=nativeWorldToClip,model,transform;Mtx_Scale(&world,.01f,.01f,.01f);
 for(unsigned r=0;r<4;++r)model.r[r]=FVec4_New(draw.model.v[r*4],draw.model.v[r*4+1],draw.model.v[r*4+2],draw.model.v[r*4+3]);
 Mtx_Multiply(&transform,&world,&model);C3D_BindProgram(&program);
 auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,item95IconVertices,sizeof(Vertex),3,0x210);
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
 C3D_DepthMap(true,viewport.nearDepth-viewport.farDepth,1-viewport.farDepth);C3D_CullFace(GPU_CULL_NONE);
 const auto rgb=GPU_WRITEMASK(GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE);
 C3D_DepthTest(true,GPU_GEQUAL,draw.depthWrite?GPU_WRITEMASK(rgb|GPU_WRITE_DEPTH):rgb);
 C3D_AlphaTest(true,draw.blend?GPU_ALWAYS:GPU_GEQUAL,draw.blend?0:128);
 if(draw.blend)C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
 else C3D_ColorLogicOp(GPU_LOGICOP_COPY);
 for(unsigned n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
 auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
 C3D_TexBind(0,&messageIconTexture.texture);C3D_TexBind(1,nullptr);C3D_TexBind(2,nullptr);C3D_DrawArrays(GPU_TRIANGLES,start,6);
 C3D_DepthMap(true,-1,0);C2D_Prepare();C2D_SceneBegin(top);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
 C3D_AlphaTest(true,GPU_GREATER,8);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
 return true;
}
}
