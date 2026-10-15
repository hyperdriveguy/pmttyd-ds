#pragma once
#include <cmath>
#include <optional>
#include "field_camera_state.hpp"
namespace ttyd {
// No-asset fallback framing; original road assets use the authored pipeline.
// Native fallback lens 42deg, eye offsets 4.8Y/6.5Z, aim 1Y/-1.4Z. These
// are handheld framing defaults, not recovered original road-camera constants.
constexpr float FieldFov=42,FieldEyeY=4.8f,FieldEyeZ=6.5f,FieldAimY=1,FieldAimZ=-1.4f;
// Existing fixed-view fallback, explicitly separate from original camera-road
// state. Its matrix/root are in native units; source cache uses 608x480 viewport.
// Orthogonal view/perspective stay in native world units. Near.1/far80 and
// 608/480 source aspect are presentation inputs, distinct from source engine lens.
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
// Ordinary mot_bottomless entry. Native engine scale=.01; this uses the
// current frame, not Mario's cached screen words. Invalid/nonterminating
// adapter domains are unsupported instead of inventing a top height.
// Probe starts at returnY+1 then rises.1 native units until source screen
// cache exits its bounds. -20 is native adapter admission; do not manufacture
// a result for invalid projection or a nonadvancing float increment.
inline std::optional<float> bottomlessProbeHeight(CameraPoint retainedReturn,const FieldCameraFrame& frame) {
    if(!std::isfinite(retainedReturn.y) || retainedReturn.y<-20)return std::nullopt;
    CameraPoint point=retainedReturn;point.y+=1;
    for(;;) {
        const auto screen=projectCameraRoot(point,frame.view,frame.projection,frame.viewport);
        if(!screen.valid)return std::nullopt;
        if(!screen.inScreen())return point.y;
        const float next=point.y+.1f;
        if(!std::isfinite(next) || next==point.y)return std::nullopt;
        point.y=next;
    }
}
}
