#pragma once
#include <array>
namespace ttyd {
// Native 400x240 adaptation; source book framing remains 608x480 at half scale.
// Extra horizontal field is revealed without stretching the book geometry.
struct OpeningBookView {
    static constexpr unsigned width=400,height=240,sourceWidth=608,sourceHeight=480;
    static constexpr unsigned captureLeft=110,captureWidth=193,captureHeight=240;
    static constexpr float sourceScale=.5f;
    static constexpr std::array<float,4> desktop{{0,-720.f/19.f,400,6000.f/19.f}};
};
// Source capture-rectangle placement, then ImageBaseMatrix T(offset)*S(scale).
// Offsets live in the virtual64x48 image plane, outside the crop's local scale.
inline std::array<float,3> openingBookPaperPoint(float x,float y,float z,float depth,float scale=1,float offsetX=0,float offsetY=0){
    const float vx=-32.f+64.f*124.f/608.f+(x+32.f)*386.f/608.f;
    const float vy=24.f+(y-24.f);
    return {{200.f+(offsetX+scale*vx)*304.f/64.f,120.f-(offsetY+scale*vy)*240.f/48.f,
             depth-16.f-z*scale*193.f/64.f}};
}
}
