#pragma once
#include <cmath>
#include <optional>
namespace ttyd {
// Scoped ordinary, stationary/near-equal-height evt_mario_jump_pos path. Original
// roll, nonzero travel/height and 30Hz flags require their own source services.
struct FieldScriptedJump {
    float y=0,velocity=0,acceleration=0,targetY=0,halfFrames=0;
    int phase=0,count=0;
    bool begin(float root,float target,int frames,float parameter) {
        if(std::fabs(root-target)>=1 || frames<4 || !std::isfinite(root) || !std::isfinite(parameter))return false;
        y=root;targetY=target;halfFrames=.5f*float(frames);
        if(parameter<=-1000)parameter=20;
        const float coefficient=(parameter+parameter)/std::fma(halfFrames,halfFrames,-halfFrames);
        velocity=coefficient*halfFrames;acceleration=-coefficient;
        count=int(.5f+halfFrames);phase=1;return true;
    }
    // The caller supplies the original landing service's admitted support.
    // Return1 consumes the command and yields;0 retains the USER_FUNC.
    int tick(std::optional<float> support={}) {
        if(phase==1) {
            y+=velocity;velocity+=acceleration;
            if(--count<=0){velocity=0;const float delta=targetY-y;acceleration=(delta+delta)/std::fma(halfFrames,halfFrames,-halfFrames);count=int(.5f+halfFrames);phase=2;}
            return 0;
        }
        if(phase!=2)return 1;
        velocity+=acceleration;
        if(support && y+velocity<=*support){y=*support;count=0;}
        else y+=velocity;
        if(--count>0)return 0;
        velocity=acceleration=0;phase=3;return 1;
    }
};
}
