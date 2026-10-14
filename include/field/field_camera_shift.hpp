#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "field_camera_state.hpp"
namespace ttyd {
// camShiftMain's separate work; source timestamps are already quantized to
// integer milliseconds by the supplied clock adapter. Wrap remains unsupported.
struct FieldCameraShift {
    float current=0,desired=0,blend=0,accumulation=0;
    uint32_t enabled=0;
    uint64_t previousMilliseconds=0;
    float limit=300,incrementCap=3,blendRate=.002f;
    float previousRootX=0,previousRootZ=0;
    void reset() {
        current=desired=blend=accumulation=0;enabled=0;
        limit=300;incrementCap=3;blendRate=.002f;previousRootX=previousRootZ=0;
        // Source init/reset does not write its retained timestamp pair.
    }
    bool update(int8_t mainX,CameraPoint root,uint32_t flags,float retainedCandidateRate,uint64_t nowMilliseconds) {
        if(nowMilliseconds<previousMilliseconds)return false;
        const auto dt=[&](float rate){return rate*60.f*float(nowMilliseconds-previousMilliseconds)/1000.f;};
        float direction;
        if(flags&0x20){blend=1;desired=0;enabled=1;direction=0;}
        else {
            direction=mainX>0?25:mainX<0?-25:0;
            if(root.x==previousRootX && root.z==previousRootZ)direction=0;
            previousRootX=root.x;previousRootZ=root.z;
        }
        if(direction<0) {
            accumulation=accumulation>0?dt(direction):accumulation+dt(direction);
            if(accumulation<=-limit) {
                enabled=1;if(desired>0)blend=0;desired=-retainedCandidateRate;accumulation=-limit;
            }
        } else if(direction>0) {
            accumulation=accumulation<0?dt(direction):accumulation+dt(direction);
            if(accumulation>=limit) {
                enabled=1;if(desired<0)blend=0;desired=retainedCandidateRate;accumulation=limit;
            }
        }
        if(enabled)blend=std::min(1.f,blend+dt(blendRate));
        if(desired-current==0){blend=0;enabled=0;}
        const float delta=desired-current;float increment=blend*delta;
        if(delta>0) {
            if(delta<.0001f)increment=delta;
            if(increment>incrementCap)increment=incrementCap;
        } else if(delta<0) {
            if(delta>-.0001f)increment=delta;
            if(increment<-incrementCap)increment=-incrementCap;
        }
        if(direction!=0 || (flags&0x20)) {
            current+=dt(increment);
            if(desired<0){if(current<desired)current=desired;}
            else if(current>desired)current=desired;
        } else blend=0;
        return true;
    }
    void post(uint64_t nowMilliseconds){previousMilliseconds=nowMilliseconds;}
};
// Native simulation clock bridge, independent of rendering frames.
inline uint64_t fieldCameraTickMilliseconds(uint64_t tick) {
    return tick/60*1000+(tick%60)*1000/60;
}
}
