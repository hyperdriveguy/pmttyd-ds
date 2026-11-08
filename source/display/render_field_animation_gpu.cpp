#include "platform.hpp"
namespace ttyd {
void Renderer::animationCameraTransform(const NativeCameraPacket& p){
 C3D_BindProgram(&program);C3D_Mtx transform{};
 for(unsigned i=0;i<4;++i){auto& v=transform.r[i];v.x=p.transform[i][0];v.y=p.transform[i][1];v.z=p.transform[i][2];v.w=p.transform[i][3];}
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
}
bool Renderer::fieldAnimationCameraPacket(const NativeCameraPacket& p,const NativeCameraDraw& owner){
 const auto& r=p.rect;
 if(p.kind==NativeCameraCommand::Viewport){
  // Citro3D disables scissor on viewport publication; GX preserves it.
  std::optional<NativeCameraRect> scissor;
  if(const auto& logical=owner.scissor()){
   scissor=nativeCameraRoundedRect((*logical)[0],(*logical)[1],(*logical)[2],(*logical)[3]);
   if(!scissor)return false;
  }
  C3D_SetViewport(r.x,r.y,r.width,r.height);C3D_DepthMap(true,p.depthScale,p.depthOffset);
  if(scissor)C3D_SetScissor(GPU_SCISSOR_NORMAL,scissor->x,scissor->y,scissor->x+scissor->width,scissor->y+scissor->height);
  return true;
 }
 if(p.kind==NativeCameraCommand::Scissor){C3D_SetScissor(GPU_SCISSOR_NORMAL,r.x,r.y,r.x+r.width,r.y+r.height);return true;}
 if(p.kind==NativeCameraCommand::Projection){animationCameraTransform(p);return true;}
 // Dummy primitives require the field's actual material-state owner. A battle
 // owner's initialization history cannot acknowledge field source effects.
 return false;
}
std::unique_ptr<FieldAnimationGpu> Renderer::createFieldAnimationGpu(EventAddressSpace& memory,
 const FieldAnimationSubmit& submit,const AnimationSourceRenderChildren& fallback,std::function<bool()> current){
 FieldAnimationPhysical physical{this,[](void* u,const NativeCameraPacket& p,const NativeCameraDraw& owner){return static_cast<Renderer*>(u)->fieldAnimationCameraPacket(p,owner);},
 [](void*,uint32_t mode){
  // GX NONE/FRONT/BACK/ALL. The stager consumes ALL fans without submitting
  // geometry; PICA has no equivalent all-face cull token.
  if(mode>3)return false;
  C3D_CullFace(mode==1?GPU_CULL_BACK_CCW:mode==2?GPU_CULL_FRONT_CCW:GPU_CULL_NONE);return true;
 }};
 return std::make_unique<FieldAnimationGpu>(memory,physical,submit,fallback,std::move(current));
}
}
