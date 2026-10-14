#include "field_bgm_envelope.hpp"
#include <cmath>
namespace ttyd {
std::optional<int16_t> fieldBgmDuration(int32_t ms,int32_t rate) {
 if(ms<=0 || rate<=0)return {};
 const float interval=1000.f/float(rate),count=float(ms)/interval;
 if(!std::isfinite(count) || double(count)>=2147483648.0)return {};
 const uint16_t bits=uint16_t(int32_t(count));
 const int32_t value=bits<0x8000?int32_t(bits):int32_t(bits)-65536;
 if(value<=0)return {};
 return int16_t(value);
}
void FieldBgmEnvelope::bind(uint8_t base,int8_t global,bool bypass) {
 *this={};assigned_=true;base_=base;global_=global;bypassGlobal_=bypass;primaryDuration_=primaryRemaining_=1;
}
bool FieldBgmEnvelope::off201(int16_t d) {
 if(!assigned_ || secondary_)return false;
 if(primary_==2)return true;
 if(d<=0)return false;
 primary_=2;primaryDuration_=primaryRemaining_=d;return true;
}
bool FieldBgmEnvelope::mute3800(int16_t d,bool fast) {
 if(!assigned_ || primary_)return false;
 if(secondary_==8)return true;
 if(d<=0)return false;
 secondary_=8;secondaryDuration_=secondaryRemaining_=d;fast_=fast;return true;
}
bool FieldBgmEnvelope::resumeA0(int16_t d,bool fast) {
 if(!assigned_ || primary_)return false;
 if(secondary_==8 && d<=0)return false;
 primaryDuration_=primaryRemaining_=1;
 if(secondary_!=8)return true;
 secondary_=16;secondaryDuration_=d;secondaryRemaining_=int16_t(int32_t(d)*40/100);fast_=fast;return true;
}
std::optional<FieldBgmPublication> FieldBgmEnvelope::main() {
 FieldBgmPublication out;if(!assigned_)return out;
 if((primary_ && primaryDuration_<=0) || (secondary_ && secondaryDuration_<=0))return {};
 const uint8_t primary=primary_?uint8_t(int32_t(base_)*primaryRemaining_/primaryDuration_):base_;
 const uint8_t factor=secondary_?uint8_t(int32_t(secondaryRemaining_)*127/secondaryDuration_):127;
 const uint8_t combined=uint8_t(int32_t(primary)*factor/127);
 out.published=true;out.volume=uint8_t(int32_t(combined)*(bypassGlobal_?100:int32_t(global_))/100);
 if(primary_==2) {
  --primaryRemaining_;if(primaryRemaining_<=0){primary_=0;assigned_=false;out.detach=true;out.tailMilliseconds=100;}
 }
 if(secondary_==8) {
  const int16_t target=int16_t(int32_t(secondaryDuration_)*40/100);
  if(secondaryRemaining_>target) {
   const int32_t next=int32_t(secondaryRemaining_)-(fast_?5:1);
   secondaryRemaining_=int16_t(next<target?target:next);
  }
 } else if(secondary_==16) {
  if(secondaryRemaining_>=secondaryDuration_)secondary_=0;
  else {
   const int32_t next=int32_t(secondaryRemaining_)+(fast_?5:1);
   secondaryRemaining_=int16_t(next>secondaryDuration_?secondaryDuration_:next);
  }
 }
 return out;
}
bool FieldStreamFade::begin(float volume,uint16_t ms,int32_t rate) {
 if(!std::isfinite(volume) || volume<0 || volume>255 || rate<=0)return false;
 const float interval=1000.f/float(rate),count=float(ms)/interval;
 const float denominator=count<1.f?1.f:count;
 volume_=volume;decrement_=volume/denominator;active_=true;return true;
}
FieldStreamFadePublication FieldStreamFade::main(bool gate) {
 FieldStreamFadePublication out;if(!active_ || !gate)return out;
 if(volume_==0){active_=false;out.stop=true;return out;}
 const float next=volume_-decrement_;volume_=next<=0?0:next;
 out.published=true;out.volume=uint8_t(int32_t(volume_));return out;
}
}
