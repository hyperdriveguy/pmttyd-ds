#pragma once
#include "actor_animation.hpp"
namespace ttyd {
// Recovered unit rotation bank: T(p)*Rx*Rz*Ry*T(-p). Input points are
// already scaled/faced by the native part adapter, relative to world root.
// Fresh part angles/pivots/scales are identity; other parts and Gekko rounding remain separate.
inline float battleDrawAngle(float value) {float result=float(std::fmod(double(value),360.));if(std::isnan(result))result=0;if(result<0)result+=360;if(result>=360)result=0;return result;}
inline bool battleMirrorZ(float faceAngle,float unitY,float cameraAngle,uint32_t flags) {
    if(flags&0x01000000)return false;
    const float q=battleDrawAngle(faceAngle+unitY),h=battleDrawAngle(q+cameraAngle);
    return h>=90 && h<=270;
}
struct BattleActorRotation {
    ActorMatrix matrix;
    BattleActorRotation(const ActorVector& angles,const ActorVector& sourcePivot) {
        if(!actorNonzero(angles.data()))return;
        ActorVector p=sourcePivot,negative{};for(unsigned i=0;i<3;++i){p[i]*=.01f;negative[i]=-p[i];}
        matrix=ActorMatrix::translation(p.data());
        for(unsigned axis:{0u,2u,1u}) {
            if(angles[axis]==0)continue;
            const float radians=angles[axis]*.01745329238474369f;
            const float sine=std::sin(radians),cosine=std::cos(radians);ActorMatrix rotation;
            const unsigned a=(axis+1)%3,b=(axis+2)%3;
            rotation.v[a*4+a]=rotation.v[b*4+b]=cosine;
            rotation.v[a*4+b]=-sine;rotation.v[b*4+a]=sine;matrix=matrix*rotation;
        }
        matrix=matrix*ActorMatrix::translation(negative.data());
    }
    ActorVector point(const ActorVector& p) const{return matrix.point(p);}
};
}
