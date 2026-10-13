#pragma once
#include <cmath>
#include <cstdint>
namespace ttyd {
// unk_800cbb10 ordinary pose branch; inputs retain the source speed/owner/input
// distinction. Sound helper binding remains separate.
inline unsigned partnerMovingPose(float speed,float ownerScale,int marioMovementClass,bool forceRun=false) {
    if(forceRun)return 3;
    const float boundary=ownerScale==1.f?1.649999976158142f:2.f;
    const float margin=ownerScale==1.f?.800000011920929f:0.f;
    if(boundary-margin<speed && speed<speed+margin)return marioMovementClass==1?2:3;
    return speed>=boundary?3:2;
}
struct PartnerPresentation {
    unsigned poseId=1;
    float display=0,target=0,offset=0;
    bool turnLatch=false;
    uint64_t startTick=0,nowTick=0;
    const char* name() const {return poseId==2?"PKR_W_1":poseId==3?"PKR_R_1":poseId==4?"PKR_J_1A":"PKR_S_1";}
    // Native follower adapter: actual displacement supplies provisional P+104;
    // its source homing/history/turn owners remain unbound. Source ordinary
    // Rogueport owner scale1 and raw-stick class feed the recovered pose helper.
    void update(float dx,float dz,int marioMovementClass,uint64_t tick,float cameraOffset,bool airborne=false) {
        nowTick=tick;offset=cameraOffset;
        const float speed=std::hypot(dx,dz)*100.f;
        const unsigned next=airborne?4:speed==0?1:partnerMovingPose(speed,1.f,marioMovementClass);
        if(next!=poseId){poseId=next;startTick=tick;}
        // partyMakeDispDir ordinary flags-clear route. Positive X travels at
        // source heading90 and targets display180; the c_pkr base faces left.
        if(speed!=0) {
            const float travel=wrap(std::atan2(dx,-dz)*57.29577951308232f);
            const float angle=wrap(travel-offset);
            if((target==290 || target==250) && angle>=180)target=0;
            if(angle>=190 && angle<=350)target=0;
            else if(angle>=10 && angle<=170)target=180;
        }
        if(display!=target && (target==0 || target==180)) {
            const bool lower=display<=180;
            const float goal=target==0?(turnLatch?20.f:-40.f):(turnLatch?170.f:220.f);
            display=display+.125f*(goal-display);
            if(target==0) {
                if(!turnLatch){if(display<-30)turnLatch=true;}
                else if(display>=0){display=0;turnLatch=false;}
            } else if(lower) {
                if(!turnLatch){if(display>210)turnLatch=true;}
                else if(display<=180){display=180;turnLatch=false;}
            } else if(!turnLatch) {
                if(display>210)turnLatch=true;
                else if(display<=180){display=180;turnLatch=false;}
            }
        }
    }
    float frame() const{return float(nowTick-startTick);}
    static float wrap(float angle){float a=std::fmod(angle,360.f);return a<0?a+360:a;}
    float yaw() const {return wrap(display-offset);}
    bool reflectZ() const{const float d=wrap(display);return d>=90 && d<=270;}
};
}
