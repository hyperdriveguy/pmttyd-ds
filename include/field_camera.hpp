#pragma once
#include <cmath>
namespace ttyd {
// Current handheld framing; original camera paths remain to be recovered.
constexpr float FieldFov=42,FieldEyeY=4.8f,FieldEyeZ=6.5f,FieldAimY=1,FieldAimZ=-1.4f;
inline float recoveryTopHeight(float floor) {
    const float dy=FieldEyeY-FieldAimY,dz=FieldEyeZ-FieldAimZ;
    const float tangent=std::tan(FieldFov*3.14159265358979323846f/360.f);
    const float edge=FieldEyeY+(tangent*dz*FieldEyeZ-dy*FieldEyeZ)/(dz+tangent*dy);
    // Original mot_bottomless starts the screen probe300 units above target
    // and raises it10 units until offscreen. Native engine-unit scale=.01.
    float height=3;
    while(height<=edge)height+=.1f;
    return floor+height;
}
}
