#pragma once
#include <cmath>
#include "field_camera_state.hpp"
namespace ttyd {
// Current handheld framing; original camera paths remain to be recovered.
constexpr float FieldFov=42,FieldEyeY=4.8f,FieldEyeZ=6.5f,FieldAimY=1,FieldAimZ=-1.4f;
// Existing fixed-view fallback, explicitly separate from original camera-road
// state. Its matrix/root are in native units; source cache uses 608x480 viewport.
inline FieldCameraFrame adaptedFieldCameraFrame(float x,float y,float z) {
    const float by=FieldEyeY-FieldAimY,bz=FieldEyeZ-FieldAimZ;
    const float length=std::hypot(by,bz),uy=bz/length,uz=-by/length;
    const float ey=y+FieldEyeY,ez=z+FieldEyeZ;
    FieldCameraFrame frame;frame.eye={x,ey,ez};frame.at={x,y+FieldAimY,z+FieldAimZ};
    frame.view={{{1,0,0,-x},{0,uy,uz,-uy*ey-uz*ez},{0,by/length,bz/length,-(by*ey+bz*ez)/length}}};
    const float f=1/std::tan(FieldFov*3.14159265358979323846f/360.f);
    constexpr float near=.1f,far=80;
    frame.projection={f/(608.f/480.f),0,f,0,-near/(far-near),-far*near/(far-near)};
    return frame;
}
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
