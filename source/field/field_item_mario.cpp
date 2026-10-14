#include "field_item_mario.hpp"
namespace ttyd {
bool fieldItemMarioGetAvailable(const FieldJump& w,uint32_t main,const FieldItemMarioPost& post) {
 if(w.motion==22 || (main&0x00100000))return false;
 if(w.motion==21 || w.motion==25 || w.motion==26 || (main&0x10000000))return true;
 return (w.motion==0 || w.motion==1 || w.motion==2 || w.motion==3 || w.motion==10 || w.motion==11 || w.motion==15) &&
        ((w.motion!=0 && w.motion!=10) || post.call);
}
bool requestFieldItemMarioMotion(FieldJump& w,FieldMovement& move,uint32_t main,
 MarioPresentation& display,bool get,const FieldItemMarioPost& post) {
 if(get) {
  if(w.motion==22)return false;
  if(w.motion==21 || w.motion==25){move.speed=0;return true;}
  if(w.motion==26)return true;
 } else if(w.motion==25 || w.motion==26 || w.motion==21 || w.motion==22)return true;
 if(main&0x10000000)return true;
 const unsigned old=w.motion;
 const bool needsPost=old==0 || old==10;
 if((old!=0 && old!=1 && old!=2 && old!=3 && old!=10 && old!=11 && old!=15) ||
    (needsPost && !post.call))return false;
 if(w.previousMotion==16 || w.previousMotion==17) {
  display.displayFlags&=~0x08000000u;display.displayFlags&=~4u;
 }
 w.previousMotion=old;w.motion=get?15:0;
 if(needsPost)post.call(post.user,old);
 w.pendingEntry=true;return true;
}
bool enterFieldItemMarioMotion(FieldJump& w,FieldMovement& move,uint32_t main,MarioPresentation& display) {
 if(w.motion!=15 || !w.pendingEntry || (main&2))return false;
 if(main&0x00100000)return false; // Alternate heading aliases need their source provider.
 w.pendingEntry=false;w.countdown=0;w.ticks=0;w.phase=0;move.speed=0;
 const float heading=display.adjustMoveDirection();
 const float radians=heading*.01745329238474369f;
 move.facingDegrees=heading;move.headingX=std::sin(radians);move.headingZ=-std::cos(radians);
 display.requestPose("M_I_U");return true;
}
}
