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
inline bool headAttributesSolid(uint32_t flags) {
    return !(flags&4) || (flags&0x00800000);
}
inline float headContactHeight(float triangleY) {
    return std::trunc(triangleY*10000.f+18.5f)/10000.f;
}
inline bool headContactReached(float rootY,float velocity,float contactY) {
    return std::isfinite(rootY) && std::isfinite(velocity) && std::isfinite(contactY) &&
        velocity>=-.1f && rootY+HeadBodyHeight>=contactY;
}
inline float headClampedRoot(float contactY) {
    // Original fctiwz truncation, including negative world heights.
    return std::trunc((contactY-HeadBodyHeight)*1000.f+.5f)/1000.f-.01f;
}
}
