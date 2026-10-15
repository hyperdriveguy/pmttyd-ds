#include "platform.hpp"
namespace ttyd {
// Adopt the actual already-imported font texture/metadata once; this is not
// proof that original font Init/Setup/static GX object has completed.
bool Renderer::prepareBattleFont(){
 if(!c3dReady||!programReady||!fontTexture.imported)return false;
 if(battleFontGpu.lease().current())return true;
 MessageFont font;if(!font.load("romfs:/font.pmfont"))return false;
 BattleClassSurfaceImport actual;actual.identity=&fontTexture.texture;
 actual.width=fontTexture.texture.width;actual.height=fontTexture.texture.height;actual.format=fontTexture.texture.fmt;
 actual.count=Tex3DS_GetNumSubTextures(fontTexture.imported);
 if(actual.count){const auto*s=Tex3DS_GetSubTexture(fontTexture.imported,0);actual.subWidth=s->width;actual.subHeight=s->height;actual.rotated=Tex3DS_SubTextureRotated(s);actual.uv={s->left,s->top,s->right,s->bottom};}
 return battleFontGpu.adopt(font,actual);
}
BattleFontLease Renderer::battleFontLease()const{return battleFontGpu.lease();}
void Renderer::retireBattleFont(){battleFontGpu.retire();}
// Actual battle camera8/CameraDraw projection and raster are required. Glyph
// position matrices already include source view; identity here avoids a second
// view transform. This projection adapter supplies no original camera fallback.
std::optional<BattleFontRasterView> Renderer::battleFontRasterView(const Game& game)const{
 const auto* cameras=game.trioAttack.cameras();const auto&p=finishingCameraDraw.projectionMatrix();
 if(!game.trioBattleActive||!cameras||cameras->currentIndex()!=8||!p||!iconViewport||!iconScissor)return {};
 const CameraViewMatrix identity{{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}}}};
 return BattleFontRasterView{nativeCameraTransform(*p,identity),iconViewport->rect,iconScissor->rect,iconViewport->depthScale,iconViewport->depthOffset};
}
bool Renderer::canDrawBattleFontGlyph(const BattleFontGlyphPacket&p)const{
 return c3dReady&&c2dReady&&programReady&&transformLoc>=0&&fontTexture.imported&&
     p.font.texture==&fontTexture.texture&&battleFontGpu.accepts(p.font)&&validBattleFontGlyphPacket(p);
}
bool Renderer::drawBattleFontGlyphs(const std::vector<BattleFontGlyphPacket>&packets){
 // Entire leaf batch before any GPU state. The outer ordered weapon owner
 // must additionally preflight ALL mixed packets and acknowledge its callback
 // only after actual submission. No imports/source execution/RNG during replay.
 if(!c3dReady||!c2dReady||!programReady||transformLoc<0||!battleFontGpu.lease().current())return false;
 for(const auto&p:packets)if(!canDrawBattleFontGlyph(p))return false;
 if(packets.empty())return true;
 C2D_Flush();C3D_BindProgram(&program);
 // Native attributes XYZ/UV/RGBA; ordinary source profile has ALWAYS alpha
 // compare,source-alpha blending,ALWAYS depth test with color-only writes.
 // Reset all six native TEV stages,then use C0 RGB and A0*texture-alpha.
 auto*attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);
 C3D_AlphaTest(false,GPU_ALWAYS,0);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
 for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
 auto*env=C3D_GetTexEnv(0);
 C3D_TexEnvSrc(env,C3D_RGB,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);
 C3D_TexEnvSrc(env,C3D_Alpha,GPU_PRIMARY_COLOR,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
 C3D_TexBind(0,&fontTexture.texture);parkUnusedSamplers();
 for(const auto&p:packets){
  const auto&r=p.raster;C3D_SetViewport(r.viewport.x,r.viewport.y,r.viewport.width,r.viewport.height);
  C3D_SetScissor(GPU_SCISSOR_NORMAL,r.scissor.x,r.scissor.y,r.scissor.x+r.scissor.width,r.scissor.y+r.scissor.height);C3D_DepthMap(true,r.depthScale,r.depthOffset);
  const auto transform=battleFontGlyphTransform(p);C3D_Mtx m{};
  for(unsigned i=0;i<4;++i){auto&row=m.r[i];row.x=transform[i][0];row.y=transform[i][1];row.z=transform[i][2];row.w=transform[i][3];}
  C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&m);
  const auto&q=p.quad;const float positions[4][2]={{float(q.left),float(q.top)},{float(q.right),float(q.top)},{float(q.right),float(q.bottom)},{float(q.left),float(q.bottom)}};
  // Original US glyph coordinates use512x256 texels; normalize once and flip
  // native T once. Packed RGBA bytes become0..1 vertex constants (/255).
  const float uv[4][2]={{float(q.u0)/512,1-float(q.v0)/256},{float(q.u1)/512,1-float(q.v0)/256},{float(q.u1)/512,1-float(q.v1)/256},{float(q.u0)/512,1-float(q.v1)/256}};
  C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
  for(unsigned i=0;i<4;++i){C3D_ImmSendAttrib(positions[i][0],positions[i][1],0,1);C3D_ImmSendAttrib(uv[i][0],uv[i][1],0,1);C3D_ImmSendAttrib(float(q.rgba>>24)/255,float((q.rgba>>16)&255)/255,float((q.rgba>>8)&255)/255,float(q.rgba&255)/255);}
  C3D_ImmDrawEnd();
 }
 return true;
}
}
