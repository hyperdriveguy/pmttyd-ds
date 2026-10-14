#include "platform.hpp"
#include "item_get_effect_shbin.h"
namespace ttyd {
bool Renderer::prepareItemGetEffect() {
 if(itemGetEffectProgramReady && itemGetEffectVertices && itemGetEffectTextures[0].imported && itemGetEffectTextures[1].imported)return true;
 C3D_FrameSync();
 if(!itemGetEffectShader)itemGetEffectShader=DVLB_ParseFile((u32*)item_get_effect_shbin,item_get_effect_shbin_size);
 if(!itemGetEffectShader)return false;
 if(!itemGetEffectProgramReady) {
  shaderProgramInit(&itemGetEffectProgram);itemGetEffectProgramReady=true;
  shaderProgramSetVsh(&itemGetEffectProgram,&itemGetEffectShader->DVLE[0]);
  itemGetEffectTransform=shaderInstanceGetUniformLocation(itemGetEffectProgram.vertexShader,"transform");
  itemGetEffectProjection=shaderInstanceGetUniformLocation(itemGetEffectProgram.vertexShader,"textureProjection");
 }
 if(!itemGetEffectVertices)itemGetEffectVertices=(Vertex*)linearAlloc(sizeof(Vertex)*12);
 if(!itemGetEffectVertices){releaseItemGetEffect();return false;}
 const char* paths[]={"romfs:/field-item-get-effect-44.t3x","romfs:/field-item-get-effect-42.t3x"};
 for(unsigned n=0;n<2;++n)if(!itemGetEffectTextures[n].imported) {
  FILE* file=std::fopen(paths[n],"rb");if(!file){releaseItemGetEffect();return false;}
  auto& t=itemGetEffectTextures[n];t.imported=Tex3DS_TextureImportStdio(file,&t.texture,nullptr,false);std::fclose(file);
  if(!t.imported){releaseItemGetEffect();return false;}
  const auto* sub=Tex3DS_GetSubTexture(t.imported,0);
  const unsigned expected=n?128:16;
  // Repeat/mirror must cover the original full image, not padded atlas space.
  if(Tex3DS_GetNumSubTextures(t.imported)!=1 || sub->width!=expected || sub->height!=expected ||
     t.texture.width!=expected || t.texture.height!=expected || Tex3DS_SubTextureRotated(sub)) {
   releaseItemGetEffect();return false;
  }

  C3D_TexSetFilter(&t.texture,GPU_LINEAR,GPU_LINEAR);
  C3D_TexSetWrap(&t.texture,n?GPU_MIRRORED_REPEAT:GPU_REPEAT,n?GPU_MIRRORED_REPEAT:GPU_REPEAT);
 }
 return true;
}
void Renderer::releaseItemGetEffect() {
 if(!itemGetEffectShader && !itemGetEffectVertices && !itemGetEffectTextures[0].imported && !itemGetEffectTextures[1].imported)return;
 C3D_FrameSync();
 for(auto& t:itemGetEffectTextures)if(t.imported){Tex3DS_TextureFree(t.imported);C3D_TexDelete(&t.texture);t={};}
 if(itemGetEffectVertices){linearFree(itemGetEffectVertices);itemGetEffectVertices=nullptr;}
 if(itemGetEffectProgramReady){shaderProgramFree(&itemGetEffectProgram);itemGetEffectProgramReady=false;}
 if(itemGetEffectShader){DVLB_Free(itemGetEffectShader);itemGetEffectShader=nullptr;}
}
bool Renderer::itemGetEffect(const FieldItemGetEffect6& effect,uint32_t cameraID,const C3D_Mtx& nativeWorldToClip,const CameraViewport& sourceViewport,float yaw,uint32_t shadow) {
 if(!effect.active()){releaseItemGetEffect();return true;}
 if(cameraID!=effect.camera() || !prepareItemGetEffect())return false;
 const auto draw=effect.draw(yaw,shadow);
 const auto* pattern=Tex3DS_GetSubTexture(itemGetEffectTextures[0].imported,0);
 const auto* opacity=Tex3DS_GetSubTexture(itemGetEffectTextures[1].imported,0);
 constexpr unsigned order[]={0,1,2,0,2,3};
 for(unsigned pass=0;pass<2;++pass)for(unsigned n=0;n<6;++n) {
  const unsigned q=order[n];const auto& p=draw.vertices[q];const auto& uv=draw.uv[q];const uint32_t c=draw.passes[pass].color;
  itemGetEffectVertices[pass*6+n]={p[0],p[1],p[2],opacity->left+(opacity->right-opacity->left)*uv[0],
      opacity->top+(opacity->bottom-opacity->top)*uv[1],(c>>24)/255.f,((c>>16)&255)/255.f,((c>>8)&255)/255.f,(c&255)/255.f};
 }
 C2D_Flush();GSPGPU_FlushDataCache(itemGetEffectVertices,sizeof(Vertex)*12);
 C3D_BindProgram(&itemGetEffectProgram);
 auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,itemGetEffectVertices,sizeof(Vertex),3,0x210);
 // Texture44 POS projection is independent of camera view and shadow offset.
 for(unsigned row=0;row<2;++row) {
  const float span=row?pattern->bottom-pattern->top:pattern->right-pattern->left;
  const float offset=row?pattern->top:pattern->left;const auto& m=draw.textureProjection.v;
  C3D_FVUnifSet(GPU_VERTEX_SHADER,itemGetEffectProjection+row,span*m[row*4],span*m[row*4+1],span*m[row*4+2],span*m[row*4+3]+offset);
 }
 C3D_DepthMap(true,sourceViewport.nearDepth-sourceViewport.farDepth,1-sourceViewport.farDepth);
 C3D_CullFace(GPU_CULL_NONE);C3D_AlphaTest(true,GPU_ALWAYS,0);
 // Citro3D perspective reverses depth relative to GX's LEQUAL domain.
 C3D_DepthTest(true,GPU_GEQUAL,static_cast<GPU_WRITEMASK>(GPU_WRITE_COLOR & ~GPU_WRITE_ALPHA));
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
 C3D_TexBind(0,&itemGetEffectTextures[0].texture);C3D_TexBind(1,&itemGetEffectTextures[1].texture);C3D_TexBind(2,nullptr);
 for(unsigned n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
 const auto abgr=[](uint32_t c){return C2D_Color32(c>>24,(c>>16)&255,(c>>8)&255,c&255);};
 // Two GX stages become three PICA stages because each PICA stage owns one
 // constant: gray -> lerp(gray,orange,tex44.rgb) -> raster modulation.
 auto* env=C3D_GetTexEnv(0);C3D_TexEnvColor(env,abgr(draw.colors[0]));C3D_TexEnvSrc(env,C3D_RGB,GPU_CONSTANT);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);
 C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE1);C3D_TexEnvFunc(env,C3D_Alpha,GPU_REPLACE);
 env=C3D_GetTexEnv(1);C3D_TexEnvColor(env,abgr(draw.colors[1]));C3D_TexEnvSrc(env,C3D_RGB,GPU_CONSTANT,GPU_PREVIOUS,GPU_TEXTURE0);C3D_TexEnvFunc(env,C3D_RGB,GPU_INTERPOLATE);
 env=C3D_GetTexEnv(2);C3D_TexEnvSrc(env,C3D_Both,GPU_PREVIOUS,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
 C3D_Mtx world=nativeWorldToClip;Mtx_Scale(&world,.01f,.01f,.01f); // One original-unit conversion.
 for(unsigned pass=0;pass<2;++pass) {
  C3D_Mtx model,transform;const auto& m=draw.passes[pass].model.v;
  for(unsigned r=0;r<4;++r)model.r[r]=FVec4_New(m[r*4],m[r*4+1],m[r*4+2],m[r*4+3]);
  Mtx_Multiply(&transform,&world,&model);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,itemGetEffectTransform,&transform);
  C3D_DrawArrays(GPU_TRIANGLES,pass*6,6);
 }
 C3D_DepthMap(true,-1,0);C3D_TexBind(1,nullptr);C2D_Prepare();C2D_SceneBegin(top);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
 C3D_AlphaTest(true,GPU_GREATER,8);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
 return true;
}
}
