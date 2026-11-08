#include "battle_weapon_native_raster_plan.hpp"
#include "battle_weapon_icon_evidence.hpp"
#include <cmath>
namespace ttyd {namespace {
struct Checking{bool&value;explicit Checking(bool&v):value(v){value=true;}~Checking(){value=false;}};
bool geometry(const BattleWeaponRasterPrimitive&p){
 if(!p.texture||!p.count||p.count>36||p.count%4)return false;
 for(const auto&row:p.transform)for(float v:row)if(!std::isfinite(v))return false;
 for(unsigned i=0;i<p.count;++i){const auto&v=p.vertices[i];for(float f:{v.x,v.y,v.z,v.u,v.v})if(!std::isfinite(f))return false;
  for(unsigned row=0;row<4;++row){float sum=0;for(unsigned k=0;k<4;++k){const float coordinate=k==0?v.x:k==1?v.y:k==2?v.z:1;const volatile float product=p.transform[row][k]*coordinate;if(!std::isfinite(product))return false;const volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;}}
 }return true;
}
bool resources(const BattleWeaponOrderedBatch&b,const BattleWeaponRasterResources&r){
 if(!r.font||!r.font->lease().current())return false;
 for(const auto&q:b.packets){
  if(const auto*p=std::get_if<BattleFontGlyphPacket>(&q)){if(!r.font->accepts(p->font)||!validBattleFontGlyphPacket(*p))return false;}
  else if(const auto*p=std::get_if<BattleSubmenuPacketPtr>(&q)){if(!*p||!r.submenu||!validateBattleSubmenuPacket(**p,*r.submenu)||(*p)->camera.generation!=b.generation||(*p)->camera.pass!=b.pass||(*p)->sourceResourceOwner!=b.sourceResourceOwner||(*p)->sourceResourceOwner.owner_before(b.sourceResourceOwner)||b.sourceResourceOwner.owner_before((*p)->sourceResourceOwner))return false;}
  else if(const auto*p=std::get_if<BattleWindowPacketPtr>(&q)){if(!*p||!r.window||!validateBattleWindowPacket(**p,*r.window)||(*p)->camera.generation!=b.generation||(*p)->camera.pass!=b.pass)return false;}
  else if(const auto*p=std::get_if<BattleClassSurfacePacketPtr>(&q)){if(!*p||!r.classes||!validateBattleClassSurfacePacket(**p,*r.classes)||(*p)->camera.generation!=b.generation||(*p)->camera.pass!=b.pass||(*p)->sourceResourceOwner!=b.sourceResourceOwner||(*p)->sourceResourceOwner.owner_before(b.sourceResourceOwner)||b.sourceResourceOwner.owner_before((*p)->sourceResourceOwner))return false;}
  else{const auto&icon=std::get<BattleEnemyHpIconPacket>(q);if(!b.iconEvidence||!b.iconEvidence->size()||!b.iconEvidence->current()||!validBattleEnemyHpIconPacket(icon.packet)||!validBattleEnemyHpIconResidency(icon.residency))return false;}
 }return true;
}
bool camera(BattleWeaponRasterPrimitive&o,const BattleSubmenuCameraProof&c){
 const auto&v=c.viewport;const auto&s=c.scissor;const auto vp=nativeCameraRect(v[0],v[1],v[2],v[3]),sc=nativeCameraRect(float(s[0]),float(s[1]),float(s[2]),float(s[3]));if(!vp||!sc)return false;
 o.viewport=*vp;o.scissor=*sc;o.depthScale=v[4]-v[5];o.depthOffset=1-v[5];return true;
}
}
bool BattleWeaponRasterPlan::current(const void*owner,const BattleWeaponRasterResources&r)const{
 if(checking_)return false;
 Checking guard(checking_);
 if(owner!=owner_||!batch_||!r.font||!r.font->accepts(fontAnchor_)||!resources(*batch_,r))return false;
 for(const auto&p:primitives_)if(!geometry(p))return false;
 // Callback-based source proofs above cannot revive a retired native lease.
 if(!r.font->accepts(fontAnchor_))return false;
 for(const auto&q:batch_->packets){if(const auto*p=std::get_if<BattleSubmenuPacketPtr>(&q)){if(!r.submenu->accepts((*p)->surface))return false;}else if(const auto*p=std::get_if<BattleWindowPacketPtr>(&q)){if(!r.window->accepts((*p)->surface))return false;}else if(const auto*p=std::get_if<BattleClassSurfacePacketPtr>(&q)){if(!r.classes->accepts((*p)->surface))return false;}}
 return true;
}
std::shared_ptr<const BattleWeaponRasterPlan>prepareBattleWeaponRasterGeometry(const void*owner,std::shared_ptr<const BattleWeaponOrderedBatch>b,const BattleWeaponRasterResources&r){
 if(!owner||!b||!b->allocationOwner||!b->sourceResourceOwner||!b->allocation||!b->generation||!b->pass||!resources(*b,r))return {};
 auto out=std::shared_ptr<BattleWeaponRasterPlan>(new BattleWeaponRasterPlan);out->owner_=owner;out->batch_=std::move(b);out->fontAnchor_=r.font->lease();out->primitives_.reserve(out->batch_->packets.size());
 for(const auto&q:out->batch_->packets){BattleWeaponRasterPrimitive item;
  if(const auto*p=std::get_if<BattleFontGlyphPacket>(&q)){
   item.kind=BattleWeaponRasterKind::Font;item.texture=p->font.texture;item.transform=battleFontGlyphTransform(*p);item.viewport=p->raster.viewport;item.scissor=p->raster.scissor;item.depthScale=p->raster.depthScale;item.depthOffset=p->raster.depthOffset;item.count=4;
   const auto&g=p->quad;item.vertices[0]={float(g.left),float(g.top),0,float(g.u0)/512,1-float(g.v0)/256,g.rgba};item.vertices[1]={float(g.right),float(g.top),0,float(g.u1)/512,1-float(g.v0)/256,g.rgba};item.vertices[2]={float(g.right),float(g.bottom),0,float(g.u1)/512,1-float(g.v1)/256,g.rgba};item.vertices[3]={float(g.left),float(g.bottom),0,float(g.u0)/512,1-float(g.v1)/256,g.rgba};
  }else if(const auto*p=std::get_if<BattleSubmenuPacketPtr>(&q)){
   item.kind=BattleWeaponRasterKind::Submenu;item.texture=(*p)->surface.texture;const auto vertices=battleSubmenuVertices(**p);if(!vertices||!camera(item,(*p)->camera))return {};const CameraViewMatrix identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};item.transform=nativeCameraTransform((*p)->camera.projection,identity);item.count=4;for(unsigned i=0;i<4;++i){const auto&v=(*vertices)[i];item.vertices[i]={v.x,v.y,v.z,v.u,v.v,v.rgba};}
  }else if(const auto*p=std::get_if<BattleWindowPacketPtr>(&q)){
   item.kind=BattleWeaponRasterKind::Window;item.texture=(*p)->surface.texture;item.transform=battleWindowTransform(**p);if(!camera(item,(*p)->camera))return {};item.count=36;for(unsigned i=0;i<36;++i){const auto&v=(*p)->vertices[i];item.vertices[i]={v[0],v[1],v[2],v[3]*(72.f/128),1-v[4]*(72.f/128),(*p)->rgba};}
  }else if(const auto*p=std::get_if<BattleClassSurfacePacketPtr>(&q)){
   item.kind=BattleWeaponRasterKind::ClassSurface;item.texture=(*p)->surface.textures[0];item.classRGB=(*p)->rgb;item.classMaterial=(*p)->material;
   const auto vertices=battleClassSurfaceVertices(**p);if(!vertices||!camera(item,(*p)->camera))return {};const CameraViewMatrix identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};item.transform=nativeCameraTransform((*p)->camera.projection,identity);item.count=4;
   for(unsigned i=0;i<4;++i){const auto&v=(*vertices)[i];item.vertices[i]={v.x,v.y,v.z,v.u,v.v,v.rgba};}
  }else{
   const auto&icon=std::get<BattleEnemyHpIconPacket>(q);const auto&v=icon.packet;const auto&r=icon.residency;item.kind=BattleWeaponRasterKind::Icon;item.texture=r.texture;item.transform=v.transform;item.viewport=v.viewport;item.scissor=v.scissor;item.depthScale=v.depthScale;item.depthOffset=v.depthOffset;item.iconMaterial=v.material;item.iconPipeline=v.pipeline;item.count=4;
   for(unsigned i=0;i<4;++i){const auto&xyz=v.quad.positions[i];const auto&uv=v.quad.uv[i];item.vertices[i]={xyz[0],xyz[1],xyz[2],(2.f+uv[0]*r.width)/r.gpuWidth,1.f-(2.f+uv[1]*r.height)/r.gpuHeight,0xffffffff};}
  }
  if(!geometry(item))return {};
  out->primitives_.push_back(item);
 }
 return out->current(owner,r)?out:nullptr;
}
}
