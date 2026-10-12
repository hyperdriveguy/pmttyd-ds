#pragma once
#include <cmath>
namespace ttyd {
// Ordinary Mario's slope limit is50 degrees; face orientation is handled separately.
inline bool floorSlopeAllowed(float nx,float ny,float nz) {
    if(!std::isfinite(nx) || !std::isfinite(ny) || !std::isfinite(nz))return false;
    const float horizontal=std::hypot(nx,nz);
    if(horizontal<1e-8f && std::fabs(ny)<1e-8f)return false;
    return std::atan2(horizontal,std::fabs(ny))*57.295779513f<50.f;
}
// Normal marioSearchGround candidate branches8008ef24–8008eff4, native scale.01.
inline bool floorCandidateReachable(float rootY,float velocity,float floorY) {
    if(!std::isfinite(rootY) || !std::isfinite(velocity) || !std::isfinite(floorY))return false;
    const float below=rootY-floorY;
    if(below>=.02f)return below<=std::fabs(velocity);
    return std::fabs(below)<=.11f;
}
}
