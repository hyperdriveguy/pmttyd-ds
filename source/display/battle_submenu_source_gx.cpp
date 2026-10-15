#include "battle_submenu_packets.hpp"
#include <cstring>
namespace ttyd {
namespace {
using F=BattleTexturePlaneCall;using Q=BattleTexturePlaneRequest;
struct Call{uint32_t site;F function;std::vector<uint32_t> args;};
const std::vector<Call> init{
 {0x80103c4c,F::Cull,{0}},{0x80103c54,F::DepthLocation,{1}},
 {0x80103c6c,F::AlphaCompare,{7,0,0,7,0}},{0x80103c80,F::BlendMode,{1,4,5,5}},
 {0x80103c90,F::DepthMode,{1,7,0}},{0x80103c94,F::ClearDescriptors,{}},
 {0x80103c9c,F::Channels,{1}},{0x80103cbc,F::ChannelControl,{4,0,1,1,0,2,2}},
 {0x80103a2c,F::Descriptor,{9,1}},{0x80103a38,F::Descriptor,{11,1}},{0x80103a44,F::Descriptor,{13,1}},
 {0x80103a5c,F::AttributeFormat,{0,9,1,4,0}},{0x80103a74,F::AttributeFormat,{0,11,1,5,0}},{0x80103a8c,F::AttributeFormat,{0,13,1,4,0}},
 {0x80103a94,F::TextureGenerators,{1}},{0x80103ab0,F::TextureGenerator,{0,1,4,60,0,125}},
 {0x80103ab8,F::TevStages,{1}},{0x80103ad4,F::ColorOp,{0,0,0,0,1,0}},
 {0x80103aec,F::ColorIn,{0,15,10,8,15}},{0x80103b08,F::AlphaOp,{0,0,0,0,1,0}},
 {0x80103b20,F::AlphaIn,{0,7,5,4,7}},{0x80103b34,F::Order,{0,0,0,4}}
};
bool same(const BattleSubmenuSourceServices&a,const BattleSubmenuSourceServices&b){return a.user==b.user&&a.current==b.current&&a.word==b.word&&a.request==b.request&&a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime);}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Guard{bool&b;explicit Guard(bool&v):b(v){b=true;}~Guard(){b=false;}};
}
struct BattleSubmenuSourceCursor::State {
 unsigned kind=0,phase=0,index=0,vertex=0,lane=0;uint32_t pending=0;bool queued=false,poisoned=false;
 BattleSubmenuSourceServices services;std::array<float,4> xy{};std::array<uint32_t,4> rgba{};float u=0,v=0,z=0;Q request;
};
BattleSubmenuSourceCursor::BattleSubmenuSourceCursor()=default;BattleSubmenuSourceCursor::~BattleSubmenuSourceCursor()=default;
bool BattleSubmenuSourceCursor::reset(){if(invoking_)return false;state_.reset();return true;}
uint32_t BattleSubmenuSourceCursor::pending()const{return state_?state_->pending:0;}
bool BattleSubmenuSourceCursor::resume(unsigned kind,const std::array<float,4>&xy,const std::array<uint32_t,4>&rgba,const BattleSubmenuSourceServices&s,BattleSubmenuSourceCursor&cursor){
 if(cursor.invoking_)return false;
 Guard guard(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<State>();cursor.state_->kind=kind;cursor.state_->services=s;cursor.state_->xy=xy;cursor.state_->rgba=rgba;}
 auto&c=*cursor.state_;if(c.kind!=kind||!same(c.services,s)||std::memcmp(c.xy.data(),xy.data(),sizeof(xy))||c.rgba!=rgba)return false;
 auto current=[&](){if(c.poisoned)return false;if(!s.lifetime||!s.current||!s.current(s.user)){c.poisoned=true;return false;}return true;};
 auto read=[&](uint32_t a,float&out){c.pending=a;if(!s.word)return false;auto v=s.word(s.user,a);if(!current()||!v)return false;out=scalar(*v);return true;};
 auto send=[&](Q q){if(!c.queued){c.request=std::move(q);c.queued=true;}c.pending=c.request.kind==BattleTexturePlaneKind::Call?uint32_t(c.request.function):0xcc008000;if(!s.request)return false;auto v=s.request(s.user,c.request);if(!current()||!v)return false;c.queued=false;return true;};
 while(current()){
  if(kind==0){if(c.index==init.size()){cursor.state_.reset();return true;}Q q;q.function=init[c.index].function;q.integers=init[c.index].args;if(!send(q))return false;++c.index;continue;}
  if(c.phase==0){Q q;q.function=F::Begin;q.integers={128,0,4};if(!send(q))return false;c.phase=1;continue;}
  if(c.vertex==4){cursor.state_.reset();return true;}
  if(c.phase==1){
   if(c.vertex==0){if(!read(0x80422240,c.u))return false;c.v=c.u;c.phase=3;continue;}
   if(c.vertex==1){if(!read(0x8042224c,c.u))return false;c.phase=2;continue;}
   if(c.vertex==2){if(!read(0x8042224c,c.u))return false;c.v=c.u;c.phase=3;continue;}
   if(!read(0x80422240,c.u))return false;
   c.phase=2;continue;
  }
  if(c.phase==2){if(!read(c.vertex==1?0x80422240:0x8042224c,c.v))return false;c.phase=3;continue;}
  // Vertex helper reads its fresh Z only AFTER X and Y FIFO stores.
  if(c.phase==3&&c.lane==2){if(!read(0x80422240,c.z))return false;c.phase=4;continue;}
  Q q;if(c.lane>=3&&c.lane<=6){q.kind=BattleTexturePlaneKind::FifoByte;q.integers={c.rgba[c.lane-3]&255};}
  else{q.kind=BattleTexturePlaneKind::FifoFloat;float f=0;if(c.lane==0)f=c.xy[c.vertex==1||c.vertex==2?2:0];else if(c.lane==1)f=c.xy[c.vertex>=2?3:1];else if(c.lane==2)f=c.z;else f=c.lane==7?c.u:c.v;q.reals={f};}
  if(!send(q))return false;
  ++c.lane;if(c.lane==9){c.lane=0;++c.vertex;c.phase=1;}
 }
 return false;
}
bool resumeBattleSubmenuInit(const BattleSubmenuSourceServices&s,BattleSubmenuSourceCursor&c){return BattleSubmenuSourceCursor::resume(0,{},{},s,c);}
bool resumeBattleSubmenuQuad(const std::array<float,4>&xy,const std::array<uint32_t,4>&rgba,const BattleSubmenuSourceServices&s,BattleSubmenuSourceCursor&c){return BattleSubmenuSourceCursor::resume(1,xy,rgba,s,c);}
}
