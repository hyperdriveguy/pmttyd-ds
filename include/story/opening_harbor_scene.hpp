#pragma once
#include "field_camera_events.hpp"
#include "opening_harbor_jump.hpp"
#include <array>
#include <cstdint>
namespace ttyd {
// Used US gor_00 arrival branch. Clock is native simulation ticks, not draws.
struct OpeningHarborScene {
 uint64_t now=0,started=0,firstClosed=UINT64_MAX,secondClosed=UINT64_MAX,paperAt=0,departureAt=UINT64_MAX;
 float x=-600,y=0,wakeX=-600,yaw=180,rock=0,amplitude=0,heldAmplitude=0,speed=0;
 OpeningHarborJump jump;
 bool jumpStarted=false;uint64_t jumpAt=0;float work8=0;
 std::array<float,4> parts{};bool rockingDown=false,segmentDone=true;
 bool paperCapturePending=false,paperLive=false,paperDone=false,paperFinalPresented=false;
 bool visible=true,marioVisible=false,storyPublished=false,complete=false;
 CameraPoint eye{0,700,1420},at{0,-10,0};
 void begin(uint64_t tick,float resolvedWork8=0){*this={};now=started=tick;work8=y=resolvedWork8;}
 float paperFrame() const{return paperLive?float(std::min<uint64_t>(now-paperAt,60)):0;}
 bool messageReady(unsigned index) const{return index==0?now-started>=480:index==1 && firstClosed!=UINT64_MAX && now-firstClosed>=120;}
 void messageComplete(unsigned index){if(index==0)firstClosed=now;else if(index==1)secondClosed=now;}
 void captureComplete(){if(paperCapturePending){paperCapturePending=false;paperLive=true;paperAt=now;}}
 void presented(float frame){if(paperLive && frame>=60)paperFinalPresented=true;}
 static float q(float value){return std::trunc(value*1024.f)/1024.f;}
 static float interpolate(unsigned curve,float a,float b,uint64_t elapsed,uint64_t duration){return q(*cameraInterpolation(curve,int32_t(std::min(elapsed,duration)),int32_t(duration),a,b));}
 void tick(){
  ++now;const uint64_t t=now-started;
  if(t<=180){x=-600;amplitude=0;}else if(t<=300){x=interpolate(0,-600,-270,t-180,120);amplitude=5;}else if(t<=420){x=interpolate(4,-270,-75,t-300,120);amplitude=3;}else {x=-75;amplitude=1;}
  if(t>30){const float fraction=*cameraInterpolation(11,int32_t(std::min<uint64_t>(t-30,180)),180,0,1);eye=FieldCameraEvents::blend({0,700,1420},{2,225,1015},fraction);at=FieldCameraEvents::blend({0,-10,0},{2,50,243},fraction);}
  marioVisible=t>=420;
  const bool after=secondClosed!=UINT64_MAX;const uint64_t post=after?now-secondClosed:0;
  if(after && post>=60 && !jumpStarted){jumpStarted=true;jumpAt=now;jump.begin({{0,work8,0}},{{0,int32_t(std::trunc(work8)),0}},1800,100,0x400000a0u);}
  if(jumpStarted){jump.advance(uint32_t((now-jumpAt)*1000/60));work8=y=q(jump.position[1]);}
  const bool paused=jumpStarted && jump.active;
  if(after && post>=84 && !paperLive && !paperDone)paperCapturePending=true;
  if(paperLive && paperFrame()>=60 && paperFinalPresented){paperLive=false;paperDone=true;yaw=0;}
  if(after && post>=288 && departureAt==UINT64_MAX && paperDone)departureAt=now+1;
  const bool departing=departureAt!=UINT64_MAX && now>=departureAt;
  if(departing){const uint64_t d=now-departureAt;amplitude=d<=120?3:5;
   x=d<=120?interpolate(1,-75,-270,d,120):interpolate(0,-270,-1260,d-120,360);
   wakeX=-475;storyPublished=d>=180;visible=d<480;complete=d>=480;
  }else wakeX=x;
  parts[0]=interpolate(11,(t/120)%2?-5:5,(t/120)%2?5:-5,t%120,120);
  if(!paused){const bool slowing=(t>300 && !departing);speed=q(speed+(slowing?-.0693359375f:.099609375f));if(int(speed)>=10)speed=10;if(int(speed)<=0 && slowing)speed=0;
   for(unsigned n=1;n<4;++n){if(parts[n]>360)parts[n]=q(parts[n]-360);if(parts[n]<0)parts[n]=q(parts[n]+360);parts[n]=q(parts[n]+speed);}
   if(segmentDone){heldAmplitude=amplitude;segmentDone=false;rockingDown=!rockingDown;}
   if(heldAmplitude==0){rock=0;segmentDone=true;}else {const float step=(2*heldAmplitude)/120;rock+=rockingDown?-step:step;const float bound=rockingDown?-heldAmplitude:heldAmplitude;if((rockingDown && rock<=bound)||(!rockingDown && rock>=bound)){rock=bound;segmentDone=true;}}
  }
 }
 void advance(uint64_t tick){while(now<tick)tickOnce();}
private:void tickOnce(){tick();}
};
}
