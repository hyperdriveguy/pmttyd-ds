#pragma once
#include "camera_road_state.hpp"
namespace ttyd {
// Portable calcCurrentCamState / look-at / GX perspective adapter. Authored
// geometry *10 engine *.01 native. SDK rounding and event/dolly remain separate.
inline std::optional<FieldCameraFrame> cameraRoadFrame(const CameraRoadState& state) {
    CameraPoint eye{0,10,10},at{};
    if(state.active) {
        const float a=state.pitch*3.141592025756836f/180;
        const CameraPoint offset{state.direction.x*state.distance*std::cos(a),state.distance*std::sin(a),state.direction.z*state.distance*std::cos(a)};
        eye={state.anchor.x+offset.x,state.anchor.y+offset.y,state.anchor.z+offset.z};
        const float length=std::hypot(state.direction.x,state.direction.z);if(length==0)return std::nullopt;
        const CameraPoint axis{-state.direction.z/length,0,state.direction.x/length};
        const float b=state.secondary*.01745329238474369f,co=std::cos(b),si=std::sin(b);
        const CameraPoint v{-offset.x,-offset.y,-offset.z};const float dot=axis.x*v.x+axis.z*v.z;
        at={eye.x+v.x*co-axis.z*v.y*si+axis.x*dot*(1-co),
            eye.y+v.y*co+(axis.z*v.x-axis.x*v.z)*si,
            eye.z+v.z*co+axis.x*v.y*si+axis.z*dot*(1-co)};
        eye={eye.x*.1f,eye.y*.1f,eye.z*.1f};at={at.x*.1f,at.y*.1f,at.z*.1f};
    }
    const float dx=eye.x-at.x,dy=eye.y-at.y,dz=eye.z-at.z,length=std::sqrt(dx*dx+dy*dy+dz*dz);
    const float horizontal=std::hypot(dx,dz);
    if(length==0 || horizontal==0 || !std::isfinite(length))return std::nullopt;
    const CameraPoint back{dx/length,dy/length,dz/length},right{dz/horizontal,0,-dx/horizontal};
    const CameraPoint up{back.y*right.z,back.z*right.x-back.x*right.z,-back.y*right.x};
    FieldCameraFrame frame;frame.eye=eye;frame.at=at;frame.fov=25;frame.nearPlane=.01f;frame.farPlane=327.68f;
    const auto row=[&](CameraPoint v){return std::array<float,4>{v.x,v.y,v.z,-v.x*eye.x-v.y*eye.y-v.z*eye.z};};
    frame.view={row(right),row(up),row(back)};
    const float f=1/std::tan(25*3.14159265358979323846f/360);
    frame.projection={f/(608.f/480),0,f,0,-frame.nearPlane/(frame.farPlane-frame.nearPlane),-frame.farPlane*frame.nearPlane/(frame.farPlane-frame.nearPlane)};
    frame.yaw=std::atan2(at.x-eye.x,eye.z-at.z)*180/3.14159265358979323846f;
    if(frame.yaw<0)frame.yaw+=360;
    return frame;
}
}
