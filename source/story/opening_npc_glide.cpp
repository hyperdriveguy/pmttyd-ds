#include "opening_npc_glide.hpp"
#include <cmath>
namespace ttyd {
float OpeningNpcGlide::curve11(float fraction){
    if(fraction>1)fraction=1;
    const float phase=3.1415927410125732f*fraction;
    const float cosine=float(std::cos(double(phase)));
    return (1-cosine)*.5f;
}
bool OpeningNpcGlide::begin(Point from,Point to,uint32_t command){
    *this=OpeningNpcGlide{};start=position=from;target=to;sourceCommand=command;
    for(float value:from)if(!std::isfinite(value))failed=true;
    for(float value:to)if(!std::isfinite(value))failed=true;
    if(failed)return false;
    const float dx=to[0]-from[0],dy=to[1]-from[1],dz=to[2]-from[2];
    const float xx=dx*dx,yy=dy*dy,zz=dz*dz;
    const float distance=std::sqrt(zz+(xx+yy)); // Native sqrt adapter to PPC estimate/refinement.
    if(distance==0){result=1;return true;}
    duration=distance/speed;active=true;return true;
}
int32_t OpeningNpcGlide::step(float time,float dt){
    if(!active || failed)return result;
    if(!std::isfinite(time) || !std::isfinite(dt) || time<0 || dt<=0 || time<elapsed){failed=true;return result;}
    elapsed=time;delta=dt;bool complete=time>=duration;
    // Preserve source terminal arithmetic at800515dc..f8, including its
    // subtraction order. Replacing it with a clamped absolute endpoint changes
    // the original callback's last incremental delta for nonintegral duration.
    if(time>duration){const float difference=duration-time;dt=dt-difference;if(dt<=0){active=false;result=2;return result;}}
    const float current=curve11(time/duration),previous=curve11((time-dt)/duration);
    const float difference=current-previous;
    for(unsigned axis=0;axis<3;++axis)pending[axis]=std::fma(target[axis]-start[axis],difference,0.f);
    const float oldPhase=3.1415927410125732f*previous,newPhase=3.1415927410125732f*current;
    const float oldSine=float(std::sin(double(oldPhase))),newSine=float(std::sin(double(newPhase)));
    const float oldHeight=height*oldSine;
    const float heightDifference=std::fma(height,newSine,-oldHeight);
    pending[1]=pending[1]+heightDifference;
    for(unsigned axis=0;axis<3;++axis)position[axis]=position[axis]+pending[axis];
    result=complete?1:0;if(complete)active=false;return result;
}
}
