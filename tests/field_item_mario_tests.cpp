#include "field_item_mario.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
struct Post {FieldJump* motion;unsigned calls=0,old=99;};
int main() {
 FieldJump w;FieldMovement move;MarioPresentation pose;
 pose.update(0,0,20,7,100);const char* before=pose.selected;
 w.pendingEntry=false;w.phase=7;w.countdown=9;w.ticks=13;w.peakTicks=17;
 w.height=2;w.velocity=.03f;w.jumping=true;move.speed=2;
 assert(!fieldItemMarioGetAvailable(w,0,{}));
 assert(!requestFieldItemMarioMotion(w,move,0,pose,true,{}));assert(w.motion==0 && !w.pendingEntry);
 Post context{&w};FieldItemMarioPost post{&context,[](void* p,unsigned old){auto& c=*static_cast<Post*>(p);++c.calls;c.old=old;assert(c.motion->motion==15 && c.motion->previousMotion==old && !c.motion->pendingEntry);}};
 assert(requestFieldItemMarioMotion(w,move,0,pose,true,post));
 assert(w.motion==15 && w.pendingEntry && pose.selected==before && context.calls==1 && context.old==0);
 assert(!enterFieldItemMarioMotion(w,move,2,pose) && w.pendingEntry && pose.selected==before);
 pose.display=100;assert(enterFieldItemMarioMotion(w,move,0,pose));
 assert(!w.pendingEntry && !w.phase && !w.countdown && !w.ticks && move.speed==0);
 assert(w.peakTicks==17 && w.height==2 && w.velocity==.03f && w.jumping);
 assert(pose.display==180 && pose.target==180 && pose.facing==110);
 pose.displayFlags|=0x80000000;pose.update(15,0,20,7,200);assert(!(pose.displayFlags&0x80000000) && move.facingDegrees==110);assert(pose.supported && !std::strcmp(pose.selected,"M_I_U"));
 const auto start=pose.poseStartMilliseconds;assert(!enterFieldItemMarioMotion(w,move,0,pose));
 pose.update(15,0,20,7,300);assert(pose.poseStartMilliseconds==start);
 assert(requestFieldItemMarioMotion(w,move,0,pose,false,{}));
 assert(w.motion==0 && w.previousMotion==15 && w.pendingEntry && !std::strcmp(pose.selected,"M_I_U"));
 // Request admission/retained previous flags, special returns and pose lock.
 w.motion=1;w.previousMotion=16;pose.displayFlags=0x08000004;
 assert(requestFieldItemMarioMotion(w,move,0,pose,true,{}));assert(!(pose.displayFlags&0x08000004));
 pose.displayFlags=2;const char* locked=pose.selected;assert(enterFieldItemMarioMotion(w,move,0,pose));assert(pose.selected==locked);
 w.motion=1;w.pendingEntry=false;assert(requestFieldItemMarioMotion(w,move,0x10000000,pose,true,{}));assert(w.motion==1 && !w.pendingEntry);
 w.motion=25;move.speed=3;assert(requestFieldItemMarioMotion(w,move,0,pose,true,{}));assert(w.motion==25 && move.speed==0);
 assert(requestFieldItemMarioMotion(w,move,0,pose,false,{}));assert(w.motion==25);
 w.motion=22;assert(!requestFieldItemMarioMotion(w,move,0,pose,true,{}));
 w.motion=10;assert(!requestFieldItemMarioMotion(w,move,0,pose,true,{}));assert(w.motion==10);
 w.motion=15;w.pendingEntry=true;assert(!enterFieldItemMarioMotion(w,move,0x00100000,pose));assert(w.pendingEntry);
}
