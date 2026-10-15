#include "platform.hpp"
#include "battle_weapon_native_raster_plan.hpp"
namespace ttyd {namespace {struct WeaponRasterInvocation{bool&value;explicit WeaponRasterInvocation(bool&v):value(v){value=true;}~WeaponRasterInvocation(){value=false;}};}
std::shared_ptr<const BattleWeaponRasterPlan>Renderer::prepareWeaponOrderedRaster(std::shared_ptr<const BattleWeaponOrderedBatch>b)const{
 if(weaponRasterInvoking_)return {};
 WeaponRasterInvocation guard(weaponRasterInvoking_);
 if(!b||!c3dReady||!c2dReady||!programReady||!top||transformLoc<0||!fontTexture.imported||!battleFontGpu.lease().current())return {};
 // Every actual icon allocation and physical surface must exist before plan
 // allocation/geometry. No source request/import is used as an admission stub.
 for(const auto&q:b->packets){
  if(const auto*p=std::get_if<BattleEnemyHpIconPacket>(&q)){if(!canDrawEnemyHpIcon(*p))return {};}
  else if(const auto*p=std::get_if<BattleSubmenuPacketPtr>(&q)){if(!*p||!submenuSurfaceGpu||!submenuSurfaceGpu->texture((*p)->surface))return {};}
  else if(const auto*p=std::get_if<BattleWindowPacketPtr>(&q)){if(!*p||!windowPrimitiveGpu||!windowPrimitiveGpu->texture((*p)->surface))return {};}
 }
 BattleWeaponRasterResources resources{&battleFontGpu,submenuSurfaceGpu?&submenuSurfaceGpu->resource():nullptr,windowPrimitiveGpu?&windowPrimitiveGpu->resource():nullptr};
 auto plan=prepareBattleWeaponRasterGeometry(this,std::move(b),resources);return plan&&canDrawWeaponOrderedRasterImpl(*plan)?plan:nullptr;
}
bool Renderer::canDrawWeaponOrderedRaster(const BattleWeaponRasterPlan&p)const{
 if(weaponRasterInvoking_)return false;
 WeaponRasterInvocation guard(weaponRasterInvoking_);return canDrawWeaponOrderedRasterImpl(p);
}
bool Renderer::canDrawWeaponOrderedRasterImpl(const BattleWeaponRasterPlan&p)const{
 if(!c3dReady||!c2dReady||!programReady||!top||transformLoc<0||!fontTexture.imported||p.owner_!=this||p.fontAnchor_.texture!=&fontTexture.texture)return false;
 const BattleWeaponRasterResources resources{&battleFontGpu,submenuSurfaceGpu?&submenuSurfaceGpu->resource():nullptr,windowPrimitiveGpu?&windowPrimitiveGpu->resource():nullptr};if(!p.current(this,resources))return false;
 for(const auto&q:p.batch_->packets){
  if(const auto*v=std::get_if<BattleFontGlyphPacket>(&q)){if(!canDrawBattleFontGlyph(*v))return false;}
  else if(const auto*v=std::get_if<BattleEnemyHpIconPacket>(&q)){if(!canDrawEnemyHpIcon(*v))return false;}
  else if(const auto*v=std::get_if<BattleSubmenuPacketPtr>(&q)){if(!submenuSurfaceGpu||!submenuSurfaceGpu->texture((*v)->surface))return false;}
  else{const auto&window=std::get<BattleWindowPacketPtr>(q);if(!windowPrimitiveGpu||!windowPrimitiveGpu->texture(window->surface))return false;}
 }
 return c3dReady&&c2dReady&&programReady&&top&&transformLoc>=0&&fontTexture.imported&&battleFontGpu.accepts(p.fontAnchor_);
}
bool Renderer::submitWeaponOrderedRaster(const BattleWeaponRasterPlan&p){
 if(weaponRasterInvoking_)return false;
 WeaponRasterInvocation guard(weaponRasterInvoking_);
 // ALL fallible checks, callbacks, resource queries and prepared geometry are
 // above this first GPU effect. The caller serializes actual retirement here.
 if(!canDrawWeaponOrderedRasterImpl(p))return false;
 if(p.primitives_.empty())return true;
 C2D_Flush();
 for(const auto&item:p.primitives_){auto*texture=static_cast<C3D_Tex*>(const_cast<void*>(item.texture));
  const auto&v=item.viewport;const auto&s=item.scissor;C3D_SetViewport(v.x,v.y,v.width,v.height);C3D_SetScissor(GPU_SCISSOR_NORMAL,s.x,s.y,s.x+s.width,s.y+s.height);C3D_DepthMap(true,item.depthScale,item.depthOffset);
  if(item.kind==BattleWeaponRasterKind::Icon){
   // These existing concrete setters are nonallocating/nonfailing: they use
   // copied admitted material/pipeline and the exact prepared native texture.
   publishSourceIconProgram(item.iconMaterial,*texture);finishingDisplayPipeline(item.iconPipeline);
  }else{
   C3D_BindProgram(&program);auto*attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
   C3D_CullFace(GPU_CULL_NONE);C3D_EarlyDepthTest(false,GPU_EARLYDEPTH_GEQUAL,0);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);C3D_FogGasMode(GPU_NO_FOG,GPU_PLAIN_DENSITY,false);
   C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
   for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
   auto*env=C3D_GetTexEnv(0);
   if(item.kind==BattleWeaponRasterKind::Font){C3D_TexEnvSrc(env,C3D_RGB,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);C3D_TexEnvSrc(env,C3D_Alpha,GPU_PRIMARY_COLOR,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);}
   else{C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);}
   C3D_TexBind(0,texture);parkUnusedSamplers();
  }
  C3D_Mtx matrix{};for(unsigned i=0;i<4;++i){auto&r=matrix.r[i];r.x=item.transform[i][0];r.y=item.transform[i][1];r.z=item.transform[i][2];r.w=item.transform[i][3];}C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&matrix);
  for(unsigned q=0;q<item.count;q+=4){C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);for(unsigned i=0;i<4;++i){const auto&v=item.vertices[q+i];C3D_ImmSendAttrib(v.x,v.y,v.z,1);C3D_ImmSendAttrib(v.u,v.v,0,1);C3D_ImmSendAttrib(float(v.rgba>>24)/255,float((v.rgba>>16)&255)/255,float((v.rgba>>8)&255)/255,float(v.rgba&255)/255);}C3D_ImmDrawEnd();}
 }
 return true;
}
}
