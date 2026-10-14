#pragma once
#include "actor_animation.hpp"
#include "field_camera_state.hpp"
namespace ttyd {
// Handheld capture adapter: deform original material geometry in the frozen
// capture camera plane. Source OFF_d_saku supplies the four animated corners.
inline ActorVector paperFenceWarp(ActorVector p,const CameraView& view,
                                  const std::array<float,4>& bounds,const std::array<ActorVector,4>& corners) {
    const float x=view[0][0]*p[0]+view[0][1]*p[1]+view[0][2]*p[2]+view[0][3];
    const float y=view[1][0]*p[0]+view[1][1]*p[1]+view[1][2]*p[2]+view[1][3];
    const float u=(x-bounds[0])/bounds[2],v=(y-bounds[1])/bounds[3];
    ActorVector delta{};
    for(unsigned k=0;k<3;++k)delta[k]=u+v<=1?
        corners[0][k]+u*(corners[1][k]-corners[0][k])+v*(corners[2][k]-corners[0][k]):
        corners[3][k]+(1-u)*(corners[2][k]-corners[3][k])+(1-v)*(corners[1][k]-corners[3][k]);
    delta[0]=(delta[0]-(-32+64*u))*bounds[2]/64;
    delta[1]=(delta[1]-(-24+48*v))*bounds[3]/48;
    delta[2]*=bounds[2]/64;
    for(unsigned k=0;k<3;++k)p[k]+=view[0][k]*delta[0]+view[1][k]*delta[1]+view[2][k]*delta[2];
    return p;
}
}
