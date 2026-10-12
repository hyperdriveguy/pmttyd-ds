#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "floor_contact.hpp"

namespace ttyd {
// Ordinary Mario, scale1, without movement badges/transformation overrides.
// Native units are original engine units * .01. The heading/trig service is
// represented by unit XZ vectors; this is not original numerical ABI emulation.
struct FieldMovement {
    float speed=0,headingX=1,headingZ=0;
    float headingOffsetDegrees=0; // Native fixed Z-facing camera; original camera owner pending.
    FieldSlope slope;
    static constexpr float Walk=.01f,Dash=.0225f,DashStick=55.f/70.f;
    struct Displacement {float x=0,z=0;};
    float movementRate() const {
        // marioGetMoveRate: u(H)=(sin(H),-cos(H)). Represent its degree trig
        // service through cached unit travel and the separately owned offset.
        const float radians=headingOffsetDegrees*3.14159265358979323846f/180.f;
        const float cosine=std::fma(headingX,std::sin(radians),-headingZ*std::cos(radians));
        return std::fma(.190000057220459f,std::fabs(cosine),1.f);
    }
    Displacement applyMovementRate(Displacement travel) const {
        const float rate=movementRate();return {travel.x*rate,travel.z*rate};
    }
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
    Displacement reduceSlope(Displacement out) const {
        const float cosine=std::cos(slope.inclination*3.14159265358979323846f/180.f);
        if(slope.tiltX!=0)out.x*=cosine;
        if(slope.tiltZ!=0)out.z*=cosine;
        return out;
    }
    Displacement retained() const {return reduceSlope({headingX*speed,headingZ*speed});}
    Displacement airborne(float x,float z,bool suppressDrift=false) const {
        const float magnitude=std::hypot(x,z);
        const float drift=std::min(magnitude,1.f)*70.f*.0000375f;
        Displacement out{headingX*speed,headingZ*speed};
        if(magnitude>0 && !suppressDrift){out.x+=x/magnitude*drift;out.z+=z/magnitude*drift;}
        out=reduceSlope(out); // Previous query record, before the air cap.
        const float combined=std::hypot(out.x,out.z);
        if(combined>Dash){out.x*=Dash/combined;out.z*=Dash/combined;}
        return out;
    }
};
}
