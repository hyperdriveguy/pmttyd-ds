#pragma once
#include "opening_voyage_callbacks.hpp"
namespace ttyd {
struct OpeningVoyageScene {
 uint64_t started=0,now=0,acceptedTick=UINT64_MAX;
 mutable OpeningVoyageLogo logo;
 float shipX=-2000,shipDelta=0,white=0,yaw=0;
 std::array<float,3> position{{-2000,0,-3000}};
 std::array<float,4> parts{{5,0,0,0}};
 float rotationZ=0;
 bool inputEligible=false,headingRequested=false,headingReady=false;
 mutable bool whitePresented=false;
 void begin(uint64_t tick){*this={};started=now=tick;}
 void presentedWhite(uint8_t alpha) const{if(headingRequested && alpha==255)whitePresented=true;}
 // Catalog8/9 choice freezes its threshold branch at acceptance time once
 // accepted. Before acceptance, native1141-tick threshold applies to live t;
 // do not rewrite both cases as a current-time music switch.
 int music() const{const uint64_t t=now-started;if(t<30)return -1;return (acceptedTick==UINT64_MAX?t>=1141:acceptedTick-started>=1141)?9:8;}
 // Initial source globals(-2000,0,-3000) precede approach-1800->0 after
 // tick240; reset shipDelta at that source reseed. Input eligibility841 is
 // separate from logo resource/state admission. Only final catch-up tick gets
 // the physical edge, preventing replay across every missed simulation step.
 void advance(uint64_t tick,uint32_t keys=0){
  while(now<tick){++now;const uint64_t t=now-started;const float old=shipX;
   if(t==240)logo.begin(now);
   if(t>=240 && acceptedTick==UINT64_MAX)shipX=std::trunc(*cameraInterpolation(11,int32_t(std::min<uint64_t>(t-240,600)),600,-1800,0)*1024.f)/1024.f;
   inputEligible=t>=841;
   // A single physical edge belongs to the current simulation tick only.
   logo.advance(now,now==tick?keys:0,inputEligible);
   if(logo.accepted && acceptedTick==UINT64_MAX)acceptedTick=logo.acceptedTick;
   // Accepted logo input starts departure immediately; heading request at
   // +240 ticks, white fade48, then actual white255 publication AND +288 gate.
   // Logo fade/cleanup is a distinct owner, not heading completion evidence.
   if(acceptedTick!=UINT64_MAX){const uint64_t post=now-acceptedTick;
    shipX=std::trunc(*cameraInterpolation(11,int32_t(std::min<uint64_t>(post,600)),600,0,1800)*1024.f)/1024.f;
    headingRequested=post>=240;if(headingRequested)white=std::min(1.f,float(post-240)/48.f);headingReady=post>=288 && whitePresented;
   }
   shipDelta=t==240?0:std::trunc((shipX-old)*1024.f)/1024.f;
   // Original interpolation and divide100 EACH publish Q1024; wheel/screw
   // angle additions likewise quantize separately. Preserve ordering, including
   // >360-only wrap. Sway starts after90 ticks/1500ms, not on scene creation.
   parts[0]=std::trunc(*cameraInterpolation(11,int32_t(t%120),120,(t/120)%2?-500:500,(t/120)%2?500:-500)*1024.f)/1024.f;
   parts[0]=std::trunc((parts[0]/100.f)*1024.f)/1024.f;
   for(unsigned n=1;n<4;++n){parts[n]=std::trunc((parts[n]+std::trunc((2*shipDelta)*1024.f)/1024.f)*1024.f)/1024.f;if(parts[n]>360)parts[n]=std::trunc((parts[n]-360)*1024.f)/1024.f;}
   // anm_yure starts its first2-second sway after its1500ms sound wait.
   const uint64_t sway=t>=90?t-90:0;
   float theta=t>=90?std::trunc(*cameraInterpolation(11,int32_t(sway%120),120,(sway/120)%2?-500:500,(sway/120)%2?500:-500)*1024.f)/1024.f:0;
   theta=std::trunc((theta/100.f)*1024.f)/1024.f;
   if(t<90){position={{shipX,0,-3000}};rotationZ=0;yaw=0;}else{const auto transform=openingVoyageLocalTransform({{shipX,0,-3000}},theta);position=transform.position;rotationZ=transform.rotationZ;yaw=180;}
  }
 }
};
}
