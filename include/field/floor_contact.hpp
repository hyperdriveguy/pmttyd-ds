#pragma once
#include <cmath>
namespace ttyd {
// Native lengths: 11-engine-unit rise,.02 native near-drop=2 engine units,
// front probe5.3 engine units. These policies differ from step minimum 5 units.
constexpr float GroundedMaxRise=.11f,GroundedNearDrop=.02f;
constexpr float FrontStepDistance=.053f;
// Native representations of the separately owned +218/+21c/+224 angles.
// Host atan2/cos are adaptations, not the original SDK numerical service.
struct FieldSlope {
    float inclination=0,tiltZ=0,tiltX=0;
    // Inclination and projected axis tilts are distinct source caches. The
    // projected angle wraps then rounds to.01 degree; atan2 is a native adapter.
    void normal(float nx,float ny,float nz) {
        constexpr float degrees=57.295779513f;
        auto projected=[&](float component) {
            float angle=std::atan2(component,-ny)*degrees;
            if(angle<0)angle+=360;
            return std::trunc(std::fma(100.f,angle-180.f,.5f))/100.f;
        };
        inclination=std::atan2(std::hypot(nx,nz),ny)*degrees;
        tiltZ=projected(nz);tiltX=projected(nx);
    }
};
// Deliberate double scale: round source height at.01 engine unit, then
// return native height at.0001. A single native hundredth rounding is wrong.
inline float supportMetadataHeight(float height) {
    // +1c4 rounds in original engine units before conversion to native units.
    return std::trunc(std::fma(100.f,height*100.f,.5f))*.0001f;
}
// Step rise5..11 engine units is inclusive; slope<=30 differs from ordinary
// floor admissibility's strict<50. Invalid/degenerate normals reject as90deg.
inline bool stairRiseAllowed(float rootY,float floorY,float slope) {
    const float rise=floorY-rootY;
    return std::isfinite(rise) && std::isfinite(slope) && rise>=.05f && rise<=GroundedMaxRise && slope<=30;
}
// Ordinary Mario's slope limit is50 degrees; face orientation is handled separately.
inline float floorSlopeDegrees(float nx,float ny,float nz) {
    if(!std::isfinite(nx) || !std::isfinite(ny) || !std::isfinite(nz))return 90;
    const float horizontal=std::hypot(nx,nz);
    if(horizontal<1e-8f && std::fabs(ny)<1e-8f)return 90;
    return std::atan2(horizontal,ny)*57.295779513f;
}
inline bool floorSlopeAllowed(float nx,float ny,float nz){return floorSlopeDegrees(nx,ny,nz)<50;}
// Normal marioSearchGround candidate branches8008ef24–8008eff4, native scale.01.
inline bool floorCandidateReachable(float rootY,float velocity,float floorY) {
    if(!std::isfinite(rootY) || !std::isfinite(velocity) || !std::isfinite(floorY))return false;
    const float below=rootY-floorY;
    if(below>=GroundedNearDrop)return below<=std::fabs(velocity);
    return std::fabs(below)<=.11f;
}
}
