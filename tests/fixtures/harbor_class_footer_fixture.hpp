#pragma once
// Explicit development composition over the actual tutorial VM and its real
// relocated common bank. Camera ownership remains the frame fixture's declared
// same-view fallback, not original battle-camera scheduling.
#include "harbor_class_frame_fixture.hpp"
#include "battle_class_footer_provider.hpp"
struct HarborClassFooterFixture {
 HarborClassFrameFixture& frame;
 const ttyd::BattleClassFooterGpuResource& resource;
 std::unique_ptr<ttyd::BattleClassFooterProvider> provider;
 ttyd::BattleTexturePlaneServices plane;
 ttyd::BattleTexturePlaneCursor planeCursor;
 HarborClassFooterFixture(HarborClassFrameFixture& f,
                          const ttyd::BattleClassFooterGpuResource& r):frame(f),resource(r){
  ttyd::BattleClassFooterQueryServices query;query.user=this;
  query.camera=[](void* u,std::optional<uint32_t> index)->std::optional<uint32_t>{
   const auto& f=static_cast<HarborClassFooterFixture*>(u)->frame;
   // This fixture explicitly dispatches the class callback under camera8.
   if(index && *index!=8)return {};
   return f.camera.count(f.Cam8+0x11c)?std::optional<uint32_t>(f.Cam8):std::nullopt;
  };
  query.texture=[](void* u,uint32_t palette,uint32_t index)->std::optional<ttyd::AnimationTPLImage>{
   auto& f=static_cast<HarborClassFooterFixture*>(u)->frame;
   ttyd::BattleClassFooterMemoryReader reader;reader.user=&f.scene;
   reader.word=[](void* v,uint32_t address)->std::optional<uint32_t>{
    uint32_t value=0;return static_cast<ttyd::HarborTutorial*>(v)->readPackedWord(address,value)?std::optional<uint32_t>(value):std::nullopt;
   };
   return ttyd::readBattleClassFooterTexture(reader,palette,index);
  };
  ttyd::BattleClassFooterQuadSink sink;sink.user=this;
  sink.append=[](void* u,const ttyd::BattleClassFooterAtlasQuad& q){return static_cast<HarborClassFooterFixture*>(u)->frame.backend.appendFooter(q);};
  provider=std::make_unique<ttyd::BattleClassFooterProvider>(resource,query,sink);
  plane.actors=f.memory;plane.user=this;
  plane.request=[](void* u,const ttyd::BattleTexturePlaneRequest& q){return static_cast<HarborClassFooterFixture*>(u)->provider->request(q);};
  plane.matrix=[](void*,ttyd::BattleAudienceMatrixRequest& q){ttyd::BattleAudienceAuxServices math;ttyd::bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
  f.footerRasterUser=this;
  f.footerRaster=[](void* u,const ttyd::BattleClassRasterRequest& q)->std::optional<uint32_t>{
   auto& owner=*static_cast<HarborClassFooterFixture*>(u);
   if(q.function==ttyd::BattleClassRasterService::TexturePlane2){
    if(q.integers.size()!=3 || q.integers[2]!=0 || !q.reals.empty() || !q.matrix)return {};
    return ttyd::resumePackedBattleTexturePlane2(*q.matrix,q.integers[0],q.integers[1],owner.plane,owner.planeCursor)?std::optional<uint32_t>(0):std::nullopt;
   }
   return owner.provider->rasterRequest(q,owner.frame.backend);
  };
 }
 // Caller must retire the source pipeline before destroying this provider.
 ~HarborClassFooterFixture(){frame.footerRaster=nullptr;frame.footerRasterUser=nullptr;}
 void marioHp(int16_t hp){
  const auto unit=ttyd::battleMarioUnit(ttyd::currentBattleAddress(frame.memory),frame.memory);
  assert(unit && frame.memory.memory.half(frame.memory.memory.user,unit+0x108)>5);
  frame.memory.memory.storeHalf(frame.memory.memory.user,unit+0x10c,uint16_t(hp));
  assert(int16_t(frame.memory.memory.half(frame.memory.memory.user,unit+0x10c))==hp);
 }
};
