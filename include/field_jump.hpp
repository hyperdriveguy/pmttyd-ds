#pragma once
#include <algorithm>
namespace ttyd {
// Ordinary Mario's original jumpData, at native engine-unit scale0.01.
struct FieldJump {
    float height=0,velocity=0,acceleration=0,jerk=0,jerkRate=0;
    int ticks=0;bool airborne=false,falling=false;
    void begin(){height=0;velocity=.08f;acceleration=-.0084f;jerk=.0008f;jerkRate=-.0002f;ticks=1;airborne=true;falling=false;}
    void tick(bool held) {
        if(!airborne)return;
        ++ticks;
        if(ticks<3)return; // mot_jump two-count startup, first count on begin.
        jerk+=jerkRate;
        if(falling)jerk=std::min(0.f,jerk);
        acceleration+=jerk;velocity+=acceleration;
        if(!falling) {
            if(velocity>0 && ticks>=4 && !held)velocity-=.00526f;
            if(velocity<=0) {
                falling=true;velocity=-.001f;acceleration=-.002f;jerk=.00024f;jerkRate=-.00004f;
                // marioJump integrates the new fall tuple on the transition tick.
                jerk+=jerkRate;acceleration+=jerk;velocity+=acceleration;
            }
        } else if(velocity<=-.18f){velocity=-.18f;acceleration=jerk=jerkRate=0;}
        height+=velocity;
        if(height<=0){height=0;airborne=false;}
    }
};
}
