#include "opening_harbor_jump.hpp"
#include <cmath>
namespace ttyd {
bool OpeningHarborJump::begin(Point from,std::array<int32_t,3> to,int32_t milliseconds,int32_t height,uint32_t originalFlags){
    *this={};start=position=from;duration=milliseconds;flags=originalFlags;
    for(float value:from)if(!std::isfinite(value))failed=true;
    if(failed || milliseconds<0){failed=true;return false;}
    for(unsigned i=0;i<3;++i)target[i]=float(to[i]);
    // Original r2-30276 at8041fdfc is C0800000 (-4.0).
    coefficient=-4.f*float(height);linear=(target[1]-start[1])-coefficient;
    savedGravityFlag=flags&0x80000u;flags&=~0x80000u;active=true;return true;
}
int32_t OpeningHarborJump::advance(uint32_t elapsedMilliseconds){
    if(failed || !active)return result;
    if(elapsedMilliseconds>=uint32_t(duration)){
        position=target;flags|=savedGravityFlag;active=false;result=2;return result;
    }
    // intplGetValue selector0 retains s*d/n then addition, independently
    // from the callback's explicit fused quadratic Y calculation.
    const float step=float(elapsedMilliseconds),count=float(duration),ratio=step/count;
    for(unsigned i:{0u,2u}){const float delta=target[i]-start[i];position[i]=start[i]+(step*delta)/count;}
    const float velocity=std::fma(coefficient,ratio,linear);
    position[1]=std::fma(ratio,velocity,start[1]);
    return 0;
}
}
