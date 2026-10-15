#include "battle_class_native_footers.hpp"
#include "battle_act_menu.hpp"
#include "battle_audience_item_on_native.hpp"
#include "battle_packed_status.hpp"
#include <cstdlib>
namespace ttyd {
BattleClassNativeFooters::BattleClassNativeFooters(BattleClassNativeFrame& frame,
    BattleClassNativeNumeric& numeric,const BattleClassFooterGpuResource& resource,
    BattleActorServices actors,
    std::function<std::optional<uint32_t>(uint32_t)> resourceWord,
    std::function<bool()> current,std::function<bool(uint32_t)> sound)
    :frame_(frame),numeric_(numeric),actors_(actors),resourceWord_(std::move(resourceWord)),
      current_(std::move(current)),sound_(std::move(sound)) {
  BattleClassFooterQueryServices query;query.user=this;
  query.camera=[](void* p,std::optional<uint32_t> index){auto& c=*static_cast<BattleClassNativeFooters*>(p);
    return c.current()?c.numeric_.camera(index):std::nullopt;};
  query.texture=[](void* p,uint32_t palette,uint32_t index)->std::optional<AnimationTPLImage>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);if(!c.current())return {};
    BattleClassFooterMemoryReader reader{p,[](void* u,uint32_t at)->std::optional<uint32_t>{
      auto& c=*static_cast<BattleClassNativeFooters*>(u);return c.current()&&c.resourceWord_?c.resourceWord_(at):std::nullopt;}};
    const auto value=readBattleClassFooterTexture(reader,palette,index);
    return c.current()?value:std::nullopt;
  };
  BattleClassFooterQuadSink sink{this,[](void* p,const BattleClassFooterAtlasQuad& q){
    auto& c=*static_cast<BattleClassNativeFooters*>(p);return c.current()&&c.frame_.appendFooter(q);}};
  provider_=std::make_unique<BattleClassFooterProvider>(resource,query,sink);
  footer_.raster=numeric_.raster();footer_.raster.user=this;
  // Raster circles are read-only, whereas the original party footer writes
  // its actual W+120 angle. Keep the numeric camera reader and explicitly
  // forward the footer's real VM writes through their original owner.
  footer_.raster.memory=actors_;
  auto& memory=footer_.raster.memory.memory;memory={};memory.user=this;
  memory.word=[](void* p,uint32_t at){auto& c=*static_cast<BattleClassNativeFooters*>(p);
    const auto s=c.numeric_.raster();return s.memory.memory.word(s.memory.memory.user,at);};
  if(actors_.memory.half)memory.half=[](void* p,uint32_t at){auto& c=*static_cast<BattleClassNativeFooters*>(p);
    const auto s=c.numeric_.raster();return s.memory.memory.half(s.memory.memory.user,at);};
  if(actors_.memory.storeWord)memory.storeWord=[](void* p,uint32_t at,uint32_t value){
    auto& c=*static_cast<BattleClassNativeFooters*>(p);
    if(!c.current()||c.frame_.camera()->owns(at))std::abort();
    c.actors_.memory.storeWord(c.actors_.memory.user,at,value);};
  footer_.raster.request=[](void* p,const BattleClassRasterRequest& q){return static_cast<BattleClassNativeFooters*>(p)->raster(q);};
  // identity/truncate have their own numeric user; these callbacks do not use
  // footer_.raster.user. Preserve that stable numeric context explicitly.
  footer_.raster.identity=[](void* p)->std::optional<BattleActMatrix>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);const auto s=c.numeric_.raster();
    return c.current()&&s.identity?s.identity(s.user):std::nullopt;};
  footer_.raster.truncate=[](void* p,float value)->std::optional<int32_t>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);const auto s=c.numeric_.raster();
    return c.current()&&s.truncate?s.truncate(s.user,value):std::nullopt;};
  footer_.user=this;
  footer_.project=[](void* p,uint32_t at)->std::optional<std::array<float,3>>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);return c.current()?c.numeric_.project(at):std::nullopt;};
  footer_.audienceItem=[](void* p,uint32_t)->std::optional<int32_t>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);if(!c.current())return {};
    const auto item=nativeBattleAudienceItemOn(c.actors_,BattleAudiencePrivateOutputs::IndexAndPosition);
    return c.current()?std::optional<int32_t>(item.index):std::nullopt;};
  footer_.changePosition=[](void* p,uint32_t battle)->std::optional<bool>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);if(!c.current())return {};
    BattleActMenuServices s;s.memory=c.actors_;s.user=p;
    s.request=[](void* p,const BattleActRequest& q)->std::optional<uint32_t>{
      auto& c=*static_cast<BattleClassNativeFooters*>(p);if(!c.current())return {};
      using F=BattleActService;const auto& a=q.arguments;
      if(q.function==F::Mario)return battleMarioUnit(a[0],c.actors_);
      if(q.function==F::Party)return battlePartyUnit(a[0],c.actors_);
      if(q.function==F::Status)return checkPackedBattleStatus(a[0],int32_t(a[1]),c.actors_)?1u:0u;
      return {};
    };
    const auto result=checkPackedBattleChangePosition(battle,s);
    return c.current()&&result?std::optional<bool>(*result!=0):std::nullopt;
  };
  footer_.sound=[](void* p,uint32_t key){auto& c=*static_cast<BattleClassNativeFooters*>(p);
    return c.current()&&c.sound_&&c.sound_(key)&&c.current();};
  footer_.button=[](void* p,int32_t type,float x,float y,float angle){
    auto& c=*static_cast<BattleClassNativeFooters*>(p);
    return c.current()&&resumeBattleClassFooterButton(type,x,y,angle,c.footer_,c.button_)&&c.current();};
  plane_.actors=footer_.raster.memory;plane_.user=this;
  plane_.request=[](void* p,const BattleTexturePlaneRequest& q)->std::optional<uint32_t>{
    auto& c=*static_cast<BattleClassNativeFooters*>(p);if(!c.current())return {};
    const auto result=c.provider_->request(q);return c.current()?result:std::nullopt;};
  plane_.matrix=[](void* p,BattleAudienceMatrixRequest& q){auto& c=*static_cast<BattleClassNativeFooters*>(p);
    const auto* bridge=c.current()?c.frame_.camera():nullptr;
    return bridge&&bridge->matrix(q)&&c.current();};
}
bool BattleClassNativeFooters::current() const {
  return current_&&current_()&&frame_.camera();
}
std::optional<uint32_t> BattleClassNativeFooters::raster(const BattleClassRasterRequest& q){
  if(!current())return {};
  if(q.function==BattleClassRasterService::Camera){
    const auto s=numeric_.raster();return s.request?s.request(s.user,q):std::nullopt;
  }
  if(q.function==BattleClassRasterService::TexturePlane2){
    if(!q.matrix||q.integers.size()!=3||q.integers[2])return {};
    // These are the retained original footer request's actual matrix/RGBA
    // objects. Their addresses survive the nested TexturePlane2 continuation.
    if(!resumePackedBattleTexturePlane2(*q.matrix,q.integers[0],q.integers[1],plane_,planeCursor_))return {};
    return current()?std::optional<uint32_t>(0):std::nullopt;
  }
  return frame_.footerRasterRequest(q,*provider_);
}
bool BattleClassNativeFooters::party(){
  if(invoking_||!current())return false;
  invoking_=true;struct Guard{bool& active;~Guard(){active=false;}} guard{invoking_};
  return resumeBattleClassPartyFooter(footer_,party_)&&current();
}
bool BattleClassNativeFooters::pinch(){
  if(invoking_||!current())return false;
  invoking_=true;struct Guard{bool& active;~Guard(){active=false;}} guard{invoking_};
  return resumeBattleClassPinchFooter(footer_,pinch_)&&current();
}
}
