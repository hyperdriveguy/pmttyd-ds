#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
namespace ttyd {
struct MarioPresentation {
    float facing=0,storedSpeed=0,storedHeading=0;
    int32_t directionCountdown=0;
    float display=0,target=0,offset=0;
    uint32_t turnFlags=0;
    unsigned previousMotion=~0u;
    const char* front="M_S_1";
    const char* selected="M_S_1R";
    uint64_t poseStartMilliseconds=0;
    bool supported=true,poseUploaded=false;
    void wallRequest(float speed,float heading,int16_t airCounter) {
        if(speed!=0 && airCounter==0) {
            if(speed>storedSpeed){storedSpeed=speed;storedHeading=heading;directionCountdown=14;}
            else if(--directionCountdown<=0){directionCountdown=0;storedSpeed=speed;storedHeading=heading;}
            facing=storedHeading;
        } else storedSpeed=0;
    }
    static float wrap(float angle){angle=std::fmod(angle,360.f);return angle<0?angle+360:angle;}
    void turn(float baseSpeed) {
        const float h=wrap(facing-offset);target=std::trunc(target+.5f);
        if((target==290 || target==250) && h>=180)target=0;
        if(baseSpeed!=0) {
            if(h>=190 && h<=350)target=0;
            else if(h>=10 && h<=170)target=180;
        }
        const float initial=display;if(display==target)return;
        if(target==0) {
            if(turnFlags&0x400)turnFlags&=~0x500u;
            turnFlags|=0x200;
            display+=(turnFlags&0x100)?.30000001192092896f*(10-display):.125f*(-30-display);
            if(turnFlags&0x100){if(display>=0){display=0;turnFlags&=~0x100u;}}
            else if(display<-20)turnFlags|=0x100;
        } else if(target==180) {
            if(turnFlags&0x200)turnFlags&=~0x300u;
            turnFlags|=0x400;
            display+=(turnFlags&0x100)?.30000001192092896f*(170-display):.125f*(210-display);
            if(initial<=180) {
                if(turnFlags&0x100){if(display<=180){display=180;turnFlags&=~0x100u;}}
                else if(display>200)turnFlags|=0x100;
            } else if(!(turnFlags&0x100)) {
                if(display>200)turnFlags|=0x100;
                else if(display<=180){display=180;turnFlags&=~0x100u;}
            }
        }
    }
    void update(unsigned motion,float speed,float cameraOffset,int hp,uint64_t now) {
        offset=cameraOffset;
        if(motion!=previousMotion) {
            switch(motion) {
            case 0:front="M_S_1";break;
            case 1:front="M_W_1";break;
            case 2:front="M_R_1";break;
            case 3:front="M_J_1B";break;
            case 10:front="M_J_1C";break;
            default:break; // Motion11 keeps its earlier pose; other domains stay explicit.
            }
            previousMotion=motion;
        }
        supported=(motion<=3 || motion==10 || motion==11) && (motion!=0 || hp>5);
        const float heading=wrap(facing-offset);const bool rear=heading<=70 || heading>=290;
        const char* next=front;
        if(rear) {
            if(std::strcmp(front,"M_S_1")==0)next="M_S_1R";
            else if(std::strcmp(front,"M_W_1")==0)next="M_W_1R";
            else if(std::strcmp(front,"M_R_1")==0)next="M_R_1R";
            else if(std::strcmp(front,"M_J_1B")==0)next="M_J_1BR";
            else if(std::strcmp(front,"M_J_1C")==0)next="M_J_1CR";
        }
        if(!poseUploaded || std::strcmp(selected,next)!=0){selected=next;poseStartMilliseconds=now;poseUploaded=true;}
        turn(speed); // Rear selection precedes display-direction update.
    }
    float moveHeading() const {
        const float a=270-display+offset;
        return wrap(std::trunc(std::fma(1000.f,a,a<0?-.5f:.5f))/1000.f);
    }
    float drawYaw() const{return wrap(display-offset);}
    bool reflectZ() const {const float d=wrap(display);return d>=90 && d<=270;}
};
}
