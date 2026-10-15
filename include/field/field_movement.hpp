#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "floor_contact.hpp"

namespace ttyd {
// Ordinary Mario, scale1, without movement badges/transformation overrides.
// Native units are original engine units * .01. The heading/trig service is
// represented by unit XZ vectors; this is not original numerical ABI emulation.
// Rates are native distance per simulation visit: walk1/dash2.25 source units
// times.01. Stick domain 70 and dash threshold 55 are separate input units.
struct FieldMovementRates {float walk=.01f,dash=.0225f;};
struct FieldMovement {
    float speed=0,headingX=0,headingZ=-1;
    float facingDegrees=0; // Prior wall-wrapper facing1a0, distinct from travel1a4.
    float headingOffsetDegrees=0; // marioPreCamera's current camera-produced snapshot.
    FieldSlope slope;
    static constexpr float Walk=.01f,Dash=.0225f,DashStick=55.f/70.f;
    struct Displacement {float x=0,z=0;};
    Displacement worldStick(float x,float z) const {
        if(headingOffsetDegrees==0)return {x,z};
        const float radians=headingOffsetDegrees*3.14159265358979323846f/180.f;
        const float sine=std::sin(radians),cosine=std::cos(radians);
        return {x*cosine-z*sine,x*sine+z*cosine};
    }
    // Original heading-dependent multiplier1+.190000057*abs(cos). Compare
    // facing, not travel direction, against the retained camera heading offset.
    float movementRate() const {
        const float radians=(facingDegrees-headingOffsetDegrees)*3.14159265358979323846f/180.f;
        return std::fma(.190000057220459f,std::fabs(std::cos(radians)),1.f);
    }
    Displacement applyMovementRate(Displacement travel) const {
        const float rate=movementRate();return {travel.x*rate,travel.z*rate};
    }
    Displacement grounded(float x,float z,FieldMovementRates rates={}) {
        const float magnitude=std::hypot(x,z);
        if(magnitude==0){speed=0;return {};}
        speed=magnitude>DashStick?rates.dash:rates.walk;
        const auto world=worldStick(x,z);
        headingX=world.x/magnitude;headingZ=world.z/magnitude;
        return {headingX*speed,headingZ*speed};
    }
    Displacement groundedStick(int8_t x,int8_t z,FieldMovementRates rates={}) {
        grounded(float(x)/70.f,float(z)/70.f,rates);
        // The source threshold is on signed-byte integer axes, before magnitude
        // clamping. Preserve exact squared-radius equality at55, including33/44.
        speed=(x==0 && z==0)?0:(int(x)*x+int(z)*z>3025)?rates.dash:rates.walk;
        return {headingX*speed,headingZ*speed};
    }
    Displacement reduceSlope(Displacement out) const {
        const float cosine=std::cos(slope.inclination*3.14159265358979323846f/180.f);
        if(slope.tiltX!=0)out.x*=cosine;
        if(slope.tiltZ!=0)out.z*=cosine;
        return out;
    }
    Displacement retained() const {return reduceSlope({headingX*speed,headingZ*speed});}
    // Offscreen subtracts10% regardless of controlsOff; onscreen uses the
    // source .01*(-.005-speed) FMA only with controls enabled. No render clock.
    void dampFallBase(bool onScreen,bool controlsOff=false,bool bypass=false) {
        if(bypass)return;
        if(!onScreen)speed=std::fma(.10000000149011612f,-speed,speed);
        else if(!controlsOff)speed=std::max(0.f,std::fma(.009999999776482582f,-.005f-speed,speed));
    }
    // Native drift=.0000375 per source stick unit, after mapping full70.
    // Previous slope attenuation precedes combined-vector cap; do not cap the
    // retained speed first or overwrite its heading with current air input.
    Displacement airborne(float x,float z,bool suppressDrift=false,bool cap=true,float airCap=Dash) const {
        const float magnitude=std::hypot(x,z);
        const float drift=std::min(magnitude,1.f)*70.f*.0000375f;
        Displacement out{headingX*speed,headingZ*speed};
        if(magnitude>0 && !suppressDrift){const auto world=worldStick(x,z);out.x+=world.x/magnitude*drift;out.z+=world.z/magnitude*drift;}
        out=reduceSlope(out); // Previous query record, before the air cap.
        const float combined=std::hypot(out.x,out.z);
        if(cap && combined>airCap){out.x*=airCap/combined;out.z*=airCap/combined;}
        return out;
    }
};
}
