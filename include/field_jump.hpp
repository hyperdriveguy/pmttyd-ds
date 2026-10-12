#pragma once
#include <algorithm>
#include <cstdint>
namespace ttyd {
// Ordinary Mario's original jumpData, at native engine-unit scale0.01.
struct FieldJump {
    float height=0,velocity=0,acceleration=0,jerk=0,jerkRate=0;
    float takeoffY=0,takeoffSpeed=0;
    int phase=0,countdown=0;
    int16_t ticks=0,peakTicks=0;
    unsigned motion=0,previousMotion=0;
    bool airborne=false,jumping=false,falling=false,pendingEntry=true;
    bool knownLowerFloor=false,suppressDrift=false;
    bool hasAirFlags() const{return jumping || falling;}
    void clearTuple(){velocity=acceleration=jerk=jerkRate=0;}
    void fallTuple(){velocity=-.001f;acceleration=-.002f;jerk=.00024f;jerkRate=-.00004f;}
    void begin(float rootY=0,float baseSpeed=0) {
        height=0;takeoffY=rootY;takeoffSpeed=baseSpeed;
        velocity=.08f;acceleration=-.0084f;jerk=.0008f;jerkRate=-.0002f;
        previousMotion=motion;motion=3;phase=0;countdown=2;ticks=peakTicks=0;
        airborne=jumping=true;falling=pendingEntry=knownLowerFloor=suppressDrift=false;
        dispatch(rootY); // The entry callback also decrements/counts once.
    }
    void beginFall(bool known=false) {
        height=0;fallTuple();previousMotion=motion;motion=known?11:10;knownLowerFloor=known;
        airborne=pendingEntry=true; // Late request; entry owns flags/counter next call.
    }
    void ceilingFall(){beginFall();jumping=falling=false;}
    void dispatch(float rootY=0) {
        if(!airborne)return;
        if(pendingEntry) {
            pendingEntry=false;jumping=false;falling=true;phase=0;
            if(motion==11)suppressDrift=true;
        }
        if(jumping && phase==0 && --countdown==0)phase=10;
        if(falling && phase==0 && rootY<=takeoffY)phase=10;
        const uint16_t next=uint16_t(uint16_t(ticks)+1);
        ticks=next<0x8000?int16_t(next):int16_t(int(next)-0x10000);
    }
    void land(bool moving,bool retainTuple=false) {
        if(!retainTuple)clearTuple();
        suppressDrift=false; // marioLandOn clears main0x80; air flags await dispatch.
        ticks=peakTicks=0;height=0;previousMotion=motion;motion=moving?1:0;
        pendingEntry=true;airborne=false; // Flags/phase await ground dispatch.
    }
    void enterGround(unsigned nextMotion=0,float rootY=0) {
        const bool entry=pendingEntry || motion!=nextMotion;
        if(motion!=nextMotion)previousMotion=motion;
        motion=nextMotion;
        if(entry && nextMotion==0) {
            clearTuple();ticks=0;countdown=phase=0;takeoffY=rootY;
        }
        jumping=falling=pendingEntry=knownLowerFloor=suppressDrift=false;
    }
    void integrate(bool held) {
        if(!airborne || (!falling && (!jumping || phase<10)))return;
        const bool previouslyRising=velocity>0;
        jerk+=jerkRate;
        if(falling)jerk=std::min(0.f,jerk);
        acceleration+=jerk;velocity+=acceleration;
        if(jumping) {
            if(velocity>0 && ticks>=4 && !held)velocity-=.00526f;
            if(velocity<=0) {
                if(previouslyRising)peakTicks=ticks;
                fallTuple();previousMotion=motion;motion=10;pendingEntry=true;
                // marioJump integrates the new fall tuple on the transition tick.
                jerk+=jerkRate;acceleration+=jerk;velocity+=acceleration;
            }
        } else if(velocity<=-.18f){velocity=-.18f;acceleration=jerk=jerkRate=0;}
        height+=velocity;
    }
    // Equal-height host fixture helper. Native landing uses the geometry owner.
    void tick(bool held,bool clampToBase=true) {
        dispatch(takeoffY+height);integrate(held);
        if(clampToBase && height<=0){height=0;airborne=false;}
    }
};
}
