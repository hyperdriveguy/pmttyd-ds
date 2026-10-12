#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace ttyd {
// Source camera/Mario values below are engine units, unless a projection's
// caller supplies another consistent matrix/root space. No 3DS services here.
struct CameraPoint {float x=0,y=0,z=0;};
struct CameraShift {
    float current=0,target=0,rate=0;
    void tick() {
        if(current>target)current=std::max(target,current+rate*(target-1-current));
        else if(current<target)current=std::min(target,current+rate*(target+1-current));
    }
};
struct MarioCameraTarget {
    CameraPoint retained;
    CameraShift side{0,0,.03f},vertical{0,0,.06f},depth{0,0,.06f};
    float drawOffset=0,depthValue=0;
    // Bounded ordinary target owner; other display/motion/zoom domains need
    // their own source composition rather than an invented ordinary fallback.
    bool publish(CameraPoint root,float viewYaw,uint32_t displayFlags=0,int motion=0) {
        if(motion==29)return false;
        side.tick();vertical.tick();depth.tick();
        if(displayFlags&0x10000)return false;
        if(motion==25)return false;
        const float yaw=(viewYaw-90)*3.14159265358979323846f/180;
        const float sine=std::sin(yaw),cosine=std::cos(yaw);
        retained.x=root.x+side.current*sine;
        retained.z=root.z-side.current*cosine;
        if(displayFlags&0x20000) {
            retained.x+=depth.current*sine;retained.z-=depth.current*cosine;
        } else retained.y=root.y+drawOffset+vertical.current;
        depthValue=depth.current;return true;
    }
};
struct FieldCameraTarget {
    CameraPoint point;
    float followRate=.20000000298023224f;
    void consume(const MarioCameraTarget& mario,uint32_t cameraFlags=0) {
        if(!(cameraFlags&0x10))point.y=std::fma(mario.retained.y+1-point.y,followRate,point.y);
        point.x=mario.retained.x+.01f;point.z=mario.retained.z;
    }
};
inline float cameraHeadingSnapshot(float yaw) {
    const float rounded=std::trunc(std::fma(yaw,1000.f,yaw<0?-.5f:.5f))/1000.f;
    float wrapped=std::fmod(rounded,360.f);return wrapped<0?wrapped+360:wrapped;
}
struct CameraScreenCache {
    int32_t x=0,y=0,z=0;
    bool valid=true;
    bool inScreen() const {return valid && x>=0 && x<=600 && y>=0 && y<=535;}
};
using CameraView=std::array<std::array<float,4>,3>;
// GX projection vector's six coefficients (p[1]..p[6]); perspective p[0]=0.
using CameraProjection=std::array<float,6>;
struct CameraViewport {float x=0,y=0,width=608,height=480,nearDepth=0,farDepth=1;};
struct FieldCameraFrame {
    CameraView view{};
    CameraProjection projection{};
    CameraViewport viewport;
    float yaw=0;
    CameraPoint eye,at; // Native units for the rendering backend.
    float fov=42,nearPlane=.1f,farPlane=80;
};
inline CameraScreenCache projectCameraRoot(CameraPoint root,const CameraView& view,
                                          const CameraProjection& p,const CameraViewport& viewport={}) {
    const auto transform=[&](const auto& row){return row[0]*root.x+row[1]*root.y+row[2]*root.z+row[3];};
    const float vx=transform(view[0]),vy=transform(view[1]),vz=transform(view[2]);
    const float invW=1/-vz;
    const float sx=viewport.x+invW*(p[0]*vx+p[1]*vz)*viewport.width*.5f+viewport.width*.5f;
    const float sy=viewport.y-invW*(p[2]*vy+p[3]*vz)*viewport.height*.5f+viewport.height*.5f;
    const float sz=viewport.farDepth+invW*(p[4]*vz+p[5])*(viewport.farDepth-viewport.nearDepth);
    CameraScreenCache result;
    // Outside the recovered finite/representable fctiwz domain, report an
    // invalid adapter result instead of executing undefined host conversions.
    for(float value:{sx,sy,sz})if(!std::isfinite(value) || double(value)<-2147483648. || double(value)>=2147483648.)result.valid=false;
    if(result.valid){result.x=int32_t(sx);result.y=int32_t(sy);result.z=int32_t(sz);}
    return result;
}
}
