#include "battle_class_draw_continuation.hpp"
#include "battle_act_icon_continuation.hpp"
namespace ttyd {
namespace {
bool same(const EventMemoryServices& a,const EventMemoryServices& b) {
  return a.user==b.user && a.word==b.word && a.half==b.half &&
      a.storeWord==b.storeWord && a.storeHalf==b.storeHalf &&
      a.storeByte==b.storeByte && a.flushBlock==b.flushBlock &&
      a.systemCall==b.systemCall;
}
bool same(const BattleActorServices& a,const BattleActorServices& b) {
  return same(a.memory,b.memory) && a.user==b.user && a.byte==b.byte;
}
bool same(const BattleActDrawServices& a,const BattleActDrawServices& b) {
  return same(a.memory,b.memory) && a.user==b.user && a.camera==b.camera &&
      a.center==b.center && a.offset==b.offset && a.interpolate==b.interpolate &&
      a.translation==b.translation && a.rotation==b.rotation && a.scale==b.scale &&
      a.concat==b.concat && a.icons==b.icons && a.partyButton==b.partyButton &&
      a.pinchMark==b.pinchMark;
}
bool same(const BattleClassDrawContinuationServices& a,
          const BattleClassDrawContinuationServices& b) {
  return same(a.raster.memory,b.raster.memory) &&
      same(a.raster.math,b.raster.math) && a.raster.user==b.raster.user &&
      a.raster.request==b.raster.request && a.raster.truncate==b.raster.truncate &&
      a.raster.identity==b.raster.identity && a.fontUser==b.fontUser &&
      a.retainedFontString==b.retainedFontString;
}
}
struct BattleClassDrawContinuationCursor::State {
  BattleClassDrawContinuationServices owner;
  BattleClassRasterServices raster;
  BattleActIconDrawServices icons;
  BattleActIconDrawCursor outer;
  BattleClassRasterCursor bubble,circle;
  const std::vector<BattleActDrawSlot>* slots=nullptr;
  uint32_t work=0,rows=0;
  explicit State(const BattleClassDrawContinuationServices& services)
      :owner(services),raster(services.raster) {
    raster.user=this;
    raster.request=[](void* p,const BattleClassRasterRequest& request) {
      auto& c=*static_cast<State*>(p);
      if(request.function==BattleClassRasterService::FontString)
        return c.owner.retainedFontString?
            c.owner.retainedFontString(c.owner.fontUser,request):
            std::optional<uint32_t>{};
      const auto& s=c.owner.raster;
      return s.request?s.request(s.user,request):std::optional<uint32_t>{};
    };
    raster.truncate=[](void* p,float value) {
      const auto& s=static_cast<State*>(p)->owner.raster;
      return s.truncate?s.truncate(s.user,value):std::optional<int32_t>{};
    };
    raster.identity=[](void* p) {
      const auto& s=static_cast<State*>(p)->owner.raster;
      return s.identity?s.identity(s.user):std::optional<BattleActMatrix>{};
    };
    // Preserve source memory/math contexts, which can be independent owners.
    icons.memory=services.raster.memory;icons.user=this;
    icons.bubble=[](void* p,uint32_t work,uint32_t row,
                     const BattleActMatrix& base,float parameter) {
      auto& c=*static_cast<State*>(p);
      return resumePackedBattleClassBubble(work,row,base,parameter,c.raster,c.bubble);
    };
    icons.circle=[](void* p,uint32_t,uint32_t row,const BattleActMatrix& base,
                     const std::array<float,3>& position,float parameter) {
      auto& c=*static_cast<State*>(p);
      return resumePackedBattleClassCircle(row,base,position,parameter,c.raster,c.circle);
    };
  }
};
BattleClassDrawContinuationCursor::BattleClassDrawContinuationCursor()=default;
BattleClassDrawContinuationCursor::~BattleClassDrawContinuationCursor()=default;
bool BattleClassDrawContinuationCursor::active() const{return bool(state_);}
void BattleClassDrawContinuationCursor::reset(){state_.reset();}
bool resumeBattleClassDrawIcons(uint32_t work,uint32_t rows,
    const std::vector<BattleActDrawSlot>& slots,
    const BattleClassDrawContinuationServices& s,
    BattleClassDrawContinuationCursor& cursor) {
  if(cursor.state_) {
    const auto& c=*cursor.state_;
    if(c.work!=work || c.rows!=rows || c.slots!=&slots || !same(c.owner,s))return false;
  } else {
    cursor.state_=std::make_unique<BattleClassDrawContinuationCursor::State>(s);
    cursor.state_->work=work;cursor.state_->rows=rows;cursor.state_->slots=&slots;
  }
  auto& c=*cursor.state_;
  if(!resumePackedBattleActIcons(work,rows,slots,c.icons,c.outer))return false;
  cursor.reset();return true;
}
}
