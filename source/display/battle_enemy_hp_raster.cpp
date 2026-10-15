#include "battle_enemy_hp_raster.hpp"
#include <cstring>
namespace ttyd {
namespace {
using F=BattleTexturePlaneCall;using Q=BattleTexturePlaneRequest;
Q call(uint32_t fn,std::vector<uint32_t> args={}){Q q;q.function=F(fn);q.integers=std::move(args);return q;}
const std::vector<Q> rastaInit{
  call(0x802b5c8c,{0}),call(0x802b940c,{1}),call(0x802b8d60,{7,0,0,7,0}),
  call(0x802b932c,{1,4,5,5}),call(0x802b93d8,{1,7,0}),call(0x802b4520),
  call(0x802b6f3c,{1}),call(0x802b6f78,{4,0,1,1,0,2,2}),
  call(0x802b3c08,{9,1}),call(0x802b3c08,{11,1}),
  call(0x802b4558,{0,9,1,4,0}),call(0x802b4558,{0,11,1,5,0}),
  call(0x802b5084,{0}),call(0x802b8fcc,{1}),
  call(0x802b89e8,{0,0,0,0,1,0}),call(0x802b8960,{0,15,15,15,10}),
  call(0x802b8a50,{0,0,0,0,1,0}),call(0x802b89a4,{0,7,7,7,5}),
  call(0x802b8e30,{0,255,255,4})};
float real(const BattleTexturePlaneServices& s,uint32_t address){
  const auto n=s.actors.memory.word(s.actors.memory.user,address);float f;std::memcpy(&f,&n,4);return f;
}
bool same(const BattleTexturePlaneServices& a,const BattleTexturePlaneServices& b){
  const auto& x=a.actors.memory;const auto& y=b.actors.memory;
  return a.user==b.user && a.request==b.request && a.matrix==b.matrix &&
    a.actors.user==b.actors.user && a.actors.byte==b.actors.byte &&
    x.user==y.user && x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
    x.storeHalf==y.storeHalf && x.storeByte==y.storeByte && x.flushBlock==y.flushBlock && x.systemCall==y.systemCall;
}
bool valid(const BattleEnemyHpRasterRequest& q){
  switch(q.function){
  case 0x80103b48:return q.integers.empty() && q.reals.empty() && !q.matrix;
  case 0x80103918:return q.integers.size()==4 && q.reals.size()==4 && !q.matrix;
  case 0x801032c8:return q.integers.size()==3 && q.reals.size()==5 && !q.matrix;
  case 0x802ba2fc:return q.integers==std::vector<uint32_t>{0} && q.reals.empty() && bool(q.matrix);
  default:return false;
  }
}
}
struct BattleEnemyHpRasterCursor::State {
  const BattleEnemyHpRasterRequest* borrowed=nullptr;
  BattleEnemyHpRasterRequest input;
  BattleTexturePlaneServices owner;
  unsigned phase=0,index=0;bool held=false,mathHeld=false;
  uint32_t camera=0;
  Q request;BattleAudienceMatrixRequest math;
  BattleActMatrix scale{},translation{},local{},view{};
  std::array<uint32_t,4> colors{};
  BattleTexturePlaneCursor child;
};
BattleEnemyHpRasterCursor::BattleEnemyHpRasterCursor()=default;
BattleEnemyHpRasterCursor::~BattleEnemyHpRasterCursor()=default;
bool BattleEnemyHpRasterCursor::active()const{return bool(state_);}
uint32_t BattleEnemyHpRasterCursor::pending()const{
  if(!state_)return 0;
  const auto& c=*state_;
  return c.mathHeld?c.math.function:c.held?
      (c.request.kind==BattleTexturePlaneKind::Call?uint32_t(c.request.function):0xcc008000):c.input.function;
}
void BattleEnemyHpRasterCursor::reset(){state_.reset();}
bool resumeBattleEnemyHpRaster(const BattleEnemyHpRasterRequest& input,
    const BattleTexturePlaneServices& s,BattleEnemyHpRasterCursor& cursor){
  if(cursor.state_){if(cursor.state_->borrowed!=&input || !same(cursor.state_->owner,s))return false;}
  else {
    if(!valid(input))return false;
    if((input.function==0x801032c8 || input.function==0x80103918) && !s.actors.memory.word)return false;
    cursor.state_=std::make_unique<BattleEnemyHpRasterCursor::State>();
    cursor.state_->borrowed=&input;cursor.state_->input=input;cursor.state_->owner=s;
  }
  auto& c=*cursor.state_;const auto& q=c.input;
  const auto prepare=[&](Q next){if(!c.held){c.request=std::move(next);c.held=true;}};
  const auto ack=[&]()->std::optional<uint32_t>{
    if(!s.request)return {};
    auto result=s.request(s.user,c.request);if(result)c.held=false;return result;
  };
  const auto math=[&](uint32_t function,const BattleActMatrix& left,const BattleActMatrix& right,
                      const std::array<float,3>& xyz,BattleActMatrix& out){
    if(!c.mathHeld){c.math={};c.math.function=function;c.math.left=left;c.math.right=right;c.math.xyz=xyz;c.mathHeld=true;}
    if(!s.matrix || !s.matrix(s.user,c.math))return false;
    out=c.math.output;c.mathHeld=false;return true;
  };
  if(q.function==0x80103b48){
    while(c.index<rastaInit.size()){
      prepare(rastaInit[c.index]);if(!ack())return false;
      ++c.index;
    }
    cursor.reset();return true;
  }
  if(q.function==0x802ba2fc){
    Q next=call(q.function,q.integers);BattleActMatrix m{};
    for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)m[4*row+col]=(*q.matrix)[row][col];
    next.matrix=m;prepare(std::move(next));if(!ack())return false;cursor.reset();return true;
  }
  if(q.function==0x80103918){
    if(c.phase==0){prepare(call(0x802b5a6c,{128,0,4}));if(!ack())return false;c.phase=1;}
    while(c.index<28){
      if(!c.held){Q next;const auto vertex=c.index/7,lane=c.index%7;
        if(lane<3){next.kind=BattleTexturePlaneKind::FifoFloat;
          next.reals={lane==0?q.reals[(vertex==0 || vertex==3)?0:2]:lane==1?q.reals[vertex<2?1:3]:real(s,0x80422240)};
        }else {next.kind=BattleTexturePlaneKind::FifoByte;next.integers={uint8_t(q.integers[lane-3])};}
        prepare(std::move(next));
      }
      if(!ack())return false;
      ++c.index;
    }
    cursor.reset();return true;
  }
  // TexPlane801032c8 captures five scalar arguments at entry. Its original
  // borrowed color pointer is dereferenced only after GXLoadPosMtxImm returns.
  for(;;)switch(c.phase){
  case 0:if(!resumeBattleTexturePlaneInit(s,c.child))return false;c.phase=1;break;
  case 1:{prepare(call(0x8000f2e4));auto result=ack();if(!result)return false;c.camera=*result;c.phase=2;break;}
  case 2:
    if(!c.mathHeld){c.math={};c.math.function=0x8029e2b0;c.math.xyz={q.reals[3],q.reals[4],real(s,0x8042224c)};c.mathHeld=true;}
    if(!s.matrix || !s.matrix(s.user,c.math))return false;
    c.scale=c.math.output;c.mathHeld=false;c.phase=3;break;
  case 3:if(!math(0x8029e230,{},{},{q.reals[0],q.reals[1],q.reals[2]},c.translation))return false;c.phase=4;break;
  case 4:if(!math(0x8029dd64,c.translation,c.scale,{},c.local))return false;c.phase=5;break;
  case 5:
    if(!c.mathHeld){for(unsigned i=0;i<12;++i)c.view[i]=real(s,c.camera+0x11c+4*i);}
    if(!math(0x8029dd64,c.view,c.local,{},c.local))return false;
    c.phase=6;break;
  case 6:{Q next=call(0x802ba2fc,{0});next.matrix=c.local;prepare(std::move(next));if(!ack())return false;c.phase=7;break;}
  case 7:c.colors.fill(c.borrowed->integers[1]);c.phase=8;break;
  case 8:if(!resumePackedBattleTexturePlainGX(q.integers[0],c.colors,s,c.child))return false;cursor.reset();return true;
  default:return false;
  }
}
}
