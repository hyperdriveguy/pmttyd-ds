#include "field_coin_get.hpp"
#include "npc_presentation.hpp"
#include <cmath>
namespace ttyd {
namespace {
constexpr const char* Name="iraiCoin";
float sine(float x) {const float u=x*x;return std::fma(std::fma(.00761000020429492f,u,-.1660500019788742f),u,1.f)*x;}
float cosine(float x) {const float u=x*x;return std::fma(std::fma(.037050001323223114f,u,-.4966999888420105f),u,1.f);}
std::optional<bool> elapsed(const FieldCoinClock& now,const FieldCoinClock& saved,int32_t threshold) {
 const uint32_t divisor=(now.hardwareFrequency>>2)/1000;
 if(!divisor)return {}; // Unbound native clock, not an invented timeout.
 return int32_t(uint32_t(now.low-saved.low)/divisor)>threshold;
}
}
FieldCoinPlacement fieldCoinPlacement(ItemGetPoint mario,float heading) {
 const float normalized=npcReviseAngle(std::fma(100.f,heading,5.f)/100.f);
 const float angle=normalized>=360.f?0.f:normalized;
 const float r=(6.2831854820251465f*angle)/360.f;
 constexpr float pi=3.1415927410125732f,h=1.5707963705062866f,t=4.71238899230957f;
 float s,c;
 if(r<=h){s=sine(r);c=cosine(r);}
 else if(r<=pi){const float x=h-(r-h);s=sine(x);c=-cosine(x);}
 else if(r<t){const float x=r-pi;s=-sine(x);c=-cosine(x);}
 else {const float x=h-(r-t);s=-sine(x);c=cosine(x);}
 const float x=mario.x+10.f*s,z=mario.z-10.f*c;
 return {{x,mario.y+62.f,z},{x,mario.y+52.f,z}};
}
bool fieldCoinGetPresentationReady(const FieldCoinGetServices& s) {
 return s.ready && s.mario && s.heading && s.clock && s.effect && s.effectPayload && s.softDelete &&
    s.iconEnter && s.iconFlags && s.iconDelete && s.request && s.window && s.windowClose &&
    s.windowAction && s.addCoins && s.ready(s.user);
}
bool FieldCoinGet::bind(const FieldCoinGetServices& s) {
 if(retained_ || !fieldCoinGetPresentationReady(s))return false;
 services_=s;owner_=this;return true;
}
bool FieldCoinGet::retireDeletedWindow() {
 if(!canAdvance() || !retained_ || state_!=101 || !services_.windowRetained ||
    services_.windowRetained(services_.user,window_))return false;
 retained_=false;
 return true;
}
std::optional<int32_t> FieldCoinGet::invoke(int32_t amount,bool firstCall,uint32_t triggers) {
 if(!canAdvance())return {};
 // Source resolves amount and recomputes placement even on cleanup polls.
 const auto mario=services_.mario(services_.user);
 const float heading=services_.heading(services_.user);
 const auto xyz=fieldCoinPlacement(mario,heading);
 if(firstCall) {
  if(retained_)return {}; // Native finite owner cannot overwrite a borrowed parameter.
  state_=0;effect_=services_.effect(services_.user,7,xyz.effect);
  if(!effect_)return {};
  parameter_={0,0};window_=-1;retained_=true;
 } else if(!retained_)return {};
 switch(state_) {
 case 0:
  services_.iconEnter(services_.user,Name,403,xyz.icon);
  services_.iconFlags(services_.user,Name,0x102,0);
  services_.effectPayload(services_.user,effect_,5);
  services_.request(services_.user,FieldCoinRequest::GetMotion);
  window_=services_.window(services_.user,0x80415e84,parameter_.data());
  parameter_={amount,0};
  services_.request(services_.user,FieldCoinRequest::BgmOff3800);
  services_.request(services_.user,FieldCoinRequest::FanfareOn201);
  saved_=services_.clock(services_.user);state_=1;return 0;
 case 1: {
  const auto ready=elapsed(services_.clock(services_.user),saved_,200);if(!ready)return {};
  if(!*ready)return 0;
  services_.iconFlags(services_.user,Name,0,2);parameter_[1]=1;
  saved_=services_.clock(services_.user);state_=2;return 0;
 }
 case 2: {
  const auto ready=elapsed(services_.clock(services_.user),saved_,500);if(!ready)return {};
  if(*ready)state_=10;
  return 0;
 }
 case 10:if(triggers&0x100)state_=100;return 0;
 case 100:
  services_.addCoins(services_.user,amount);
  services_.softDelete(services_.user,effect_);
  services_.iconDelete(services_.user,Name);
  services_.request(services_.user,FieldCoinRequest::StayMotion);
  services_.request(services_.user,FieldCoinRequest::BgmResumeA0);
  services_.request(services_.user,FieldCoinRequest::BgmOff201);
  services_.windowClose(services_.user,window_);parameter_[1]=0;state_=101;return 2;
 case 101:
  if(services_.windowAction(services_.user,window_)!=0)return 0;
  retained_=false;return 2;
 default:return 2;
 }
}
}
