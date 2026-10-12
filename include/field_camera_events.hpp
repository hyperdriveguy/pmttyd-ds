#pragma once
#include "field_camera_state.hpp"
#include <optional>
namespace ttyd {
// cam3dMain logical state; all points are engine units. Native time is a
// simulation-millisecond bridge, not the source OS tick/timebase service.
struct CameraBlendTimer {
    uint16_t state=0;
    float progress=0;
    uint64_t start=0;
    int64_t duration=0;
    uint8_t curve=11;
    bool update(uint64_t now) {
        if(state>3 || now<start)return false;
        if(!state)return true;
        if(duration<=0){progress=state==1?0:1;state=0;return true;}
        const float ratio=float(now-start)/float(duration);
        progress=state==1?1-ratio:ratio;
        if(state==1 && progress<0){progress=0;state=0;}
        else if(state!=1 && progress>1){progress=1;state=0;}
        return true;
    }
};
inline std::optional<float> cameraInterpolation(int32_t mode,int32_t step,int32_t count,float a,float b) {
    if(count==0)return b;
    if(mode<0 || mode>13)return std::nullopt; // Source callback-pointer domain remains separate.
    const float s=float(step),n=float(count),d=b-a,r=n-s,pi=3.141592025756836f,halfPi=1.570796012878418f;
    const auto cosine=[](float phase){return float(std::cos(double(phase)));};
    const auto sine=[](float phase){return float(std::sin(double(phase)));};
    switch(mode) {
    case 0:return a+(s*d)/n;
    case 1:return a+((s*s)*d)/(n*n);
    case 2:return a+((s*(s*s))*d)/(n*(n*n));
    case 3:return a+((s*(s*(s*s)))*d)/(n*(n*(n*n)));
    case 4:return (a+d)-((r*r)*d)/(n*n);
    case 5:return (a+d)-((r*(r*r))*d)/(n*(n*n));
    case 6:return (a+d)-((r*(r*(r*r)))*d)/(n*(n*(n*n)));
    case 7:return b-(r*(r*(d*cosine(4*(pi*(s/n))))))/(n*n);
    case 8:return b-(r*(r*(d*cosine((4*(pi*((s*s)/n)))/((15*n)/100)))))/(n*n);
    case 9:return b-(s*(s*(d*cosine((4*(pi*((s*s)/n)))/((15*n)/100)))))/(n*n);
    case 10:return b-std::fabs((r*(r*cosine((4*(pi*((s*s)/n)))/((40*n)/100))))/(n*n))*d;
    case 11:return a+((d*(1-cosine((pi*s)/n)))*.5f);
    case 12:return a+d*sine((halfPi*s)/n);
    case 13:return a+d*(1-cosine((halfPi*s)/n));
    }
    return std::nullopt;
}
inline std::optional<float> cameraEventCurve(uint8_t curve,float progress) {
    if(!std::isfinite(progress))return std::nullopt;
    const double steps=std::trunc(double(100000.f*progress));
    if(steps<-2147483648. || steps>=2147483648.)return std::nullopt;
    return cameraInterpolation(curve,int32_t(steps),100000,0,1);
}

struct FieldCameraEvents {
    CameraBlendTimer view,target;
    CameraPoint storedEye,storedAt,presetEye,presetAt;
    CameraPoint follow,look,anchor,previousAnchor;
    float dolly=0,zeroStart=0,endPercent=0,previousPercent=0;
    float fov=25,nearPlane=1,farPlane=32768; // Live engine-space lens.
    static CameraPoint blend(CameraPoint a,CameraPoint b,float t) {
        const float inverse=1-t;
        return {a.x*inverse+b.x*t,a.y*inverse+b.y*t,a.z*inverse+b.z*t};
    }
    bool prepare(CameraPoint rawFollow,CameraPoint rawLook,uint64_t now) {
        auto next=*this;
        if(!next.view.update(now)||!next.target.update(now))return false;
        const auto t=cameraEventCurve(next.target.curve,next.target.progress);if(!t)return false;
        next.zeroStart=0;
        if(next.target.state==3) {
            next.follow=blend(next.previousAnchor,next.anchor,*t);next.look=next.follow;
            next.dolly=next.previousPercent*(1-*t)+next.endPercent * *t;
        } else {
            next.follow=blend(rawFollow,next.anchor,*t);next.look=blend(rawLook,next.anchor,*t);
            next.dolly=next.zeroStart*(1-*t)+next.endPercent * *t;
        }
        *this=next;return true;
    }
    // Source candidate before A blending. Road eye/at inputs remain engine units.
    bool finish(CameraPoint roadEye,CameraPoint roadAt,CameraPoint& eye,CameraPoint& at,bool roadUsed=true) const {
        if(view.state>3)return false;
        const auto t=cameraEventCurve(view.curve,1-view.progress);if(!t)return false;
        CameraPoint candidateEye=roadEye,candidateAt=roadAt;
        if(view.state==3){candidateEye=presetEye;candidateAt=presetAt;}
        else if(roadUsed) {
            candidateEye.x+=(candidateAt.x-candidateEye.x)*dolly/100;
            candidateEye.y+=(candidateAt.y-candidateEye.y)*dolly/100;
            candidateEye.z+=(candidateAt.z-candidateEye.z)*dolly/100;
        }
        eye=blend(storedEye,candidateEye,*t);at=blend(storedAt,candidateAt,*t);return true;
    }
    bool setTarget(CameraPoint xyz,float percent,int64_t duration,uint8_t curve,uint64_t now) {
        if(curve>13 || duration<0)return false;
        previousAnchor=follow;anchor=xyz;previousPercent=dolly;endPercent=percent;
        target.state=3;target.start=now;target.duration=duration;target.curve=curve;return true;
    }
    bool targetOff(int64_t duration,uint8_t curve,uint64_t now) {
        if(curve>13 || duration<0)return false;
        previousAnchor=follow;previousPercent=dolly;
        target.state=1;target.start=now;target.duration=duration;target.curve=curve;return true;
    }
    bool setView(CameraPoint eye,CameraPoint at,CameraPoint actualEye,CameraPoint actualAt,
                 int64_t duration,uint8_t curve,uint64_t now) {
        if(curve>13 || duration<0)return false;
        presetEye=actualEye;presetAt=actualAt;storedEye=eye;storedAt=at;
        view.state=3;view.start=now;view.duration=duration;view.curve=curve;return true;
    }
    bool viewOff(CameraPoint actualEye,CameraPoint actualAt,int64_t duration,uint8_t curve,uint64_t now) {
        if(curve>13 || duration<0)return false;
        presetEye=actualEye;presetAt=actualAt;
        view.state=1;view.start=now;view.duration=duration;view.curve=curve;return true;
    }
    void roomLoad() {
        // camLoadRoad resets these fields, retaining Bstate/times, anchors,
        // dollyEnd/previousPercent and stored/preset views.
        view.state=0;view.progress=0;view.curve=11;target.progress=0;target.curve=11;follow={};fov=25;
    }
};
}
