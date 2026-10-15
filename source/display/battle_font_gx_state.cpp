#include "battle_font_gx_state.hpp"
#include <cmath>
#include <algorithm>
namespace ttyd {namespace {
enum Bits:uint32_t {Indirect=1u<<0,Cull=1u<<1,ZComp=1u<<2,Alpha=1u<<3,Blend=1u<<4,ZMode=1u<<5,
 Clear=1u<<6,Channel=1u<<7,Channels=1u<<8,TexGens=1u<<9,Stages=1u<<10,Order=1u<<11,
 ColorOp=1u<<12,AlphaOp=1u<<13,ColorIn=1u<<14,AlphaIn=1u<<15,Texture=1u<<16,TexCoord=1u<<17};
constexpr uint32_t All=(1u<<18)-1;
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
bool finite(const BattleActMatrix&m){for(float f:m)if(!std::isfinite(f))return false;return true;}
bool ordinaryTextureMatrix(const BattleActMatrix&m){
 const BattleActMatrix actual{1.f/512,0,0,0,0,1.f/256,0,0,0,0,1,0};
 return m==actual; // Both signed zeros have identical effect in this finite scale.
}
}
bool BattleFontGxState::bind(BattleFontGxBinding b){
 if(bound_||invoking_||!b.sink||!b.font.current()||!b.resources||!b.sourceCurrent||!b.publicationCurrent||
 !b.staticTextureCurrent||!b.sourceCurrent()||!b.publicationCurrent())return false;
 binding_=std::move(b);bound_=true;stale_=false;state_={};return true;
}
bool BattleFontGxState::source()const{
 if(!bound_||stale_)return false;
 if(!binding_.font.current()||!binding_.publicationCurrent()||!binding_.sourceCurrent()){stale_=true;return false;}
 return true;
}
bool BattleFontGxState::texture()const{
 if(!source())return false;
 const bool current=binding_.staticTextureCurrent(0x8041e97c,binding_.font);
 if(!current&&(state_.fixedState&Texture))stale_=true;
 return source()&&current;
}
bool BattleFontGxState::load(const BattleActMatrix&m,uint32_t slot){
 if(slot||!finite(m)||!source())return false;
 if(!binding_.sink->loadMatrix(m,slot)||!source())return false;
 state_.positionMatrix=m;return true;
}
bool BattleFontGxState::color(uint32_t slot,uint32_t rgba){
 if(slot!=1||!source())return false;
 if(!binding_.sink->tevColor(slot,rgba)||!source())return false;
 state_.color=rgba;return true;
}
bool BattleFontGxState::loadMatrix(const BattleActMatrix&m,uint32_t slot){
 if(invoking_)return false;
 Running run(invoking_);return load(m,slot);
}
bool BattleFontGxState::tevColor(uint32_t slot,uint32_t rgba){
 if(invoking_)return false;
 Running run(invoking_);return color(slot,rgba);
}
bool BattleFontGxState::request(const SourceFontStateCall&q){
 if(invoking_||!source())return false;
 Running run(invoking_);
 auto integers=[&](std::initializer_list<uint32_t> a){return q.integers.size()==a.size()&&std::equal(a.begin(),a.end(),q.integers.begin());};
 auto args=[&](std::initializer_list<uint32_t> a){return integers(a)&&!q.color&&!q.matrix;};
 auto reject=[&](){state_.unsupported=true;return false;};
 uint32_t bit=0;
 switch(q.function){
 case 0x802b8880:if(!args({0}))return reject();state_={};bit=Indirect;break;
 case 0x802b5c8c:if(!args({0}))return reject();bit=Cull;break;
 case 0x802b940c:if(!args({1}))return reject();bit=ZComp;break;
 case 0x802b8d60:if(!args({7,0,0,7,0}))return reject();bit=Alpha;break;
 case 0x802b932c:if(!args({1,4,5,7}))return reject();bit=Blend;break;
 case 0x802b93d8:if(!args({1,7,0}))return reject();bit=ZMode;break;
 case 0x802b4520:if(!args({}))return reject();state_.descriptors=0;bit=Clear;break;
 case 0x802b3c08:{
  const uint32_t attributes[]{9,11,13,14};bool found=false;
  for(unsigned i=0;i<4;++i)if(args({attributes[i],1})){state_.descriptors|=1u<<i;found=true;break;}
  if(!found)return reject();
  return source();
 }
 case 0x802b4558:{
  const uint32_t attributes[]{9,11,13,14},component[]{1,1,1,1},types[]{3,5,2,0};bool found=false;
  for(unsigned i=0;i<4;++i)if(args({0,attributes[i],component[i],types[i],0})){state_.formats|=1u<<i;found=true;break;}
  if(!found)return reject();
  return source();
 }
 case 0x802b6f78:if(!args({4,0,1,1,0,0,2}))return reject();bit=Channel;break;
 case 0x802b6f3c:if(!args({1}))return reject();bit=Channels;break;
 case 0x802b5084:if(!args({1}))return reject();bit=TexGens;break;
 case 0x802b8fcc:if(!args({1}))return reject();bit=Stages;break;
 case 0x802b8e30:if(!args({0,0,0,255}))return reject();bit=Order;break;
 case 0x802b89e8:if(!args({0,0,0,0,1,0}))return reject();bit=ColorOp;break;
 case 0x802b8a50:if(!args({0,0,0,0,1,0}))return reject();bit=AlphaOp;break;
 case 0x802b8960:if(!args({0,15,2,12,15}))return reject();bit=ColorIn;break;
 case 0x802b89a4:if(!args({0,7,1,4,7}))return reject();bit=AlphaIn;break;
 case 0x802b7880:if(!args({0x8041e97c,0}))return reject();if(!texture())return false;bit=Texture;break;
 case 0x802ba3d0:
  if(!integers({30,1})||q.color||!q.matrix||!ordinaryTextureMatrix(*q.matrix))return reject();
  state_.textureMatrix=q.matrix;return source();
 case 0x802b4e04:if(!args({0,1,4,30,0,125}))return reject();bit=TexCoord;break;
 case 0x802ba2fc:
  if(!integers({0})||q.color||!q.matrix)return reject();
  return load(*q.matrix,0);
 case 0x802b8ab8:
  if(!integers({1})||!q.color||q.matrix)return reject();
  return color(1,*q.color);
 case 0x802ba39c:
  if(!args({0}))return reject();
  state_.currentMatrix=0;return source();
 default:return reject();
 }
 state_.fixedState|=bit;return source();
}
bool BattleFontGxState::fog(const SourceFontDrawStartFog&q){
 if(invoking_||!source())return false;
 Running run(invoking_);
 if(q.function!=0x802b8ff4||q.type){state_.unsupported=true;return false;}
 for(float f:q.reals)if(f!=0){state_.unsupported=true;return false;}
 state_.fog=q;return source();
}
bool BattleFontGxState::ordinaryProfile()const{
 if(invoking_)return false;
 Running run(invoking_);
 return source()&&!state_.unsupported&&state_.fixedState==All&&state_.descriptors==15&&state_.formats==15&&
 state_.textureMatrix&&state_.positionMatrix&&state_.color&&state_.currentMatrix&&*state_.currentMatrix==0&&state_.fog&&texture();
}
std::optional<BattleFontGxSnapshot> BattleFontGxState::snapshot()const{return source()?std::optional<BattleFontGxSnapshot>(state_):std::nullopt;}
void BattleFontGxState::retire(){stale_=true;}
}
