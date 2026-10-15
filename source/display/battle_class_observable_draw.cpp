#include "battle_class_observable_draw.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
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
namespace {
int32_t integer(uint32_t n){int32_t result;std::memcpy(&result,&n,4);return result;}
bool same(const BattleActDrawSlot& a,const BattleActDrawSlot& b) {
  if(a.matrix.has_value()!=b.matrix.has_value() ||
     a.parameter.has_value()!=b.parameter.has_value() ||
     a.position.has_value()!=b.position.has_value())return false;
  return (!a.matrix || !std::memcmp(a.matrix->data(),b.matrix->data(),48)) &&
      (!a.parameter || !std::memcmp(&*a.parameter,&*b.parameter,4)) &&
      (!a.position || !std::memcmp(a.position->data(),b.position->data(),12));
}
}
struct BattleClassObservableDrawCursor::State {
  BattleClassDrawContinuationServices owner;
  BattleClassRasterServices raster;
  BattleClassDrawContinuationServices adapted;
  BattleClassDrawContinuationCursor exact;
  BattleClassRasterCursor circle,bubble;
  const std::vector<BattleActDrawSlot>* slots=nullptr;
  uint32_t work=0,rows=0,battle=0,actor=0,unit=0,kind=0,cursor=0,count=0;
  std::vector<uint32_t> flags,order;
  std::vector<BattleActDrawSlot> operands;
  size_t index=0;
  bool full=false,unavailable=false,exactComplete=false,invalidated=false;
  explicit State(const BattleClassDrawContinuationServices& s):owner(s),raster(s.raster) {
    raster.user=this;
    raster.request=[](void* p,const BattleClassRasterRequest& request) {
      auto& c=*static_cast<State*>(p);
      if(!c.providerBoundary())return std::optional<uint32_t>{};
      std::optional<uint32_t> result;
      if(request.function==BattleClassRasterService::FontString) {
        if(c.owner.retainedFontString)result=c.owner.retainedFontString(c.owner.fontUser,request);
      } else if(c.owner.raster.request)
        result=c.owner.raster.request(c.owner.raster.user,request);
      return c.providerBoundary()?result:std::optional<uint32_t>{};
    };
    raster.truncate=[](void* p,float value) {
      auto& c=*static_cast<State*>(p);const auto& source=c.owner.raster;
      if(!c.providerBoundary())return std::optional<int32_t>{};
      const auto result=source.truncate?source.truncate(source.user,value):std::optional<int32_t>{};
      return c.providerBoundary()?result:std::optional<int32_t>{};
    };
    raster.identity=[](void* p) {
      auto& c=*static_cast<State*>(p);const auto& source=c.owner.raster;
      if(!c.providerBoundary())return std::optional<BattleActMatrix>{};
      const auto result=source.identity?source.identity(source.user):std::optional<BattleActMatrix>{};
      return c.providerBoundary()?result:std::optional<BattleActMatrix>{};
    };
    raster.math.user=this;
    raster.math.translation=[](void* p,const std::array<float,3>& xyz) {
      auto& c=*static_cast<State*>(p);const auto& m=c.owner.raster.math;
      if(!c.providerBoundary())return std::optional<BattleActMatrix>{};
      const auto result=m.translation?m.translation(m.user,xyz):std::optional<BattleActMatrix>{};
      return c.providerBoundary()?result:std::optional<BattleActMatrix>{};
    };
    raster.math.scale=[](void* p,float x,float y,float z) {
      auto& c=*static_cast<State*>(p);const auto& m=c.owner.raster.math;
      if(!c.providerBoundary())return std::optional<BattleActMatrix>{};
      const auto result=m.scale?m.scale(m.user,x,y,z):std::optional<BattleActMatrix>{};
      return c.providerBoundary()?result:std::optional<BattleActMatrix>{};
    };
    raster.math.rotation=[](void* p,float radians,uint32_t axis) {
      auto& c=*static_cast<State*>(p);const auto& m=c.owner.raster.math;
      if(!c.providerBoundary())return std::optional<BattleActMatrix>{};
      const auto result=m.rotation?m.rotation(m.user,radians,axis):std::optional<BattleActMatrix>{};
      return c.providerBoundary()?result:std::optional<BattleActMatrix>{};
    };
    raster.math.concat=[](void* p,const BattleActMatrix& left,const BattleActMatrix& right) {
      auto& c=*static_cast<State*>(p);const auto& m=c.owner.raster.math;
      if(!c.providerBoundary())return std::optional<BattleActMatrix>{};
      const auto result=m.concat?m.concat(m.user,left,right):std::optional<BattleActMatrix>{};
      return c.providerBoundary()?result:std::optional<BattleActMatrix>{};
    };
    adapted=owner;adapted.raster=raster;adapted.fontUser=this;
    adapted.retainedFontString=[](void* p,const BattleClassRasterRequest& request) {
      auto& c=*static_cast<State*>(p);return c.raster.request(p,request);
    };
  }
  bool providerBoundary() {
    if(invalidated || !valid()){invalidated=true;return false;}
    return true;
  }

  bool valid() const {
    const auto& a=owner.raster.memory;const auto& m=a.memory;
    if(currentBattleAddress(a)!=battle || m.word(m.user,battle+0x420)!=actor ||
       battleRegistryUnit(battle,integer(actor),a)!=unit ||
       m.word(m.user,unit+8)!=kind || m.word(m.user,cursor+8)!=count)return false;
    for(uint32_t i=0;i<count;++i)
      if((m.word(m.user,work+0x20+4*i)&3)!=flags[i])return false;
    if(!full)for(size_t i=0;i<order.size();++i)
      if(order[i]>=slots->size() || !same((*slots)[order[i]],operands[i]))return false;
    return true;
  }
};
BattleClassObservableDrawCursor::BattleClassObservableDrawCursor()=default;
BattleClassObservableDrawCursor::~BattleClassObservableDrawCursor()=default;
bool BattleClassObservableDrawCursor::active() const{return bool(state_);}
void BattleClassObservableDrawCursor::reset(){state_.reset();}
bool resumeBattleClassObservableDraw(uint32_t work,uint32_t rows,
    const std::vector<BattleActDrawSlot>& slots,
    const BattleClassDrawContinuationServices& s,BattleClassObservableDrawCursor& holder) {
  if(holder.state_) {
    const auto& c=*holder.state_;
    if(c.work!=work || c.rows!=rows || c.slots!=&slots || !same(c.owner,s))return false;
  } else {
    if(!s.raster.memory.memory.word)return false;
    holder.state_=std::make_unique<BattleClassObservableDrawCursor::State>(s);
    auto& c=*holder.state_;c.work=work;c.rows=rows;c.slots=&slots;
    const auto& a=s.raster.memory;const auto& m=a.memory;
    c.battle=currentBattleAddress(a);c.actor=m.word(m.user,c.battle+0x420);
    c.unit=battleRegistryUnit(c.battle,integer(c.actor),a);c.kind=m.word(m.user,c.unit+8);
    c.cursor=c.battle+(c.kind==222?0x1bb4:0x1bc0);c.count=m.word(m.user,c.cursor+8);
    if(integer(c.count)<0 || c.count>slots.size()) {c.unavailable=true;return false;}
    c.full=true;
    for(uint32_t i=0;i<c.count;++i) {
      c.flags.push_back(m.word(m.user,work+0x20+4*i)&3);
      if(!slots[i].position)c.full=false;
      if(c.flags.back()&1)c.order.push_back(i);
    }
    if(!c.full) {
      for(const auto row:c.order) {
        const auto& slot=slots[row];
        if(!slot.matrix || !slot.parameter || (!(c.flags[row]&2) && !slot.position))
          c.unavailable=true;
        if(c.order.size()>1 && (!slot.position || !std::isfinite((*slot.position)[1])))
          c.unavailable=true;
      }
      if(c.order.size()>1 && !c.unavailable) {
        for(size_t i=0;i<c.order.size();++i)for(size_t j=i+1;j<c.order.size();++j)
          if((*slots[c.order[i]].position)[1]==(*slots[c.order[j]].position)[1])c.unavailable=true;
        if(!c.unavailable)std::sort(c.order.begin(),c.order.end(),[&](uint32_t x,uint32_t y) {
          return (*slots[x].position)[1]>(*slots[y].position)[1];
        });
      }
      for(const auto row:c.order)c.operands.push_back(slots[row]);
    }
  }
  auto& c=*holder.state_;
  if(c.unavailable || c.invalidated || !c.valid())return false;
  if(c.full) {
    if(!c.exactComplete) {
      if(!resumeBattleClassDrawIcons(work,rows,slots,c.adapted,c.exact))return false;
      c.exactComplete=true;
    }
    if(!c.valid())return false;
  } else {
    while(c.index<c.order.size()) {
      if(!c.valid())return false;
      const auto row=c.order[c.index];const auto& slot=c.operands[c.index];
      const bool ready=c.flags[row]&2?
          resumePackedBattleClassBubble(work,rows+20*row,*slot.matrix,*slot.parameter,c.raster,c.bubble):
          resumePackedBattleClassCircle(rows+20*row,*slot.matrix,*slot.position,*slot.parameter,c.raster,c.circle);
      if(!ready)return false;
      ++c.index; // Advance before revalidation: an acknowledged child never replays.
    }
    if(!c.valid())return false;
  }
  holder.reset();return true;
}
}
