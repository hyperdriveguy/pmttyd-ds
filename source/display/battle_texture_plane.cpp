#include "battle_texture_plane.hpp"
#include <cstring>
namespace ttyd {
namespace {
using F=BattleTexturePlaneCall;
using Q=BattleTexturePlaneRequest;
Q request(F function,std::vector<uint32_t> integers={}) {
  Q q;q.function=function;q.integers=std::move(integers);return q;
}
const std::vector<Q> init{
  request(F::Cull,{0}),request(F::ClearDescriptors),
  request(F::Descriptor,{9,1}),request(F::Descriptor,{11,1}),request(F::Descriptor,{13,1}),
  request(F::AttributeFormat,{0,9,1,4,0}),request(F::AttributeFormat,{0,11,1,5,0}),
  request(F::AttributeFormat,{0,13,1,4,0}),request(F::Channels,{1}),
  request(F::ChannelControl,{4,0,0,1,0,2,2}),request(F::TextureGenerators,{1}),
  request(F::TextureGenerator,{0,1,4,60,0,125})};
const std::vector<Q> intensity{
  request(F::TevStages,{1}),request(F::ColorOp,{0,0,0,0,1,0}),
  request(F::ColorIn,{0,15,15,15,10}),request(F::AlphaOp,{0,0,0,0,1,0}),
  request(F::AlphaIn,{0,7,5,4,7}),request(F::Order,{0,0,0,4})};
const std::vector<Q> modulate{
  request(F::TevStages,{1}),request(F::ColorOp,{0,0,0,0,1,0}),
  request(F::AlphaOp,{0,0,0,0,1,0}),request(F::Order,{0,0,0,4}),request(F::TevOp,{0,0})};
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool same(const BattleTexturePlaneServices& a,const BattleTexturePlaneServices& b) {
  const auto& x=a.actors.memory;const auto& y=b.actors.memory;
  return a.user==b.user && a.request==b.request && a.matrix==b.matrix &&
      a.actors.user==b.actors.user && a.actors.byte==b.actors.byte &&
      x.user==y.user && x.word==y.word && x.half==y.half &&
      x.storeWord==y.storeWord && x.storeHalf==y.storeHalf && x.storeByte==y.storeByte &&
      x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
enum class Phase {Init,Copy,Camera,View,Concat,LoadMatrix,Color,Palette,
                  Texture,LoadTexture,Format,Material,Width,Height,Begin,Fifo};
}
struct BattleTexturePlaneCursor::State {
  BattleTexturePlaneServices owner;
  const BattleActMatrix* base=nullptr;
  const uint32_t* rgba=nullptr;
  uint32_t texture=0,camera=0,color=0,battle=0,palette=0,handle=0,format=0;
  Phase phase=Phase::Init;
  size_t index=0;
  bool pending=false,mathPending=false;
  Q request;
  BattleAudienceMatrixRequest math;
  BattleActMatrix local{},view{};
  float halfWidth=0,halfHeight=0;
};
BattleTexturePlaneCursor::BattleTexturePlaneCursor()=default;
BattleTexturePlaneCursor::~BattleTexturePlaneCursor()=default;
bool BattleTexturePlaneCursor::active() const{return bool(state_);}
void BattleTexturePlaneCursor::reset(){state_.reset();}
bool resumePackedBattleTexturePlane2(const BattleActMatrix& base,uint32_t texture,
    const uint32_t& rgba,const BattleTexturePlaneServices& s,BattleTexturePlaneCursor& cursor) {
  if(cursor.state_) {
    const auto& c=*cursor.state_;
    if(c.base!=&base || c.rgba!=&rgba || c.texture!=texture || !same(c.owner,s))return false;
  } else {
    if(!s.actors.memory.word)return false;
    cursor.state_=std::make_unique<BattleTexturePlaneCursor::State>();
    cursor.state_->owner=s;cursor.state_->base=&base;cursor.state_->rgba=&rgba;cursor.state_->texture=texture;
  }
  auto& c=*cursor.state_;const auto& m=s.actors.memory;
  const auto word=[&](uint32_t address){return m.word(m.user,address);};
  const auto real=[&](uint32_t address){return scalar(word(address));};
  const auto dimension=[&](uint32_t n) {
    // Exact source unsigned16 insertion, fresh .5 then fresh binary64 bias.
    const float half=real(0x80422234);
    const uint32_t biasHigh=word(0x802ee400),biasLow=word(0x802ee404);
    const uint64_t biasBits=(uint64_t(biasHigh)<<32)|biasLow;
    const uint64_t convertedBits=(uint64_t(0x43300000)<<32)|(n&0xffff);
    double bias,value;std::memcpy(&bias,&biasBits,8);std::memcpy(&value,&convertedBits,8);
    return half*float(value-bias);
  };
  const auto prepare=[&](Q q){if(!c.pending){c.request=std::move(q);c.pending=true;}};
  const auto acknowledge=[&]()->std::optional<uint32_t> {
    if(!s.request)return {};
    const auto value=s.request(s.user,c.request);
    if(value)c.pending=false;
    return value;
  };
  for(;;)switch(c.phase) {
  case Phase::Init:
    if(c.index==init.size()){c.phase=Phase::Copy;c.index=0;break;}
    prepare(init[c.index]);if(!acknowledge())return false;++c.index;break;
  case Phase::Copy:
    if(!c.mathPending){c.math={};c.math.function=0x8029dd30;c.math.left=*c.base;c.mathPending=true;}
    if(!s.matrix || !s.matrix(s.user,c.math))return false;
    c.local=c.math.output;c.mathPending=false;c.phase=Phase::Camera;break;
  case Phase::Camera: {
    prepare(request(F::CurrentCamera));const auto value=acknowledge();if(!value)return false;
    c.camera=*value;c.phase=Phase::View;break;
  }
  case Phase::View:
    for(unsigned i=0;i<12;++i)c.view[i]=real(c.camera+0x11c+4*i);
    c.phase=Phase::Concat;break;
  case Phase::Concat:
    if(!c.mathPending){c.math={};c.math.function=0x8029dd64;c.math.left=c.view;c.math.right=c.local;c.mathPending=true;}
    if(!s.matrix || !s.matrix(s.user,c.math))return false;
    c.local=c.math.output;c.mathPending=false;c.phase=Phase::LoadMatrix;break;
  case Phase::LoadMatrix: {
    auto q=request(F::LoadMatrix,{0});q.matrix=c.local;prepare(std::move(q));
    if(!acknowledge())return false;
    c.phase=Phase::Color;break;
  }
  case Phase::Color:c.color=*c.rgba;c.phase=Phase::Palette;break;
  case Phase::Palette:
    c.battle=currentBattleAddress(s.actors);
    c.palette=word(word(word(c.battle+0x163f8)+0xa0));c.phase=Phase::Texture;break;
  case Phase::Texture: {
    prepare(request(F::TextureObject,{c.palette,c.texture}));const auto value=acknowledge();if(!value)return false;
    c.handle=*value;c.phase=Phase::LoadTexture;break;
  }
  case Phase::LoadTexture:
    prepare(request(F::LoadTexture,{c.handle,0}));if(!acknowledge())return false;c.phase=Phase::Format;break;
  case Phase::Format: {
    prepare(request(F::TextureFormat,{c.handle}));const auto value=acknowledge();if(!value)return false;
    c.format=*value;c.phase=Phase::Material;c.index=0;break;
  }
  case Phase::Material: {
    const auto& sequence=c.format==0?intensity:modulate;
    if(c.index==sequence.size()){c.phase=Phase::Width;break;}
    prepare(sequence[c.index]);if(!acknowledge())return false;++c.index;break;
  }
  case Phase::Width: {
    prepare(request(F::TextureWidth,{c.handle}));const auto value=acknowledge();if(!value)return false;
    c.halfWidth=dimension(*value);c.phase=Phase::Height;break;
  }
  case Phase::Height: {
    prepare(request(F::TextureHeight,{c.handle}));const auto value=acknowledge();if(!value)return false;
    c.halfHeight=dimension(*value);c.phase=Phase::Begin;break;
  }
  case Phase::Begin:
    prepare(request(F::Begin,{128,0,4}));if(!acknowledge())return false;
    c.index=0;c.phase=Phase::Fifo;break;
  case Phase::Fifo: {
    if(c.index==36){cursor.reset();return true;}
    if(!c.pending) {
      Q q;const auto vertex=c.index/9,lane=c.index%9;
      if(lane>=3 && lane<=6){q.kind=BattleTexturePlaneKind::FifoByte;q.integers={uint8_t(c.color>>(24-8*(lane-3)))};}
      else {
        q.kind=BattleTexturePlaneKind::FifoFloat;float value=0;
        if(lane==0)value=(vertex==0 || vertex==3)?-c.halfWidth:c.halfWidth;
        else if(lane==1)value=vertex<2?c.halfHeight:-c.halfHeight;
        else if(lane==2)value=real(0x80422240);
        else if(lane==7)value=real((vertex==1 || vertex==2)?0x8042224c:0x80422240);
        else value=real(vertex>=2?0x8042224c:0x80422240);
        q.reals={value};
      }
      prepare(std::move(q));
    }
    if(!acknowledge())return false;
    ++c.index;break;
  }
  }
}
}
