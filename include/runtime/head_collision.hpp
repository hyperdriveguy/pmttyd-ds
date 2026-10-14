#pragma once
#include <cmath>
#include <cstdint>
namespace ttyd {
// Ordinary Mario, US marioSearchHead/marioChkOverhead. Native units=.01 engine units.
// Geometry selection and face culling must precede these contact rules.
constexpr float HeadBodyHeight=.37f;
constexpr float HeadProbeRadius=.0975f; // .5*(base width20-.5), converted once.
// GameCube US marioChkOverhead8008cc9c: ordinary jump and jumpStand only.
constexpr bool headMotionResponds(unsigned motion){return motion==3 || motion==6;}
// Trigger mask4 is nonsolid unless inclusion0x00800000 overrides it.
// This head policy differs from broader wall/ground inclusion masks.
inline bool headAttributesSolid(uint32_t flags) {
    return !(flags&4) || (flags&0x00800000);
}
// Source head-hit Y adds18.5 in the scaled truncation expression: native
// geometry quantizes at1/10000 with that source offset, not std::round.
inline float headContactHeight(float triangleY) {
    return std::trunc(std::fma(triangleY,10000.f,18.5f))/10000.f;
}
// Native velocity -.1 equals original -10: near-apex/falling tolerance
// precedes checking root+37-engine-unit body height against contact.
inline bool headContactReached(float rootY,float velocity,float contactY,float bodyHeight=HeadBodyHeight) {
    return std::isfinite(rootY) && std::isfinite(velocity) && std::isfinite(contactY) &&
        velocity>=-.1f && rootY+bodyHeight>=contactY;
}
// Clamp root uses /1000 quantization after +.5, then subtracts one engine
// unit (.01 native). Positive bias applies even for negative world positions.
inline float headClampedRoot(float contactY,float bodyHeight=HeadBodyHeight) {
    // Original fctiwz truncation, including negative world heights.
    return std::trunc(std::fma(contactY-bodyHeight,1000.f,.5f))/1000.f-.01f;
}
// Reject inclusive ordinary angle cone[50,130] degrees; alternate narrow
// cone[75,105]. These are source response gates, not render-facing cutaways.
inline bool headInclinationAllowed(float angle,bool narrow=false) {
    return !(angle>=(narrow?75.f:50.f) && angle<=(narrow?105.f:130.f));
}
}
