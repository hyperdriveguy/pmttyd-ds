#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace ttyd {
// Ordinary Mario, scale1, without movement badges/transformation overrides.
// Native units are original engine units * .01. The heading/trig service is
// represented by unit XZ vectors; this is not original numerical ABI emulation.
struct FieldMovement {
    float speed=0,headingX=1,headingZ=0;
    static constexpr float Walk=.01f,Dash=.0225f,DashStick=55.f/70.f;
    struct Displacement {float x=0,z=0;};
    Displacement grounded(float x,float z) {
        const float magnitude=std::hypot(x,z);
        if(magnitude==0){speed=0;return {};}
        speed=magnitude>DashStick?Dash:Walk;
        headingX=x/magnitude;headingZ=z/magnitude;
        return {headingX*speed,headingZ*speed};
    }
    Displacement groundedStick(int8_t x,int8_t z) {
        grounded(float(x)/70.f,float(z)/70.f);
        // The source threshold is on signed-byte integer axes, before magnitude
        // clamping. Preserve exact squared-radius equality at55, including33/44.
        speed=(x==0 && z==0)?0:(int(x)*x+int(z)*z>3025)?Dash:Walk;
        return {headingX*speed,headingZ*speed};
    }
    Displacement airborne(float x,float z) const {
        const float magnitude=std::hypot(x,z);
        const float drift=std::min(magnitude,1.f)*70.f*.0000375f;
        Displacement out{headingX*speed,headingZ*speed};
        if(magnitude>0){out.x+=x/magnitude*drift;out.z+=z/magnitude*drift;}
        const float combined=std::hypot(out.x,out.z);
        if(combined>Dash){out.x*=Dash/combined;out.z*=Dash/combined;}
        return out;
    }
};
}
