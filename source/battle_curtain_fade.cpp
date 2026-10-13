#include "battle_curtain_fade.hpp"
#include <cassert>
#include <initializer_list>
namespace ttyd {
bool BattleCurtainFade::enter(int32_t kind,int32_t duration,uint32_t color){
 if(kind!=23)return false;
 const auto previousPose=pose;
 *this={};pose=previousPose;active=true;flags=1;type=kind;rawDuration=duration;entryColor=color;return true;
}
void BattleCurtainFade::tick(const BattleCurtainPoseServices& s){
 if(!active || !(flags&1) || step>1)return;
 assert(s.ready && s.entry && s.release && s.flagsOn && s.flagsOff && s.animation && s.color && s.loopTimes && s.main);
 if(step==0){
  if(!s.ready(s.user,"OFF_d_maku_left_right",2))return;
  if(pose>=0)s.release(s.user,pose);
  pose=s.entry(s.user,"OFF_d_maku_left_right",2);
  // Original entry's allocation is a required service, not a completion signal.
  assert(pose>=0);
  s.flagsOn(s.user,pose,0x1800);
  s.animation(s.user,pose,"A_2",1);
  poseColor=0xffffffff;s.flagsOff(s.user,pose,0x40);++step;
 }
 if(s.loopTimes(s.user,pose)>=.5f){
  uint32_t c=poseColor&255;
  for(unsigned shift:{24u,16u,8u})c|=uint32_t(float((poseColor>>shift)&255)*.98f)<<shift;
  poseColor=c;s.color(s.user,pose,c);s.flagsOn(s.user,pose,0x40);
 }
 s.main(s.user,pose);
 if(s.loopTimes(s.user,pose)>=1.f){flags|=2;++step;s.release(s.user,pose);pose=-1;}
}
}
