#pragma once
#include <cmath>
namespace ttyd {
constexpr float GroundedMaxRise=.11f,GroundedNearDrop=.02f;
inline bool groundedHeightCanSnap(float rootY,float floorY) {
    return std::isfinite(rootY) && std::isfinite(floorY) &&
        floorY-rootY<=GroundedMaxRise && rootY-floorY<GroundedNearDrop;
}
// Ordinary Mario's slope limit is50 degrees; face orientation is handled separately.
inline float floorSlopeDegrees(float nx,float ny,float nz) {
    if(!std::isfinite(nx) || !std::isfinite(ny) || !std::isfinite(nz))return 90;
    const float horizontal=std::hypot(nx,nz);
    if(horizontal<1e-8f && std::fabs(ny)<1e-8f)return 90;
    return std::atan2(horizontal,std::fabs(ny))*57.295779513f;
}
inline bool floorSlopeAllowed(float nx,float ny,float nz){return floorSlopeDegrees(nx,ny,nz)<50;}
// Normal marioSearchGround candidate branches8008ef24–8008eff4, native scale.01.
inline bool floorCandidateReachable(float rootY,float velocity,float floorY) {
    if(!std::isfinite(rootY) || !std::isfinite(velocity) || !std::isfinite(floorY))return false;
    const float below=rootY-floorY;
    if(below>=.02f)return below<=std::fabs(velocity);
    return std::fabs(below)<=.11f;
}
}
