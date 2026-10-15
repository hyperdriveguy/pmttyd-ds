#include "battle_class_frame_pipeline.hpp"
#include "battle_class_observable_draw.hpp"
namespace ttyd {
struct BattleClassFramePipeline::State {
  std::shared_ptr<const uint8_t> owner;
  uint32_t allocation=0,camera=0,battle=0;
  uint64_t completed=0;
  bool active=false,sourceComplete=false,invalidated=false;
  BattleClassPublicationKey key;
  BattleClassDisplayServices source,display;
  BattleClassRasterServices raster;
  BattleClassDrawContinuationServices icons;
  BattleClassBackend* backend=nullptr;
  const BattleClassAssets* assets=nullptr;
  const MessageFont* font=nullptr;
  std::function<bool()> allocationCurrent;
  BattleClassDisplayCursor queued;
  BattleClassObservableDrawCursor iconCursor;
  bool current() const {
    return allocationCurrent && allocationCurrent() && backend &&
        backend->assets==assets && backend->font==font && assets && assets->ready();
  }
  template<class F> auto call(F function)->decltype(function()) {
    using R=decltype(function());
    if(invalidated || !current()){invalidated=true;return R{};}
    auto result=function();
    if(invalidated || !current()){invalidated=true;return R{};}
    return result;
  }
  State(std::shared_ptr<const uint8_t> token,uint32_t at,
        const BattleClassDisplayServices& input,const BattleClassRasterServices& art,
        BattleClassBackend& output,std::function<bool()> live)
      :owner(std::move(token)),allocation(at),source(input),raster(art),
       backend(&output),assets(output.assets),font(output.font),allocationCurrent(std::move(live)) {
    display=source;display.user=this;display.draw.user=this;
    display.cursorLookup=[](void* p,uint32_t menu,uint32_t type,std::optional<uint32_t>& out) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.cursorLookup && c.source.cursorLookup(c.source.user,menu,type,out);});
    };
    display.draw.camera=[](void* p,uint32_t id) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.camera && c.source.draw.camera(c.source.draw.user,id);});
    };
    display.draw.center=[](void* p) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.center?c.source.draw.center(c.source.draw.user):std::optional<std::array<float,3>>{};});
    };
    display.draw.offset=[](void* p,float angle) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.offset?c.source.draw.offset(c.source.draw.user,angle):std::optional<std::array<float,2>>{};});
    };
    display.draw.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float from,float to) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.interpolate?c.source.draw.interpolate(c.source.draw.user,mode,step,duration,from,to):std::optional<float>{};});
    };
    display.draw.translation=[](void* p,const std::array<float,3>& xyz) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.translation?c.source.draw.translation(c.source.draw.user,xyz):std::optional<BattleActMatrix>{};});
    };
    display.draw.rotation=[](void* p,float angle,uint32_t axis) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.rotation?c.source.draw.rotation(c.source.draw.user,angle,axis):std::optional<BattleActMatrix>{};});
    };
    display.draw.scale=[](void* p,float x,float y,float z) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.scale?c.source.draw.scale(c.source.draw.user,x,y,z):std::optional<BattleActMatrix>{};});
    };
    display.draw.concat=[](void* p,const BattleActMatrix& left,const BattleActMatrix& right) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.concat?c.source.draw.concat(c.source.draw.user,left,right):std::optional<BattleActMatrix>{};});
    };
    display.draw.partyButton=[](void* p) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.partyButton && c.source.draw.partyButton(c.source.draw.user);});
    };
    display.draw.pinchMark=[](void* p) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.source.draw.pinchMark && c.source.draw.pinchMark(c.source.draw.user);});
    };
    icons.raster=raster;icons.raster.user=this;
    icons.raster.request=[](void* p,const BattleClassRasterRequest& request) {
      auto& c=*static_cast<State*>(p);
      return c.call([&] {
        if(request.function==BattleClassRasterService::Camera)
          return c.raster.request?c.raster.request(c.raster.user,request):std::optional<uint32_t>{};
        return c.backend->request(request);
      });
    };
    icons.raster.identity=[](void* p) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.raster.identity?c.raster.identity(c.raster.user):std::optional<BattleActMatrix>{};});
    };
    icons.raster.truncate=[](void* p,float value) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.raster.truncate?c.raster.truncate(c.raster.user,value):std::optional<int32_t>{};});
    };
    // Raster math can have its own context; each SDK operation still crosses
    // the allocation-current boundary before/after that provider executes.
    icons.raster.math=raster.math;
    icons.raster.math.user=this;
    // DrawMainMenu and raster SDK providers can deliberately be distinct.
    icons.raster.math.translation=[](void* p,const std::array<float,3>& xyz) {
      auto& c=*static_cast<State*>(p);const auto& m=c.raster.math;
      return c.call([&]{return m.translation?m.translation(m.user,xyz):std::optional<BattleActMatrix>{};});
    };
    icons.raster.math.rotation=[](void* p,float angle,uint32_t axis) {
      auto& c=*static_cast<State*>(p);const auto& m=c.raster.math;
      return c.call([&]{return m.rotation?m.rotation(m.user,angle,axis):std::optional<BattleActMatrix>{};});
    };
    icons.raster.math.scale=[](void* p,float x,float y,float z) {
      auto& c=*static_cast<State*>(p);const auto& m=c.raster.math;
      return c.call([&]{return m.scale?m.scale(m.user,x,y,z):std::optional<BattleActMatrix>{};});
    };
    icons.raster.math.concat=[](void* p,const BattleActMatrix& left,const BattleActMatrix& right) {
      auto& c=*static_cast<State*>(p);const auto& m=c.raster.math;
      return c.call([&]{return m.concat?m.concat(m.user,left,right):std::optional<BattleActMatrix>{};});
    };
    icons.fontUser=this;
    icons.retainedFontString=[](void* p,const BattleClassRasterRequest& request) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return c.backend->request(request);});
    };
    display.draw.icons=[](void* p,uint32_t work,uint32_t rows,const std::vector<BattleActDrawSlot>& slots) {
      auto& c=*static_cast<State*>(p);
      return c.call([&]{return work==c.allocation && resumeBattleClassObservableDraw(work,rows,slots,c.icons,c.iconCursor);});
    };
  }
};
BattleClassFramePipeline::BattleClassFramePipeline()=default;
BattleClassFramePipeline::~BattleClassFramePipeline()=default;
bool BattleClassFramePipeline::bind(std::shared_ptr<const uint8_t> owner,uint32_t allocation,
    const BattleClassDisplayServices& display,const BattleClassRasterServices& raster,
    BattleClassBackend& backend,std::function<bool()> current) {
  if(invoking_ || state_ || !owner || !allocation || !current || !current() ||
     !backend.assets || !backend.assets->ready() || !backend.font ||
     backend.font->glyphs.empty() || !backend.label || !backend.random)return false;
  state_=std::make_unique<State>(owner,allocation,display,raster,backend,std::move(current));
  publication_.bind(std::move(owner),allocation);return true;
}
bool BattleClassFramePipeline::active() const{return state_ && state_->active && !retirePending_;}
const BattleClassPublication& BattleClassFramePipeline::publication() const {
  return state_ && !retirePending_ && !state_->invalidated && state_->current()?
      publication_:unavailable_;
}
bool BattleClassFramePipeline::admit(const BattleClassPublicationKey& key) {
  return state_ && !retirePending_ && !state_->invalidated && state_->current() &&
      publication_.admit(key);
}
std::optional<BattleClassPublicationKey> BattleClassFramePipeline::consumeAdmission() {
  if(!state_ || retirePending_ || state_->invalidated || !state_->current())return {};
  return publication_.consumeAdmission();
}
void BattleClassFramePipeline::retire(){
  publication_.retire();
  if(invoking_) {
    retirePending_=true;
    if(state_)state_->invalidated=true;
  } else {state_.reset();retirePending_=false;}
}
bool BattleClassFramePipeline::resume(const BattleClassPublicationKey& key,uint32_t camera,uint32_t battle) {
  if(invoking_ || !state_)return false;
  invoking_=true;
  struct Guard {
    BattleClassFramePipeline& owner;
    ~Guard() {
      owner.invoking_=false;
      if(owner.retirePending_) {owner.state_.reset();owner.retirePending_=false;}
    }
  } guard{*this};
  auto& c=*state_;
  if(!key.owner || key.owner!=c.owner || key.allocation!=c.allocation || !key.sequence ||
     c.invalidated || !c.current())return false;
  if(c.active) {
    if(key.sequence!=c.key.sequence || camera!=c.camera || battle!=c.battle)return false;
  } else {
    if(key.sequence<c.completed)return false;
    if(key.sequence==c.completed)return camera==c.camera && battle==c.battle;
    c.key=key;c.camera=camera;c.battle=battle;c.sourceComplete=false;c.active=true;
    c.backend->begin();
  }
  if(!c.sourceComplete) {
    if(!resumePackedBattleClassQueuedDisplay(c.camera,c.battle,c.display,c.queued))return false;
    c.sourceComplete=true;
  }
  if(!c.current() || c.invalidated || !c.backend->complete() || !publication_.publish(key,*c.backend))return false;
  c.completed=key.sequence;c.active=false;return true;
}
}
