#include "battle_class_surface_packets.hpp"
#include <cstring>
namespace ttyd {namespace {
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
bool same(const BattleClassRasterRequest&a,const BattleClassRasterRequest&b){return a.function==b.function&&a.integers==b.integers&&a.reals.size()==b.reals.size()&&(a.reals.empty()||!std::memcmp(a.reals.data(),b.reals.data(),a.reals.size()*sizeof(float)))&&bool(a.matrix)==bool(b.matrix)&&(!a.matrix||!std::memcmp(a.matrix->data(),b.matrix->data(),sizeof(BattleActMatrix)));}
}
struct BattleClassSurfaceRaster::State {
 BattleSubmenuSourceServices source;BattleClassSurfacePacketProvider&provider;
 BattleSubmenuSourceCursor init,quad;
 std::optional<BattleClassRasterRequest>call;
 unsigned lane=0;std::optional<float>z;uint32_t pending=0;bool poisoned=false;
 State(BattleSubmenuSourceServices s,BattleClassSurfacePacketProvider&p):source(std::move(s)),provider(p){}
 bool current(){if(poisoned)return false;if(!source.lifetime||!source.current||!source.current(source.user)){poisoned=true;return false;}return true;}
};
BattleClassSurfaceRaster::BattleClassSurfaceRaster(BattleSubmenuSourceServices s,BattleClassSurfacePacketProvider&p):state_(new State(std::move(s),p)){}
BattleClassSurfaceRaster::~BattleClassSurfaceRaster()=default;
uint32_t BattleClassSurfaceRaster::pending()const{return state_->pending;}
std::optional<uint32_t>BattleClassSurfaceRaster::request(const BattleClassRasterRequest&q){
 if(invoking_)return {};
 Running running(invoking_);auto&s=*state_;if(!s.current())return {};
 using F=BattleClassRasterService;using T=BattleTexturePlaneCall;using K=BattleTexturePlaneKind;
 switch(q.function){
 case F::Init2D:case F::Quad:case F::Point:case F::MaterialColor:case F::TevStages:case F::ColorOp:case F::ColorIn:case F::AlphaOp:case F::AlphaIn:case F::Order:case F::LoadMatrix:case F::TextureObject:case F::LoadTexture:case F::Begin:break;
 default:return {};
 }
 if(s.call){if(!same(*s.call,q))return {};}else{s.call=q;s.lane=0;s.z.reset();}
 const auto&c=*s.call;s.pending=uint32_t(c.function);
 auto finish=[&](std::optional<uint32_t>v)->std::optional<uint32_t>{if(!s.current()||!v)return {};s.call.reset();s.pending=0;return v;};
 if(c.function==F::Init2D){if(!c.integers.empty()||!c.reals.empty()||c.matrix)return {};
  // Actual22 calls, including source Rasta, not a synthetic state-ready bit.
  if(!resumeBattleSubmenuInit(s.source,s.init)){s.pending=s.init.pending();return {};}
  return finish(0);
 }
 if(c.function==F::Quad){if(c.integers.size()!=4||c.reals.size()!=4||c.matrix)return {};
  const std::array<float,4>xy{c.reals[0],c.reals[1],c.reals[2],c.reals[3]};const std::array<uint32_t,4>rgba{c.integers[0],c.integers[1],c.integers[2],c.integers[3]};
  if(!resumeBattleSubmenuQuad(xy,rgba,s.source,s.quad)){s.pending=s.quad.pending();return {};}
  return finish(0);
 }
 if(c.function==F::Point){if(c.integers.size()!=4||c.reals.size()!=4||c.matrix||!s.source.word||!s.source.request)return {};
  // Whole Vertex801038e8/30: X,Y FIFO first; fresh source Z at80422240;
  // Z, four low-byte color stores, then U,V. No source read is replayed.
  while(s.lane<9){
   if(s.lane==2&&!s.z){s.pending=0x80422240;const auto v=s.source.word(s.source.user,s.pending);if(!s.current()||!v)return {};float z;std::memcpy(&z,&*v,4);s.z=z;}
   BattleTexturePlaneRequest r;
   if(s.lane>=3&&s.lane<=6){r.kind=K::FifoByte;r.integers={c.integers[s.lane-3]&255};}
   else{r.kind=K::FifoFloat;r.reals={s.lane==0?c.reals[0]:s.lane==1?c.reals[1]:s.lane==2?*s.z:c.reals[s.lane-5]};}
   s.pending=0xcc008000;const auto v=s.source.request(s.source.user,r);if(!s.current()||!v)return {};++s.lane;
  }
  return finish(0);
 }
 BattleTexturePlaneRequest r;r.function=static_cast<T>(uint32_t(c.function));r.integers=c.integers;r.reals=c.reals;r.matrix=c.matrix;
 return finish(s.provider.request(r));
}
}
