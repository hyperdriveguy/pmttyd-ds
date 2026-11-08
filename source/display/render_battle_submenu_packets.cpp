#include "platform.hpp"
#include "battle_submenu_packets.hpp"
#include "native_camera_draw.hpp"
#ifdef __3DS__
namespace ttyd {
// Empty spans consume no GPU state and need no resident atlas. Nonempty
// spans must share one exact camera/source owner (checked by batch validation).
bool Renderer::canDrawSubmenuPackets(const std::vector<BattleSubmenuPacketPtr>&packets)const{
 if(packets.empty())return true;
 if(!c3dReady||!programReady||!top||transformLoc<0||!fontTexture.imported||!submenuSurfaceGpu)return false;
 return validateBattleSubmenuBatch(packets,submenuSurfaceGpu->resource());
}
bool Renderer::drawSubmenuPackets(const std::vector<BattleSubmenuPacketPtr>&packets){
 if(!canDrawSubmenuPackets(packets))return false;
 if(packets.empty())return true;
 // Build all transformed geometry before C2D flush; a bad final packet must
 // reject the whole span without a native prefix. Recheck proofs afterward.
 std::vector<std::array<BattleClassVertex,4>> vertices;vertices.reserve(packets.size());
 for(const auto&p:packets){const auto v=battleSubmenuVertices(*p);if(!v)return false;vertices.push_back(*v);}
 // Whole-batch admission precedes the first physical mutation. No callback or
 // source read runs during the native draw; source/frame owner serializes GPU
 // resource retirement. Return true acknowledges every ordered primitive once.
 if(!canDrawSubmenuPackets(packets))return false;
 auto* texture=submenuSurfaceGpu->texture(packets.front()->surface);if(!texture)return false;
 // Vertices already contain loaded model/view. Add only projection and tilt.
 const CameraViewMatrix identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
 const auto native=nativeCameraTransform(packets.front()->camera.projection,identity);
 C3D_Mtx transform;for(unsigned i=0;i<4;++i){auto&r=transform.r[i];r.x=native[i][0];r.y=native[i][1];r.z=native[i][2];r.w=native[i][3];}
 const auto&vp=packets.front()->camera.viewport;const auto&sc=packets.front()->camera.scissor;
 const auto viewport=nativeCameraRect(vp[0],vp[1],vp[2],vp[3]);const auto scissor=nativeCameraRect(float(sc[0]),float(sc[1]),float(sc[2]),float(sc[3]));
 if(!viewport||!scissor)return false;
 // Source viewport near/far maps to reversed PICA scale near-far, offset
 // 1-far. Scissor API consumes endpoints, while viewport consumes dimensions.
 C2D_Flush();C3D_SetViewport(viewport->x,viewport->y,viewport->width,viewport->height);C3D_DepthMap(true,vp[4]-vp[5],1-vp[5]);
 C3D_SetScissor(GPU_SCISSOR_NORMAL,scissor->x,scissor->y,scissor->x+scissor->width,scissor->y+scissor->height);
 C3D_BindProgram(&program);auto*attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
 AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
 // Shared XYZ/ST/RGBA shader: slot0 is submenu2/3, slots1/2 use the live
 // parked font image. Reset all six TEV stages before stage0 modulation.
 C3D_TexBind(0,texture);C3D_TexBind(1,&fontTexture.texture);C3D_TexBind(2,&fontTexture.texture);
 for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
 auto*env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
 C3D_CullFace(GPU_CULL_NONE);C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
 // Preserve packet order as four-vertex fans; RGBA byte channels divide by 255.
 for(const auto&quad:vertices){C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);for(const auto&v:quad){C3D_ImmSendAttrib(v.x,v.y,v.z,1);C3D_ImmSendAttrib(v.u,v.v,0,1);C3D_ImmSendAttrib(float(v.rgba>>24)/255,float((v.rgba>>16)&255)/255,float((v.rgba>>8)&255)/255,float(v.rgba&255)/255);}C3D_ImmDrawEnd();}
 return true;
}
}
#endif
