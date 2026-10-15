#include "platform.hpp"
#ifdef __3DS__
#include "actor_mesh.hpp"
namespace ttyd {
namespace {
constexpr auto SourceRGB=GPU_WRITEMASK(GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE);
constexpr auto SourceRGBDepth=GPU_WRITEMASK(SourceRGB|GPU_WRITE_DEPTH);
}
bool Renderer::prepareLegacyMarioTexture(){
 if(characterTextures.imported)return true;
 FILE* file=std::fopen("romfs:/characters-mesh.t3x","rb");
 if(!file){frontFailure="Legacy Mario texture missing";return false;}
 characterTextures.imported=Tex3DS_TextureImportStdio(file,&characterTextures.texture,nullptr,false);std::fclose(file);
 if(!characterTextures.imported){frontFailure="Legacy Mario texture import failed";return false;}
 C3D_TexSetFilter(&characterTextures.texture,GPU_LINEAR,GPU_LINEAR);
 C3D_TexSetWrap(&characterTextures.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);return true;
}
bool Renderer::nativeMarioModel(const Game& g,float rootY){
 if(g.nativeAnimationPending())return true; // Retain publication; suppress stale mesh during producer hold.
 (void)rootY;
 const auto* p=g.nativeMarioPose();if(!p || !p->sourceBuffers || !nativeMarioTexture.imported || nativeMarioTextureBank!=int(g.nativeMarioBank()))return false;
 const auto& s=p->shape;if(s.vertices.size()>size_t(MaxVertices-count) || s.colors.size()!=s.vertices.size())return false;
 const ActorRootTransform transform(g.nativeMarioRoot(),g.nativeMarioYaw(),g.nativeMarioReflect());
 const uint32_t start=count;
 for(size_t i=0;i<s.vertices.size();++i){const auto& v=s.vertices[i];const auto xyz=transform.point(v);vertex(xyz[0],xyz[1],xyz[2],v.u,1-(1-v.v)*nativeMarioUVScale,0xffffffff,&s.colors[i]);}
 for(auto b:s.batches){b.start+=start;nativeMarioBatches.push_back(b);}return true;
}
void Renderer::drawNativeMario(bool translucent){
 if(!nativeMarioTexture.imported)return;
 for(const auto& b:nativeMarioBatches){
  if((b.pass==3)!=translucent || !b.count || b.cull==2)continue;
  C3D_TexBind(0,&nativeMarioTexture.texture);
  constexpr GPU_CULLMODE culls[]={GPU_CULL_FRONT_CCW,GPU_CULL_BACK_CCW,GPU_CULL_NONE,GPU_CULL_NONE};C3D_CullFace(culls[b.cull]);
  C3D_DepthTest(true,GPU_GEQUAL,translucent?SourceRGB:SourceRGBDepth);
  C3D_AlphaTest(!translucent,b.pass==2?GPU_GEQUAL:GPU_GREATER,b.pass==2?128:8);
  C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,b.pass==2?GPU_ONE:GPU_SRC_ALPHA,b.pass==2?GPU_ZERO:GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,b.pass==2?GPU_ZERO:GPU_ONE_MINUS_SRC_ALPHA);
  C3D_DrawArrays(GPU_TRIANGLES,b.start,b.count);
 }
 C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,SourceRGBDepth);C3D_AlphaTest(true,GPU_GREATER,8);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
}
}
#endif
