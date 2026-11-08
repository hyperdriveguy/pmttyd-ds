#include "native_harbor_reward_presentation.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Actual initialized DATA operands of original 802149ec/80215088.
constexpr uint32_t SourceScalars[]={0x8042722c,0x80427230,0x80427234,
 0x80427238,0x8042723c,0x80427240,0x80427244,0x804271e0,0x804271c0};
enum Scalar:unsigned {TensOrigin,TensCenter,TensAdvance,TensScale,OnesScale,
 OnesOrigin,OnesAdvance,TensY,Zero};
constexpr int32_t DecimalBase=10,FadeIncrement=64,OpaqueAlpha=255;
constexpr unsigned MaximumTensPhase=16;
struct Guard{bool&invoking;~Guard(){invoking=false;}};
}
bool validNativeHarborRewardArt(const MessageIcon&icon){
 constexpr unsigned EffectNamespace=0x10000;
 constexpr unsigned Widths[]={200,100,480,320,296,148,328,164};
 constexpr unsigned Heights[]={48,24,48,32,48,24,48,24};
 if(icon.id==404){if(icon.loop!=1||icon.frames.size()!=4)return false;}
 else if(icon.id<EffectNamespace+124||icon.id>EffectNamespace+131||icon.loop!=0||icon.frames.size()!=1)return false;
 for(const auto&frame:icon.frames){
  const unsigned index=icon.id==404?0:icon.id-EffectNamespace-124;
  if(frame.width!=(icon.id==404?40:Widths[index])||frame.height!=(icon.id==404?40:Heights[index])||
   frame.delay!=(icon.id==404?6:0)||!std::isfinite(frame.u0)||!std::isfinite(frame.v0)||!std::isfinite(frame.u1)||!std::isfinite(frame.v1)||
   frame.u0<0||frame.u1>1||frame.u0>=frame.u1||frame.v1<0||frame.v0>1||frame.v1>=frame.v0)return false;
 }return true;
}
NativeHarborRewardPresentation::NativeHarborRewardPresentation(NativeHarborRewardPresentationSource s):source_(std::move(s)){}
bool NativeHarborRewardPresentation::current()const{
 if(checking_||!alive_||!source_.lifetime||!source_.current||!source_.word)return false;
 checking_=true;Guard guard{checking_};return source_.current(source_.user)&&alive_;
}
bool NativeHarborRewardPresentation::admit(int32_t total){
 if(invoking_||!current())return false;
 invoking_=true;Guard guard{invoking_};
 if(admitting_&&total_!=total)return false;
 if(admitted_)return total_==total;
 // Original end count is a signed halfword. Native model does not clamp it.
 if(total<0||total>INT16_MAX)return false;
 admitting_=true;total_=total;
 while(loaded_<data_.size()){
  const auto word=source_.word(source_.user,SourceScalars[loaded_]);
  if(!word||!current())return false;
  float value;const uint32_t bits=*word;std::memcpy(&value,&bits,sizeof value);
  if(!std::isfinite(value))return false;
  data_[loaded_++]=value;
 }
 const int32_t tens=total/DecimalBase;
 auto candidate=icons_;
 for(unsigned index=0;index<IconCount;++index){auto&icon=candidate[index];
  const bool large=index<TensSlots;
  icon.hidden=true;icon.alpha=0;icon.animationTick=0;
  icon.scale=data_[large?TensScale:OnesScale];
  icon.position={large?std::fma(data_[TensAdvance],float(index),std::fma(-data_[TensCenter],float(tens),data_[TensOrigin])):
   std::fma(-data_[OnesAdvance],float(index-(TensSlots-1)),data_[OnesOrigin]),
   data_[large?TensY:Zero],data_[Zero]};
  for(float value:icon.position)if(!std::isfinite(value))return false;
 }
 if(!current())return false;
 icons_=candidate;admitted_=true;iconsActive_=total>0;return true;
}
bool NativeHarborRewardPresentation::enterPointGet(){
 if(invoking_||!admitted_||total_<=0||!current())return false;
 if(entered_)return true;
 // Original btlseqEnd0a: effPointGetEntry(0,total,0,90,0,0).
 pointGet_.enter(total_,0,90,0,0);entered_=true;return true;
}
bool NativeHarborRewardPresentation::update(int32_t remaining,uint64_t tick){
 if(invoking_||!admitted_||remaining<0||remaining>total_||!current())return false;
 invoking_=true;Guard guard{invoking_};
 if(hasTick_&&tick==tick_)return remaining==lastRemaining_;
 if(hasTick_&&tick<tick_)return false;
 auto candidate=icons_;auto phase=phase_;auto point=pointGet_;
 const int32_t tens=remaining/DecimalBase,ones=remaining-tens*DecimalBase;
 if(iconsActive_)for(unsigned index=0;index<IconCount;++index){auto&icon=candidate[index];
  const bool large=index<TensSlots;
  const bool shown=large?int32_t(index)<tens:int32_t(index-TensSlots)<ones;
  icon.hidden=!shown;
  if(shown){icon.alpha=uint8_t(std::min(int32_t(icon.alpha)+FadeIncrement,OpaqueAlpha));
   if(large){if(phase[index]<MaximumTensPhase)++phase[index];
    icon.position[0]=std::fma(data_[TensAdvance],float(index),std::fma(-data_[TensCenter],float(tens),data_[TensOrigin]));}}
  else{icon.alpha=0;if(large)phase[index]=0;}
  ++icon.animationTick; // Original hidden icons still animate.
 }
 point.tick();
 if(!current())return false;
 icons_=candidate;phase_=phase;pointGet_=point;tick_=tick;hasTick_=true;lastRemaining_=remaining;return true;
}
bool NativeHarborRewardPresentation::hideIcons(){
 if(invoking_||!admitted_||!current())return false;
 iconsActive_=false;for(auto&icon:icons_){icon.hidden=true;icon.alpha=0;}return true;
}
bool NativeHarborRewardPresentation::softDeletePointGet(){
 if(invoking_||!entered_||!current())return false;
 pointGet_.softDelete=true;return true;
}
bool NativeHarborRewardPresentation::pointGetReady()const{return current()&&entered_&&pointGet_.active&&pointGet_.phase==1;}
bool NativeHarborRewardPresentation::pointGetFinished()const{return current()&&entered_&&!pointGet_.active;}
bool NativeHarborRewardPresentation::retire(){if(invoking_||checking_)return false;alive_=false;iconsActive_=false;source_={};icons_={};pointGet_={};return true;}
}
