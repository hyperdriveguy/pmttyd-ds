#include "battle_window_packets.hpp"
#include <string>
#include <cstdio>
#include <cmath>
#ifdef __3DS__
#include <tex3ds.h>
#include <citro2d.h>
#endif
namespace ttyd {
struct NativeBattleWindowSurfaceGpu::State {
 BattleWindowSurfaceResource resource;
#ifdef __3DS__
 C3D_Tex texture{};Tex3DS_Texture imported=nullptr;
#endif
};
NativeBattleWindowSurfaceGpu::NativeBattleWindowSurfaceGpu():state_(std::make_unique<State>()){}
NativeBattleWindowSurfaceGpu::~NativeBattleWindowSurfaceGpu(){release();}
const BattleWindowSurfaceResource&NativeBattleWindowSurfaceGpu::resource()const{return state_->resource;}
bool NativeBattleWindowSurfaceGpu::load(const char*root){
#ifdef __3DS__
 if(!root||state_->imported||state_->resource.lease().current())return false;
 std::string path=root;if(!path.empty()&&path.back()!='/')path+='/';path+="battle-window-panel.t3x";
 FILE*f=std::fopen(path.c_str(),"rb");if(!f)return false;state_->imported=Tex3DS_TextureImportStdio(f,&state_->texture,nullptr,false);std::fclose(f);if(!state_->imported)return false;
 auto&t=state_->texture;BattleClassSurfaceImport facts;facts.identity=&t;facts.width=t.width;facts.height=t.height;facts.format=t.fmt;facts.count=Tex3DS_GetNumSubTextures(state_->imported);
 if(facts.count){const auto*s=Tex3DS_GetSubTexture(state_->imported,0);facts.subWidth=s->width;facts.subHeight=s->height;facts.rotated=Tex3DS_SubTextureRotated(s);facts.uv={s->left,s->top,s->right,s->bottom};}
 if(!state_->resource.adopt(facts,static_cast<const uint8_t*>(t.data),t.size)){release();return false;}
 C3D_TexSetFilter(&t,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&t,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);return true;
#else
 (void)root;return false;
#endif
}
void NativeBattleWindowSurfaceGpu::release(){state_->resource.retire();
#ifdef __3DS__
 if(state_->imported){C3D_FrameSync();Tex3DS_TextureFree(state_->imported);C3D_TexDelete(&state_->texture);state_->imported=nullptr;}
#endif
}
#ifdef __3DS__
bool NativeBattleWindowSurfaceGpu::canDraw(const BattleWindowPacket&p,const shaderProgram_s*program,int location,const C3D_Tex*parking)const{
 if(!program||location<0||!parking||!state_->imported||!validateBattleWindowPacket(p,state_->resource))return false;
 const auto m=battleWindowTransform(p);for(const auto&row:m)for(float v:row)if(!std::isfinite(v))return false;return true;
}
bool NativeBattleWindowSurfaceGpu::draw(const BattleWindowPacket&p,shaderProgram_s*program,int location,C3D_Tex*parking){
 if(!canDraw(p,program,location,parking))return false;
 const auto t=battleWindowTransform(p);C3D_Mtx matrix{};for(unsigned i=0;i<4;++i){auto&r=matrix.r[i];r.x=t[i][0];r.y=t[i][1];r.z=t[i][2];r.w=t[i][3];}
 const auto&v=p.camera.viewport;const auto&s=p.camera.scissor;const auto viewport=*nativeCameraRect(v[0],v[1],v[2],v[3]);const auto scissor=*nativeCameraRect(float(s[0]),float(s[1]),float(s[2]),float(s[3]));
 C2D_Flush();C3D_SetViewport(viewport.x,viewport.y,viewport.width,viewport.height);C3D_SetScissor(GPU_SCISSOR_NORMAL,scissor.x,scissor.y,scissor.x+scissor.width,scissor.y+scissor.height);C3D_DepthMap(true,v[4]-v[5],1-v[5]);
 C3D_BindProgram(program);auto*attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,location,&matrix);
 C3D_CullFace(GPU_CULL_NONE);C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);C3D_FogGasMode(GPU_NO_FOG,GPU_PLAIN_DENSITY,false);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
 for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
 auto*env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
 C3D_TexBind(0,&state_->texture);C3D_TexBind(1,parking);C3D_TexBind(2,parking);
 for(unsigned q=0;q<9;++q){C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);for(unsigned i=0;i<4;++i){const auto&pt=p.vertices[4*q+i];C3D_ImmSendAttrib(pt[0],pt[1],pt[2],1);C3D_ImmSendAttrib(pt[3]*(72.f/128),1-pt[4]*(72.f/128),0,1);C3D_ImmSendAttrib(float(p.rgba>>24)/255,float((p.rgba>>16)&255)/255,float((p.rgba>>8)&255)/255,float(p.rgba&255)/255);}C3D_ImmDrawEnd();}return true;
}
#endif
}
