#pragma once
#include "field_wall.hpp"
#include <cmath>
#include <cstdint>

namespace ttyd {
struct FieldPartyAppearanceHit {bool hit=false;float pointY=0;};
// Live owner/math/ray providers. Numeric and geometry adaptation is separate
// from the original appearance2 query/write policy; no invented floor filter.
struct FieldPartyAppearanceServices {
    void* user=nullptr;
    FieldWallVector (*root)(void*)=nullptr;
    float (*display)(void*)=nullptr;
    int32_t (*auxiliary)(void*)=nullptr;
    float (*revise)(void*,float)=nullptr;
    float (*heading)(void*,float,float,bool)=nullptr;
    FieldWallVector (*sincos)(void*,float)=nullptr;
    void (*move)(void*,FieldWallVector&,float,float)=nullptr;
    FieldPartyAppearanceHit (*ray)(void*,FieldWallVector,FieldWallVector,float)=nullptr;
    bool ready() const {return root && display && auxiliary && revise && heading && sincos && move && ray;}
};
inline float fieldPartyWrap(float angle) {
    while(angle<0)angle+=360.f;
    while(angle>=360)angle-=360.f;
    return angle;
}
// Ordinary finite/signed-conversion domain of toMovedir and toMovedir2.
// Host/ARM libm and fused rounding do not claim original Gekko bit equality.
inline float fieldPartyHeading(float display,float camera,uint32_t flags,float current,float offset=0,bool second=false) {
    if(flags&0x100000) {
        if(current==280 || current==460)display=0;
        else if(current==260 || current==440)display=180;
        else display=fieldPartyWrap(current)>=180?0:180;
    }
    const float base=(270.f-display)+camera;
    const float heading=fieldPartyWrap(float(int32_t(std::fma(1000.f,base,base>=0?.5f:-.5f)))/1000.f);
    if(!second)return heading;
    const float relative=fieldPartyWrap(heading-camera);
    return fieldPartyWrap(heading+(relative>=180?-offset:offset));
}
inline FieldWallVector fieldPartySinCos(float degrees) {
    const float radians=(3.1415927410125732f*degrees)/180.f;
    return {float(std::sin(double(radians))),0,-float(std::cos(double(radians)))};
}
inline void fieldPartyMove(FieldWallVector& output,float distance,float degrees) {
    const float radians=(6.2831854820251465f*degrees)/360.f;
    const float sine=float(std::sin(double(radians))),cosine=float(std::cos(double(radians)));
    output.x=std::fma(distance,sine,output.x);
    output.z=-std::fma(distance,cosine,-output.z);
}
inline bool fieldPartyAppearPosition2(float height,int32_t slot,FieldWallVector& output,const FieldPartyAppearanceServices& s) {
    const float length=30.f+height;
    s.revise(s.user,180.f+s.display(s.user)); // Original request precedes retained facing and output seed.
    const float retained=s.display(s.user);
    output=s.root(s.user);
    auto horizontal=[&](float heading,bool owner) {
        const auto direction=s.sincos(s.user,heading);
        auto origin=owner?s.root(s.user):output;origin.y+=37.f;
        return s.ray(s.user,origin,direction,length).hit;
    };
    auto down=[&]() {
        auto origin=output;origin.y+=37.f;
        const auto hit=s.ray(s.user,origin,{0,-1,0},38.f);
        if(hit.hit)output.y=hit.pointY;
        return hit.hit;
    };
    if(s.auxiliary(s.user)<0) {
        const float heading=s.heading(s.user,180.f+s.display(s.user),0,false);
        if(horizontal(heading,false))return false;
        const float move=s.heading(s.user,s.display(s.user),0,false);
        s.move(s.user,output,-30.f,move);
        return down();
    }
    output=s.root(s.user);
    const float first=s.heading(s.user,retained,-90,true);
    if(!horizontal(first,true)) {
        output=s.root(s.user);
        const float move=s.heading(s.user,retained,slot==0?20.f:-20.f,true);
        s.move(s.user,output,-30.f,move);
        return down(); // A floor miss does not admit the opposite side.
    }
    const float second=s.heading(s.user,retained,90,true);
    if(horizontal(second,true))return false;
    output=s.root(s.user);
    const float move=s.heading(s.user,retained,slot==0?-20.f:20.f,true);
    s.move(s.user,output,-30.f,move);
    return down();
}
}
