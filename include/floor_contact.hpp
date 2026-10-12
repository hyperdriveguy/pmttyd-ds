#pragma once
#include <cmath>
namespace ttyd {
// Normal marioSearchGround candidate branches8008ef24–8008eff4, native scale.01.
inline bool floorCandidateReachable(float rootY,float velocity,float floorY) {
    if(!std::isfinite(rootY) || !std::isfinite(velocity) || !std::isfinite(floorY))return false;
    const float below=rootY-floorY;
    if(below>=.02f)return below<=std::fabs(velocity);
    return std::fabs(below)<=.11f;
}
}
